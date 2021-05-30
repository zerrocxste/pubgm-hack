#include "../../includes.h"

Vector Rotate(Vector& src, float rotate)
{
	/*float x1 = src.x * cos(rotate) - src.y * sin(rotate);
	float y1 = src.x * sin(rotate) + src.y * cos(rotate);*/

	float z1 = src.z * sin(rotate);

	return Vector(src.x, src.y, z1);
}