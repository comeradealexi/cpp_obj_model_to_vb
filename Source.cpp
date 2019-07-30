#include "ObjParser.h"
#include "ObjExporter.h"
#include "ObjFileWriter.h"
#include "ObjFileReader.h"
#include <iostream>
#include <sstream>

void ConvertTextures(const ObjExporter& objExport, const std::string& directoryIn, std::string directoryOut)
{
	std::cout << "Converting Textures...\n";

	if (directoryOut[directoryOut.length() - 1] == '\\')
		directoryOut = directoryOut.substr(0, directoryOut.length() - 1);

	for (auto& m : objExport.m_materials)
	{
		const std::string* pTextures[8];
		pTextures[0] = &m.textureAmbient;
		pTextures[1] = &m.textureDiffuse;
		pTextures[2] = &m.textureSpecular;
		pTextures[3] = &m.textureHighlight;
		pTextures[4] = &m.textureBump;
		pTextures[5] = &m.textureDisplacement;
		pTextures[6] = &m.textureStencil;
		pTextures[7] = &m.textureAlpha;

		for (auto t : pTextures)
		{
			if (t->length() > 1)
			{
				const std::string texturePath = directoryIn + (*t);
				std::stringstream cmdLine;
				cmdLine << "%AHCODEDUMP_EXTERNAL%\\DirectXTex-master\\bin\\texconv.exe -o ";
				cmdLine << "\"" << directoryOut << "\" ";
				cmdLine << texturePath;
				std::cout << "Running Command Line: " << cmdLine.str() << "\n";
				std::system(cmdLine.str().c_str());
			}
		}
	}
}

std::string GetDir(std::string objFileIn)
{
	for (auto& c : objFileIn)
	{
		if (c == '/')
			c = '\\';
	}

	size_t lastSlash = objFileIn.find_last_of('\\');
	if (lastSlash == std::string::npos)
		return "";

	return objFileIn.substr(0, lastSlash + 1);
}

int main(int argc, const char *argv[])
{
	if (argc < 3)
	{
		std::cout << "Invalid arg count, expects two: 1 = in file, 2 = out file" << std::endl; 
		return -1;
	}

	const std::string objFileIn(argv[1]);
	const std::string objFileOut(argv[2]);
	const std::string objFileInDirectory = GetDir(argv[1]);
	const std::string objFileOutDirectory = GetDir(argv[2]);

	std::cout << "In File: " << argv[1] << std::endl;
	std::cout << "Out File: " << argv[2] << std::endl;

	
	ObjExporter objExport = ObjExporter(ObjParser(objFileIn));

	std::cout << "Vertex Count: " << objExport.m_vb.size() << std::endl;
	std::cout << "Index Count: " << objExport.m_indices.size() << std::endl;
	std::cout << "Model Count: " << objExport.m_models.size() << std::endl;

	if (objExport.m_vb.size() > INT32_MAX) std::cout << "VB Count exceeeds maximum for INT32_MAX of " << INT32_MAX << std::endl;
	if (objExport.m_indices.size() > INT32_MAX) std::cout << "IB Count exceeeds maximum for INT32_MAX of " << INT32_MAX << std::endl;
	if (objExport.m_models.size() > INT32_MAX) std::cout << "MODEL Count exceeeds maximum for INT32_MAX of " << INT32_MAX << std::endl;

	if (objExport.m_vb.size() == 0)
	{
		std::cout << "No verts. Not writing output file.\n";
		return -1;
	}

	ObjFileWriter objFileWriter(objExport, objFileOut);
	std::cout << "Output file has " << (objFileWriter.m_bFileWritten ? "been written." : "NOT been written.") << std::endl;

	ObjFileReader objFileReader(objFileOut);


	ConvertTextures(objExport, objFileInDirectory, objFileOutDirectory);

	return 0;
}