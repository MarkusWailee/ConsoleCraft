#pragma once
#include "AsciGraphics/Math/vec.h"

struct AABB
{
	vec3 p;
	float w, h, l;
	AABB(vec3 position, float w, float h, float l);
	AABB(vec3 position, float w, float h);
	AABB(vec3 position, float w);

	bool Collision_test(const AABB& box2);
	vec3 Collide(const AABB& box2, int& face_hit);
};


inline AABB::AABB(vec3 position, float w, float h, float l) : p(position), w(w), h(h), l(l) {}
inline AABB::AABB(vec3 position, float w, float h) : p(position), w(w), h(h), l(w) {}
inline AABB::AABB(vec3 position, float w) : p(position), w(w), h(w), l(w) {}


inline bool AABB::Collision_test(const AABB& box2)
{
	bool x = (p.x <= box2.p.x + box2.w && p.x + w >= box2.p.x);
	bool y = (p.y <= box2.p.y + box2.h && p.y + h >= box2.p.y);
	bool z = (p.z <= box2.p.z + box2.l && p.z + l >= box2.p.z);
	return x && y && z;
}


inline vec3 AABB::Collide(const AABB& box2, int& face_hit) //returns position b1 should ben after collision
{
	float face[6];
	face[0] = box2.p.z + box2.l - p.z;//-z
	face[1] = p.x + w - box2.p.x;//+x
	face[2] = p.z + l - box2.p.z;//+z
	face[3] = box2.p.x + box2.w - p.x;//-x
	face[4] = p.y + h - box2.p.y;//+y
	face[5] = box2.p.y + box2.h - p.y;//-y

	int index_found = 0;
	float smallest = FLT_MAX;

	for (int i = 0; i < 6; i++)
		if (face[i] < smallest)
		{
			smallest = face[i];
			index_found = i;
			face_hit = i;
		}

	const float epsilon = 0.002;

	switch (index_found)
	{
	case 0:
		return vec3(p.x, p.y, box2.p.z + box2.l + epsilon);
		break;
	case 1:
		return vec3(box2.p.x - w - epsilon, p.y, p.z);
		break;
	case 2:
		return vec3(p.x, p.y, box2.p.z - l - epsilon);
		break;
	case 3:
		return vec3(box2.p.x + box2.w + epsilon, p.y, p.z);
		break;
	case 4:
		return vec3(p.x, box2.p.y - h - epsilon, p.z);
		break;
	case 5:
		return vec3(p.x, box2.p.y + box2.h + epsilon, p.z);
		break;
	}
}