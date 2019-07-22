#pragma once
#include "ObjExporter.h"

struct ObjFileWriter
{
	struct ModelInfo
	{
		uint32_t m_indexOffset = 0;
		uint32_t m_indexCount = 0;
	};
	struct FileHeader
	{
		const char m_header[8] = { 'a','h','o','b','j','f','m','t' }; 
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;
		uint32_t m_modelCount = 0;
	};
	static_assert(sizeof(FileHeader::m_header) == sizeof(uint64_t), "Must be 8 bytes");
	static_assert(sizeof(FileHeader) == 20, "Should be 20?");

	ObjFileWriter(const ObjExporter& obj, const std::string& strOutFileName);
	void WriteMaterialFile(std::ofstream& outFile, const Material& material);

	bool m_bFileWritten = false;
};