#pragma once
#include <iostream>
#include <vector>
#include <fstream>

struct ASCI_Texture
{
	char* data = NULL;
	int width = 0;
	int height = 0;
	char GetCoord(int x, int y)
	{
		if (x < 0 || x >= width || y < 0 || y >= height) return '?';
		int offset = x + (height-y-1) * width;
		return data[offset];
	}
	ASCI_Texture(){}
	~ASCI_Texture() { delete[] data; }
};

class ASCI_TextureManager
{
protected:
	ASCI_Texture textures[128];
	ASCI_TextureManager(){}
public:
	void add_texture(char tex_code, char* tex_data,const int width, const int height)
	{
		//Im assigning letters to each texture;
		ASCI_Texture& tex = textures[tex_code];
		if (tex.data != NULL)
		{
			std::cout << "Texture is taken\n";
			return;
		}
		tex.data = tex_data;
		tex.width = width;
		tex.height = height;
	}
	void add_texture_ppm(char tex_code, std::string file_name)
	{
		std::string ASCI_Gradient = " .:'-~=<\*({[%08O#@Q&";		//" .:-~=+*%08O#@Q";
		//std::string ASCI_Gradient = " .,*#jH@OQ";
		ASCI_Texture& texture = textures[tex_code];
		std::ifstream Image(file_name, std::ios::binary);
		if (!Image.is_open())
		{
			std::cout << "Failed to open file\b";
			throw;
		}
		std::string type;
		Image >> type;
		if (type != "P6")
		{
			std::cout << "incorrect p type\n";
			throw;
		}
		int max_value;
		Image >> texture.width;
		Image >> texture.height;
		Image >> max_value;
		size_t current_pos = Image.tellg();
		Image.seekg(0, std::ios::end);
		size_t end_pos = Image.tellg();
		Image.seekg(current_pos, std::ios::beg);
		size_t rgb_size = end_pos - current_pos - 1;

		//unsigned char* rgb_data = new unsigned char[rgb_size];
		if (texture.data != NULL) delete[] texture.data;
		texture.data = new char[texture.width * texture.height];
		for (int i = 0; i < texture.width * texture.height; ++i)
		{
			unsigned char r, g, b;
			Image.read(reinterpret_cast<char*>(&r), sizeof(unsigned char));
			Image.read(reinterpret_cast<char*>(&g), sizeof(unsigned char));
			Image.read(reinterpret_cast<char*>(&b), sizeof(unsigned char));

			// Calculate brightness gradient and map to ASCII character
			float gradient = (r + g + b) / (3.0f * max_value);
			texture.data[i] = ASCI_Gradient[ASCI_Gradient.size() * gradient];
		}
		Image.close();
	}
	ASCI_Texture& get_texture(char tex_code)
	{
		return textures[tex_code];
	}
};