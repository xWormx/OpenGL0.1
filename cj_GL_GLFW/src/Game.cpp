#include "Game.h"

void matrix_makeIdentity(float matrix4x4[4][4])
{
		matrix4x4[0][0] = 1.0f;
		matrix4x4[0][1] = 0.0f;
		matrix4x4[0][2] = 0.0f;
		matrix4x4[0][3] = 0.0f;

		matrix4x4[1][0] = 0.0f;
		matrix4x4[1][1] = 1.0f;
		matrix4x4[1][2] = 0.0f;
		matrix4x4[1][3] = 0.0f;

		matrix4x4[2][0] = 0.0f;
		matrix4x4[2][1] = 0.0f;
		matrix4x4[2][2] = 1.0f;
		matrix4x4[2][3] = 0.0f;

		matrix4x4[3][0] = 0.0f;
		matrix4x4[3][1] = 0.0f;
		matrix4x4[3][2] = 0.0f;
		matrix4x4[3][3] = 1.0f;
}

void matrix_setWidth(float matrix4x4[4][4], float width)
{
	matrix4x4[0][0] += width;
	matrix4x4[0][3] += width;
}
