#include "ObjParser.h"
#include <iostream>
#include <string>
#include <functional>
#include <map>

class LineParser
{
public:
	using Func = std::function<void(ObjParser*, const std::string&)>;
	LineParser(std::string token, Func func) : m_func(func), m_token(token + " ")
	{

	}

	virtual bool Parse(ObjParser* pObjParser, std::string& line)
	{
		if (DoesMatchToken(line))
		{
			m_func(pObjParser, line);
			return true;
		}
		return false;
	}

	bool DoesMatchToken(std::string& line)
	{
		if (line.length() < m_token.length())
			return false;

		for (size_t i = 0; i < m_token.length(); i++)
		{
			if (m_token[i] != line[i])
				return false;
		}

		//Fix the string

		size_t offset = m_token.length();
		while (line[offset] == ' ' || line[offset] == '\t')
			offset++;

		line = line.substr(offset);

		return true;
	}

private:
	Func m_func;
	const std::string m_token;
};

ObjParser::ObjParser(const std::string& strObjFile) : m_strObjFile(strObjFile)
{
	m_positions.push_back( {0.0f,0.0f,0.0f});
	m_normals.push_back({ 0.0f, 1.0f, 0.0f });
	m_uvs.push_back({ 0.0f, 0.0f });

	std::fstream fStream(m_strObjFile);
	Parse(fStream);
}


ObjParser::~ObjParser()
{
}

template<size_t ARRAY_SIZE>
void ObjParser::ProcessFloats(std::array<float, ARRAY_SIZE>& floatArray, const std::string& line)
{
	const std::size_t strLength = line.length();
	std::size_t nextPos = 0;
	size_t index = 0;

	while (nextPos < strLength && (line[nextPos] != '\n' || line[nextPos] != '\r' || line[nextPos] != '\0'))
	{
		if (line[nextPos] == ' ')
		{
			nextPos++;
			continue;
		}
		size_t shiftPosition = 0;
		floatArray[index] = std::stof(&line.at(nextPos), &shiftPosition);
		nextPos += shiftPosition;
		index++;
		if (index >= floatArray.size())
			break;
	}
}

void ObjParser::Parse(std::fstream& fStream)
{
	std::vector<LineParser> parserList;
	parserList.emplace_back("v", &ObjParser::ProcessVert);
	parserList.emplace_back("vn", &ObjParser::ProcessNormal);
	parserList.emplace_back("vt", &ObjParser::ProcessTextureCoordinate);
	parserList.emplace_back("f", &ObjParser::ProcessFace);
	parserList.emplace_back("g", &ObjParser::ProcessGroup);
	parserList.emplace_back("usemtl", &ObjParser::ProcessMaterialUsage);
	parserList.emplace_back("mtllib", &ObjParser::ProcessMaterialSource);

	m_groups.emplace_back();

	std::string line;
	while (std::getline(fStream, line))
	{
		for (auto& parser : parserList)
		{
			if (parser.Parse(this, line))
				break;
		}
	}

	LoadMaterialFile();
}

std::string ObjParser::CreateMaterialFilePath(const std::string& str)
{
	std::string strObjPath = m_strObjFile;
	for (auto& c : strObjPath)
		if (c == '\\') c = '/';

	size_t lastPos = strObjPath.find_last_of('/');
	if (lastPos == std::string::npos)
		return m_materialFile;

	std::string strOut = strObjPath.substr(0, lastPos + 1);
	strOut += m_materialFile;
	return strOut;
}

void ObjParser::StripInitialDeadSpace(std::string& str)
{
	size_t uiOffset = 0;

	for (auto c : str)
	{
		if (c == '\t' || c == ' ')
			uiOffset++;
		else
			break;
	}
	str = str.substr(uiOffset, str.size() - uiOffset);
}

