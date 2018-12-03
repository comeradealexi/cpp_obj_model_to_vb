#include "ObjExporter.h"
#include <map>

ObjExporter::ObjExporter(const ObjParser& obj)
{
	using IndexType = uint32_t;
	std::map<ObjParser::Face, IndexType> faceMap;

	for (auto& g : obj.m_groups)
	{
		std::vector<IndexType> indexBuffer;
		for (auto& face : g.faces)
		{
			auto it = faceMap.find(face);
			if (it != faceMap.end())
			{
				indexBuffer.push_back(it->second);
			}
			else
			{
				VB vb;
				vb.m_position = obj.m_positions[face.positionIndex];
				vb.m_normal = obj.m_normals[face.normalIndex];
				vb.m_uv = obj.m_uvs[face.uvIndex];
				m_vb.push_back(vb);

				IndexType newIndex = static_cast<IndexType>(m_vb.size() - 1);
				indexBuffer.push_back(newIndex);
				faceMap.insert(std::make_pair(face, newIndex));
			}
		}
		if (indexBuffer.size() > 0)
		{

		}
	}
}
