#include "Input.h"


void checkInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		keyPressed[SPACEBAR] = true;	
	} 
	else 
	{	
		keyPressed[SPACEBAR] = false;	
	}	

	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		keyPressed[LEFT_ARROW] = true;	
	} 
	else 
	{	
		keyPressed[LEFT_ARROW] = false;	
	}	

	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		keyPressed[RIGHT_ARROW] = true;	
	}
	else
	{
		keyPressed[RIGHT_ARROW] = false;	
	}

	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		keyPressed[UP_ARROW] = true;	
	}
	else
	{
		keyPressed[UP_ARROW] = false;	
	}
	
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		keyPressed[DOWN_ARROW] = true;	
	}
	else
	{
		keyPressed[DOWN_ARROW] = false;	
	}

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		keyPressed['W'] = true;	
	}
	else
	{
		keyPressed['W'] = false;	
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		keyPressed['D'] = true;	
	}
	else
	{
		keyPressed['D'] = false;	
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		keyPressed['A'] = true;	
	}
	else
	{
		keyPressed['A'] = false;	
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		keyPressed['S'] = true;	
	}
	else
	{
		keyPressed['S'] = false;	
	}

	if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		keyPressed['P'] = true;	
	}
	else
	{
		keyPressed['P'] = false;	
	}
	if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		keyPressed['O'] = true;	
	}
	else
	{
		keyPressed['O'] = false;	
	}
	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		keyPressed['X'] = true;	
	}
	else
	{
		keyPressed['X'] = false;	
	}
	if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		keyPressed['Y'] = true;	
	}
	else
	{
		keyPressed['Y'] = false;	
	}
}

void processInput(Brick *playerbrick)
{
		if(keyPressed['W'])
		{
				playerbrick->moveHeight(0.01f);
		}
		if(keyPressed['S'])
		{
				playerbrick->moveHeight(-0.01f);
		}
		if(keyPressed['D'])
		{
				playerbrick->moveWidth(0.01f);
		}
		if(keyPressed['A'])
		{
				playerbrick->moveWidth(-0.01f);
		}
		if(keyPressed['X'])
		{
		}
		if(keyPressed['Y'])
		{
		}
		if(keyPressed['O'])
		{
				playerbrick->setWidth(0.03f);
		}
		if(keyPressed['P'])
		{
				printf("\nplayerbrick->width: \t%0.2f\n", playerbrick->getWidth());
				printf("playerbrick->height: \t%0.2f\n", playerbrick->getHeight());
				printf("playerbrick->x: \t%0.2f\n", playerbrick->getX());
				printf("playerbrick->y: \t%0.2f\n", playerbrick->getY());
		}

		if(keyPressed[UP_ARROW])
		{
			playerbrick->moveY(0.03f);
		}
		if(keyPressed[DOWN_ARROW])
		{
			playerbrick->moveY(-0.03f);
		}
		if(keyPressed[RIGHT_ARROW])
		{
			playerbrick->moveX(0.03f);
		}
		if(keyPressed[LEFT_ARROW])
		{
			playerbrick->moveX(-0.03f);
		}

		if(keyPressed[SPACEBAR])
		{
				for(int i = 0; i < 4; i++)
				{
					for(int j = 0; j < 4; j++)
					{
						printf("%0.2f ", playerbrick->matrix[i][j]);			
					}

					printf("\n");
				}
					printf("\n");
		}

}
