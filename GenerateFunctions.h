#pragma once
#include <math.h>
#include <iostream>

inline unsigned char sinwave_world(int x, int y, int z)
{
	float height_map = 8 + 4* (sinf(float(x)/10) + sinf(float(z) / 10));
	if (y < height_map)
		return 1;
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