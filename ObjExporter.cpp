#include "ObjExporter.h"
#include <map>

ObjExporter::ObjExporter(const ObjParser& obj)
{
	std::map<ObjParser::Face, IndexType> faceMap;

	uint32_t indexBufferStart = 0;
	uint32_t indexBufferCount = 0;

	//Copy the materials
	m_materials = obj.m_materials;

	for (auto& g : obj.m_groups)
	{
		for (auto& face : g.faces)
		{
			auto it = faceMap.find(face);	
			if (it != faceMap.end())
			{
				m_indices.push_back(it->second);
			}
			else
			{
				VB vb;
				vb.m_position = obj.m_positions[face.positionIndex];
				vb.m_normal = obj.m_normals[face.normalIndex];
				vb.m_uv = obj.m_uvs[face.uvIndex];
				m_vb.push_back(vb);

				IndexType newIndex = static_cast<IndexType>(m_vb.size() - 1);
				m_indices.push_back(newIndex);
				faceMap.insert(std::make_pair(face, newIndex));
			}
			indexBufferCount++;
		}
		if (indexBufferCount > 0)
		{
			Model newModel;
			newModel.m_indexCount = indexBufferCount;
			newModel.m_indexOffset = indexBufferStart;
			indexBufferStart = indexBufferCount;
			indexBufferCount = 0;

			uint32_t uMaterialIndex = 0;
			for (const auto& material : m_materials)
			{
				if (material.materialName == g.m_subMaterial)
				{
					newModel.m_materialIndex = uMaterialIndex;
					break;
				}
				uMaterialIndex++;
			}

			//Find the material
			m_models.push_back(newModel);
		}
	}
}