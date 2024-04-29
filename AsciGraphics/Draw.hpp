#pragma once
#include "Terminal3D.hpp"
#include <algorithm>




class Draw : Terminal3D //friend
{
private:
	Draw() {};
	static float z_formula(float z) 
	{
		float far = 200;
		float near = 0.1;
		return (far + near) / (far - near) + (1 / z) * ((-2 * far * near) / (far - near));
	}

public:
	//2D functions
	static void triangle(vec3 p1, vec3 p2, vec3 p3, float Brightness);
	static void triangle_uv(vec3 p1, vec3 p2, vec3 p3, vec2* uv, char tex_code, float Brightness);
	static void quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, float Brightness);
	static void quad(vec3* p, float Brightness);
	static void quad_uv(vec3* p, char tex_code, float Brightness);
	static void quad_uv(vec3 p1, vec3 p2, vec3 p3, vec3 p4, char tex_code, float Brightnes);
	static void circle(vec3 position, float r, char character);
};

inline void Draw::quad_uv(vec3* p, char tex_code, float Brightness) //use 4 points
{
	vec2 uv1[] = { vec2(0,0),vec2(1,0)/p[1].z,vec2(1,1)/p[2].z};
	vec2 uv2[] = { vec2(0,0),vec2(1,1)/p[2].z,vec2(0,1)/p[3].z};
	triangle_uv(p[0], p[1], p[2], uv1, tex_code, Brightness);
	triangle_uv(p[0], p[2], p[3], uv2, tex_code, Brightness);
}
inline void Draw::quad_uv(vec3 p1, vec3 p2, vec3 p3, vec3 p4, char tex_code, float Brightness)
{
	vec2 uv1[] = { vec2(0,0),vec2(1,0) / p2.z,vec2(1,1) / p3.z };
	vec2 uv2[] = { vec2(0,0),vec2(1,1) / p3.z,vec2(0,1) / p4.z };
	triangle_uv(p1, p2, p3, uv1, tex_code, Brightness);
	triangle_uv(p1, p3, p4, uv2, tex_code, Brightness);
}

inline void Draw::quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, float Brightness)
{
	triangle(p1, p2, p3, Brightness);
	triangle(p1, p3, p4, Brightness);
}
inline void Draw::quad(vec3* p, float Brightness)
{
	triangle(p[0], p[1], p[2], Brightness);
	triangle(p[0], p[2], p[3], Brightness);
}

inline void Draw::triangle_uv(vec3 p1, vec3 p2, vec3 p3, vec2* uv, char tex_code, float Brightness)
{

	ASCI_Texture& tex = Get().textures[tex_code];
	if (tex.data == NULL)return;

	//BackFace Culling
	vec2 A = vec2(p2.x, p2.y) - vec2(p1.x, p1.y);
	vec2 A_normal = vec2(-A.y, A.x);
	vec2 B = vec2(p3.x, p3.y) - vec2(p1.x, p1.y);
	if (A_normal.dot(B) < 0)return;


	//Normalizing coordinates from -1 to 1 in x and y axis
	const static float half_screen_w = (Get().screen_w / 2);
	const static float half_screen_h = (Get().screen_h / 2);
	const static float normalized_screen_width = half_screen_w / Get().aspect_ratio;
	p1.x = p1.x * normalized_screen_width + half_screen_w;
	p1.y = p1.y * half_screen_h + half_screen_h;
	p2.x = p2.x * normalized_screen_width + half_screen_w;
	p2.y = p2.y * half_screen_h + half_screen_h;
	p3.x = p3.x * normalized_screen_width + half_screen_w;
	p3.y = p3.y * half_screen_h + half_screen_h;

	float inv_z1 = 1 / p1.z;
	float inv_z2 = 1 / p2.z;
	float inv_z3 = 1 / p3.z;

	int min_x = std::min(std::min(p1.x, p2.x), p3.x);
	int min_y = std::min(std::min(p1.y, p2.y), p3.y);
	int max_x = std::max(std::max(p1.x, p2.x), p3.x);
	int max_y = std::max(std::max(p1.y, p2.y), p3.y);
	//prevent rendering off screen
	if (min_x < 0) min_x = 0;
	if (min_y < 0) min_y = 0;
	if (max_x > Terminal3D::get_screen_width()) max_x = Terminal3D::get_screen_width();
	if (max_y > Terminal3D::get_screen_height()) max_y = Terminal3D::get_screen_height();
	for (int posY = min_y; posY <= max_y; posY++)
	{
		for (int posX = min_x; posX <= max_x; posX++)
		{
			float w1, w2, w3;
			w2 = ((float(posY) - p1.y) * (p3.x - p1.x) - (float(posX) - p1.x) * (p3.y - p1.y)) /
				((p2.y - p1.y) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.y - p1.y));
			w3 = ((float(posY) - p2.y) * (p1.x - p2.x) - (float(posX) - p2.x) * (p1.y - p2.y)) /
				((p3.y - p2.y) * (p1.x - p2.x) - (p3.x - p2.x) * (p1.y - p2.y));
			w1 = 1 - w2 - w3;
			//float interpolated_z = (z_formula(p1.z) * w1 + z_formula(p2.z) * w2 + z_formula(p3.z) * w3);
			float interpolated_z = (p1.z * w1 + p2.z * w2 + p3.z * w3);
			vec3 pixel_position = vec3(posX, posY, z_formula(interpolated_z));//adds non linear z_buffer for precision

			float inv_z = inv_z1 * w1 + inv_z2 * w2 + inv_z3 * w3;

			vec2 uv_cord = (uv[0] * w1 + uv[1] * w2 + uv[2] * w3)/ inv_z;
			int uv_x = uv_cord.x * tex.width;
			int uv_y = uv_cord.y * tex.height;
			if (!(w1 >= -0 && w1 <= 1 && w2 >= -0 && w2 <= 1 && w3 >= -0 && w3 <= 1 ))continue;
			Get().set_pixel(pixel_position, tex.get_coord(uv_x,uv_y, Brightness));
		}
	}
}

