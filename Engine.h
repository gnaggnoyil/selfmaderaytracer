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
	Image render(int width,int height,double time);
private:
	// render a single primitive ray using depth first search
	void raytrace(const Ray &r,double time,int renderDepth);

	int maxRenderDepth;
	std::vector<std::tr1::shared_ptr<Shape>> scene;
	Camera camera;
	std::unordered_map<std::string,DynamicVector3Type> transforms;
	std::unordered_map<std::string,Light> lights;
};
#endif // _ENGINE_H_