#define LEFT_ARROW 511
#define RIGHT_ARROW 510
#define UP_ARROW 509
#define DOWN_ARROW 508
#define SPACEBAR 507

bool keyPressed[512] = {}; 


void checkInput(GLFWwindow *window);
void processInput(Brick playerbrick);
