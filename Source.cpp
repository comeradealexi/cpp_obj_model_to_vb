#include "ObjParser.h"

int main()
{
	//std::string objFile("F:\\comeradealexi\\Projects\\CrossPlatformGFX\\SimpleConsoleProject\\RawDataFiles\\mori_knob\\testObj.obj");
	std::string objFile("C:\\Users\\hough\\Downloads\\mitsuba\\mitsuba.obj");
	//std::string objFile("C:\\Users\\hough\\Downloads\\Skull.obj");

	ObjParser objParser(objFile);
	auto vb = objParser.GenerateExport();
}