void ObjParser::LoadMaterialFile()
{
	if (m_materialFile.size() > 0)
	{
		std::fstream matFileHandle(CreateMaterialFilePath(m_materialFile));
		if (matFileHandle.good())
		{
			std::vector<LineParser> parserList;
			parserList.emplace_back("newmtl",	[&](ObjParser* pThis, const std::string& str) { m_materials.push_back(str); });
			parserList.emplace_back("Ka",		[&](ObjParser* pThis, const std::string& str) { ProcessFloats(m_materials.back().colourAmbient, str); });
			parserList.emplace_back("Kd",		[&](ObjParser* pThis, const std::string& str) { ProcessFloats(m_materials.back().colourDiffuse, str); });
			parserList.emplace_back("Ks",		[&](ObjParser* pThis, const std::string& str) { ProcessFloats(m_materials.back().colourSpecular, str); });
			parserList.emplace_back("Ns",		[&](ObjParser* pThis, const std::string& str) { m_materials.back().weightSpecular = std::stof(str); });
			parserList.emplace_back("illum",	[&](ObjParser* pThis, const std::string& str) { m_materials.back().illuminationType = std::stoul(str); });
			parserList.emplace_back("map_Ka",	[&](ObjParser* pThis, const std::string& str) { m_materials.back().textureAmbient = str; });
			parserList.emplace_back("map_Kd",	[&](ObjParser* pThis, const std::string& str) { m_materials.back().textureDiffuse = str; });
			parserList.emplace_back("map_bump", [&](ObjParser* pThis, const std::string& str) { m_materials.back().textureBump = str; });
			parserList.emplace_back("bump",		[&](ObjParser* pThis, const std::string& str) { m_materials.back().textureBump = str; });
			parserList.emplace_back("map_Ks",	[&](ObjParser* pThis, const std::string& str) { m_materials.back().textureSpecular = str; });
			parserList.emplace_back("map_Ns",	[&](ObjParser* pThis, const std::string& str) { m_materials.back().textureHighlight = str; });
			parserList.emplace_back("map_d",	[&](ObjParser* pThis, const std::string& str) { m_materials.back().textureAlpha = str; });
			parserList.emplace_back("disp",		[&](ObjParser* pThis, const std::string& str) { m_materials.back().textureDisplacement = str; });
			parserList.emplace_back("d",		[&](ObjParser* pThis, const std::string& str) { m_materials.back().alpha = std::stof(str); });

			std::string line;
			while (std::getline(matFileHandle, line))
			{
				StripInitialDeadSpace(line);
				for (auto& parser : parserList)
				{
					if (parser.Parse(this, line))
						break;
				}
			}
		}
	}
}

void ObjParser::ProcessGroup(const std::string& line)
{
	m_groups.emplace_back();
	m_groups.back().name = line;
}

void ObjParser::ProcessMaterialUsage(const std::string& line)
{
	m_groups.back().m_subMaterial = line;
}

void ObjParser::ProcessMaterialSource(const std::string& line)
{
	m_materialFile = line;
}

void ObjParser::ProcessVert(const std::string& line)
{
	Float3 vPosition = { 0.0f,0.0f,0.0f };
	ProcessFloats(vPosition, line);
	m_positions.push_back(vPosition);
}

void ObjParser::ProcessNormal(const std::string& line)
{
	Float3 vNormal = { 0.0f, 1.0f, 0.0f };
	ProcessFloats(vNormal, line);
	m_normals.push_back(vNormal);
}

void ObjParser::ProcessTextureCoordinate(const std::string& line)
{
	Float2 vTexCoord = { 0.0f,  0.0f };
	ProcessFloats(vTexCoord, line);
	m_uvs.push_back(vTexCoord);
}

void ObjParser::ProcessFace(const std::string& line)
{
	const std::size_t strLength = line.length();

	Face currentFace;
	std::vector<Face> newFaces;
	int currentFaceType = 0;
	std::size_t nextPos = 0;

	while (nextPos < strLength && (IsNotEndLineCharacter(line[nextPos])))
	{
		if (line[nextPos] == '/')
		{
			currentFaceType++;
			nextPos++;
			continue;
		}
		else if (line[nextPos] == ' ' || IsEndLineCharacter(line[nextPos]))
		{
			currentFaceType = 0;
			newFaces.push_back(currentFace);
			currentFace = {};
			nextPos++;
			continue;
		}

		size_t shiftPosition = 0;
		int iVal = std::stoi(&line.at(nextPos), &shiftPosition);
		if (iVal < 0)
		{
			int sizeOfLast;
			if (currentFaceType == 0)
				sizeOfLast = m_positions.size();
			else if (currentFaceType == 1)
				sizeOfLast = m_uvs.size();
			else
				sizeOfLast = m_normals.size();
			iVal = iVal + sizeOfLast;
		}
		currentFace.Set(static_cast<uint32_t>(iVal), currentFaceType);
		nextPos += shiftPosition;
	}
	//newFaces.push_back(currentFace);

	//We don't have to offset the array indices because there is a default values at index 0, so the obj format matches up.

	//Add tris, if there are 4 (1,2,3,4) it will add 1,2,3 then 2,3,4.
	size_t faceCount = newFaces.size();
	int idx = 0;
	while (faceCount >= 3)
	{
		m_groups.back().faces.push_back(newFaces[idx]);
		m_groups.back().faces.push_back(newFaces[idx + 1]);
		m_groups.back().faces.push_back(newFaces[idx + 2]);
		faceCount--;
		idx++;
	}
}
