#include "Draw.hpp"
#include "Camera3D.hpp"



/* To use the Draw3D functions, you must pass an array of 4 vec3's to specify its vertices
*/
class Draw3D : Draw
{
public:
	static void plain_uv(const vec3 position, const vec3* vertices4, char tex_code, float Brightness, Camera3D camera);
	static void plain(const vec3 position, const vec3* vertices4, float Brightness, Camera3D camera);
	static void cube(const vec3 position, float size, float Brightness, Camera3D camera);

	//Minecraft function
	static void sun(vec3 position, float size, float Brightness, Camera3D camera);
};

inline void Draw3D::plain_uv(vec3 position, const vec3* vertices4, char tex_code, float Brightness, Camera3D camera)
{
	vec3 quad_vertices[4];
	for (int i = 0; i < 4; i++)
	{
		//converting 3D vertices to 2D vertices
		vec3 vertice = vertices4[i] + position;
		vertice -= camera.position;
		vertice = camera.RotXAxis() * camera.RotYAxis() * vertice;
		vertice = vec3(vertice.x / vertice.z, vertice.y / vertice.z, vertice.z);

		//Temporary fix for rendering -z values
		if (vertice.z < 0) return;
		quad_vertices[i] = vertice;
	}
	//Render 2D vertices
	quad_uv(quad_vertices, tex_code, Brightness);
}

inline void Draw3D::plain(const vec3 position, const vec3* vertices4, float Brightness, Camera3D camera)
{
	vec3 quad_vertices[4];
	for (int i = 0; i < 4; i++)
	{
		vec3 vertice = vertices4[i] + position;
		vertice -= camera.position;
		vertice = camera.RotXAxis() * camera.RotYAxis() * vertice;
		vertice = vec3(vertice.x / vertice.z, vertice.y / vertice.z, vertice.z);

		if (vertice.z < 0)return;
		quad_vertices[i] = vertice;
	}
	quad(quad_vertices, Brightness);
}

inline void Draw3D::cube(const vec3 position, float size, float Brightness, Camera3D camera)
{
	vec3 v[] =
	{
		vec3(-1,-0.5,-2),
		vec3(1,-0.5,-2),
		vec3(1,0.5,-2),
		vec3(-1,0.5,-2),
		vec3(-1,-0.5,2),
		vec3(1,-0.5,2),
		vec3(1,0.5,2),
		vec3(-1,0.5,2),
	};
	for (int i = 0; i < 8; i++)
	{
		v[i] *= size;
		v[i] += position;
		v[i] -= camera.position;
		v[i] = camera.RotXAxis() * camera.RotYAxis() * v[i];
		v[i] = vec3(v[i].x / v[i].z, v[i].y / v[i].z, v[i].z);
		if (v[i].z < 0)return;
	}
	quad(v[0], v[1], v[2], v[3], Brightness*0.8);
	quad(v[1], v[2], v[6], v[5], Brightness*0.95);
	quad(v[0], v[3], v[7], v[4], Brightness*0.8);
	quad(v[4], v[5], v[6], v[7], Brightness*0.95);
	quad(v[3], v[2], v[6], v[7], Brightness*1);
	quad(v[0], v[1], v[5], v[4], Brightness*1);
}

inline void Draw3D::sun(vec3 position, float size, float Brightness, Camera3D camera)
{
	//position -= camera.position;
	position = camera.RotXAxis() * camera.RotYAxis() * position;
	position = vec3(position.x / position.z, position.y / position.z, position.z);
	if (position.z < 0)return;
	Draw::circle(vec3(position.x, position.y, 1), size, get_asci_gradient(Brightness));

}