inline void Draw::triangle(vec3 p1, vec3 p2, vec3 p3, float Brightness) //RASTERIZER using barycentric coordinates
{
	//Normalizing coordinates from -1 to 1 in x and y axis
	const static float half_screen_w = (Get().screen_w / 2);
	const static float half_screen_h = (Get().screen_h / 2);
	const static float normalized_screen_width = half_screen_w / Get().aspect_ratio;
	p1.x = p1.x * normalized_screen_width + half_screen_w;
	p1.y = p1.y * half_screen_h + half_screen_h;
	p2.x = p2.x * normalized_screen_width + half_screen_w;
	p2.y = p2.y * half_screen_h + half_screen_h;
	p3.x = p3.x * normalized_screen_width + half_screen_w;
	p3.y = p3.y * half_screen_h + half_screen_h;

	int min_x = std::min(std::min(p1.x, p2.x), p3.x);
	int min_y = std::min(std::min(p1.y, p2.y), p3.y);
	int max_x = std::max(std::max(p1.x, p2.x), p3.x);
	int max_y = std::max(std::max(p1.y, p2.y), p3.y);
	//prevent rendering off screen
	if (min_x < 0) min_x = 0;
	if (min_y < 0) min_y = 0;
	if (max_x > Terminal3D::get_screen_width()) max_x = Terminal3D::get_screen_width();
	if (max_y > Terminal3D::get_screen_height()) max_y = Terminal3D::get_screen_height();
	for (int posY = min_y; posY <= max_y; posY++)
	{
		for (int posX = min_x; posX <= max_x; posX++)
		{
			float w1, w2, w3;
			w2 = ((posY - p1.y) * (p3.x - p1.x) - (posX - p1.x) * (p3.y - p1.y)) /
				((p2.y - p1.y) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.y - p1.y));
			w3 = ((posY - p2.y) * (p1.x - p2.x) - (posX - p2.x) * (p1.y - p2.y)) /
				((p3.y - p2.y) * (p1.x - p2.x) - (p3.x - p2.x) * (p1.y - p2.y));
			w1 = 1 - w2 - w3;
			float interpolated_z = (p1.z * w1 + p2.z * w2 + p3.z * w3);
			vec3 pixel_position = vec3(posX, posY, z_formula(interpolated_z));
			if (!(w1 >= -0 && w1 <= 1 && w2 >= -0 && w2 <= 1 && w3 >= -0 && w3 <= 1))continue;
			Get().set_pixel(pixel_position, get_asci_gradient(Brightness));
		}
	}
}


inline void Draw::circle(vec3 p, float r, char character)
{
	const static float half_screen_w = (Get().screen_w / 2);
	const static float half_screen_h = (Get().screen_h / 2);
	const static float normalized_screen_width = half_screen_w / Get().aspect_ratio;
	p.x = p.x * normalized_screen_width + half_screen_w;
	p.y = p.y * half_screen_h + half_screen_h;


	int min_x = p.x - r;
	int min_y = p.y - r;
	int max_x = p.x + r;
	int max_y = p.y + r;
	if (min_x < 0) min_x = 0;
	if (min_y < 0) min_y = 0;
	if (max_x > Terminal3D::get_screen_width()) max_x = Terminal3D::get_screen_width();
	if (max_y > Terminal3D::get_screen_height()) max_y = Terminal3D::get_screen_height();
	for (int y = min_y; y <= max_y; y++)
		for (int x = min_x; x <= max_x; x++)
		{
			vec2 d = (vec2(x, y) - vec2(p.x, p.y));
			if ((d.x * d.x)*Get().aspect_ratio + d.y * d.y < r * r)
				Terminal3D::set_pixel(vec3(x, y, p.z), character);
		}
}

