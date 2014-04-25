#include "Engine.h"
#include <string>
#include <fstream>
#include <stdio.h>
#include "Vector3.h"
#include "Common.h"
#include <math.h>
#include <functional>
#include "Material.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Polygon.h"
#include <vector>
#include "PolygonalPatch.h"
#include "TrianglePatch.h"
#include "Triangle.h"
#include "ImageTexture.h"
#include <string.h>
#include "Matrix.h"
#include <unordered_map>
#include "Mesh.h"
#include "MeshTriangle.h"

Engine::Engine(int _maxRenderDepth=6)
	:maxRenderDepth(_maxRenderDepth),currentMaterial(nullptr,rgb(0,0,0),0,0,0,0,0),background(0,0,0),ambient(1.0f,1.0f,1.0f),
	currentDetailLevel(0){
		animationStack.push(ZeroAnimation());
}

void Engine::parseComment(FILE *f,std::string filename){
	for(char ch;ch=getc(f)!='\n';);
}

void Engine::parseViewpoint(FILE *f,std::string filename){
	double x,y,z;
	
	if(fscanf(f,"from %lf %lf %lf",&x,&y,&z)!=3)
		goto fmterr;
	Vector3 orig(x,y,z);

	if(fscanf(f,"at %lf %lf %lf",&x,&y,&z)!=3)
		goto fmterr;
	Vector3 lookat(x,y,z);

	if(fscanf(f,"up %lf %lf %lf",&x,&y,&z)!=3)
		goto fmterr;
	Vector3 up(x,y,z);
	
	double angle,hither;
	if(fscanf(f,"angle %lf",&angle)!=1)
		goto fmterr;

	if(fscanf(f,"hither %lf",&hither)!=1)
		goto fmterr;

	if(fscanf(f,"resolution %d %d",&picwidth,&picheight)!=2)
		goto fmterr;

	Vector3 axis=lookat-orig;
	double h=axis.length()*tan(angle/2.0f);
	camera=std::make_shared<Camera>(Camera(orig,lookat,h*picwidth/picheight,h,hither,animationStack.top()*transforms["camera"]));
	return;

fmterr:
	fprintf(stderr,"Parse viewpoint error in file %s at byte %d\n",filename.c_str(),ftell(f));
	exit(-1);
}

std::string ITOS(int x,int base){
	std::string tmp;
	for(;x>0;x/=base)
		tmp+=x%base;
	return std::string(tmp.rbegin(),tmp.rend());
}

