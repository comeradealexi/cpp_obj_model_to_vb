#pragma once
#include "ObjExporter.h"

struct ObjFileReader
{
	ObjFileReader(const std::string& strInFile);
	ObjExporter& GetModel() { return m_loadedModel; }
	ObjExporter m_loadedModel;
};