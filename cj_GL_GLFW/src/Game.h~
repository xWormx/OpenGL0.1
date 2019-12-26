
void matrix_makeIdentity(float matrix4x4[4][4]);
void matrix_setWidth(float matrix4x4[4][4], float width);

struct Brick
{
	float matrix[4][4];

	float x 		= matrix[0][0];
	float y 		= matrix[1][0];
	float width 	= matrix[0][0];
	float height 	= matrix[1][3];


	float getX()
	{
		return matrix[0][3] - matrix[0][0];
	}

	float getY()
	{
		return matrix[1][3] - matrix[1][1];
	}

	float getWidth()
	{
		return matrix[0][0];
	}

	float getHeight()
	{
		return matrix[1][1];
	}

	void setX(float value)
	{
		matrix[0][3] = value + 1.0f; 
	}

	void setY(float value)
	{
		matrix[1][3] = value + 1.0f; 
	}
	void moveX(float value)
	{
		matrix[0][3] += value; 
	}

	void moveY(float value)
	{
		matrix[1][3] += value; 
	}

	void setWidth(float value)
	{
		matrix[0][0]  += value - 1.0f;
		matrix[0][3]  += value - 1.0f;
	}

	void setHeight(float value)
	{
		matrix[1][1]  += value - 1.0f;
		matrix[1][3]  += value - 1.0f;
	}

	void moveHeight(float value)
	{
		matrix[1][1]  += value;
		matrix[1][3]  += value;
	}
	void moveWidth(float value)
	{
		matrix[0][0]  += value;
		matrix[0][3]  += value;
	}

};
