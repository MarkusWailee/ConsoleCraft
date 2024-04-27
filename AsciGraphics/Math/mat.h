#pragma once
#include  "vec.h"

//DEFAULT TO ROW MAJOR ORDERING
//ADD TRANSPOSE IF NEEDED
struct mat2
{
	float n[4] =
	{
		1,0,
		0,1
	};
	vec2 operator*(const vec2& vec);
	mat2 operator*(const mat2& mat);
};
inline vec2 mat2::operator*(const vec2& vec)
{
	return vec2(vec.x * n[0] + vec.y * n[1], vec.x * n[2] + vec.y * n[3]);
}
inline mat2 mat2::operator*(const mat2& m)
{
	return mat2
	{
		n[0] * m.n[0] + n[1] * m.n[2], n[0] * m.n[1] + n[1] * m.n[3],
		n[2] * m.n[0] + n[3] * m.n[2], n[2] * m.n[1] + n[3] * m.n[3]
	};
}


//MATRIX 3
struct mat3
{
	float n[9] =
	{
		1,0,0,
		0,1,0,
		0,0,1
	};
	vec3 operator*(const vec3& vec);
	mat3 operator*(const mat3& mat);
};
inline vec3 mat3::operator*(const vec3& vec)
{
	return vec3(
		vec.x * n[0] + vec.y * n[1] + vec.z * n[2],	//x
		vec.x * n[3] + vec.y * n[4] + vec.z * n[5],	//y
		vec.x * n[6] + vec.y * n[7] + vec.z * n[8]	//z
	);
}
inline mat3 mat3::operator*(const mat3& m)
{
	return mat3
	{
		n[0] * m.n[0] + n[1] * m.n[3] + n[2] * m.n[6], n[0] * m.n[1] + n[1] * m.n[4] + n[2] * m.n[7], n[0] * m.n[2] + n[1] * m.n[5] + n[2] * m.n[8],
		n[3] * m.n[0] + n[4] * m.n[3] + n[5] * m.n[6], n[0] * m.n[1] + n[4] * m.n[4] + n[5] * m.n[7], n[3] * m.n[2] + n[4] * m.n[5] + n[5] * m.n[8],
		n[6] * m.n[0] + n[7] * m.n[3] + n[8] * m.n[6], n[0] * m.n[1] + n[7] * m.n[4] + n[8] * m.n[7], n[6] * m.n[2] + n[7] * m.n[5] + n[8] * m.n[8]
	};
}

//Matrix 4
struct mat4
{
	float n[16] =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	vec4 operator*(const vec4& vec);
	mat4 operator*(const mat4& mat);
};
inline vec4 mat4::operator*(const vec4& vec)
{
	return vec4(
		vec.x * n[0] + vec.y * n[1] + vec.z * n[2] + vec.w * n[3],
		vec.x * n[4] + vec.y * n[5] + vec.z * n[6] + vec.w * n[7],
		vec.x * n[8] + vec.y * n[9] + vec.z * n[10] + vec.w * n[11],
		vec.x * n[12] + vec.y * n[13] + vec.z * n[14] + vec.w * n[15]
	);
}
inline mat4 mat4::operator*(const mat4& m)
{
	return mat4
	{
		n[0] * m.n[0] + n[1] * m.n[4] + n[2] * m.n[8] + n[3] * m.n[12],  n[0] * m.n[1] + n[1] * m.n[5] + n[2] * m.n[9] + n[3] * m.n[13],  n[0] * m.n[2] + n[1] * m.n[6] + n[2] * m.n[10] + n[3] * m.n[14],  n[0] * m.n[3] + n[1] * m.n[7] + n[2] * m.n[11] + n[3] * m.n[15],
		n[4] * m.n[0] + n[5] * m.n[4] + n[6] * m.n[8] + n[7] * m.n[12],  n[4] * m.n[1] + n[5] * m.n[5] + n[6] * m.n[9] + n[7] * m.n[13],  n[4] * m.n[2] + n[5] * m.n[6] + n[6] * m.n[10] + n[7] * m.n[14],	 n[4] * m.n[3] + n[5] * m.n[7] + n[6] * m.n[11] + n[7] * m.n[15],
		n[8] * m.n[0] + n[9] * m.n[4] + n[10] * m.n[8] + n[10] * m.n[12],  n[8] * m.n[1] + n[9] * m.n[5] + n[10] * m.n[9] + n[11] * m.n[13],  n[8] * m.n[2] + n[9] * m.n[6] + n[10] * m.n[10] + n[11] * m.n[14],  n[8] * m.n[3] + n[9] * m.n[7] + n[10] * m.n[11] + n[11] * m.n[15],
		n[12] * m.n[0] + n[13] * m.n[4] + n[14] * m.n[8] + n[15] * m.n[12],  n[12] * m.n[1] + n[13] * m.n[5] + n[14] * m.n[9] + n[15] * m.n[13],  n[12] * m.n[2] + n[13] * m.n[6] + n[14] * m.n[10] + n[15] * m.n[14],	n[12] * m.n[3] + n[13] * m.n[7] + n[14] * m.n[11] + n[15] * m.n[15]
	};
}


namespace mat
{
	inline mat3 GetRotY(float amount)
	{
		return mat3
		{
			cosf(amount), 0, sinf(amount),
			0,1,0,
			-sinf(amount), 0 ,cosf(amount)
		};
	}
	
	inline mat3 GetRotX(float amount)
	{
		return mat3
		{
			1,0,0,
			0,cosf(amount), -sinf(amount),
			0,sinf(amount), cosf(amount)
		};
	}

}