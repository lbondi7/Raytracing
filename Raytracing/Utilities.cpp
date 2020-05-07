#include "Utilities.h"

std::random_device Utilities::rd;

void Utilities::Translate(const Matrix& m, Vec3& v)
{
	v.axis[0] = (m.data[0][0] * v.axis[0]) + (m.data[0][1] * v.axis[1]) + (m.data[0][2] * v.axis[2]) + m.data[0][3];
	v.axis[1] = (m.data[1][0] * v.axis[0]) + (m.data[1][1] * v.axis[1]) + (m.data[1][2] * v.axis[2]) + m.data[1][3];
	v.axis[2] = (m.data[2][0] * v.axis[0]) + (m.data[2][1] * v.axis[1]) + (m.data[2][2] * v.axis[2]) + m.data[2][3];
}

void Utilities::Translate(const Matrix& m, Vec4& v)
{
	v.axis[0] = (m.data[0][0] * v.axis[0]) + (m.data[0][1] * v.axis[1]) + (m.data[0][2] * v.axis[2]) + m.data[0][3];
	v.axis[1] = (m.data[1][0] * v.axis[0]) + (m.data[1][1] * v.axis[1]) + (m.data[1][2] * v.axis[2]) + m.data[1][3];
	v.axis[2] = (m.data[2][0] * v.axis[0]) + (m.data[2][1] * v.axis[1]) + (m.data[2][2] * v.axis[2]) + m.data[2][3];
	v.axis[3] = (m.data[3][0] * v.axis[0]) + (m.data[3][1] * v.axis[1]) + (m.data[3][2] * v.axis[2]) + m.data[3][3];
}

void Utilities::Scale(const Matrix& m, Vec3& v)
{
	v.axis[0] = (m.data[0][0] * v.axis[0]) + (m.data[0][1] * v.axis[1]) + (m.data[0][2] * v.axis[2]) * m.data[0][3];
	v.axis[1] = (m.data[1][0] * v.axis[0]) + (m.data[1][1] * v.axis[1]) + (m.data[1][2] * v.axis[2]) * m.data[1][3];
	v.axis[2] = (m.data[2][0] * v.axis[0]) + (m.data[2][1] * v.axis[1]) + (m.data[2][2] * v.axis[2]) * m.data[2][3];
}

void Utilities::Scale(const Matrix& m, Vec4& v)
{
	v.axis[0] = (m.data[0][0] * v.axis[0]) + (m.data[0][1] * v.axis[1]) + (m.data[0][2] * v.axis[2]) * m.data[0][3];
	v.axis[1] = (m.data[1][0] * v.axis[0]) + (m.data[1][1] * v.axis[1]) + (m.data[1][2] * v.axis[2]) * m.data[1][3];
	v.axis[2] = (m.data[2][0] * v.axis[0]) + (m.data[2][1] * v.axis[1]) + (m.data[2][2] * v.axis[2]) * m.data[2][3];
	v.axis[3] = (m.data[3][0] * v.axis[0]) + (m.data[3][1] * v.axis[1]) + (m.data[3][2] * v.axis[2]) * m.data[3][3];
}

float Utilities::Randomf(float min, float max)
{
	std::uniform_real_distribution<float> rand(min, max);
	return rand(rd);
}

const Vec3& Utilities::RandomVec3(const Vec3& min, const Vec3& max)
{
	std::uniform_real_distribution<float> randX(min.x(), max.x());
	std::uniform_real_distribution<float> randY(min.y(), max.y());
	std::uniform_real_distribution<float> randZ(min.z(), max.z());
	Vec3 v(randX(rd), randY(rd), randZ(rd));
	return v;
}
