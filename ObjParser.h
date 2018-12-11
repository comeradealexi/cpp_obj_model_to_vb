#pragma once
#include <fstream>
#include <array>
#include <vector>
#include <utility>

class ObjParser
{
	friend struct ObjExporter;
public:
	using Float4 = std::array<float, 4>;
	using Float3 = std::array<float, 3>;
	using Float2 = std::array<float, 2>;

public:
	ObjParser(const std::string& strObjFile);
	~ObjParser();

public:
	struct Face
	{
		uint32_t positionIndex = 0;
		uint32_t normalIndex = 0;
		uint32_t uvIndex = 0;

		inline bool operator==(const Face &f)  const
		{
			return f.positionIndex == positionIndex && f.normalIndex == normalIndex && f.uvIndex == uvIndex;
		}

		inline bool operator<(const Face &f)  const 
		{
			return std::tie(positionIndex, normalIndex, uvIndex) < std::tie(f.positionIndex, f.normalIndex, f.uvIndex);
		}

		void Set(uint32_t value, int index)
		{
			if (index == 0)
			{
				positionIndex = value;
			}
			else if (index == 1)
			{
				uvIndex = value;
			}
			else if (index == 2)
			{
				normalIndex = value;
			}
		}
	};

	struct Group
	{
		std::string name;
		std::vector<Face> faces;
		std::string m_subMaterial;
	};

	//http://paulbourke.net/dataformats/mtl/
	struct Material
	{
		Material(std::string strName) : name(strName) { }
		std::string name;
		Float3		colourAmbient = { 1.0f,1.0f,1.0f };
		Float3		colourDiffuse = { 1.0f,1.0f,1.0f };
		Float3		colourSpecular = { 1.0f,1.0f,1.0f };
		float		weightSpecular = 1;
		float		alpha = 1.0f;
		uint32_t	illuminationType = 0;

		std::string textureAmbient;
		std::string textureDiffuse;
		std::string textureSpecular;
		std::string textureHighlight;
		std::string textureBump;
		std::string textureDisplacement;
		std::string textureStencil;
		std::string textureAlpha;
	};

private:
	bool IsEndLineCharacter(char c)
	{
		return (c == '\n' || c == '\r' || c == '\0');
	}
	bool IsNotEndLineCharacter(char c)
	{
		return (c != '\n' || c != '\r' || c != '\0');
	}
	void Parse(std::fstream& fStream);
	void LoadMaterialFile();
	void ProcessGroup(const std::string& line);
	void ProcessMaterialUsage(const std::string& line);
	void ProcessMaterialSource(const std::string& line);
	void ProcessVert(const std::string& line);
	void ProcessNormal(const std::string& line);
	void ProcessTextureCoordinate(const std::string& line);
	void ProcessFace(const std::string& line);
	std::string CreateMaterialFilePath(const std::string& str);

	void StripInitialDeadSpace(std::string& str);
	template<size_t ARRAY_SIZE>
	void ProcessFloats(std::array<float, ARRAY_SIZE>& floatArray, const std::string& line);


	std::string m_strObjFile;
	std::vector<Float3> m_positions;
	std::vector<Float3> m_normals;
	std::vector<Float2> m_uvs;
	std::vector<Group>	m_groups;
	std::string m_materialFile;
	std::vector<Material> m_materials;
};