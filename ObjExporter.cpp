#include "ObjExporter.h"
#include <map>

ObjExporter::ObjExporter(const ObjParser& obj)
{
	std::map<ObjParser::Face, IndexType> faceMap;

	uint32_t indexBufferStart = 0;
	uint32_t indexBufferCount = 0;
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
			m_models.push_back(newModel);
		}
	}
	
}
