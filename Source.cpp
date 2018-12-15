#include "ObjParser.h"
#include "ObjExporter.h"
#include "ObjFileWriter.h"
#include "ObjFileReader.h"
#include <iostream>

int main(int argc, const char *argv[])
{
	if (argc < 3)
	{
		std::cout << "Invalid arg count, expects two: 1 = in file, 2 = out file" << std::endl; 
		return -1;
	}

	const std::string objFileIn(argv[1]);
	const std::string objFileOut(argv[2]);

	std::cout << "In File: " << argv[1] << std::endl;
	std::cout << "Out File: " << argv[2] << std::endl;

	
	ObjExporter objExport = ObjExporter(ObjParser(objFileIn));

	std::cout << "Vertex Count: " << objExport.m_vb.size() << std::endl;
	std::cout << "Index Count: " << objExport.m_indices.size() << std::endl;
	std::cout << "Model Count: " << objExport.m_models.size() << std::endl;

	if (objExport.m_vb.size() > INT32_MAX) std::cout << "VB Count exceeeds maximum for INT32_MAX of " << INT32_MAX << std::endl;
	if (objExport.m_indices.size() > INT32_MAX) std::cout << "IB Count exceeeds maximum for INT32_MAX of " << INT32_MAX << std::endl;
	if (objExport.m_models.size() > INT32_MAX) std::cout << "MODEL Count exceeeds maximum for INT32_MAX of " << INT32_MAX << std::endl;

	ObjFileWriter objFileWriter(objExport, objFileOut);
	std::cout << "Output file has " << (objFileWriter.m_bFileWritten ? "been written." : "NOT been written.") << std::endl;

	ObjFileReader objFileReader(objFileOut);

	return 0;
}