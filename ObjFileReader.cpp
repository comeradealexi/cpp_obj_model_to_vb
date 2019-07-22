#include "ObjFileReader.h"
#include "ObjFileWriter.h"

ObjFileReader::ObjFileReader(const std::string& strInFile)
{
	std::ifstream inFile(strInFile, std::ios::binary | std::ios::in);
	if (inFile.good())
	{
		ObjFileWriter::FileHeader fileHeader;
		inFile.read((char*)&fileHeader, sizeof(fileHeader));
		m_loadedModel.m_vb.resize(fileHeader.m_vertexCount);
		m_loadedModel.m_indices.resize(fileHeader.m_indexCount);
		m_loadedModel.m_models.resize(fileHeader.m_modelCount);

		inFile.read((char*)m_loadedModel.m_vb.data(), m_loadedModel.m_vb.size() * sizeof(ObjExporter::VB));
		inFile.read((char*)m_loadedModel.m_indices.data(), m_loadedModel.m_indices.size() * sizeof(ObjExporter::IndexType));
		inFile.read((char*)m_loadedModel.m_models.data(), m_loadedModel.m_models.size() * sizeof(ObjExporter::Model));

		if (inFile.eof() == false)
		{
			uint32_t materialsCount;
			inFile >> materialsCount;
			m_loadedModel.m_materials.resize(materialsCount);
			for (auto& material : m_loadedModel.m_materials)
			{
				inFile.read((char*)&material.header, sizeof(material.header));

				std::getline(inFile, material.materialName, '\0');
				std::getline(inFile, material.textureAmbient, '\0');
				std::getline(inFile, material.textureDiffuse, '\0');
				std::getline(inFile, material.textureSpecular, '\0');
				std::getline(inFile, material.textureHighlight, '\0');
				std::getline(inFile, material.textureBump, '\0');
				std::getline(inFile, material.textureDisplacement, '\0');
				std::getline(inFile, material.textureStencil, '\0');
				std::getline(inFile, material.textureAlpha, '\0');
			}
		}
	}
}
