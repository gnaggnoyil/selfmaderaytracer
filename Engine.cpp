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

#define EPSILON (1e-6)

class ZeroAnimation:public DynamicVector3Type{
public:
	Vector3 operator()(const Vector3 &orig,double){
		return Vector3(orig);
	}
};

DynamicVector3Type operator*(DynamicVector3Type func1,DynamicVector3Type func2){
	return [&func1,&func2](const Vector3 &orig,double time){
		func2(func1(orig,time),time);
	};
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
	camera=Camera(orig,lookat,h*picwidth/picheight,h,hither,animationStack.top());
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

	char ch=getc(f);
	if(ch=='a'){
		for(;ch=getc(f)==' ';);
		for(name.push_back(ch);ch=getc(f)!=' ';name.push_back(ch));
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

	lights.insert(std::make_pair(name,Light(col,pos,animationStack.top())));
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

void Engine::parseInclude(FILE *f,std::string filename){
	int level;
	if(fscanf(f,"%d",&level)!=1){
		fprintf(stderr,"Error: could not parse include in file %s at byte %d",filename.c_str(),ftell(f));
		exit(-1);
	}

	std::string ifilename;
	char ch;
	for(;ch=getc(f)==' ';);
	for(ifilename.push_back(ch);ch=getc(f)!=' ';ifilename.push_back(ch));
	if(level<=currentDetailLevel){
		FILE *ifile;
		if(ifile=fopen(ifilename.c_str(),"r")){
			parseFile(ifile,ifilename);
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

class TimeInterpolate:public DynamicVector3Type{
public:
	TimeInterpolate(const std::vector<Vector3> &_v,const std::vector<double> &_t):
		v(_v),t(_t){}
	Vector3 operator()(const Vector3 &_orig,double _time){
		if(_time<t[0])
			return Vector3(_orig);
		if(_time>t[t.size()])
			return v[v.size()];
		int i=search(_time);
		if(fabs(t[i]-_time)<EPSILON)
			return v[i];
		double c=(t[i]-_time)/(t[i]-t[i-1]);
		return v[i-1]*c+v[i]*(1-c);
	}
private:
	int search(double o){
		int st=0,en=t.size()-1,mid=0;
		for(;st<en;){
			mid=(st+en)>>1;
			if(fabs(t[mid]-o)<EPSILON)
				return mid;
			if(t[mid]>o)
				en=mid;
			else
				st=mid+1;
		}
		return en;
	}
	std::vector<Vector3> v;
	std::vector<double> t;
};

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
			for(;ch1=getc(f)==' ';);
			for(texturename.push_back(ch1);ch1=getc(f)!=' ';texturename.push_back(ch1));
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
			for(;ch1=getc(f)==' ';);
			for(texturename.push_back(ch1);ch1=getc(f)!=' ';texturename.push_back(ch1));
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

bool Engine::parseFile(FILE *f,std::string filename){
	int ch;
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
		parseInclude(f,filename);
		break;
		case 'd':	// detail level of file (used to exclude objects from rendering)
		parseDetailLevel(f,filename);
		break;
		case 't':	// textured triangle, or texture tripatch, or animated triangle
		parseTriangle(f,filename);
		break;
		}
	}
}

bool Engine::parseAFF(std::string filename){
	FILE *in=fopen(filename.c_str(),"r");

}