#include "Draw.hpp"
#include "Camera3D.hpp"



/* To use the Draw3D functions, you must pass an array of 4 vec3's to specify its vertices
*/
class Draw3D : Draw
{
public:
	static void Plain_uv(const vec3 position, const vec3* vertices4, char tex_code, Camera3D camera);
	static void Plain(const vec3 position, const vec3* vertices4, char character, Camera3D camera);
};

inline void Draw3D::Plain_uv(vec3 position, const vec3* vertices4, char tex_code, Camera3D camera)
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
		if (vertice.z < 0)return;
		quad_vertices[i] = vertice;
	}
	//Render 2D vertices
	Quad_uv(quad_vertices, tex_code);
}

inline void Draw3D::Plain(const vec3 position, const vec3* vertices4, char character, Camera3D camera)
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
	Quad(quad_vertices, character);
}


