#pragma once
#include <math.h>
#include <iostream>

#define PI 3.14159265358979323846f
//VECTOR 2
struct vec2
{
	float x = 0;
	float y = 0;

	vec2() : x(0), y(0){}
	vec2(const float x, const float y) : x(x), y(y) {}
	void operator*=(const float s);
	void operator/=(const float s);
	void operator+=(const vec2& v);
	void operator-=(const vec2& v);
	
	vec2 operator*(const float s) const;
	vec2 operator/(const float s) const;
	vec2 operator+(const vec2& v) const;
	vec2 operator-(const vec2& v) const;
	friend vec2 operator*(const float s, const vec2& v);
	bool operator==(const vec2& v) const ;
	bool operator!=(const vec2& v) const ;
	float& operator[](const int index);
	friend std::ostream& operator<<(std::ostream& os, const vec2& v);

	vec2 Normalize() const;
	float mag() const;
	float dot(const vec2& v) const;
};
inline void vec2::operator*=(const float s)
{
	x *= s;
	y *= s;
}
inline void vec2::operator/=(const float s)
{
	x /= s;
	y /= s;
}
inline void vec2::operator+=(const vec2& v)
{
	x += v.x;
	y += v.y;
}
inline void vec2::operator-=(const vec2& v)
{
	x -= v.x;
	y -= v.y;
}
inline vec2 vec2::operator*(const float s) const
{
	return vec2(x * s, y * s);
}
inline vec2 vec2::operator/(const float s) const 
{
	return vec2(x / s, y / s);
}
inline vec2 vec2::operator+(const vec2& v) const 
{
	return vec2(x + v.x, y + v.y);
}
inline vec2 vec2::operator-(const vec2& v) const 
{
	return vec2(x - v.x, y - v.y);
}
inline vec2 operator*(const float s, const vec2& v)
{
	return vec2(s * v.x, s * v.y);
}
inline bool vec2::operator==(const vec2& v) const 
{
	return (x == v.x && y == v.y);
}
inline bool vec2::operator!=(const vec2& v) const
{
	return !(x == v.x && y == v.y);
}
inline float& vec2::operator[](const int index)
{
	return(&x)[index];
}
inline std::ostream& operator<<(std::ostream& os, const vec2& v) {
	os << '(' << v.x << ", " << v.y << ')';
	return os;
}
inline vec2 vec2::Normalize() const
{
	float denominator = 1 / sqrtf(x * x + y * y);
	return vec2(x * denominator, y * denominator);
}
inline float vec2::mag() const
{
	return (sqrtf(x * x + y * y));
}
inline float vec2::dot(const vec2& v) const
{
	return x * v.x + y * v.y;
}


//VECTOR 3
struct vec3
{
	float x = 0;
	float y = 0;
	float z = 0;

	vec3() : x(0), y(0), z(0) {}
	vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	void operator*=(const float s);
	void operator/=(const float s);
	void operator+=(const vec3& v);
	void operator-=(const vec3& v);

	vec3 operator*(const float s) const;
	vec3 operator/(const float s) const;
	vec3 operator+(const vec3& v) const;
	vec3 operator-(const vec3& v) const;
	friend vec3 operator*(const float s, const vec3& v);
	bool operator==(const vec3& v) const;
	bool operator!=(const vec3& v) const;
	float& operator[](const int index);
	friend std::ostream& operator<<(std::ostream& os, const vec3& v);

	vec3 Normalize() const;
	float mag() const;
	float dot(const vec3& v) const;
};
inline void vec3::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
}
inline void vec3::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
}
inline void vec3::operator+=(const vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}
inline void vec3::operator-=(const vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
inline vec3 vec3::operator*(const float s) const
{
	return vec3(x * s, y * s, z * s);
}
inline vec3 vec3::operator/(const float s) const
{
	return vec3(x / s, y / s, z / s);
}
inline vec3 vec3::operator+(const vec3& v) const
{
	return vec3(x + v.x, y + v.y, z + v.z);
}
inline vec3 vec3::operator-(const vec3& v) const
{
	return vec3(x - v.x, y - v.y, z - v.z);
}
inline vec3 operator*(const float s, const vec3& v)
{
	return vec3(s * v.x, s * v.y, s * v.z);
}
inline bool vec3::operator==(const vec3& v) const 
{
	return (x == v.x && y == v.y && z == v.z);
}
inline bool vec3::operator!=(const vec3& v) const
{
	return !(x == v.x && y == v.y && z == v.z);

}
inline float& vec3::operator[](const int index)
{
	return(&x)[index];
}
inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
	os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
	return os;
}
inline vec3 vec3::Normalize() const
{
	float denominator = 1 / sqrtf(x * x + y * y + z * z);
	return vec3(x * denominator, y * denominator, z * denominator);
}
inline float vec3::mag() const
{
	return (sqrtf(x * x + y * y + z * z));
}
inline float vec3::dot(const vec3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}


//VECTOR 4
struct vec4
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	vec4() : x(0), y(0), z(0), w(0) {}
	vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	void operator*=(const float s);
	void operator/=(const float s);
	void operator+=(const vec4& v);
	void operator-=(const vec4& v);

	vec4 operator*(const float s) const;
	vec4 operator/(const float s) const;
	vec4 operator+(const vec4& v) const;
	vec4 operator-(const vec4& v) const;
	friend vec4 operator*(const float s, const vec4& v);
	bool operator==(const vec4& v) const;
	bool operator!=(const vec4& v) const;
	float& operator[](const int index);
	friend std::ostream& operator<<(std::ostream& os, const vec4& v);

	vec4 Normalize() const;
	float mag() const;
	float dot(const vec4& v) const;
};
inline void vec4::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}
inline void vec4::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}
inline void vec4::operator+=(const vec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}
inline void vec4::operator-=(const vec4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}
inline vec4 vec4::operator*(const float s) const
{
	return vec4(x * s, y * s, z * s, w * s);
}
inline vec4 vec4::operator/(const float s) const
{
	return vec4(x / s, y / s, z / s, w / s);
}
inline vec4 vec4::operator+(const vec4& v) const
{
	return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}
inline vec4 vec4::operator-(const vec4& v) const
{
	return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}
inline vec4 operator*(const float s, const vec4& v)
{
	return vec4(s * v.x, s * v.y, s * v.z, s * v.w);
}
inline bool vec4::operator==(const vec4& v) const
{
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}
inline bool vec4::operator!=(const vec4& v) const
{
	return !(x == v.x && y == v.y && z == v.z && w == v.w);
}
inline float& vec4::operator[](const int index)
{
	return(&x)[index];
}
inline std::ostream& operator<<(std::ostream& os, const vec4& v) {
	os << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
	return os;
}
inline vec4 vec4::Normalize() const
{
	float denominator = 1 / sqrtf(x * x + y * y + z * z + w * w);
	return vec4(x * denominator, y * denominator, z * denominator, w * denominator);
}
inline float vec4::mag() const
{
	return (sqrtf(x * x + y * y + z * z + w * w));
}
inline float vec4::dot(const vec4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}