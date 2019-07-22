#pragma once
#include <array>
#include <string>

//http://paulbourke.net/dataformats/mtl/

using Float4 = std::array<float, 4>;
using Float3 = std::array<float, 3>;
using Float2 = std::array<float, 2>;

struct Material
{
	struct Header
	{
		std::array<float, 3> colourAmbient;
		std::array<float, 3> colourDiffuse;
		std::array<float, 3> colourSpecular;
		float		weightSpecular = 1;
		float		alpha = 1.0f;
		uint32_t	illuminationType = 0;
	};

	Material(std::string name) : materialName(name) { }
	Material() : materialName("not set") { }

	Header header;

	std::string materialName;
	std::string textureAmbient;
	std::string textureDiffuse;
	std::string textureSpecular;
	std::string textureHighlight;
	std::string textureBump;
	std::string textureDisplacement;
	std::string textureStencil;
	std::string textureAlpha;
};