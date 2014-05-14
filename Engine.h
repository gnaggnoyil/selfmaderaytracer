#ifndef _ENGINE_H_
#define _ENGINE_H_ 1

#include <string>
#include "Ray.h"
#include <vector>
#include "Shape.h"
#include <memory>
#include "Camera.h"
#include "Image.h"
#include <unordered_map>
#include "Common.h"
#include "Light.h"
#include <stack>
#include "rgb.h"
#include "Material.h"
#include <stdio.h>

/**
 * this class defines a engine that packs the needed operations for render an image
 */
class Engine{
public:
	// constructor
	Engine(int _maxRenderDepth=6);
	// returns true if parse succeed, otherwise false
	bool parseAFF(std::string filename);
	// renders a single image at a certain time and returns that rendered image
	Image render(int frameindex);
private:
	// render a single primitive ray using depth first search
	void raytrace(const Ray &r,double time,rgb &result,double rIndex,double &dist,int renderDepth);

	// parser methods
	void parseComment(FILE *f,std::string filename);
	void parseViewpoint(FILE *f,std::string filename);
	void parseViewpointPass1(FILE *f,std::string filename);
	void parseLight(FILE *f,std::string filename);
	void parseLightPass1(FILE *f,std::string filename);
	void parseBackground(FILE *f,std::string filename);
	void parseBackgroundPass1(FILE *f,std::string filename);
	void parseFill(FILE *f,std::string filename);
	void parseFillPass1(FILE *f,std::string filename);
	void parseCone(FILE *f,std::string filename);
	void parseConePass1(FILE *f,std::string filename);
	void parseSphere(FILE *f,std::string filename);
	void parseSpherePass1(FILE *f,std::string filename);
	void parsePoly(FILE *f,std::string filename);
	void parsePolyPass1(FILE *f,std::string filename);
	void parseInclude(FILE *f,std::string filename,int pass);
	void parseDetailLevel(FILE *f,std::string filename);
	void parseAnimatedTriangle(FILE *f,std::string filename);
	void parseAnimatedTrianglePass1(FILE *f,std::string filename);
	void parseTriangle(FILE *f,std::string filename);
	void parseTrianglePass1(FILE *f,std::string filename);
	DynamicVector3Type parseKeyFramesTranslate(FILE *f,std::string filename,int num);
	DynamicVector3Type parseKeyFramesRotate(FILE *f,std::string filename,int num);
	DynamicVector3Type parseKeyFramesScale(FILE *f,std::string filename,int num);
	void parseKeyFrames(FILE *f,std::string filename);
	void parseXform(FILE *f,std::string filename);
	void parseXformPass1(FILE *f,std::string filename);
	void parseAmbient(FILE *f,std::string filename);
	void parseAmbientPass1(FILE *f,std::string filename);
	void parseA(FILE *f,std::string filename);
	void parseAPass1(FILE *f,std::string filename);
	void parseMesh(FILE *f,std::string filename);
	void parseMeshPass1(FILE *f,std::string filename);
	bool parseFilePass1(FILE *f,std::string filename);	// pass 1 to get key frame animation names
	bool parseFilePass2(FILE *f,std::string filename);

	int maxRenderDepth;
	std::vector<std::tr1::shared_ptr<Shape>> scene;
	std::tr1::shared_ptr<Camera> camera;
	std::unordered_map<std::string,DynamicVector3Type> transforms;
	std::stack<DynamicVector3Type> animationStack;	// should be initialized with a ZeroAnimation functor in it
	std::unordered_map<std::string,Light> lights;
	int picwidth,picheight;
	rgb background,ambient;
	Material currentMaterial;	// initialized with init material
	int currentDetailLevel;
	double starttime,endtime;
	int numframes;
	std::string rootpath;
};
#endif // _ENGINE_H_