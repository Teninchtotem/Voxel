///////////////////////////////////////////////////////////////////////////////
// ModelGL.h
// =========
// Model component of OpenGL
// 
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-10
// UPDATED: 2013-11-27
///////////////////////////////////////////////////////////////////////////////

#ifndef MODEL_GL_H
#define MODEL_GL_H
#include "CA\CASimulator.h"
#include "GL\glut.h"
#include <time.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>


class ModelGL
{

protected:
	// members
	float diff = 0.3;
	HANDLE  thread;
	DWORD   threadId;
	bool safeToDraw;
	bool drawing;
	mutex drawMutex;
	int drawHideBuffer;
	bool threeD;
	float colours[100][3];
	int selectedID;
	bool tempCameraReset;
	bool drawCompleteFromLastStep;
	float tempCameraInfo[5];
	int windowWidth;
	int windowHeight;
	float startDistance;
	float width;
	float height;
	float depth;
	int longestInt;
	float shortest;
	bool disco;
	bool animateFlag;
	unsigned int listId;                            // display list ID
	bool mouseLeftDown;
	bool mouseRightDown;
	bool mouseMDown;
	int mouseX;
	int mouseY;
	int frameTick;
	int frameCounter;
	float speed;
	float cameraX;
	float cameraY;
	float cameraAngleX;
	float cameraAngleY;
	float cameraDistance;
	float bgColor[4];
	float cellColor[4];
	float gridColor[3];
	bool bgFlag;
	bool cellFlag;
	bool lines;
	bool windowResized;
	unsigned char* frameBuffer;                     // framebuffer to store RGBA color
	int bufferSize;


public:
    ModelGL();
	~ModelGL();

	CASimulator	simulator;

    void init();                                    // initialize OpenGL states
    void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
    void setViewport(int width, int height);
    void resizeWindow(int width, int height);
	void draw();
	void drawSquareGrid();
	void drawHexGrid();
	void drawTriangleGrid();
	void drawCells();
	void drawHexCells();
	void drawTriangleCells();
	void update();
	
	void toggleDisco();
	void calculateLineColour();
	bool getAnimateFlag() { return animateFlag; };
	void setSelectedID(int in) { selectedID = in; };
	int getSelectedID() { return selectedID; };
	float* getIDcolourRGB(int id) { return colours[id]; };
	void setMouseLeft(bool flag) { mouseLeftDown = flag; if (flag){ mouseMDown = mouseRightDown = false; } };
	void setMouseRight(bool flag) { mouseRightDown = flag; if (flag){ mouseMDown = mouseLeftDown = false; } };
	void setMouseM(bool flag) { mouseMDown = flag; if (flag){ mouseRightDown = mouseLeftDown = false; } };
	HANDLE getUpdateHandle() { return thread; };
	mutex *getDrawMutex(){ return &drawMutex; };
	void setSpeed(float speedIn) { speed = speedIn; };
	float getSpeed(float speedIn) { return speed; };
    void setMousePosition(int x, int y) { mouseX = x; mouseY = y; };
	void animate(bool flag) { animateFlag = flag; };

	void cancelDrawMutex() {  };

	void scaleZoom(float scaleIn);
	void moveCamera(float x, float y, float z);

	void resetCamera();
    void rotateCamera(int x, int y);
    void zoomCamera(int dist);

	void toggleGrid();

	void setBackgroundRed(float value);             // change background colour
	void setBackgroundGreen(float value);
	void setBackgroundBlue(float value);
	void setCellRed(float value);             // change cell colour
	void setCellGreen(float value);
	void setCellBlue(float value);
	void setSafeToDraw(bool in);
	void setDrawing(bool in);
	void setThreeD(bool in);
	void calculateDrawInfo();
	void calculateHexDrawInfo();
	void setTempCamera(bool in);
	void setDrawHideBuffer(int in);
	void alterDrawHideBuffer(int in); 
	bool setTwoDimentionalTessellation(int in);                              // framebuffer size in bytes
};
#endif
