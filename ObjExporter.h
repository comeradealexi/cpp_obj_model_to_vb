#pragma once
#include "ObjParser.h"

struct ObjExporter
{
	using IndexType = uint32_t;
	ObjExporter(const ObjParser& obj);

	struct VB
	{
		ObjParser::Float3 m_position;
		ObjParser::Float3 m_normal;
		ObjParser::Float2 m_uv;
	};

	struct Model
	{

		IndexType m_indexOffset = 0;
		IndexType m_indexCount = 0;
	};

	std::vector<VB>			m_vb;
	std::vector<IndexType>	m_indices;
	std::vector<Model>		m_models;
};