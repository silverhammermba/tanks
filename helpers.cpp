#include "helpers.hpp"

float deg2rad(float deg)
{
	return deg * M_PI / 180.f;
}

float deadzone(float val, float zone, float max)
{
	if (std::abs(val) <= zone)
		return 0.f;
	if (val < -zone)
		return max * (val + zone) / (max - zone);
	if (val > zone)
		return max * (val - zone) / (max - zone);
	return val;
}
