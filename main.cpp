#include "Engine.h"
#include "Image.h"
#include <fstream>

int main(int argc,char **argv){
	Engine engine;
	engine.parseAFF("H:\\useful things\\RealisticRayTracing\\BART\\museum\\museum3.aff");
	fprintf(stderr,"parse complete\n");
	Image res=engine.render(30);
	std::ofstream fout("test.ppm",std::ios::out | std::ios::binary);
	res.writePPM(fout);
	fout.close();
	//fprintf(stderr,"%lf\n",res.getRGB(399,599).getR());
	fgetc(stdin);
}