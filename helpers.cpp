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

int sign(float x)
{
	if (x < 0.f) return -1;
	if (x > 0.f) return 1;
	return 0;
}

float minmax(float before, float amount, float target)
{
	if (target < before)
		return std::max(amount, target);
	else if (target > before)
		return std::min(amount, target);
	return amount;
}
