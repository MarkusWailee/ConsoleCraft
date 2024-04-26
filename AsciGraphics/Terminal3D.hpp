#pragma once
#include <iostream>
#include "ASCI_Texture.hpp"
#include "DeltaTime.hpp"
#include "Math/mat.h"


class Terminal3D : ASCI_TextureManager
{
	friend class Draw;//to access private members
	Terminal3D() {};
public:
	static void Add_Texture(const char tex_code, char* tex_data, const int width, const int height) { Get().add_texture(tex_code, tex_data, width, height); }
	static void Add_Texture_ppm(const char tex_code, std::string file_name) { Get().add_texture_ppm(tex_code, file_name); }
	static ASCI_Texture& Get_texture(char tex_code) { Get().get_texture(tex_code); }
	static void Init(int ScreenWidth, int ScreenHeight, char background_character);
	static void Init(int ScreenWidth, int ScreenHeight, float aspect_ratio, char background_character);
	static void ClearBuffer();
	static void Render() { std::cout << "\x1B[H" << Get().front_buffer; }
	static void SetPixel(vec3 position, char character);
	static int GetScreenWidth() { return Get().screen_w; }
	static int GetScreenHeight() { return Get().screen_h; }
	static float GetAspectRatio() { return Get().aspect_ratio; }
	static void Terminate()
	{
		delete[] Get().z_back_buffer;
		delete[] Get().z_front_buffer;
		delete[] Get().back_buffer;
		delete[] Get().front_buffer;
	}
protected:
	float* z_back_buffer = NULL;
	float* z_front_buffer = NULL;
	int PIXEL_COUNT = 0;

	char* back_buffer = NULL;
	char* front_buffer = NULL;
	int screen_w = 0;
	int screen_h = 0;
	float aspect_ratio = 0;
	int SCREEN_CHAR_COUNT = 0;
	int SCREEN_CHAR_WIDTH = 0;
	void init_i(int ScreenWidth, int ScreenHeight, char background_character);
	static Terminal3D& Get() { static Terminal3D instance; return instance; };
};

inline void Terminal3D::Init(int ScreenWidth, int ScreenHeight, char background_character)
{
	Get().aspect_ratio = float(ScreenWidth) / ScreenHeight;
	Get().init_i(ScreenWidth, ScreenHeight, background_character); 
};
inline void Terminal3D::Init(int ScreenWidth, int ScreenHeight, float aspect_ratio, char background_character)
{
	Get().aspect_ratio = aspect_ratio;
	Get().init_i(ScreenWidth, ScreenHeight, background_character);
};
inline void Terminal3D::ClearBuffer()
{
	//clears both zbuffer and front char buffer
	std::memcpy(Get().z_front_buffer, Get().z_back_buffer, Get().PIXEL_COUNT * sizeof(float));
	std::memcpy(Get().front_buffer, Get().back_buffer, Get().SCREEN_CHAR_COUNT);
}
inline void Terminal3D::init_i(int ScreenWidth, int ScreenHeight, char background_character)
{
	//String setup
	std::cout.sync_with_stdio(false);
	SCREEN_CHAR_WIDTH = ScreenWidth * 2 + 1;
	SCREEN_CHAR_COUNT = (ScreenWidth * 2 + 1) * ScreenHeight + 1;
	screen_w = ScreenWidth;
	screen_h = ScreenHeight;
	back_buffer = new char[SCREEN_CHAR_COUNT];
	front_buffer = new char[SCREEN_CHAR_COUNT];
	std::fill_n(back_buffer, SCREEN_CHAR_COUNT, background_character);
	for (int i = 0; i <= ScreenHeight; i++)
		back_buffer[(ScreenWidth * 2) + i * (ScreenWidth * 2+1)] = '\n';
	back_buffer[SCREEN_CHAR_COUNT - 2] = '\n';
	back_buffer[SCREEN_CHAR_COUNT - 1] = NULL;
	std::memcpy(front_buffer, back_buffer, SCREEN_CHAR_COUNT);
	//z buffer setup
	PIXEL_COUNT = ScreenWidth * ScreenHeight;
	z_back_buffer = new float[PIXEL_COUNT];
	z_front_buffer = new float[PIXEL_COUNT];
	//z_buffer is a floating point nuumber 0 to 1
	std::fill_n(z_back_buffer, PIXEL_COUNT, 1);
	std::memcpy(z_front_buffer, z_back_buffer, PIXEL_COUNT * sizeof(float));
}
inline void Terminal3D::SetPixel(vec3 position, char character)
{
	int x = position.x;
	int y = Get().screen_h - position.y;//invert the y value
	if (x < 0 || x >= Get().screen_w || y < 0 || y >= Get().screen_h) return;

	int z_buffer_offset = x + y * Get().screen_w;//I HAD THIS SET TO "screen_h". F#%& THAT TOOK 2 hours to figure out

	if (position.z > Get().z_front_buffer[z_buffer_offset] || position.z<-1)return;
	Get().z_front_buffer[z_buffer_offset] = position.z;
	int front_buffer_offset1 = x * 2 + y * (Get().SCREEN_CHAR_WIDTH);
	int front_buffer_offset2 = x * 2 + 1 + y * (Get().SCREEN_CHAR_WIDTH);
	Get().front_buffer[front_buffer_offset1] = character;
	Get().front_buffer[front_buffer_offset2] = character;
}




