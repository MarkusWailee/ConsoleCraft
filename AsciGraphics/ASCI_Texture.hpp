#pragma once
#include <iostream>
#include <vector>

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
	void add_texture(char tex_code, char* tex_data, int width, int height)
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
	ASCI_Texture& get_texture(char tex_code)
	{
		return textures[tex_code];
	}
};