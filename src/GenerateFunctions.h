#pragma once
#include <math.h>
#include <iostream>


inline float get_height_map(int x, int z)
{
	return 16 + 8 * (sinf(float(z) / 20 + sinf(float(z) / 20)) + 2 * sinf(float(x) / 30 + sinf(float(x) / 20)));
}

inline unsigned char sinwave_world(int x, int y, int z)
{
	//float height_map = 8 + 4* (sinf(float(x)/10) + sinf(float(z) / 10));
	float height_map = get_height_map(x, z);
	if (y < height_map && y > height_map - 2)
		return 1;
	else if (y < height_map - 2)
		return 2;
	
	return 0;
}

inline unsigned char sphere_world(int x, int y, int z)
{
	int posx = 0, posy = 0, posz = 0;
	int dx = x - posx;
	int dy = y - posy;
	int dz = z - posz;

	if ((sqrtf(dx * dx + dy * dy + dz * dz) < 300))
		return rand() % 2 + 1;
	return 0;
}