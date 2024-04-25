#pragma once
#include "Math/mat.h"
#include "Windows.h"




struct Camera3D
{
	vec3 position;
	vec2 view;
	vec3 Direction;
	Camera3D(){}
	Camera3D(vec3 position, vec2 view, vec3 Direction) : position(position), view(view), Direction(Direction){}
	mat3 RotXAxis();
	mat3 RotYAxis();
	void FreeCam(float FrameTime);
};

inline mat3 Camera3D::RotXAxis()
{
	return mat3
	{
		1,0,0,
		0,cosf(view.y), -sinf(view.y),
		0,sinf(view.y), cosf(view.y)
	};
}
inline mat3 Camera3D::RotYAxis()
{
	return mat3
	{
		cosf(view.x), 0, sinf(view.x),
		0,1,0,
		-sinf(view.x), 0 ,cosf(view.x)
	};
}

inline void Camera3D::FreeCam(float FrameTime)
{
	vec2 movement_direction = vec2(0, 0);
	float speed = 5;
	float sensitivity = 2;
	mat3 RotY =
	{
		cosf(-view.x), 0, sinf(-view.x),
		0,1,0,
		-sinf(-view.x), 0 , cosf(-view.x),
	};
	mat3 RotX =
	{
		1,0,0,
		 0,cosf(-view.y), -sinf(-view.y),
		 0,sinf(-view.y), cosf(-view.y),
	};
	Direction = RotY * RotX * vec3(0, 0, 1);

	//Camera movement windows users only
	if (GetAsyncKeyState('W') & 0x8000)
		movement_direction += vec2(-sin(view.x), cos(view.x));
	if (GetAsyncKeyState('A') & 0x8000)
		movement_direction -= vec2(cos(view.x), sin(view.x));
	if (GetAsyncKeyState('D') & 0x8000)
		movement_direction += vec2(cos(view.x), sin(view.x));
	if (GetAsyncKeyState('S') & 0x8000)
		movement_direction -= vec2(-sin(view.x), cos(view.x));
	if (movement_direction.mag() > 1)
		movement_direction = movement_direction.Normalize();
	if (GetAsyncKeyState('C') & 0x8000)
		position.y += -speed * FrameTime;
	if (GetAsyncKeyState(' ') & 0x8000)
		position.y += speed * FrameTime;
	if (GetAsyncKeyState(37) & 0x8000)
		view.x += sensitivity * FrameTime;
	if (GetAsyncKeyState(39) & 0x8000)
		view.x -= sensitivity * FrameTime;
	if (GetAsyncKeyState(40) & 0x8000)
		view.y -= sensitivity * FrameTime;
	if (GetAsyncKeyState(38) & 0x8000)
		view.y += sensitivity * FrameTime;
	position += speed * vec3(movement_direction.x, 0, movement_direction.y) * FrameTime;
}