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
	}
}
