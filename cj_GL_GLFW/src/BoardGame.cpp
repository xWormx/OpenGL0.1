#include "glad\glad.c"
#include "GLFW\glfw3.h"

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <stdint.h>

// 1 byte = 8 bits 		1111 1111
// 1 signed byte (char)
typedef int8_t 		i8;
// 1 unsigned byte (unsigned char)
typedef uint8_t 	u8;

// 2 signed bytes (short) 	1111 1111 1111 1111
typedef int16_t 	i16;
// 2 unsigned bytes (unsigned short) 
typedef uint16_t 	u16;

// 4 signed bytes (int) 	1111 1111 1111 1111	1111 1111 1111 1111
typedef int32_t 	i32;
// 4 unsigned bytes (unsigned int)
typedef uint32_t 	u32;

// 8 signed bytes (long long) 	1111 1111 1111 1111	1111 1111 1111 1111 	1111 1111 1111 1111	1111 1111 1111 1111
typedef int64_t 	i64;
// 8 unsigned bytes (unsigned long long)
typedef uint64_t 	u64;

#define STB_TRUETYPE_IMPLEMENTATION
#include "include\stb_truetype.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include\stb_image.h"

#include <CJGL_Input.h>
#include "include\CJ_Structs.h"
#include "include\CJ_GameBuffer.h"
#include "include\CJ_Window.h"
#include "include\CJ_Shader.h"
#include "include\CJ_Defines.h"


GLenum err;
bool running = true;

struct GameObject
{
	//float x;
	//float y;
	//float w;
	//float h;

	CJ_V3f pos;
	CJ_V2f dimension;

	u32 renderIndex;

	CJ_V2f texDiv;
	CJ_V2f texFrag;
	CJ_V4f solidColor;

	void init(float in_x, float in_y, float in_w, float in_h) 
	{
		pos.x = in_x;
		pos.y = in_y;
		dimension.w = in_w;
		dimension.h = in_h;
	}
};

#include "include\CJ_FuncDecl.h"
#include "include\CJ_FuncDef.h"

struct Player
{
	char txt_Territories[50] = {};
	char txt_Tanks[50] = {};
	char txt_GoldCollector[50] = {};
	char txt_Barracks[50] = {};
	char txt_Gold[50] = {};

	i32 Territories = 0;
	i32 Tanks = 0;
	i32 GoldCollector = 0;
	i32 Gold = 0;
	i32 Barracks = 0;



};
void writer(int *cursor, char *input, int textBufferSize)
{
	// Upper case Chars, not using tolower();
	for(int character = 0; character <= 512; character++)
	{
		if(keyPressed[character] && keyPressed[LEFT_SHIFT])
		{
			if((int)isPressed[character] == 0)
			{
				if(*cursor < textBufferSize)
				{
					if(keyPressed[SPACEBAR])
					{
						input[*cursor] = ' ';
						++*cursor;
					}
					else if(keyPressed[BACKSPACE])
					{
						if(*cursor > 0)
						{
							*cursor = *cursor - 1;
							input[*cursor] = ' ';
						}
					}
					else if(keyPressed[UP_ARROW])
					{
					}
					else if(keyPressed[DOWN_ARROW])
					{
					}
					else if(keyPressed[LEFT_ARROW])
					{
					}
					else if(keyPressed[RIGHT_ARROW])
					{
					}
					else
					{
						input[*cursor] = (char)character;
						++*cursor;
					}
				}

				isPressed[character] = 1;
			}
		}
		// Same code as above but with tolower((char)character); for lower case chars.
		else if(keyPressed[character])
		{
			if((int)isPressed[character] == 0)
			{
				if(*cursor < textBufferSize)
				{
					if(keyPressed[SPACEBAR])
					{
						input[*cursor] = ' ';
						++*cursor;
					}
					else if(keyPressed[BACKSPACE])
					{
						if(*cursor > 0)
						{
							*cursor = *cursor - 1;
							input[*cursor] = ' ';
						}
					}
					else if(keyPressed[UP_ARROW])
					{
					}
					else if(keyPressed[DOWN_ARROW])
					{
					}
					else if(keyPressed[LEFT_ARROW])
					{
					}
					else if(keyPressed[RIGHT_ARROW])
					{
					}
					else
					{
						input[*cursor] = tolower((char)character);
						++*cursor;
					}
				}

				isPressed[character] = 1;
			}

		}
		else
		{
			isPressed[character] = 0;
		}

	}
}




int main()
{
	printf("GROUND_TILE_FIRST: %d\n", GROUND_TILE_FIRST);
	printf("GROUND_TILE_LAST: %d\n", GROUND_TILE_LAST);
	printf("TEXT_FIRST: %d\n", TEXT_FIRST);
	printf("TEXT_LAST: %d\n", TEXT_LAST);
	printf("SELECT_MENU_FIRST: %d\n", SELECT_MENU_FIRST);
	printf("SELECT_MENU_LAST: %d\n", SELECT_MENU_LAST);

	printf("char: %d\n", 		sizeof(char));
	printf("short: %d\n",	 	sizeof(short));
	printf("int: %d\n",  	   	sizeof(int));
	printf("long long: %d\n", 	sizeof(long long));

	printf("i8: %d\n",  sizeof(i8));
	printf("i16: %d\n", sizeof(i16));
	printf("i32: %d\n", sizeof(i32));
	printf("i64: %d\n", sizeof(i64));


	srand(time(NULL));
	

	// SET LAST PARAMETER TO 1 FOR FULLSCREN
	GLFWwindow *window = windowSetup(SCR_WIDTH, SCR_HEIGHT, "BoardGame", 0);

	unsigned int shaderProgram 		= compileShaderProgram("shader\\vertexShader.glsl", "shader\\fragmentShader.glsl");
	unsigned int textShaderProgram 		= compileShaderProgram("shader\\vertexShader.glsl", "shader\\textFS.glsl");
	unsigned int solidColorRectProgram 	= compileShaderProgram("shader\\vertexShader.glsl", "shader\\solidColorFS.glsl");

	VBOBundle vboB = {};

	GLfloat MAIN_GAMEBUFFER[NUMOBJECTS * 16] = {};

	GLuint indexBuffer[NUMOBJECTS * 6] = {};

	//0, 1, 2, 2, 3, 0    4, 5, 6, 6, 7, 4

	for(int i = 0; i < NUMOBJECTS; i++)
	{
		indexBuffer[0 + i * 6] = i * 4 + 0;
		indexBuffer[1 + i * 6] = i * 4 + 1;
		indexBuffer[2 + i * 6] = i * 4 + 2;
		indexBuffer[3 + i * 6] = i * 4 + 2;
		indexBuffer[4 + i * 6] = i * 4 + 3;
		indexBuffer[5 + i * 6] = i * 4 + 0;
	}

	glGenVertexArrays(1, &vboB.VAO);
	glGenBuffers(1, &vboB.posVBO);
	glGenBuffers(1, &vboB.colVBO);
	glGenBuffers(1, &vboB.texVBO);
	glGenBuffers(1, &vboB.IBO);

	glBindVertexArray(vboB.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vboB.posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MAIN_GAMEBUFFER), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vboB.colVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MAIN_GAMEBUFFER), 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vboB.texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MAIN_GAMEBUFFER), 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboB.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

	GameBuffer gameBuffer[NUMOBJECTS] = {};
	for(int i = 0; i < NUMOBJECTS; i++)
	{
		gameBuffer[i].bufferIndex = i;
	}

	// LOADING TEXTURE FILES
	u8 *data[10] = {};
	TextureInfo textureInfo[10] = {};
	textureInfo[0].filePath = "..\\assets\\mount.bmp";
	textureInfo[1].filePath = "..\\assets\\myPng.png";
	textureInfo[2].filePath = "..\\assets\\Bases.png";
	
	
	for(int i = 0; i < 3; i++)
	{
		loadTextures(&textureInfo[i], &data[i]);
	}

	stbtt_bakedchar BCD[96] = {};
	unsigned char fontBuffer[30000000] = {};
	const int fontMapWidth  = 512;
	const int fontMapHeight = 512;
	float pixelHeight 	= 32.0f;
	unsigned char fontData[fontMapWidth*fontMapHeight] = {};

	FILE *hFontFile = fopen("C:\\windows\\fonts\\times.ttf", "rb");
