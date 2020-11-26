#pragma once
#include "../GameHeader.h"
class Maths {
public:
	static constexpr float PI = 3.141592653589793f;
	static constexpr float HALF_PI = 1.57079632679f;
	static constexpr float DOUBLE_PI = 6.28318530718f;
	static constexpr float DEG_RAD_CONST = 57.2957795131f;
	static constexpr float FLOAT_MAX = 2147483647.0f;

	static float Clamp(float value, float min, float max) {
		if (value > max) value = max;
		if (value < min) value = min;
		return value;
	}
	static float Min(float a, float b) {
		return a < b ? a : b;
	}
	static float Max(float a, float b) {
		return a > b ? a : b;
	}

	static float Sin(float a) {
		return std::sinf(a);
	}
	static float Cos(float a) {
		return std::cosf(a);
	}
	static float Tan(float a) {
		return std::tanf(a);
	}
	static float Sqrt(float a) {
		return std::sqrtf(a);
	}
	static float Pow(float a, float exponent) {
		return std::powf(a, exponent);
	}

	//static const double PI = atan(1) * 4;
	//static constexpr double DOUBLE_EPSILON = pow(2, -1022);
	//static constexpr double DOUBLE_MAX = pow(2, 1024) - pow(2, 971);

	static double DegreesToRadians(double degrees) {
		return degrees / DEG_RAD_CONST;
	}
	static double RadiansToDegrees(double radians) {
		return radians * DEG_RAD_CONST;
	}
	//static String DoubleToCleanString(double value);
	//static void CleanPrint(int values, ...);

	static bool IsInfinite(float f) {
		return f == FLOAT_MAX;
	}

	static bool IsFinite(float f) {
		return f < FLOAT_MAX && f > -FLOAT_MAX;
	}

	static int Sign(float f) {
		return (0 < f) - (f < 0);
	}
};