void Engine::parseLight(FILE *f,std::string filename){
	std::string name;
	DynamicVector3Type animation=animationStack.top();

	char ch=getc(f);
	if(ch=='a'){
		char s[100];
		if(fscanf(f,"%s",s)!=1){
			fprintf(stderr,"Light source name syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
			exit(-1);
		}
		name=std::string(s);
		auto it=transforms.find(name);
		if(it!=transforms.end())
			animation=animation*transforms[name];
	}
	else
		name=ITOS(lights.size(),10);

	double a,b,c;
	if(fscanf(f,"%lf %lf %lf",&a,&b,&c)!=3){
		fprintf(stderr,"Light source position syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
		exit(-1);
	}
	Vector3 pos(a,b,c);

	int num=fscanf(f,"%lf %lf %lf",&a,&b,&c);
	if((num!=0)&&(num!=3)){
		fprintf(stderr,"Light source color syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
		exit(-1);
	}
	rgb col;
	if(num==0)
		col=rgb(1.0f,1.0f,1.0f);
	else
		col=rgb(a,b,c);

	lights.insert(std::make_pair(name,Light(col,pos,animation)));
}

void Engine::parseBackground(FILE *f,std::string filename){
	double a,b,c;
	if(fscanf(f,"%lf %lf %lf",&a,&b,&c)==3)
		background=rgb(a,b,c);
	else{
		fprintf(stderr,"background color syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
		exit(-1);
	}
}

void Engine::parseFill(FILE *f,std::string filename){
	double r,g,b,dr,dg,db,sr,sg,sb,shine,T,rIndex;
	char ch;
	if(ch=getc(f)!='m'){
		ungetc(ch,f);
		if(fscanf(f,"%lf %lf %lf",&r,&g,&b)!=3){
			fprintf(stderr,"fill color syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
			exit(-1);
		}
		if(fscanf(f,"%lf %lf %lf %lf %lf",&dr,&sr,&shine,&T,&rIndex)!=5){
			fprintf(stderr,"fill material syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
			exit(-1);
		}
		currentMaterial=Material(nullptr,rgb(r,g,b),dr,sr,shine,T,rIndex);
	}
	else{
		if(fscanf(f,"%lf %lf %lf",&r,&g,&b)!=3){
			fprintf(stderr,"fill material ambient syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
			exit(-1);
		}
		if(fscanf(f,"%lf %lf %lf",&dr,&dg,&db)!=3){
			fprintf(stderr,"fill material diffuse syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
			exit(-1);
		}
		if(fscanf(f,"%lf %lf %lf",&sr,&sg,&sb)!=3){
			fprintf(stderr,"fill material specular syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
			exit(-1);
		}
		if(fscanf(f,"%lf %lf %lf",&shine,&T,&rIndex)!=3){
			fprintf(stderr,"fill material (phong, transp, IOR) syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
			exit(-1);
		}
		currentMaterial=Material(nullptr,rgb(r,g,b),rgb(dr,dg,db),rgb(sr,sg,sb),shine,T,rIndex);
	}
}

void Engine::parseCone(FILE *f,std::string filename){
	double x1,y1,z1,x2,y2,z2,r1,r2;
	if(fscanf(f,"%lf %lf %lf %lf %lf %lf %lf %lf",&x1,&y1,&z1,&r1,&x2,&y2,&z2,&r2)!=8){
		fprintf(stderr,"cylinder or cone syntax error in file %s at byte %d\n",filename.c_str,ftell(f));
		exit(-1);
	}
	if(r1<0.0f){
		r1=-r1;
		r2=-r2;
	}
	scene.push_back(std::tr1::shared_ptr<Cylinder>(new Cylinder(Vector3(x1,y1,z1),r1,Vector3(x2,y2,z2),r2,currentMaterial,animationStack.top())));
}

void Engine::parseSphere(FILE *f,std::string filename){
	double x,y,z,r;
	if(fscanf(f,"%lf %lf %lf %lf",&x,&y,&z,&r)!=4){
		fprintf(stderr,"sphere syntax error in file %s at byte %d\n",filename.c_str(),ftell(f));
		exit(-1);
	}
	scene.push_back(std::tr1::shared_ptr<Sphere>(new Sphere(Vector3(x,y,z),r,currentMaterial,animationStack.top())));
}

void Engine::parsePoly(FILE *f,std::string filename){
	char ch=fgetc(f);
	if(ch=='p'){
		int n;
		if(fscanf(f,"%d",&n)!=1)
			goto fmterr;
		
		std::vector<Vector3> vert;
		std::vector<Vector3> norm;
		double x,y,z,a,b,c;
		for(;n>0;--n){
			if(fscanf(f,"%lf %lf %lf %lf %lf %lf",&x,&y,&z,&a,&b,&c)!=6)
				goto fmterr;
			vert.push_back(Vector3(x,y,z));
			norm.push_back(Vector3(a,b,c));
		}
		scene.push_back(std::tr1::shared_ptr<PolygonalPatch>(new PolygonalPatch(vert,norm,currentMaterial,animationStack.top())));
	}
	else{
		ungetc(ch,f);

		int n;
		if(fscanf(f,"%d",&n)!=1)
			goto fmterr;

		std::vector<Vector3> vert;
		double x,y,z;
		for(;n>0;--n){
			if(fscanf(f,"%lf %lf %lf",&x,&y,&z)!=3)
				goto fmterr;
			vert.push_back(Vector3(x,y,z));
		}
		scene.push_back(std::tr1::shared_ptr<Polygon>(new Polygon(vert,currentMaterial,animationStack.top())));
	}
	return;

fmterr:
	fprintf(stderr,"polygon or patch syntax error in file %d at byte %d\n",filename.c_str(),ftell(f));
	exit(-1);
}

void Engine::parseInclude(FILE *f,std::string filename,int pass){
	int level;
	if(fscanf(f,"%d",&level)!=1){
		fprintf(stderr,"Error: could not parse include in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}

	std::string ifilename;
	char ch[100];
	if(fscanf(f,"%s",ch)!=1){
		fprintf(stderr,"parse include file error in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}
	ifilename=std::string(ch);
	if(level<=currentDetailLevel){
		FILE *ifile;
		if(ifile=fopen(ifilename.c_str(),"r")){
			if(pass==1)
				parseFilePass1(ifile,ifilename);
			if(pass==2)
				parseFilePass2(ifile,ifilename);
			fclose(ifile);
		}
		else{
			fprintf(stderr,"Error: could not open include file: <%s> in file %s at byte %d",ifilename.c_str(),filename.c_str(),ftell(f));
			exit(-1);
		}
	}
	else
		fprintf(stderr,"Skipping include file: %s in file %s at byte %d",ifilename.c_str(),filename.c_str(),ftell(f));
}

void Engine::parseDetailLevel(FILE *f,std::string filename){
	if(fscanf(f,"%d",&currentDetailLevel)!=1){
		fprintf(stderr,"Error: could not parse detail level in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}
}

void Engine::parseAnimatedTriangle(FILE *f,std::string filename){
	int num;
	std::vector<Vector3> v1;
	std::vector<Vector3> v2;
	std::vector<Vector3> v3;
	std::vector<Vector3> n1;
	std::vector<Vector3> n2;
	std::vector<Vector3> n3;
	std::vector<double> time;
	for(double x,y,z,a,b,c,t;num>0;--num){
		if(fscanf(f,"%lf",&t)!=1)
			goto fmterr;
		time.push_back(t);

		if(fscanf(f,"%lf %lf %lf %lf %lf %lf",&x,&y,&z,&a,&b,&c)!=6)
			goto fmterr;
		v1.push_back(Vector3(x,y,z));
		n1.push_back(Vector3(a,b,c));

		if(fscanf(f,"%lf %lf %lf %lf %lf %lf",&x,&y,&z,&a,&b,&c)!=6)
			goto fmterr;
		v2.push_back(Vector3(x,y,z));
		n2.push_back(Vector3(a,b,c));

		if(fscanf(f,"%lf %lf %lf %lf %lf %lf",&x,&y,&z,&a,&b,&c)!=6)
			goto fmterr;
		v3.push_back(Vector3(x,y,z));
		n3.push_back(Vector3(a,b,c));
	}

	Vector2 v(0,0);
	std::tr1::shared_ptr<TrianglePatch> ptr(new TrianglePatch(v1[0],v2[0],v3[0],n1[0],n2[0],n3[0],v,v,v,currentMaterial,
		TimeInterpolate(v1,time)*animationStack.top(),TimeInterpolate(v2,time)*animationStack.top(),TimeInterpolate(v3,time)*animationStack.top(),
		TimeInterpolate(n1,time)*animationStack.top(),TimeInterpolate(n2,time)*animationStack.top(),TimeInterpolate(n3,time)*animationStack.top()));
	scene.push_back(ptr);
	return;
fmterr:
	fprintf(stderr,"Error: could not parse animated triangle (tpa) in file %s at byte %d",filename.c_str(),ftell(f));
	exit(-1);
}

void Engine::parseTriangle(FILE *f,std::string filename){
	char ch=getc(f);
	if(ch=='p'){
		char ch1=getc(f);
		if(ch1=='a'){
			parseAnimatedTriangle(f,filename);
			return;
		}
		else{
			fprintf(stderr,"Error: could not parse animated triangle in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}
	}
	if(ch=='t'){
		char ch1=fgetc(f);
		if(ch1=='p'){
			std::string texturename;
			char s[100];
			if(fscanf(f,"%s",s)!=1)
				goto fmterr;
			texturename=std::string(s);

			FILE *tf;
			if(tf=fopen(texturename.c_str(),"rb"))
				fclose(tf);
			else
				goto fmterr;

			Material material(currentMaterial);
			material.setTex(std::shared_ptr<ImageTexture>(new ImageTexture(texturename)));

			int a,b,c,x,y,z,u,v;
			if(fscanf(f,"%lf %lf %lf %lf %lf %lf %lf %lf",&x,&y,&z,&a,&b,&c,&u,&v)!=8)
				goto fmterr;
			Vector3 v1(x,y,z);
			Vector3 n1(a,b,c);
			Vector2 t1(u,v);

			if(fscanf(f,"%lf %lf %lf %lf %lf %lf %lf %lf",&x,&y,&z,&a,&b,&c,&u,&v)!=8)
				goto fmterr;
			Vector3 v2(x,y,z);
			Vector3 n2(a,b,c);
			Vector2 t2(u,v);

			if(fscanf(f,"%lf %lf %lf %lf %lf %lf %lf %lf",&x,&y,&z,&a,&b,&c,&u,&v)!=8)
				goto fmterr;
			Vector3 v3(x,y,z);
			Vector3 n3(a,b,c);
			Vector2 t3(u,v);

			std::tr1::shared_ptr<TrianglePatch> ptr(new TrianglePatch(v1,v2,v3,n1,n2,n3,t1,t2,t3,material,
				animationStack.top(),animationStack.top(),animationStack.top(),animationStack.top(),animationStack.top(),animationStack.top()));
			scene.push_back(ptr);
			return;
		}
		else{
			ungetc(ch1,f);
			std::string texturename;
			char s[100];
			if(fscanf(f,"%s",s)!=1)
				goto fmterr;
			texturename=std::string(s);

			FILE *tf;
			if(tf=fopen(texturename.c_str(),"rb"))
				fclose(tf);
			else
				goto fmterr;

			Material material(currentMaterial);
			material.setTex(std::shared_ptr<ImageTexture>(new ImageTexture(texturename)));

			int x,y,z,u,v;
			if(fscanf(f,"%lf %lf %lf %lf %lf",&x,&y,&z,&u,&v)!=5)
				goto fmterr;
			Vector3 v1(x,y,z);
			Vector2 t1(u,v);

			int x,y,z,u,v;
			if(fscanf(f,"%lf %lf %lf %lf %lf",&x,&y,&z,&u,&v)!=5)
				goto fmterr;
			Vector3 v2(x,y,z);
			Vector2 t2(u,v);

			int x,y,z,u,v;
			if(fscanf(f,"%lf %lf %lf %lf %lf",&x,&y,&z,&u,&v)!=5)
				goto fmterr;
			Vector3 v3(x,y,z);
			Vector2 t3(u,v);

			std::tr1::shared_ptr<Triangle> ptr(new Triangle(v1,v2,v3,t1,t2,t3,material,animationStack.top(),animationStack.top(),animationStack.top()));
			scene.push_back(ptr);
			return;
		}
	}

fmterr:
	fprintf(stderr,"Error: could not parse textured triangle in file %s at byte %d",filename.c_str(),ftell(f));
	exit(-1);
}

DynamicVector3Type Engine::parseKeyFramesTranslate(FILE *f,std::string filename,int num){
	std::vector<double> time,tension,continuity,bias;
	std::vector<Vector3> shift;
	for(double x,y,z,a,b,c,t;num>0;--num){
		if(fscanf(f,"%lf %lf %lf %lf %lf %lf %lf",&t,&x,&y,&z,&a,&b,&c)!=7)
			goto fmterr;
		time.push_back(t);
		shift.push_back(Vector3(x,y,z));
		tension.push_back(a);
		continuity.push_back(b);
		bias.push_back(c);
	}

	return [&time,&shift,&tension,&continuity,&bias](const Vector3 &_orig,double _time){
		//these codes are fake, they are used to show the behaviour
		if(_time<time[0])
			return _orig+shift[0];
		if(_time>time[time.size()])
			return _orig+shift[shift.size()];
		int i=search(time,_time);
		if(fabs(time[i]-_time)<EPSILON)
			return _orig+shift[i];
		double c=(time[i]-_time)/(time[i]-time[i-1]);
		return _orig+shift[i-1]*c+shift[i]*(1-c);
	};
fmterr:
	fprintf(stderr,"parse key frame translation error in file %s at byte %d",filename.c_str(),ftell(f));
	exit(-1);
}

DynamicVector3Type Engine::parseKeyFramesRotate(FILE *f,std::string filename,int num){
	std::vector<double> time,tension,continuity,bias,angle;
	std::vector<Vector3> axis;
	for(double x,y,z,a,b,c,g,t;num>0;--num){
		if(fscanf(f,"%lf %lf %lf %lf %lf %lf %lf %lf",&t,&x,&y,&z,&g,&a,&b,&c)!=8)
			goto fmterr;

		time.push_back(t);
		axis.push_back(Vector3(x,y,z));
		tension.push_back(a);
		continuity.push_back(b);
		bias.push_back(c);
		angle.push_back(g*M_PI/180.0f);
	}
	
	std::vector<Matrix> srotate;
	srotate.push_back(rotate(axis[0],angle[0]));
	for(int i=1;i<angle.size();i++)
		srotate.push_back(*(srotate.end())*rotate(axis[i],angle[i]));

	return [&time,&axis,&angle,&srotate,&tension,&continuity,&bias](const Vector3 &_orig,double _time){
		//these codes are fake, they are used to show the behaviour
		if(_time<time[0])
			return srotate[0]*_orig;
		if(_time>time[time.size()])
			return srotate[srotate.size()]*_orig;
		int i=search(time,_time);
		if(fabs(time[i]-_time)<EPSILON)
			return srotate[i]*_orig;
		double c=(time[i]-_time)/(time[i]-time[i-1]);
		double a=angle[i-1]*(1-c);
		return srotate[i-1]*rotate(axis[i-1],a)*_orig;
	};
fmterr:
	fprintf(stderr,"parse key frame rotation error in file %s at byte %d",filename.c_str(),ftell(f));
	exit(-1);
}

DynamicVector3Type Engine::parseKeyFramesScale(FILE *f,std::string filename,int num){
	std::vector<double> time,tension,continuity,bias;
	std::vector<Vector3> scale;
	for(double t,a,b,c,x,y,z;num>0;--num){
		if(fscanf(f,"%lf %lf %lf %lf %lf %lf %lf",&t,&x,&y,&z,&a,&b,&c)!=7)
			goto fmterr;

		time.push_back(t);
		scale.push_back(Vector3(x,y,z));
		tension.push_back(a);
		continuity.push_back(b);
		bias.push_back(c);
	}

	return [&time,&scale,&tension,&continuity,&bias](const Vector3 &_orig,double _time){
		//these codes are fake, they are used to show the behaviour
		if(_time<time[0])
			return Vector3(_orig[0]*scale[0][0],_orig[1]*scale[0][1],_orig[2]*scale[0][2]);
		if(_time>time[time.size()])
			return Vector3(_orig[0]*scale[scale.size()][0],_orig[1]*scale[scale.size()][1],_orig[2]*scale[scale.size()][2]);
		int i=search(time,_time);
		if(fabs(time[i]-_time)<EPSILON)
			return Vector3(_orig[0]*scale[i][0],_orig[1]*scale[i][1],_orig[2]*scale[i][2]);
		double c=(time[i]-_time)/(time[i]-time[i-1]);
		Vector3 scaleV=scale[i-1]*c+scale[i]*(1-c);
		return Vector3(_orig[0]*scaleV[0],_orig[1]*scaleV[1],_orig[2]*scaleV[2]);
	};
fmterr:
	fprintf(stderr,"parse key frame scaling error in file %s at byte %d",filename.c_str(),ftell(f));
	exit(-1);
}

void Engine::parseKeyFrames(FILE *f,std::string filename){
	char s[100];
	if(fscanf(f,"%s",s)!=1)
		goto fmterr;
	std::string transformname(s);

	char ch;
	for(ch=getc(f);(ch==' ')||(ch=='\t')||(ch=='\n')||(ch=='\r')||(ch=='\f');ch=getc(f));
	
	if(ch!='{')
		goto fmterr;
	for(ch=getc(f);(ch==' ')||(ch=='\t')||(ch=='\n')||(ch=='\r')||(ch=='\f');ch=getc(f));
	DynamicVector3Type transl,rot,scale;
	for(;ch!='}';ch=getc(f)){
		ungetc(ch,f);
		char type[100];
		int num;
		bool success=false;
		if(fscanf(f,"%s %d",type,&num)!=2)
			goto fmterr;

		if((strcmp(type,"visibility")!=0)&&(num<4))
			goto fmterr;

		if(strcmp(type,"transl")==0){
			transl=parseKeyFramesTranslate(f,filename,num);
			success=true;
		}
		if(strcmp(type,"rot")==0){
			rot=parseKeyFramesRotate(f,filename,num);
			success=true;
		}
		if(strcmp(type,"scale")==0){
			scale=parseKeyFramesScale(f,filename,num);
			success=true;
		}
		if(strcmp(type,"visibility")==0){
			// to be done
			success=true;
		}
		if(!success)
			goto fmterr;
		for(ch=getc(f);(ch==' ')||(ch=='\t')||(ch=='\n')||(ch=='\r')||(ch=='\f');ch=getc(f));
		ungetc(ch,f);
	}

	transforms.insert(std::make_pair(transformname,scale*rot*transl));
	/*if(transformname.compare("camera")==0)
		camera.setAnimation(camera.getAnimation()*scale*rot*transl);
	
	auto it=lights.find(transformname);
	if(it!=lights.end())
		lights[transformname].setAnimation(lights[transformname].getAnimation()*scale*rot*transl);*/

	return;
fmterr:
	fprintf(stderr,"parse key frames error in file %s at byte %d",filename.c_str(),ftell(f));
	exit(-1);
}

void Engine::parseXform(FILE *f,std::string filename){
	char is_static=fgetc(f);
	if(is_static=='s'){
		double sx,sy,sz,rx,ry,rz,deg,tx,ty,tz;
		if(fscanf(stderr,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&sx,&sy,&sz,&rx,&ry,&rz,&deg,&tx,&ty,&tz)!=10){
			fprintf(stderr,"Error: could not read static transform in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}

		char ch;
		for(ch=getc(f);(ch==' ')||(ch=='\t')||(ch=='\n')||(ch=='\r')||(ch=='\f');ch=getc(f));
		if(ch!='{'){
			fprintf(stderr,"Error: { expected in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}

		Matrix m=translate(tx,ty,tz)*rotate(Vector3(rx,ry,rz),deg)*scale(sx,sy,sz);
		animationStack.push([&m](const Vector3 &_orig,double){
			return m*_orig;
		}*animationStack.top());
	}
	else{
		ungetc(is_static,f);
		char s[100];
		if(fscanf(f,"%s",s)!=1){
			fprintf(stderr,"Error: could not read transform name in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}

		std::string name(s);
		auto it=transforms.find(name);
		if(it==transforms.end()){
			fprintf(stderr,"No transform name %s find in file %s at byte %d",name.c_str(),filename.c_str(),ftell(f));
			exit(-1);
		}

		char ch;
		for(ch=getc(f);(ch==' ')||(ch=='\t')||(ch=='\n')||(ch=='\r')||(ch=='\f');ch=getc(f));
		if(ch!='{'){
			fprintf(stderr,"Error: { expected in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}

		animationStack.push(transforms[name]*animationStack.top());
	}
}

void Engine::parseAmbient(FILE *f,std::string filename){
	double a,b,c;
	if(fscanf(f,"%lf %lf %lf",&a,&b,&c)==3)
		ambient=rgb(a,b,c);
	else{
		fprintf(stderr,"Error: could not parse ambient light (am) in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}
}

void Engine::parseA(FILE *f,std::string filename){
	char is_ambient=getc(f);
	if(is_ambient=='m')
		parseAmbient(f,filename);
	else{
		ungetc(is_ambient,f);
		if(fscanf(f,"%lf %lf %d",&starttime,&endtime,&numframes)!=3){
			fprintf(stderr,"Error: could not parse animations parameters in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}
	}
}

void Engine::parseMesh(FILE *f,std::string filename){
	char str[100];
	double a,b,c;
	std::vector<Vector3> vert,norm;
	std::vector<Vector2> tex;
	Material material(currentMaterial);
	if(fscanf(f,"%s",str)!=1){
		fprintf(stderr,"Error: could not parse mesh (could not find 'vertices') in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}
	if(strcmp(str,"vertices")!=0){
		fprintf(stderr,"Error: could not parse mesh (expected 'vertices') in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}
	int numVerts;
	if(fscanf(f,"%d",&numVerts)!=1){
		fprintf(stderr,"Error: could not parse mesh (expected 'num_vertices') in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}
	for(;numVerts>0;--numVerts){
		if(fscanf(f,"%lf %lf %lf",&a,&b,&c)!=3){
			fprintf(stderr,"Error: could not read vertices of mesh in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}
		vert.push_back(Vector3(a,b,c));
	}

	bool hasNorm=false,hasTex=false;
	fscanf(f,"%s",str);
	if(strcmp(str,"normals")==0){
		hasNorm=true;
		int numNorms;
		if(fscanf(f,"%d",&numNorms)!=1){
			fprintf(stderr,"Error: could not parse mesh (expected 'num_vertices') in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}
		for(;numNorms>0;--numNorms){
			if(fscanf(f,"%lf %lf %lf",&a,&b,&c)!=3){
				fprintf(stderr,"Error: could not read normals of mesh in file %s at byte %d",filename.c_str(),ftell(f));
				exit(-1);
			}
			norm.push_back(Vector3(a,b,c));
		}
		fscanf(f,"%s",str);
	}

	if(strcmp(str,"texturecoords")==0){
		hasTex=true;
		int numTex;
		if(fscanf(f,"%d",&numTex)!=1){
			fprintf(stderr,"Error: could not parse mesh (expected 'num_txts') in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}
		char s[100];
		if(fscanf(f,"%s",s)!=1){
			fprintf(stderr,"Error: could not parse mesh (expected 'texturename') in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}
		FILE *ifile;
		if(ifile=fopen(s,"r"))
			fclose(ifile);
		else{
			fprintf(stderr,"Error: could not open texture file %s in file %s at byte %d",s,filename.c_str(),ftell(f));
			exit(-1);
		}
		material.setTex(std::shared_ptr<ImageTexture>(new ImageTexture(std::string(s))));

		for(;numTex>0;--numTex){
			if(fscanf(f,"%lf %lf",&a,&b)!=2){
				fprintf(stderr,"Error: could not read texturecoord indices of mesh in file %s at byte %d",filename.c_str(),ftell(f));
				exit(-1);
			}
			tex.push_back(Vector2(a,b));
		}
		fscanf(f,"%s",str);
	}

	std::tr1::shared_ptr<Mesh> mesh;
	if((!hasNorm)&&(!hasTex))
		mesh=std::make_shared<Mesh>(Mesh(vert,material));
	if((hasNorm)&&(!hasTex))
		mesh=std::make_shared<Mesh>(Mesh(vert,norm,material));
	if((!hasNorm)&&(hasTex))
		mesh=std::make_shared<Mesh>(Mesh(vert,tex,material));
	if((hasNorm)&&(hasTex))
		mesh=std::make_shared<Mesh>(Mesh(vert,norm,tex,material));

	if(strcmp(str,"triangles")==0){
		int numTri;
		if(fscanf(f,"%d",&numTri)!=1){
			fprintf(stderr,"Error: could not parse mesh (expected 'num_triangles') in file %s at byte %d",filename.c_str(),ftell(f));
			exit(-1);
		}
		for(int p0,p1,p2,n0,n1,n2,t0,t1,t2;numTri>0;--numTri){
			p0=-1,p1=-1,p2=-1,n0=-1,n1=-1,n2=-1,t0=-1,t1=-1,t2=-1;
			if(fscanf(f,"%d %d %d",&p0,&p1,&p2)!=3){
				fprintf(stderr,"Error: could not read vertex indices of meshin file %s at byte %d",filename.c_str(),ftell(f));
				exit(-1);
			}

			if(hasNorm){
				if(fscanf(f,"%d %d %d",&n0,&n1,&n2)!=3){
					fprintf(stderr,"Error: could not read set of normal indices of meshin file %s at byte %d",filename.c_str(),ftell(f));
					exit(-1);
				}
			}

			if(hasTex){
				if(fscanf(f,"%d %d %d",&t0,&t1,&t2)!=3){
					fprintf(stderr,"Error: could not read texturecoord indices of meshin file %s at byte %d",filename.c_str(),ftell(f));
					exit(-1);
				}
			}

			scene.push_back(std::make_shared<MeshTriangle>(MeshTriangle(p0,p1,p2,n0,n1,n2,t0,t1,t2,mesh,animationStack.top())));
		}
	}
	else{
		fprintf(stderr,"Error: expected 'triangles' in mesh in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}
}

bool Engine::parseFilePass1(FILE *f,std::string filename){
	char ch;
	for(;ch=getc(f)!=EOF;){
		switch(ch){
		case 'i':
		parseInclude(f,filename,1);
		break;
		case 'k':	// key frames for transform (or the camera)
		parseKeyFrames(f,filename);
		break;
		default:
		continue;
		}
	}
	return true;
}

bool Engine::parseFilePass2(FILE *f,std::string filename){
	char ch,ch1;
	for(;ch=getc(f)!=EOF;){
		switch(ch){
		case ' ':	// white space
		case '\t':
		case '\n':
		case '\f':
		case '\r':
		continue;
		case '#':	// comment
		case '%':	// comment;
		parseComment(f,filename);
		break;
		case 'v':
		parseViewpoint(f,filename);
		break;
		case 'l':	// light source;
		parseLight(f,filename);
		break;
		case 'b':	// background color
		parseBackground(f,filename);
		break;
		case 'f':	// fill material
		parseFill(f,filename);
		break;
		case 'c':	// cylinder or cone
		parseCone(f,filename);
		break;
		case 's':	// sphere
		parseSphere(f,filename);
		break;
		case 'p':	// polygon
		parsePoly(f,filename);
		break;
		case 'i':	// include another file
		parseInclude(f,filename,2);
		break;
		case 'd':	// detail level of file (used to exclude objects from rendering)
		parseDetailLevel(f,filename);
		break;
		case 't':	// textured triangle, or texture tripatch, or animated triangle
		parseTriangle(f,filename);
		break;
		case 'k':
		for(ch1=getc(f);ch1!='}';ch1=getc(f));
		break;
		case 'x':	// transform
		parseXform(f,filename);
		break;
		case '}':
		animationStack.pop();
		break;
		case 'a':	// animation parameters
		parseA(f,filename);
		break;
		case 'm':	// triangle mesh
		parseMesh(f,filename);
		break;
		default:
		fprintf(stderr,"unknown NFF primitive code: %c in file %s at byte %d",ch,filename.c_str(),ftell(f));
		exit(-1);
		}
	}
	return true;
}

bool Engine::parseAFF(std::string filename){
	FILE *in=fopen(filename.c_str(),"r");
	parseFilePass1(in,filename);
	fclose(in);
	in=fopen(filename.c_str(),"r");
	parseFilePass2(in,filename);
	fclose(in);
}