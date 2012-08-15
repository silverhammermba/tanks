#include "engine.hpp"

float rand_f(float max)
{
	return (rand() / (float)RAND_MAX) * max;
}

float rand_i(int max)
{
	return rand() % max;
}

float deg2rad(float deg)
{
	return deg * M_PI / 180.f;
}

float rad2deg(float rad)
{
	return rad * 180.f / M_PI;
}

// create a deadzone of -zone..zone when x ranges from -max..max
float deadzone(float val, float zone, float max)
{
	if (std::abs(val) <= zone)
		return 0.f;
	if (val < -zone)
		return max * (val + zone) / (max - zone);
	if (val > zone)
		return max * (val - zone) / (max - zone);
	// shouldn't get here
	return val;
}

b2v v2f2b2v(const v2f & v)
{
	return b2v(v.x / ppm, v.y / ppm);
}

v2f b2v2v2f(const b2v & v)
{
	return v2f(v.x, v.y) * ppm;
}
