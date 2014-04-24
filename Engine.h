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
	Image render(double time);
private:
	// render a single primitive ray using depth first search
	void raytrace(const Ray &r,double time,int renderDepth);

	void parseComment(FILE *f,std::string filename);
	void parseViewpoint(FILE *f,std::string filename);
	void parseLight(FILE *f,std::string filename);
	void parseBackground(FILE *f,std::string filename);
	void parseFill(FILE *f,std::string filename);
	void parseCone(FILE *f,std::string filename);
	void parseSphere(FILE *f,std::string filename);
	void parsePoly(FILE *f,std::string filename);
	void parseInclude(FILE *f,std::string filename);
	void parseDetailLevel(FILE *f,std::string filename);
	void parseAnimatedTriangle(FILE *f,std::string filename);
	void parseTriangle(FILE *f,std::string filename);
	bool parseFile(FILE *f,std::string filename);

	int maxRenderDepth;
	std::vector<std::tr1::shared_ptr<Shape>> scene;
	Camera camera;
	std::unordered_map<std::string,DynamicVector3Type> transforms;
	std::stack<DynamicVector3Type> animationStack;	// should be initialized with a ZeroAnimation functor in it
	std::unordered_map<std::string,Light> lights;
	int picwidth,picheight;
	rgb background;
	Material currentMaterial;	// initialized with init material
	int currentDetailLevel;
};
#endif // _ENGINE_H_