//	FILE *hFontFile = fopen("C:\\windows\\fonts\\cour.ttf", "rb");
//	FILE *hFontFile = fopen("C:\\CJ_FONTS\\FFF_Tusj.ttf", "rb");
	if(!hFontFile)
	{
		printf("fail open file\n");
	}

	fread(fontBuffer, 1, sizeof(fontBuffer), hFontFile);
	stbtt_BakeFontBitmap(fontBuffer, 0, pixelHeight, fontData, fontMapWidth, fontMapHeight, 32, 126, BCD);

	fclose(hFontFile);
	
	// LOADING TEXTURE ID'S
	GLuint textureID[10] = {};
	for(int i = 0; i < 10; i++)
	{
		glGenTextures(1, &textureID[i]);
	}


	glBindTexture(GL_TEXTURE_2D, textureID[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureInfo[0].width, textureInfo[0].height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureID[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfo[1].width, textureInfo[1].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[1]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureID[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontMapWidth, fontMapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, fontData);

	glBindTexture(GL_TEXTURE_2D, textureID[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfo[2].width, textureInfo[2].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[2]);
	glGenerateMipmap(GL_TEXTURE_2D);



	stbi_image_free(data[0]);
	stbi_image_free(data[1]);
	stbi_image_free(data[2]);

	double mouseCurPosX = 0;
	double mouseCurPosY = 0;
	char DEBUGMouseCurPos[220] = {};
	char DEBUGFramTime[220] = {};
	char DEBUGWindowDim[220] = {};
	char DEBUGTimeSinceStart[220] = {};
	float *pPosData = 0;
	float *pColData = 0;
	float *pTexData = 0;
	int winWidth = 0;
	int winHeight = 0;
	int winX = 0;
	int winY = 0;
	int iColorLoc = glGetUniformLocation(shaderProgram, "iColorData");
	int winDimLoc = glGetUniformLocation(shaderProgram, "windowDimensions");
	int textWinDimLoc = glGetUniformLocation(textShaderProgram, "windowDimensions");
	int solidColWindDimLoc = glGetUniformLocation(solidColorRectProgram, "windowDimensions");
	float textX = 0.0f;

	gameBuffer[GROUND_MAP].createRect(0.0f, 0.0f, SCR_WIDTH * 1, SCR_HEIGHT * 1);
	gameBuffer[GROUND_MAP].setTextureDivision(1.0f, 1.0f);
	gameBuffer[GROUND_MAP].pickTextureFragment(0.0f, 0.0f);
	gameBuffer[GROUND_MAP].setSolidColor(1.0f, 1.0f, 1.0f, 0.5f); 


	for ( int y = 0; y < NUMTILES_16_y; y++)
	{
		for ( int x = 0; x < NUMTILES_16_x; x++)
		{
			static int index = 0;
			gameBuffer[GROUND_TILE_FIRST + index].createRect(0.0f + x * TILE_WIDTH, 0.0f +  y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
			gameBuffer[GROUND_TILE_FIRST + index].setTextureDivision(14.0f, 1.0f);
			gameBuffer[GROUND_TILE_FIRST + index].pickTextureFragment(3.0f, 0.0f);
			gameBuffer[GROUND_TILE_FIRST + index].setSolidColor(1.0f, 1.0f, 1.0f, 1.0f);
			index++;
		}
	}

	gameBuffer[SELECT_MENU_FIRST].createRect(0.0f, 0.0f, 100.0f, 100.0f);
	gameBuffer[SELECT_MENU_FIRST].setTextureDivision(14.0f, 1.0f);
	gameBuffer[SELECT_MENU_FIRST].pickTextureFragment(0.0f, 0.0f);
	gameBuffer[SELECT_MENU_FIRST].setSolidColor(1.0f, 1.0f, 1.0f, 1.0f); 

	float texPosX = 0.0f;
	float texPosY = 0.0f;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Clock clock = {};
	int MENU_STATE = MENU_STATE_MAIN;

	// Store the index range for the text quads
	textIndexRange DBGStartGametxt = {};
	textIndexRange DBGWinDim = {};
	textIndexRange DBGMouseCur = {};

	int rect1 = 10000;
	int rect1MovX = 0.0f;
	int rect1MovY = 0.0f;
	int rect2 = 10001;
	int rect2MovX = 0.0f;
	int rect2MovY = 0.0f;

	gameBuffer[rect1].createRect(0.0f, 0.0f, 100.0f, 200.0f);
	gameBuffer[rect1].setSolidColor(1.0, 0.0f, 0.0f, 1.0f);
	gameBuffer[rect2].createRect(600.0f, 600.0f, 100.0f, 100.0f);
	gameBuffer[rect2].setSolidColor(0.0, 1.0f, 0.0f, 1.0f);
	
	mapObject(rect1, rect2, vboB, gameBuffer);  

	char txt_ENTER[200] = {};
	char txt_INPUT[200] = {};
	int txt_CURSOR = 0;
	int textBufferSize = 200;

	bool pressed = false;
	char trueSTATE[10] = "false";


	int stateOfTheGame 	= 0;
	const int mainMenu 	= 1;
	const int newGame 	= 2;
	const int sandBox 	= 9;
	const int fontTileMap 	= 10;
//	stateOfTheGame = mainMenu;
	stateOfTheGame = mainMenu;
	


	GameObject mainBase[2] = {};
	GameObject smallBase[9] = {};
	GameObject road[14] = {};
	GameObject rightSideMenu = {};
	GameObject rightSideMenuCollisionRect[2] = {};

	Player player[2] = {};


	mainBase[0].init(200.0f, 450.f, 128.0f, 128.0f);
	mainBase[0].texDiv 	= cj_vec2f(8.0f, 16.0f);
	mainBase[0].texFrag 	= cj_vec2f(0.0f, 2.0f);

	mainBase[1].init(SCR_WIDTH - (128.0f + 200.0f), 450.f, 128.0f, 128.0f);
	mainBase[1].texDiv 	= cj_vec2f(8.0f, 16.0f);
	mainBase[1].texFrag 	= cj_vec2f(0.0f, 3.0f);


	i32 increment = 0;
	
	for(i32 i = 0; i < 3; i++)
	{
		smallBase[i].init((mainBase[0].pos.x + mainBase[0].dimension.w) * 1.7, 200.0f + (increment * 280.0f), 64.0f, 64.0f);
		smallBase[i].texDiv = cj_vec2f(8.0f, 16.0f);
		smallBase[i].texFrag = cj_vec2f(0.0f, 0.0f);

		increment++;
	}
	increment = 0;

	for(i32 i = 3; i < 6; i++)
	{
		smallBase[i].init((mainBase[0].pos.x + mainBase[0].dimension.w) * 2.8, 200.0f + (increment * 280.0f), 64.0f, 64.0f);
		smallBase[i].texDiv = cj_vec2f(8.0f, 16.0f);
		smallBase[i].texFrag = cj_vec2f(0.0f, 0.0f);

		increment++;
	}
	increment = 0;

	for(i32 i = 6; i < 9; i++)
	{
		smallBase[i].init((mainBase[0].pos.x + mainBase[0].dimension.w) * 3.9f, 200.0f + (increment * 280.0f), 64.0f, 64.0f);
		smallBase[i].texDiv = cj_vec2f(8.0f, 16.0f);
		smallBase[i].texFrag = cj_vec2f(0.0f, 0.0f);

		increment++;
	}
	increment = 0;

	for(i32 i = 0; i < 3; i++)
	{
		road[i].init((mainBase[0].pos.x + mainBase[0].dimension.w)  + 100.0f, 270.0f + (increment * 160.0f), 40.0f, 165.0f);
		road[i].texDiv = cj_vec2f(16.0f, 8.0f);            
		road[i].texFrag = cj_vec2f(2.0f, 0.0f);            

		increment++;                                       
	}                                                          
	increment = 0;                                             
                                                                   
	for(i32 i = 3; i < 6; i++)                                 
	{                                                          
		road[i].init((smallBase[0].pos.x + smallBase[0].dimension.w) + 130.0f, (smallBase[increment].pos.y) - (smallBase[increment].dimension.w/1.3), 40.0f, 165.0f);
		road[i].texDiv = cj_vec2f(16.0f, 8.0f);
		road[i].texFrag = cj_vec2f(2.0f, 0.0f);

		increment++;
	}
	increment = 0;

	for(i32 i = 6; i < 9; i++)
	{
		road[i].init((smallBase[4].pos.x + smallBase[4].dimension.w) + 130.0f, (smallBase[increment].pos.y) - (smallBase[increment].dimension.w/1.3), 40.0f, 165.0f);
		road[i].texDiv = cj_vec2f(16.0f, 8.0f);
		road[i].texFrag = cj_vec2f(2.0f, 0.0f);

		increment++;
	}
	increment = 0;

	for(i32 i = 9; i < 12; i++)
	{
		road[i].init(mainBase[1].pos.x - road[0].dimension.h/1.3, 270.0f + (increment * 160.0f), 40.0f, 165.0f);
		road[i].texDiv = cj_vec2f(16.0f, 8.0f);
		road[i].texFrag = cj_vec2f(2.0f, 0.0f);

		increment++;
	}

	rightSideMenu.init(SCR_WIDTH - 64.0f, 0.0f, 64.0f, SCR_HEIGHT - 300.0f);
	rightSideMenu.texDiv = cj_vec2f(16.0f, 2.0f);
	rightSideMenu.texFrag = cj_vec2f(11.0f, 1.0f);

	rightSideMenuCollisionRect[0].init(SCR_WIDTH - 64.0f, 0.0f, 64.0f, SCR_HEIGHT);
	rightSideMenuCollisionRect[1].init(SCR_WIDTH - (5.0f * 64.0f), 0.0f, 64.0f, SCR_HEIGHT);

	increment = 0;
	i32 menuOut = 0;

	Timer timer1 = {};
	Timer timer2 = {};
	// Timer -> SetStartTime -> (Restart) -> Begin -> Finished
	timer1.SetStartTime(3.0f);
	timer2.SetStartTime(5.0f);

	clock.start();
	while(!glfwWindowShouldClose(window) && running) 
	{
        	int currTxtIndex = TEXT_FIRST; 
		clock.start();


		glfwGetWindowSize(window, &winWidth, &winHeight);

		glUseProgram(shaderProgram);
		glUniform2f(winDimLoc, (float)winWidth, (float)winHeight);

		glUseProgram(textShaderProgram);
		glUniform2f(textWinDimLoc, (float)winWidth, (float)winHeight);

		glUseProgram(solidColorRectProgram);
		glUniform2f(solidColWindDimLoc, (float)winWidth, (float)winHeight);

		if(stateOfTheGame == fontTileMap)
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else
		{
			glClearColor(0.0f, 0.08f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		checkInput(window);
		glfwGetCursorPos(window, &mouseCurPosX, &mouseCurPosY);

		sprintf(DEBUGWindowDim, "Window Dimensions: %d/%d", winWidth, winHeight);
		sprintf(DEBUGMouseCurPos, "MouseCursor: %d/%d", (int)mouseCurPosX, (int)mouseCurPosY);
		sprintf(DEBUGFramTime, "miliSecs/frame: %0.2f", clock.getMiliSeconds());
		sprintf(DEBUGTimeSinceStart, "Time Since Start: %0.2fs", clock.timeSinceStart());

		glViewport(0.0f, 0.0f, winWidth, winHeight);


		if(mouseCurPosX >= winWidth - 1)
		{
			winX -= 10;
		}
		if(mouseCurPosY >= winHeight - 1)
		{
			winY += 10;
		}
		if(mouseCurPosX <= 1)
		{
			winX += 10;
		}
		if(mouseCurPosY <= 1)
		{
			winY -= 10;
		}

		if(winX >= 30.0f)
		{
			winX = 29.0f;
		}
	       	if(winX <= -400.0f)
		{
			winX = -399.0f;
		}
		if(winY >= 30.0f)
		{
			winY = 29.0f;
		}
	       	else if(winY <= -30.0f)
		{
			winY = -29.0f;
		}
		
		

		switch(stateOfTheGame)
		{ 
			// fontTileMap
			case fontTileMap:
				{
					if(keyPressed[ESC])
					{
						if(isPressed[ESC] == 0)
						{
							stateOfTheGame = mainMenu;
							isPressed[ESC] = 1;
						}
					}
					else
					{
						isPressed[ESC] = 0; 
					}


				//	gameBuffer[9000].createRect(0.0f, 0.0f, 1920.0f, 1080.0f);
				//	gameBuffer[9000].setTextureDivision(1.0f, 1.0f);
				//	gameBuffer[9000].pickTextureFragment(cj_vec2f(1.0f, 1.0f));
				//	mapObject(9000, 		9000, 			vboB, gameBuffer);
				//	render(textShaderProgram, 	textureID[2], 	9000, 		1); 


				clearTextBuffer(gameBuffer);



				if(currentLetter <= endofLine1)
				{
					char nextChar = Line1[currentLetter]; 
					if(currTime == delay)
					{
							strcat(delayedText, &nextChar);
							currentLetter++;
							currTime = 0;
					}
					else
					{
						currTime++;
					}
				}
				else
				{
					if(currentLetter2 <= endofLine2)
					{
						char nextChar2 = Line2[currentLetter2]; 
						if(currTime2 == delay2)
						{
								strcat(delayedText2, &nextChar2);
								currentLetter2++;
								currTime2 = 0;
						}
						else
						{
							currTime2++;
						}
					}
				}

				printText(BCD, gameBuffer, &currTxtIndex, 800.0f, 500.0f, delayedText, cj_vec4f(0.0f, 1.0f, 0.0f, 1.0f));
				printText(BCD, gameBuffer, &currTxtIndex, 800.0f, 520.0f, delayedText2, cj_vec4f(0.0f, 1.0f, 0.0f, 1.0f));

				mapObject(TEXT_FIRST, TEXT_LAST, vboB, gameBuffer);
				render(textShaderProgram, textureID[2], TEXT_FIRST, 2000);
				

				} break;

			// MAINMENU
			case mainMenu:
			{
				currentLetter = 0;
				strcpy(delayedText, "");
				currentLetter2 = 0;
				strcpy(delayedText2, "");
				gameBuffer[9000].createRect(0.0f, 0.0f ,0.0f, 0.0f);
				mapObject(9000, 9000, vboB, gameBuffer);
				if(keyPressed[ESC])
				{
					if(isPressed[ESC] == 0)
					{
						running = false;
						isPressed[ESC] = 1;
					}

				}
				else
				{
					isPressed[ESC] = 0; 
				}

				if(keyPressed['S'])
				{
					stateOfTheGame = sandBox;
				}

				clearTextBuffer(gameBuffer);

				DBGStartGametxt 	  = printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 400.0f, "START GAME", color4f(1.0f, 1.0f, 1.0f, 1.0f));
				textIndexRange DBGSandBox = printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 450.0f, "SANDBOX", color4f(1.0f, 1.0f, 1.0f, 1.0f));
				textIndexRange DBGfontTileMap = printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 650.0f,  "FONT TILEMAP", color4f(1.0f, 1.0f, 1.0f, 1.0f));
				printText(BCD, gameBuffer, &currTxtIndex, 1200.0f, 0.0f, DEBUGTimeSinceStart, color4f(1.0f, 1.0f, 1.0f, 1.0f));

				gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST].createRect(	 gameBuffer[DBGStartGametxt.start].getX(), 
							   				 gameBuffer[DBGStartGametxt.start].getY(), 
							   				(gameBuffer[DBGStartGametxt.end].getX() + gameBuffer[DBGStartGametxt.end].getWidth()) - gameBuffer[DBGStartGametxt.start].getX(), 
							   				 gameBuffer[DBGStartGametxt.start].getHeight()
										);

				gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST + 1].createRect(	 gameBuffer[DBGSandBox.start].getX(), 
							   				 gameBuffer[DBGSandBox.start].getY(), 
							   				(gameBuffer[DBGSandBox.end].getX() + gameBuffer[DBGSandBox.end].getWidth()) - gameBuffer[DBGSandBox.start].getX(), 
							   				 gameBuffer[DBGSandBox.start].getHeight()
										    );

				gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST + 2].createRect(	 gameBuffer[DBGfontTileMap.start].getX(), 
							   				 gameBuffer[DBGfontTileMap.start].getY(), 
							   				(gameBuffer[DBGfontTileMap.end].getX() + gameBuffer[DBGfontTileMap.end].getWidth()) - gameBuffer[DBGfontTileMap.start].getX(), 
							   				 gameBuffer[DBGfontTileMap.start].getHeight()
										    );


				if( checkMouseCursorCollision(	mouseCurPosX , 
								mouseCurPosY , 
								gameBuffer, TEXT_HIGHLIGHT_RECT_FIRST))  
				{
					clearTextBuffer(gameBuffer);

					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST].setSolidColor(0.0f, 0.0f, 0.0f, 1.0f);

					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 400.0f, 	 "START GAME", color4f(0.0f, 1.0f, 0.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 450.0f, 	 "SANDBOX", color4f(1.0f, 1.0f, 1.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 650.0f, 	 "FONT TILEMAP", color4f(1.0f, 1.0f, 1.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, 1200.0f, 0.0f, 		 DEBUGTimeSinceStart, color4f(1.0f, 1.0f, 1.0f, 1.0f));

				}
				else
				{
					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST].setSolidColor(0.0f, 0.0f, 0.0f, 0.4f);
				}

				if( checkMouseCursorCollision(	mouseCurPosX , 
								mouseCurPosY , 
								gameBuffer, TEXT_HIGHLIGHT_RECT_FIRST + 1))  
				{
					clearTextBuffer(gameBuffer);

					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST + 1].setSolidColor(0.0f, 0.0f, 0.0f, 1.0f);

					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 400.0f, 	 "START GAME", color4f(1.0f, 1.0f, 1.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 450.0f, 	 "SANDBOX", color4f(0.0f, 1.0f, 0.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 650.0f, 	 "FONT TILEMAP", color4f(1.0f, 1.0f, 1.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, 1200.0f, 0.0f, 		 DEBUGTimeSinceStart, color4f(1.0f, 1.0f, 1.0f, 1.0f));

				}
				else
				{
					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST + 1].setSolidColor(0.0f, 0.0f, 0.0f, 0.4f);
				}

				if( checkMouseCursorCollision(	mouseCurPosX , 
								mouseCurPosY , 
								gameBuffer, TEXT_HIGHLIGHT_RECT_FIRST + 2))  
				{
					clearTextBuffer(gameBuffer);

					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST + 2].setSolidColor(0.0f, 0.0f, 0.0f, 1.0f);

					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 400.0f, 	"START GAME", color4f(1.0f, 1.0f, 1.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 450.0f, 	"SANDBOX", color4f(1.0f, 1.0f, 1.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, winWidth/2.5f, 650.0f, 	"FONT TILEMAP", color4f(0.0f, 1.0f, 0.0f, 1.0f));
					printText(BCD, gameBuffer, &currTxtIndex, 1200.0f, 0.0f, 		DEBUGTimeSinceStart, color4f(1.0f, 1.0f, 1.0f, 1.0f));

				}
				else
				{
					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST + 2].setSolidColor(0.0f, 0.0f, 0.0f, 0.4f);
				}



				printText(BCD, gameBuffer, &currTxtIndex, textX + 0.0f,   0.0f, DEBUGWindowDim, color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				printText(BCD, gameBuffer, &currTxtIndex, textX + 600.0f, 0.0f, DEBUGMouseCurPos, color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				printText(BCD, gameBuffer, &currTxtIndex, textX + 900.0f, 0.0f, DEBUGFramTime, color4f(1.0f, 1.0f, 1.0f, 1.0f)); 

				mapObject(TEXT_HIGHLIGHT_RECT_FIRST, TEXT_HIGHLIGHT_RECT_LAST, vboB, gameBuffer);
				mapObject(TEXT_FIRST, TEXT_LAST, vboB, gameBuffer);

				if(keyPressed[LEFT_MOUSE_BUTTON])
				{
					if(checkMouseCursorCollision(mouseCurPosX , mouseCurPosY , gameBuffer, TEXT_HIGHLIGHT_RECT_FIRST))
					{
						stateOfTheGame = newGame;
					}
				}

				if(keyPressed[LEFT_MOUSE_BUTTON])
				{
					if(checkMouseCursorCollision(mouseCurPosX , mouseCurPosY , gameBuffer, TEXT_HIGHLIGHT_RECT_FIRST + 1))
					{
						stateOfTheGame = sandBox;
					}
				}

				if(keyPressed[LEFT_MOUSE_BUTTON])
				{
					if(checkMouseCursorCollision(mouseCurPosX , mouseCurPosY , gameBuffer, TEXT_HIGHLIGHT_RECT_FIRST + 2))
					{
						stateOfTheGame = fontTileMap;
					}
				}



				gameBuffer[MAIN_BASE_01].createRect(0.0f, 0.0f, 0.0f, 0.0f);
				gameBuffer[MAIN_BASE_02].createRect(0.0f, 0.0f, 0.0f, 0.0f);

				menuOut = 0;
				clearBuffer(gameBuffer, MAIN_BASE_START, MAIN_BASE_END); 
				clearBuffer(gameBuffer, SMALL_BASE_START, SMALL_BASE_END); 
				clearBuffer(gameBuffer, ROAD_START, RIGHT_SIDE_MENU); 

				
				mapObject(MAIN_BASE_START, MAIN_BASE_END, vboB, gameBuffer);
				mapObject(SMALL_BASE_START, SMALL_BASE_END, vboB, gameBuffer);
				mapObject(ROAD_START, RIGHT_SIDE_MENU, vboB, gameBuffer);

				render(solidColorRectProgram, 0, TEXT_HIGHLIGHT_RECT_FIRST, TEXT_HIGHLIGHT_RECT_LAST); 
				render(textShaderProgram, textureID[2], TEXT_FIRST, 2000); 
				render(shaderProgram, textureID[3], MAIN_BASE_START, RIGHT_SIDE_MENU); 




				timer1.Reset();
				timer2.Reset();
			} break;

			case newGame:
			{
				if(keyPressed[ESC])
				{
					if(isPressed[ESC] == 0)
					{
						stateOfTheGame = mainMenu;
						isPressed[ESC] = 1;
					}
				}
				else
				{
					isPressed[ESC] = 0; 
				}




				char txt_timer1[50] = {};
				char txt_timer2[50] = {};
				char txt_PlayerTurn[50] = {};
				char txt_PlayerInfo[50] = {};
				static i32 playerTurn = 1;

				timer1.Begin(clock.getSeconds());
				timer2.Begin(clock.getSeconds());

				static bool turnChanged = false;
				
				if(timer1.Finished())
				{
					if(!turnChanged)
					{
						if(playerTurn == 1)
						playerTurn = 2;
						else if(playerTurn == 2)
						playerTurn = 1;
						turnChanged = true;
					}
				}
				else
				{
					turnChanged = false;
				}


				if(playerTurn == 1)
				{
					if(checkMouseCursorGameObjectCollision(mouseCurPosX, mouseCurPosY, mainBase[0]))
					{
						static bool PRESSED = false;
						if(keyPressed[LEFT_MOUSE_BUTTON])
						{
							if(!PRESSED)
							{
								player[0].Territories++;
								player[0].GoldCollector -= 25;
								PRESSED = true;
							}
						}
						else
						{
							PRESSED = false;
						}
					}
				}


				sprintf(txt_timer1, 			"Time1 Left: %d", 	(i32)timer1.timeLeft);
				sprintf(txt_timer2, 			"Time2 Left: %d", 	(i32)timer2.timeLeft);
				sprintf(txt_PlayerTurn, 		"Player %d Turn", 	playerTurn);
				sprintf(txt_PlayerInfo, 		"Player %d Stats", 	playerTurn);
				sprintf(player[0].txt_Territories, 	"Territories: %d", 	player[0].Territories);
				sprintf(player[0].txt_Tanks, 		"Tanks: %d", 		player[0].Tanks);
				sprintf(player[0].txt_GoldCollector, 	"GoldCollector: %d", 	player[0].GoldCollector);
				sprintf(player[0].txt_Barracks, 	"Barracks: %d", 	player[0].Barracks);

				sprintf(player[1].txt_Territories, 	"Territories: %d", 	player[1].Territories);
				sprintf(player[1].txt_Tanks, 		"Tanks: %d", 		player[1].Tanks);
				sprintf(player[1].txt_GoldCollector, 	"GoldCollector: %d", 	player[1].GoldCollector);
				sprintf(player[1].txt_Barracks, 	"Barracks: %d", 	player[1].Barracks);
				
				clearTextBuffer(gameBuffer);
				//printText(BCD, gameBuffer, &currTxtIndex, textX + 	100.0f, 	10.0f, 	1.0f, DEBUGWindowDim, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				//printText(BCD, gameBuffer, &currTxtIndex, textX + 	600.0f, 10.0f, 	1.0f, DEBUGMouseCurPos, color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				//printText(BCD, gameBuffer, &currTxtIndex, textX + 	900.0f, 10.0f, 	1.0f, DEBUGFramTime, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 


				static float playerTurnScale = 1.0f;
				static float playerTurnFade = 1.0f;

				if(keyPressed[UP_ARROW])
				{
					playerTurnScale += 0.1f;
				}
				else if(keyPressed[DOWN_ARROW])
				{
					playerTurnScale -= 0.1f;
				}

				if(playerTurnFade <= 0.0f)
				{
					playerTurnFade = 1.0f;
				}
				else
				{
					//playerTurnFade -= 0.01f;
				}

				printText(BCD, gameBuffer, &currTxtIndex, 		0.0f, 100.0f, txt_timer1, color4f(1.0f, 1.0f, 1.0f, 1.0f));
				printText(BCD, gameBuffer, &currTxtIndex, 		0.0f, 150.0f, txt_timer2, color4f(1.0f, 1.0f, 1.0f, 1.0f));
				printText(BCD, gameBuffer, &currTxtIndex, 		0.0f, 50.0f,  txt_PlayerTurn, color4f(1.0f, 1.0f, 1.0f, playerTurnFade));

				mapObject(TEXT_FIRST, TEXT_LAST, vboB, gameBuffer);

				if(menuOut == 0 && !checkMouseCursorGameObjectCollision(mouseCurPosX, mouseCurPosY, rightSideMenuCollisionRect[0]))
				{
					float newMenuWidth = 64.0f;
					float newTexDivX = 16.0f;

					rightSideMenu.pos.x = winWidth - 64.0f;
					rightSideMenu.dimension.w = newMenuWidth;

					rightSideMenu.texDiv.x = newTexDivX;
					rightSideMenu.texFrag.x = 10.0f;

				}
				else if (menuOut == 0 && checkMouseCursorGameObjectCollision(mouseCurPosX, mouseCurPosY, rightSideMenuCollisionRect[0]))
				{
					rightSideMenu.texFrag.x = 10.0f;

					if(keyPressed[LEFT_MOUSE_BUTTON] && menuOut == 0) 
					{
						menuOut = 1;

						float newMenuWidth = 64.0f * 5.0f + 150.0f;
						float newTexDivX = 16.0f / 5.0f;
						rightSideMenu.dimension.w = newMenuWidth;
						rightSideMenu.pos.x = winWidth - newMenuWidth;
						rightSideMenuCollisionRect[1].pos.x = rightSideMenu.pos.x;

						rightSideMenu.texDiv.x = newTexDivX;
						rightSideMenu.texFrag.x = 0.0f;

					}
				} 
				else if(menuOut == 1)
				{

					if(playerTurn == 1)
					{
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 130.0f, 30.0f,  txt_PlayerInfo, 			cj_vec4f(1.0f, 0.0f, 0.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 70.0f,   player[0].txt_Territories, 	cj_vec4f(1.0f, 0.0f, 0.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 100.0f,  player[0].txt_Tanks, 		cj_vec4f(1.0f, 0.0f, 0.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 130.0f,  player[0].txt_GoldCollector, 	cj_vec4f(1.0f, 0.0f, 0.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 160.0f,  player[0].txt_Barracks, 		cj_vec4f(1.0f, 0.0f, 0.0f, 1.0f));
					}
					else if(playerTurn == 2)
					{
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 130.0f, 30.0f,  txt_PlayerInfo, 			cj_vec4f(0.3f, 0.3f, 1.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 70.0f,  player[1].txt_Territories, 	cj_vec4f(0.3f, 1.0f, 1.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 100.0f,  player[1].txt_Tanks, 		cj_vec4f(0.3f, 0.3f, 1.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 130.0f,  player[1].txt_GoldCollector, 	cj_vec4f(0.3f, 0.3f, 1.0f, 1.0f));
						printText(BCD, gameBuffer, &currTxtIndex, rightSideMenu.pos.x + 30.0f, 160.0f,  player[1].txt_Barracks, 		cj_vec4f(0.3f, 0.3f, 1.0f, 1.0f));
					}
					if(checkMouseCursorGameObjectCollision(mouseCurPosX, mouseCurPosY, rightSideMenuCollisionRect[1]))
					{
						rightSideMenu.texFrag.x = 0.0f;

						if(keyPressed[LEFT_MOUSE_BUTTON])
						{
							menuOut = 0;
							float newMenuWidth = 64.0f;
							float newTexDivX = 16.0f;

							rightSideMenu.pos.x = winWidth - 64.0f;
							rightSideMenu.dimension.w = newMenuWidth;

							rightSideMenu.texDiv.x = newTexDivX;
							rightSideMenu.texFrag.x = 10.0f;
						}

					}
					else
					{
						rightSideMenu.texFrag.x = 0.0f;
					}
				}

				rightSideMenu.dimension.h = winHeight;
				rightSideMenuCollisionRect[0].dimension.h = winHeight;
				rightSideMenuCollisionRect[1].dimension.h = winHeight;

				i32 mainBaseIndex = 0;
				for(i32 i = MAIN_BASE_01; i <= MAIN_BASE_02; i++)
				{
					gameBuffer[i].createRect(mainBase[mainBaseIndex].pos.x + winX, mainBase[mainBaseIndex].pos.y - winY, mainBase[mainBaseIndex].dimension.w, mainBase[mainBaseIndex].dimension.h);
					gameBuffer[i].setTextureDivision(mainBase[mainBaseIndex].texDiv);
					gameBuffer[i].pickTextureFragment(mainBase[mainBaseIndex].texFrag);
					mainBaseIndex++;
				}	

				i32 smallBaseIndex = 0;
				for (i32 i = SMALL_BASE_01; i <= SMALL_BASE_09; i++)
				{

					gameBuffer[i].createRect(smallBase[smallBaseIndex].pos.x + winX, smallBase[smallBaseIndex].pos.y - winY, smallBase[smallBaseIndex].dimension.w, smallBase[smallBaseIndex].dimension.h); 
					gameBuffer[i].setTextureDivision(smallBase[smallBaseIndex].texDiv);
					gameBuffer[i].pickTextureFragment(smallBase[smallBaseIndex].texFrag);
					smallBaseIndex++;

				}
				i32 roadIndex = 0;
				for(i32 i = ROAD_01; i <= ROAD_12; i++)
				{
					gameBuffer[i].createRect(road[roadIndex].pos.x + winX, road[roadIndex].pos.y - winY, road[roadIndex].dimension.w, road[roadIndex].dimension.h); 
					gameBuffer[i].setTextureDivision(road[roadIndex].texDiv);
					gameBuffer[i].pickTextureFragment(road[roadIndex].texFrag);
					roadIndex++;

				}

				gameBuffer[ROAD_01].rotate(1.0f);
				gameBuffer[ROAD_02].rotate(1.5708f);
				gameBuffer[ROAD_03].rotate(2.0f);

				gameBuffer[ROAD_04].rotate(1.5708f);
				gameBuffer[ROAD_05].rotate(1.5708f);
				gameBuffer[ROAD_06].rotate(1.5708f);

				gameBuffer[ROAD_07].rotate(1.5708f);
				gameBuffer[ROAD_08].rotate(1.5708f);
				gameBuffer[ROAD_09].rotate(1.5708f);

				gameBuffer[ROAD_10].rotate(2.2f);
				gameBuffer[ROAD_11].rotate(1.5708f);
				gameBuffer[ROAD_12].rotate(1.0f);

				gameBuffer[RIGHT_SIDE_MENU].createRect(rightSideMenu.pos.x, rightSideMenu.pos.y, rightSideMenu.dimension.w, rightSideMenu.dimension.h);
				gameBuffer[RIGHT_SIDE_MENU].setTextureDivision(rightSideMenu.texDiv);
				gameBuffer[RIGHT_SIDE_MENU].pickTextureFragment(rightSideMenu.texFrag);


				// CHECK IN WHICH ORER OBJECTS ARE DEFINED IN CJ_Defines.h
				mapObject(TEXT_FIRST, 		RIGHT_SIDE_MENU, 		vboB, gameBuffer);

				render(shaderProgram, 		textureID[3], 	MAIN_BASE_01, 	7); 
				render(0, 			textureID[3], 	SMALL_BASE_01, 	9); 
				render(0, 			textureID[3], 	ROAD_01, 	14); 
				render(0, 			textureID[3],	RIGHT_SIDE_MENU, 1); 
				render(textShaderProgram, 	textureID[2], 	TEXT_FIRST, 	2000); 

			} break;

			// SANDBOX
			case sandBox:
			{
				// SET ALL OBJECTS X AND Y TO (+ winX) ( +winY) IF OBJECTS MOVE
				if(mouseCurPosX >= winWidth - 1)
				{
					winX -= 1000;
				}
				if(mouseCurPosY >= winHeight + 1)
				{
					winY += 1000;
				}
				if(mouseCurPosX <= 1)
				{
					winX += 1000;
				}
				if(mouseCurPosY <= 1)
				{
					winY -= 1000;
				}
				if(keyPressed[ESC])
				{
					if(isPressed[ESC] == 0)
					{
						stateOfTheGame = mainMenu;
						isPressed[ESC] = 1;
					}
				}
				else
				{
					isPressed[ESC] = 0; 
				}

				if(keyPressed[LEFT_MOUSE_BUTTON])
				{
					for(int i = 0; i < (GROUND_TILE_LAST); i++)
					{
						if(checkMouseCursorCollision(mouseCurPosX - winX, mouseCurPosY + winY, gameBuffer, GROUND_TILE_FIRST + i))
						{
							gameBuffer[GROUND_TILE_FIRST + i].pickTextureFragment(5.0f, 0.0f);
						}
					}
				}

				for(int i = 0; i < (GROUND_TILE_LAST); i++)
				{
					gameBuffer[i].setX(gameBuffer[i].getX() + winX * clock.getSeconds());
					gameBuffer[i].setY(gameBuffer[i].getY() - winY * clock.getSeconds());
				}
				winX = 0.0f;
				winY = 0.0f;

				gameBuffer[SELECT_MENU_FIRST].setX(-winX);
				gameBuffer[SELECT_MENU_FIRST].setY(winY);

				if(keyPressed['S'])
				{
					MENU_STATE = MENU_STATE_MAIN;
				}

				if(pressed)
				{
					sprintf(trueSTATE, "true");
				}
				else
				{
					sprintf(trueSTATE, "false");
				}

				clearTextBuffer(gameBuffer);

				char txt_rectCorners[100] = "";
				CJ_rectCorners_X rect1Corners = gameBuffer[rect1].getCornersX();
				sprintf(txt_rectCorners, "x1: %0.2f x2: %0.2f, x3: %0.2f x4: %0.2f\0", rect1Corners.x1, rect1Corners.x2, rect1Corners.x3, rect1Corners.x4);  

				writer(&txt_CURSOR, txt_INPUT, textBufferSize);
				sprintf(txt_ENTER, txt_INPUT);

				DBGStartGametxt = printText(BCD, gameBuffer, &currTxtIndex, 400.0f, 400.0f, "START GAME", color4f(1.0f, 1.0f, 1.0f, 1.0f));
				printText(BCD, gameBuffer, &currTxtIndex, textX + 	100.0f,	0.0f, 	DEBUGWindowDim, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				printText(BCD, gameBuffer, &currTxtIndex, textX + 	600.0f, 0.0f, 	DEBUGMouseCurPos, color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				printText(BCD, gameBuffer, &currTxtIndex, textX + 	900.0f, 0.0f, 	DEBUGFramTime, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				printText(BCD, gameBuffer, &currTxtIndex, 		1200.0f,   0.0f, DEBUGTimeSinceStart, color4f(1.0f, 1.0f, 1.0f, 1.0f));
				printText(BCD, gameBuffer, &currTxtIndex, 		900.0f, 500.0f,  trueSTATE, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				printText(BCD, gameBuffer, &currTxtIndex, 		900.0f, 200.0f,  txt_ENTER, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 
				printText(BCD, gameBuffer, &currTxtIndex, 		900.0f, 600.0f,  txt_rectCorners, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 

				rect1Corners = gameBuffer[rect2].getCornersX();
				sprintf(txt_rectCorners, "x1: %0.2f x2: %0.2f, x3: %0.2f x4: %0.2f\0", rect1Corners.x1, rect1Corners.x2, rect1Corners.x3, rect1Corners.x4);  
				printText(BCD, gameBuffer, &currTxtIndex, 		900.0f, 700.0f, txt_rectCorners, 	color4f(1.0f, 1.0f, 1.0f, 1.0f)); 


				gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST].createRect(gameBuffer[DBGStartGametxt.start].getX(), 
										   gameBuffer[DBGStartGametxt.start].getY(), 
										   (gameBuffer[DBGStartGametxt.end].getX() + gameBuffer[DBGStartGametxt.end].getWidth()) - gameBuffer[DBGStartGametxt.start].getX(), 
										   gameBuffer[DBGStartGametxt.start].getHeight());



				if( checkMouseCursorCollision(	mouseCurPosX - winX, 
								mouseCurPosY + winY, 
								gameBuffer, TEXT_HIGHLIGHT_RECT_FIRST))  
				{
					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST].setSolidColor(0.0f, 0.0f, 0.0f, 1.0f);
					DBGStartGametxt = printText(BCD, gameBuffer, &currTxtIndex, 400.0f, 400.0f, "START GAME", color4f(0.0f, 1.0f, 0.0f, 1.0f));
					mapObject(TEXT_FIRST, TEXT_LAST, vboB, gameBuffer);
				}
				else
				{
					gameBuffer[TEXT_HIGHLIGHT_RECT_FIRST].setSolidColor(0.0f, 0.0f, 0.0f, 0.0f);
				}

				mapObject(TEXT_FIRST, TEXT_LAST, vboB, gameBuffer);
				mapObject(TEXT_HIGHLIGHT_RECT_FIRST, TEXT_HIGHLIGHT_RECT_LAST, vboB, gameBuffer);
				mapObject(GROUND_MAP, GROUND_MAP, vboB, gameBuffer);


				if(keyPressed[UP_ARROW])
				{
					rect1MovY -= 3;
				}
				if(keyPressed[DOWN_ARROW])
				{
					rect1MovY += 3;
				}

				static float radian = 0.0f;

				if(keyPressed[RIGHT_ARROW])
				{
					rect1MovX += 3;
				}
				if(keyPressed[LEFT_ARROW])
				{
					rect1MovX -= 3;
				}
				if(keyPressed[SPACEBAR])
				{
					radian += 0.05f;
					printf("%s\n", txt_ENTER);
				}

				gameBuffer[rect1].setX(gameBuffer[rect1].getX() + rect1MovX);
				gameBuffer[rect1].setY(gameBuffer[rect1].getY() + rect1MovY);
				gameBuffer[rect1].rotate(radian);
				gameBuffer[rect2].rotate(radian);

				gameBuffer[GROUND_TILE_FIRST + 30].setX(gameBuffer[GROUND_TILE_FIRST + 30].getX() + rect1MovX);
				gameBuffer[GROUND_TILE_FIRST + 30].setY(gameBuffer[GROUND_TILE_FIRST + 30].getY() + rect1MovY);

				for(int i = 1; i < GROUND_TILE_LAST; i++)
				{
					gameBuffer[i].rotate(radian);
				}

				

				if(checkRectCollision(rect1, rect2, gameBuffer))
				{
					gameBuffer[rect1].setSolidColor(0.0f, 1.0f, 0.0f, 1.0f);
					gameBuffer[rect1].setColorOfCorners(1.0f, 1.0f, 1.0f, 1.0f);
					
				}
				else
				{
					gameBuffer[rect1].setSolidColor(1.0f, 0.0f, 0.0f, 1.0f);
					gameBuffer[rect1].setColorOfCorners(1.0f, 1.0f, 1.0f, 1.0f);
				}

				rect1MovY = 0;
				rect1MovX = 0;


				mapObject(0, 20000, vboB, gameBuffer);

				render(shaderProgram, textureID[1], GROUND_MAP, 1);
				render(0, textureID[0], GROUND_TILE_FIRST, GROUND_TILE_LAST);
				render(solidColorRectProgram, 0, TEXT_HIGHLIGHT_RECT_FIRST, 1);
				render(shaderProgram, textureID[0], SELECT_MENU_FIRST, 1);
				render(textShaderProgram, textureID[2], TEXT_FIRST, 1000);
				render(solidColorRectProgram, 0, rect1, 2);

			} break;

		}




		// In the render function, use shaderProgram ( pass 0 to keep active shaderProgram ), textureID to bind, start Index to draw and N objects to draw
		// Always render the lowest index first and the highest index last

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	for(int i = 0; i < sizeof(textureID) / sizeof(GLuint); i++)
	{
		glDeleteTextures(1, &textureID[i]);
	}

	glDeleteProgram(shaderProgram);
	glDeleteProgram(textShaderProgram);
	glDeleteProgram(solidColorRectProgram);
	glDeleteBuffers(1, &vboB.posVBO);
	glDeleteBuffers(1, &vboB.colVBO);
	glDeleteBuffers(1, &vboB.texVBO);
	glDeleteBuffers(1, &vboB.IBO);
	glDeleteVertexArrays(1, &vboB.VAO);
	
	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}

