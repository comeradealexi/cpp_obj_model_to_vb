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
		m_bFileWritten = true;
	}
}