#include "ObjFileWriter.h"
#include <fstream>

ObjFileWriter::ObjFileWriter(const ObjExporter& obj, const std::string& strOutFileName)
{
	std::ofstream outFile(strOutFileName, std::ios::out | std::ios::binary);
	if (outFile.good())
	{
		FileHeader header;
		header.m_vertexCount = static_cast<uint32_t>(obj.m_vb.size());
		header.m_indexCount = static_cast<uint32_t>(obj.m_indices.size());
		header.m_modelCount = static_cast<uint32_t>(obj.m_models.size());

		outFile.write((char*)&header, sizeof(header));
		outFile.write((char*)obj.m_vb.data(), obj.m_vb.size() * sizeof(ObjExporter::VB));
		outFile.write((char*)obj.m_indices.data(), obj.m_indices.size() * sizeof(ObjExporter::IndexType));
		outFile.write((char*)obj.m_models.data(), obj.m_models.size() * sizeof(ObjExporter::Model));

		outFile << static_cast<uint32_t>(obj.m_materials.size());
		for (const auto& currentMaterial : obj.m_materials)
		{
			WriteMaterialFile(outFile, currentMaterial);
		}

		m_bFileWritten = true;
	}
}

void ObjFileWriter::WriteMaterialFile(std::ofstream& outFile, const Material& material)
{
	outFile.write((char*)&material.header, sizeof(material.header));
	outFile << material.materialName << '\0';
	outFile << material.textureAmbient << '\0';
	outFile << material.textureDiffuse << '\0';
	outFile << material.textureSpecular << '\0';
	outFile << material.textureHighlight << '\0';
	outFile << material.textureBump << '\0';
	outFile << material.textureDisplacement << '\0';
	outFile << material.textureStencil << '\0';
	outFile << material.textureAlpha << '\0';
}