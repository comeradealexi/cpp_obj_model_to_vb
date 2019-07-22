#pragma once
#include "ObjParser.h"
#include "SharedTypes.h"

struct ObjExporter
{
	using IndexType = uint32_t;
	ObjExporter(const ObjParser& obj);
	ObjExporter() { }

	struct VB
	{
		Float3 m_position;
		Float3 m_normal;
		Float2 m_uv;
	};

	static constexpr uint32_t INVALID_MATERIAL_INDEX = ~0u;
	struct Model
	{

		IndexType m_indexOffset = 0;
		IndexType m_indexCount = 0;
		uint32_t m_materialIndex = INVALID_MATERIAL_INDEX;
	};

	std::vector<VB>			m_vb;
	std::vector<IndexType>	m_indices;
	std::vector<Model>		m_models;
	std::vector<Material>	m_materials;
};