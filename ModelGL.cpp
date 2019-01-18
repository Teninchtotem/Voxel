///////////////////////////////////////////////////////////////////////////////
// ModelGL.cpp
// ===========
// Model component of OpenGL
// 
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-10
// UPDATED: 2013-11-27
///////////////////////////////////////////////////////////////////////////////

#if _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif
#include "glew.h"
#include "freeglut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "ModelGL.h"
#include "Log.h"
#include "Bmp.h"

#define GRID_TRIANGLES false
#define LIMIT_STEPS_TO_FRAMES false
#define THREADED_UPDATE true

static float quadPositionArray[] = {

	//Front face
	-1.0f, -1.0f, 0, 1.0f,
	1.0f, -1.0f, 0, 1.0f,
	-1.0f, 1.0f, 0, 1.0f,
	1.0f, 1.0f, 0, 1.0f,

	//Back face
	-1.0f, -1.0f, -2.0f, 1.0f,
	1.0f, -1.0f, -2.0f, 1.0f,
	-1.0f, 1.0f, -2.0f, 1.0f,
	1.0f, 1.0f, -2.0f, 1.0f,

	//left face
	-1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -2.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, -2.0f, 1.0f,

	//right face
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, -2.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, -2.0f, 1.0f,

	//top face
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, -2.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, -2.0f, 1.0f,

	//bottom face
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, -2.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -2.0f, 1.0f,


	//Outline info
	-1.0f, -1.0f, 0, 1.0f,
	1.0f, -1.0f, 0, 1.0f,
	1.0f, 1.0f, 0, 1.0f,
	-1.0f, 1.0f, 0, 1.0f,
	-1.0f, -1.0f, 0, 1.0f,

	-1.0f, -1.0f, -2.0, 1.0f,
	-1.0f, 1.0f, -2.0, 1.0f,
	-1.0f, 1.0f, 0, 1.0f,

	-1.0f, 1.0f, -2.0, 1.0f,
	1.0f, 1.0f, -2.0, 1.0f,
	1.0f, -1.0f, -2.0, 1.0f,
	-1.0f, -1.0f, -2.0, 1.0f,

	1.0f, 1.0f, -2.0, 1.0f,
	1.0f, 1.0f, 0, 1.0f,

	1.0f, -1.0f, -2.0, 1.0f,
	1.0f, -1.0f, 0, 1.0f

};

GLuint					quadVertexArrayObj;
GLuint					quadVertexBuffer;
GLuint					quadTextureCoordBuffer;

GLuint					texture;

///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ModelGL::ModelGL() : windowWidth(0), windowHeight(0), disco(false), mouseLeftDown(false), speed(50), frameTick(0), frameCounter(20), lines(false),
cameraAngleX(0), cameraAngleY(0), width(0), height(0), depth(0), longestInt(0), shortest(0), cameraDistance(startDistance), cameraX(0), drawing(false),
cameraY(0), startDistance(6.2), animateFlag(false), bgFlag(0), cellFlag(0), frameBuffer(0), bufferSize(0), windowResized(false), tempCameraReset(false),
drawCompleteFromLastStep(false), threeD(false), drawHideBuffer(0)
{
	//Setting some starting colours
	colours[0][0] = 0;
	colours[0][1] = 0;
	colours[0][2] = 0;	//Black

	colours[1][0] = 1;
	colours[1][1] = 1;
	colours[1][2] = 0;	//Yellow

	colours[2][0] = 1;
	colours[2][1] = 0;
	colours[2][2] = 0;	//Red

	colours[3][0] = 0;
	colours[3][1] = 0;
	colours[3][2] = 1;	//Blue

	colours[4][0] = 0;
	colours[4][1] = 1;
	colours[4][2] = 0;	//Green

	for (int i = 5; i < 100; i++)
	{
		for (int k = 0; k < 3; k++)
			colours[i][k] = float(rand() % 100) / 100; //Randomising all other colours
	}
	selectedID = 1;
	simulator.setDrawMutex(&drawMutex);
	bgColor[0] = bgColor[1] = bgColor[2] = bgColor[3] = 0;
	gridColor[0] = gridColor[1] = gridColor[2] = 0;
	cellColor[0] = cellColor[1] = 1;
	cellColor[2] = cellColor[3] = 0;
	tempCameraInfo[0] = cameraDistance;
	tempCameraInfo[1] = tempCameraInfo[2] = tempCameraInfo[3] = tempCameraInfo[4] = 0;
}


///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
ModelGL::~ModelGL()
{
}
///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL states and scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::init()
{
	glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);   // background color
	glClearStencil(0);                              // clear stencil buffer
	glClearDepth(1.0f);                             // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);

	// initialise glew library
	GLenum err = glewInit();

	// ensure glew was initialised successfully before proceeding
	if (err == GLEW_OK)
		cout << "GLEW initialised okay\n";
	else
		cout << "GLEW could not be initialised\n";

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND); 
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup VAO for textured quad object
	glGenVertexArrays(1, &quadVertexArrayObj);
	glBindVertexArray(quadVertexArrayObj);
	// Setup vbo for position attribute
	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadPositionArray), quadPositionArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	// Enable vertex buffers for textured quad rendering (vertex positions and colour buffers)
	glEnableVertexAttribArray(0);
	// Unbind textured quad VAO
	glBindVertexArray(0);

	//Creating the starting camera position
	setCamera(0, 0, 8, 0, 0, 0);

	glEnable(GL_DEPTH_TEST); 

	//Creating some init values
	calculateLineColour();
	calculateDrawInfo();
	tempCameraInfo[0] = cameraDistance = 6.02;
}

void ModelGL::toggleGrid()
{
	lines = !lines;
}
void ModelGL::toggleDisco()
{
	disco = !disco;
}
void ModelGL::setTempCamera(bool in)
{
	if (tempCameraReset != in)
	{
		//Saves current camera position
		tempCameraReset = in;
		if (tempCameraReset)
		{
			tempCameraInfo[0] = cameraDistance;
			tempCameraInfo[1] = cameraX;
			tempCameraInfo[2] = cameraY;
			tempCameraInfo[3] = cameraAngleX;
			tempCameraInfo[4] = cameraAngleY;
			resetCamera();
		}
		else
		{
			//loads current camera position
			cameraDistance = tempCameraInfo[0];
			cameraX = tempCameraInfo[1];
			cameraY = tempCameraInfo[2];
			cameraAngleX = tempCameraInfo[3];
			cameraAngleY = tempCameraInfo[4];
		}
	}
}
void ModelGL::resetCamera()
{
	//Sets camera values to default
	cameraDistance = 6.02;
	cameraX = 0;
	cameraY = 0;
	if (simulator.getCAType() != 2 || !drawing)
		cameraAngleX = 0;
	else
		cameraAngleX = -90;
	cameraAngleY = 0;
}

///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}



///////////////////////////////////////////////////////////////////////////////
// configure projection and viewport
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setViewport(int w, int h)
{
	// assign the width/height of viewport
	windowWidth = w;
	windowHeight = h;
	//bufferSize = w * h * 4; // rgba
	//frameBuffer = new unsigned char [bufferSize];

	// set viewport to be the entire window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}



///////////////////////////////////////////////////////////////////////////////
// toggle to resize window
///////////////////////////////////////////////////////////////////////////////
void ModelGL::resizeWindow(int w, int h)
{
	// assign the width/height of viewport
	windowWidth = w;
	windowHeight = h;
	windowResized = true;
}

DWORD WINAPI threadFunction(void *simulatorIn)
{
	//Only main loop is needed in the thread
	CASimulator *simulator = (CASimulator *)simulatorIn;
	simulator->mainLoop();
	return 0;
}
void ModelGL::update()
{
	if (animateFlag)
	{
		//Step-Frame rate junk
		frameTick = clock() - frameCounter;
		if (frameTick + (speed * 10) > 1000)
		{
			frameCounter = clock();

			if (THREADED_UPDATE)
			{
				//Threaded update
				if (WaitForSingleObject(thread, 0) == -1 || !WaitForSingleObject(thread, 0))
				{
					if (!LIMIT_STEPS_TO_FRAMES || (LIMIT_STEPS_TO_FRAMES && drawCompleteFromLastStep))
					{
						thread = CreateThread(NULL, 0, threadFunction, (void*)(&simulator), 0, &threadId);
						drawCompleteFromLastStep = false;
					}
				}
			}
			else
			{
				////Unthreaded update (if you REALLY want) 
				simulator.mainLoop();
			}

		}
	}
	if (drawing)
	{
		switch (simulator.getTwoDimentionalTessellation())
		{
		case 0:	drawCells(); break;
		case 1:	drawTriangleCells(); break;
		case 2:	drawHexCells(); break;
		default:drawCells(); break;
		}
	}
}

void ModelGL::calculateDrawInfo()
{
	//Calculating the literal draw distances for the dimentions
	width = 2 / ((float)simulator.getGrid()->getWidth());
	height = 2 / ((float)simulator.getGrid()->getHeight());
	depth = 2 / ((float)simulator.getGrid()->getDepth());

	//Calculating the longest length which will be used as a reference for scale
	longestInt = simulator.getGrid()->getWidth();
	if (simulator.getGrid()->getHeight() > longestInt)
		longestInt = simulator.getGrid()->getHeight();
	if (simulator.getGrid()->getDepth() > longestInt)
		longestInt = simulator.getGrid()->getDepth();

	//Calculating the longest length (in terms of relative size scaling so the longest will be the smallest)
	//Ok so that didnt make much sence...so like putting 100 squares on screen means a smaller space for each square
	//compared to like 20 squares. So this just gets the smallest scale to use for the size of all squares so it's all relative
	shortest = width;
	if (height < shortest)
		shortest = height;
	if (depth < shortest)
		shortest = depth;
}
void ModelGL::calculateHexDrawInfo()
{
	//Calculating the literal draw distances for the dimentions
	width = 2 / (((float)simulator.getGrid()->getWidth())*1.5);
	height = (((float)simulator.getGrid()->getHeight()));
	depth = 2;

	//Calculating the longest length which will be used as a reference for scale
	longestInt = simulator.getGrid()->getWidth();
	if (simulator.getGrid()->getHeight() > longestInt)
		longestInt = simulator.getGrid()->getHeight();
	if (simulator.getGrid()->getDepth() > longestInt)
		longestInt = simulator.getGrid()->getDepth();

	//Calculating the longest length (in terms of relative size scaling so the longest will be the smallest)
	//Ok so that didnt make much sence...so like putting 100 squares on screen means a smaller space for each square
	//compared to like 20 squares. So this just gets the smallest scale to use for the size of all squares so it's all relative
	shortest = width;
	if (height < shortest)
		shortest = height;
	if (depth < shortest)
		shortest = depth;
}

void ModelGL::setSafeToDraw(bool in)
{
	safeToDraw = in;
}
///////////////////////////////////////////////////////////////////////////////
// draw 2D/3D scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::calculateLineColour()
{
	//Setting the line colour to always be visually different to the background colour
	float diff = 0.3;
	gridColor[0] = bgColor[0] - diff;
	gridColor[1] = bgColor[1] - diff;
	gridColor[2] = bgColor[2] - diff;
	if (gridColor[0] < 0.5 - diff)
		gridColor[0] += 2 * diff;
	if (gridColor[1] < 0.5 - diff)
		gridColor[1] += 2 * diff;
	if (gridColor[2] < 0.5 - diff)
		gridColor[2] += 2 * diff;
}
void ModelGL::drawSquareGrid()
{
	//These transformations center the CA so it's always default in the middle fo the screen
	if (shortest == width && width != height)
	{
		glTranslatef(0, ((simulator.getGrid()->getWidth() - simulator.getGrid()->getHeight())*shortest) / 2, 0);
	}
	else if (shortest == height && width != height)
	{
		glTranslatef(((simulator.getGrid()->getHeight() - simulator.getGrid()->getWidth())*shortest) / 2, 0, 0);
	}
	else if (shortest == depth)
	{
		glTranslatef(((simulator.getGrid()->getDepth() - simulator.getGrid()->getWidth())*shortest) / 2, 0, 0);
		glTranslatef(0, ((simulator.getGrid()->getDepth() - simulator.getGrid()->getHeight())*shortest) / 2, 0);
	}
	glPushMatrix();

	//By default the square is the ENTIRE viewport (unstreached) so this just scales it down to the relative size
	glScalef(shortest / 2, shortest / 2, shortest / 2);
	
	//Altering the depth distance if the user has cut some layers off
	if (drawHideBuffer)
	{
		if (drawing)
			glTranslatef(0, 0, longestInt * shortest * drawHideBuffer);		//Move it a layer closer to the camera for each missing layer
		else
			glTranslatef(0, 0, longestInt * shortest * drawHideBuffer / 2); //Half the distance to keep the rotation centered
	}

	glPushMatrix();

	//B-b-b-biiiiind that array 
	glBindVertexArray(quadVertexArrayObj);

	//Setting up translation matrix for each cell
	for (int i = 0; i < simulator.getGrid()->getWidth(); i++)
	{
		for (int j = 0; j < simulator.getGrid()->getHeight(); j++)
		{
			for (int k = drawHideBuffer; k < simulator.getGrid()->getDepth(); k++)
			{
				int cellID = simulator.getGrid()->getCell(i, j, k)->getCellID();
				if (cellID)
				{
					//Setting colour for each cell
					float red = 0, green = 0, blue = 0;
					if (!disco)
					{
						switch (cellID)
						{
							//Cell type of 1 is default and has special colours (maybe look int expanding to 2D array?)
						case -99: red = green = blue = 0.2; break;
						default: 
							red = colours[cellID][0];
							green = colours[cellID][1];
							blue = colours[cellID][2];
							break;
						}
					}
					else
					{
						//DISCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!
						red = ((float)(rand() % 100)) / 100;
						green = ((float)(rand() % 100)) / 100;
						blue = ((float)(rand() % 100)) / 100;
					}

					glPushMatrix();

					//square position, maths looks ugly but at least the concept is simple :D
					glTranslatef((((i + 0.5)*shortest) - 1) * longestInt, (((j + 0.5)*shortest) - 1) * longestInt, -((k*shortest)*longestInt));


					float tempDistance = shortest / 2;

					//Drawing all the edges of the cube - only front face unless threeD is true
					//front
					glColor3f(red, green, blue); 
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					if (threeD)
					{
						//back
						glColor3f(red - 0.5, green - 0.5, blue - 0.5);
						glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
						//left
						glColor3f(red - 0.1, green - 0.1, blue - 0.1);
						glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
						//right
						glColor3f(red - 0.35, green - 0.35, blue - 0.35);
						glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
						//top
						glColor3f(red - 0.2, green - 0.2, blue - 0.2);
						glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
						//bottom
						glColor3f(red - 0.43, green - 0.43, blue - 0.43);
						glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

						//Draws all of the edges of each cube
						if (lines)
						{
							glColor3f(0, 0, 0);
							for (int p = 0; p < 7; p++)
								glDrawArrays(GL_LINES, 24 + p, 2);
							for (int p = 0; p < 3; p++)
								glDrawArrays(GL_LINES, 32 + p, 2);
							glDrawArrays(GL_LINES, 36, 2);
							glDrawArrays(GL_LINES, 38, 2);
						}//C-
					}//C-
					glPopMatrix();
				}//C-
			}//C-
		}//C-
	}//C-Combo!

	//	\o/  Pop!  <o>		\o/  Pop!  <o>  
	glPopMatrix();     
	glPopMatrix();
	glPopMatrix();

	//Drawing the 2D grid
	if (lines)
	{
		glColor3f(gridColor[0], gridColor[1], gridColor[2]);
		if (!threeD || drawing)
		{
			glPushMatrix();
			glScalef(shortest / 2, shortest / 2, shortest / 2);
			if (!GRID_TRIANGLES)
			{
				//Draws lines using GL_LINES which does not scale with the camera
				glLineWidth(0.5);
				for (int i = 0; i < simulator.getGrid()->getWidth() + 1; i++)
				{
					glBegin(GL_LINES);
					glVertex3f((((i*width)) - 1) * simulator.getGrid()->getWidth(), -simulator.getGrid()->getHeight(), 0.0);
					glVertex3f((((i*width)) - 1) * simulator.getGrid()->getWidth(), simulator.getGrid()->getHeight(), 0);
					glEnd();
				}

				for (int i = 0; i < simulator.getGrid()->getHeight() + 1; i++)
				{
					glBegin(GL_LINES);
					glVertex3f(-simulator.getGrid()->getWidth(), (((i*height)) - 1) * simulator.getGrid()->getHeight(), 0.0);
					glVertex3f(simulator.getGrid()->getWidth(), (((i*height)) - 1) * simulator.getGrid()->getHeight(), 0);
					glEnd();
				}
			}
			else
			{
				//Alternate way to draw lines using triangle strips instead which allows for camera scaling
				glColor3f(gridColor[0], gridColor[1], gridColor[2]);
				float lineThickness = shortest * 30;
				for (int i = 0; i < simulator.getGrid()->getWidth() + 1; i++)
				{
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f((((i*width)) - 1) * simulator.getGrid()->getWidth(), -simulator.getGrid()->getHeight(), 0.0);
					glVertex3f((((i*width)) - 1) * simulator.getGrid()->getWidth(), simulator.getGrid()->getHeight(), 0);
					glVertex3f((((i*width)) - 1) * simulator.getGrid()->getWidth() + lineThickness, simulator.getGrid()->getHeight(), 0);

					glVertex3f((((i*width)) - 1) * simulator.getGrid()->getWidth() + lineThickness, -simulator.getGrid()->getHeight(), 0.0);
					glVertex3f((((i*width)) - 1) * simulator.getGrid()->getWidth(), -simulator.getGrid()->getHeight(), 0.0);
					glEnd();
				}
				for (int i = 0; i < simulator.getGrid()->getWidth() + 1; i++)
				{
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(-simulator.getGrid()->getWidth(), (((i*height)) - 1) * simulator.getGrid()->getHeight(), 0.0);
					glVertex3f(-simulator.getGrid()->getWidth(), (((i*height)) - 1) * simulator.getGrid()->getHeight() + lineThickness, 0.0);
					glVertex3f(simulator.getGrid()->getWidth(), (((i*height)) - 1) * simulator.getGrid()->getHeight() + lineThickness, 0);

					glVertex3f(simulator.getGrid()->getWidth(), (((i*height)) - 1) * simulator.getGrid()->getHeight(), 0);
					glVertex3f(-simulator.getGrid()->getWidth(), (((i*height)) - 1) * simulator.getGrid()->getHeight(), 0.0);
					glEnd();
				}
			}
			glPopMatrix();
		}
		else
		{
			glLineWidth(1.5);
			glPushMatrix();
			glScalef((shortest / 2) * simulator.getGrid()->getWidth(), (shortest / 2) * simulator.getGrid()->getHeight(), (shortest / 2) * simulator.getGrid()->getDepth());
			for (int p = 0; p < 7; p++)
				glDrawArrays(GL_LINES, 24 + p, 2);
			for (int p = 0; p < 3; p++)
				glDrawArrays(GL_LINES, 32 + p, 2);
			glDrawArrays(GL_LINES, 36, 2);
			glDrawArrays(GL_LINES, 38, 2);
			glPopMatrix();
			glLineWidth(0.5);
		}
	}
	glBindVertexArray(0);
}
void ModelGL::drawTriangleGrid()
{
	//Calculating the literal draw distances for the dimentions
	width = 2 / (((float)simulator.getGrid()->getWidth()));
	height = 2 / (((float)simulator.getGrid()->getHeight()));
	depth = 2;
	shortest = width;

	if (height < shortest)
		shortest = height;

	float tempShortestY = shortest;
	float tempShortestX = shortest * 0.5;

	width = 2 / ((((float)simulator.getGrid()->getWidth()))*0.5);
	height = 2 / ((((float)simulator.getGrid()->getHeight())));
	depth = 2;

	shortest = width;
	if (height < shortest)
		shortest = height;

	glTranslatef(0.5, 0, 0);

	int widthDiff = simulator.getGrid()->getWidth() - simulator.getGrid()->getHeight();
	//These transformations center the CA so it's always default in the middle fo the screen
	if (shortest == width && width != height)
	{
		glTranslatef((((simulator.getGrid()->getHeight() - simulator.getGrid()->getWidth())*shortest) / 4) * 1.5, 0, 0);
		glTranslatef((widthDiff*shortest) / 2, 0, 0);
		glTranslatef(0, (widthDiff*shortest) / 2, 0);
	}
	else if (shortest == height)
	{
		glTranslatef(((simulator.getGrid()->getHeight() - simulator.getGrid()->getWidth())*shortest) / 4, 0, 0);
		if (simulator.getGrid()->getWidth() > simulator.getGrid()->getHeight())
		{
			glTranslatef((widthDiff*shortest) / 2, 0, 0);
			glTranslatef(0, (widthDiff*shortest) / 2, 0);
		}
	}
	glPushMatrix();

	//By default the square is the ENTIRE viewport (unstreached) so this just scales it down to the relative size
	glScalef(shortest / 2, shortest / 2, shortest / 2);

	glPushMatrix();

	//B-b-b-biiiiind that array 
	glBindVertexArray(quadVertexArrayObj);

	//Setting up translation matrix for each cell
	for (int i = 0; i < simulator.getGrid()->getWidth(); i++)
	{
		for (int j = 0; j < simulator.getGrid()->getHeight(); j++)
		{
			int cellID = simulator.getGrid()->getCell(i, j, 0)->getCellID();
			if (cellID)
			{
				//Setting colour for each cell
				float red = 0, green = 0, blue = 0;
				if (!disco)
				{
					switch (cellID)
					{
						//Cell type of 1 is default and has special colours (maybe look int expanding to 2D array?)
					case -99: red = green = blue = 0.2; break;
					default:
						red = colours[cellID][0];
						green = colours[cellID][1];
						blue = colours[cellID][2];
						break;
					}
				}
				else
				{
					//DISCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!
					red = ((float)(rand() % 100)) / 100;
					green = ((float)(rand() % 100)) / 100;
					blue = ((float)(rand() % 100)) / 100;
				}

				glPushMatrix();

				//square position, maths looks ugly but at least the concept is simple :D
				glTranslatef((((i + 0.5)*tempShortestX) - 1) * longestInt, (((j + 0.5)*tempShortestY) - 1) * longestInt, 0);


				float tempDistance = shortest / 2;

				//Drawing all the edges of the cube - only front face unless threeD is true
				//front
				glColor3f(red, green, blue);

				if ((i + j) % 2 == 0)
				{
					glBegin(GL_TRIANGLE_STRIP);
					glVertex2f(-1, -1);
					glVertex2f(0, 1);
					glVertex2f(1, -1);
					glEnd();
				}
				else
				{
					glBegin(GL_TRIANGLE_STRIP);
					glVertex2f(-1, 1);
					glVertex2f(0, -1);
					glVertex2f(1, 1);
					glEnd();
				}

				glPopMatrix();
			}
		}//C-
	}//C-Combo!

	glBindVertexArray(0);



	glPopMatrix();

	//Drawing the 2D grid
	if (lines && !threeD)
	{
		glColor3f(gridColor[0], gridColor[1], gridColor[2]);
		if (!GRID_TRIANGLES)
		{
			//Draws lines using GL_LINES which does not scale with the camera
			glLineWidth(0.5);
			for (int i = 0; i < simulator.getGrid()->getWidth(); i++)
			{
				for (int j = 0; j < simulator.getGrid()->getHeight(); j++)
				{

					glPushMatrix();

					glTranslatef((((i + 0.5)*tempShortestX) - 1) * longestInt, (((j + 0.5)*tempShortestY) - 1) * longestInt, 0);

					if ((i+j)%2 == 0)
					{
						glBegin(GL_LINE_LOOP);
						glVertex2f(-1, -1);
						glVertex2f(0, 1);
						glVertex2f(1, -1);
						glEnd();
					}
					else
					{
						glBegin(GL_LINE_LOOP);
						glVertex2f(-1, 1);
						glVertex2f(0, -1);
						glVertex2f(1, 1);
						glEnd();
					}
					glPopMatrix();
				}
			}
		}
	}

	//	\o/  Pop!  <o>		\o/  Pop!  <o>         
	glPopMatrix();
	glPopMatrix();
}
void ModelGL::drawHexGrid()
{
	//Calculating the literal draw distances for the dimentions
	width = 2 / (((float)simulator.getGrid()->getWidth()));
	height = 2 / (((float)simulator.getGrid()->getHeight()));
	shortest = width;

	if (height < shortest)
		shortest = height;

	float tempShortestY = shortest / 2;
	float tempShortestX = shortest * 1.5;

	width = 2 / ((((float)simulator.getGrid()->getWidth()))*1.5);
	height = 2 / ((((float)simulator.getGrid()->getHeight())) / 2);

	shortest = width;
	if (height < shortest)
		shortest = height;

	//These transformations center the CA so it's always default in the middle fo the screen
	if (shortest == width && width != height)
	{
		if (simulator.getGrid()->getHeight() > simulator.getGrid()->getWidth())
			glTranslatef((((simulator.getGrid()->getHeight() - simulator.getGrid()->getWidth())*shortest) / 2)*1.5, 0, 0);
		else
			glTranslatef(0, (((simulator.getGrid()->getWidth() - simulator.getGrid()->getHeight())*shortest) / 4), 0);
	}
	else if (shortest == height)
	{
		glTranslatef((((simulator.getGrid()->getHeight() - simulator.getGrid()->getWidth())*shortest) / 2)*1.5, 0, 0);
	}
	glPushMatrix();

	//By default the square is the ENTIRE viewport (unstreached) so this just scales it down to the relative size
	glScalef(shortest / 2, shortest / 2, shortest / 2);

	glPushMatrix();

	//B-b-b-biiiiind that array 
	glBindVertexArray(quadVertexArrayObj);

	//Setting up translation matrix for each cell
	for (int i = 0; i < simulator.getGrid()->getWidth(); i++)
	{
		for (int j = 0; j < simulator.getGrid()->getHeight(); j++)
		{
			int cellID = simulator.getGrid()->getCell(i, j, 0)->getCellID();
			if (cellID)
			{
				//Setting colour for each cell
				float red = 0, green = 0, blue = 0;
				if (!disco)
				{
					switch (cellID)
					{
						//Cell type of 1 is default and has special colours (maybe look int expanding to 2D array?)
					case -99: red = green = blue = 0.2; break;
					default:
						red = colours[cellID][0];
						green = colours[cellID][1];
						blue = colours[cellID][2];
						break;
					}
				}
				else
				{
					//DISCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!
					red = ((float)(rand() % 100)) / 100;
					green = ((float)(rand() % 100)) / 100;
					blue = ((float)(rand() % 100)) / 100;
				}

				glPushMatrix();

				//square position, maths looks ugly but at least the concept is simple :D
				if (j % 2 == 0)
					glTranslatef((((i + 0.5)*tempShortestX) - 1.5) * longestInt, (((j + 0.5)*tempShortestY) - 0.5) * longestInt, 0);
				else
					glTranslatef((((i + 1)*tempShortestX) - 1.5) * longestInt, (((j + 0.5)*tempShortestY) - 0.5) * longestInt, 0);


				float tempDistance = shortest / 2;

				//Drawing all the edges of the cube - only front face unless threeD is true
				//front
				glColor3f(red, green, blue);

				glBegin(GL_TRIANGLE_STRIP);
				glVertex2f(-1, 0);
				glVertex2f(-0.5, 1);
				glVertex2f(-0.5, -1);
				glVertex2f(0.5, 1);
				glVertex2f(0.5, -1);
				glVertex2f(1, 0);
				glEnd();

				glPopMatrix();
			}//C-
		}//C-
	}//C-Combo!

	glBindVertexArray(0);



	glPopMatrix();

	//Drawing the 2D grid
	if (lines && !threeD)
	{
		glColor3f(gridColor[0], gridColor[1], gridColor[2]);
		if (!GRID_TRIANGLES)
		{
			//Draws lines using GL_LINES which does not scale with the camera
			glLineWidth(0.5);
			for (int i = 0; i < simulator.getGrid()->getWidth(); i++)
			{
				for (int j = 0; j < simulator.getGrid()->getHeight(); j++)
				{

					glPushMatrix();

					if (j % 2 == 0)
						glTranslatef((((i + 0.5)*tempShortestX) - 1.5) * longestInt, (((j + 0.5)*tempShortestY) - 0.5) * longestInt, 0);
					else
						glTranslatef((((i + 1)*tempShortestX) - 1.5) * longestInt, (((j + 0.5)*tempShortestY) - 0.5) * longestInt, 0);

					glBegin(GL_LINE_LOOP);
					glVertex2f(-0.5, -1);
					glVertex2f(-1, 0);
					glVertex2f(-0.5, 1);
					glVertex2f(0.5, 1);
					glVertex2f(1, 0);
					glVertex2f(0.5, -1);
					glEnd();

					glPopMatrix();

				}
			}
		}
	}
	//	\o/  Pop!  <o>		\o/  Pop!  <o>         
	glPopMatrix();
	glPopMatrix();
}

void ModelGL::draw()
{
	drawMutex.lock();
	//If lines are enabled then we need to do one of two things:
	if (lines && threeD)
	{
		//If we are doing cubes just make it so the lines are always drawn on top
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 2);
	}
	else if (lines)
	{
		//If we are in the 2D world we do some weird fancy things which means the lines dont look weird at an angle
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-0.5, 500);	//		<--------- I AM A GENIUS!!!!
	}
	else
		glDisable(GL_POLYGON_OFFSET_FILL);

	//Window resize junk
	if (windowResized)
	{
		setViewport(windowWidth, windowHeight);
		windowResized = false;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Camera transform
	gluPerspective((cameraDistance / startDistance)*60.0f, (float)(windowWidth) / windowHeight, 0.1f, 20.0f); // FOV, AspectRatio, NearClip, FarClip

	//Slight depth shift that makes sure the front edges of the box are in the right place
	glTranslatef(0, 0, -(shortest / depth));

	// switch to modelview matrix in order to set scene
	glMatrixMode(GL_MODELVIEW);

	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// save the initial ModelView matrix before modifying ModelView matrix
	glPushMatrix();

	// standard tramsforms for all CA
	glTranslatef(cameraX, cameraY, startDistance);

	//glTranslatef(0, 0, simulator.getGrid()->getDepth() / (2 / shortest));			PIEPIEPIEPIE

	if (!threeD)
		glTranslatef(0, 0, simulator.getGrid()->getDepth() / (2 / shortest));

	glRotatef(cameraAngleY, 0, 1, 0);
	if (simulator.getCAType() && simulator.getCAType() != 2)
	{
		glRotatef(cameraAngleX, 1, 0, 0);
	}
	else if (simulator.getCAType() == 0)
	{
		glRotatef(cameraAngleX + 180, 1, 0, 0);		//Cheat rotation for the step CAs (muhaha) to flip the grid over (shhhhhhhh secret)
	}
	else if (simulator.getCAType() == 2)
	{
		glRotatef(cameraAngleX + 90, 1, 0, 0);		//Second secret rotation for 2D step (buahaha) 
	}

	if (threeD)
		glTranslatef(0, 0, simulator.getGrid()->getDepth() / (2 / shortest));

	glPushMatrix();

	switch (simulator.getTwoDimentionalTessellation())
	{
	case 0:	drawSquareGrid(); break;
	case 1:	drawTriangleGrid(); break;
	case 2:	drawHexGrid(); break;
	default:drawSquareGrid(); break;
	}

	glPopMatrix();

	// check if background colour was changed
	if (bgFlag)
	{
		glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
		bgFlag = false;
	}
	drawCompleteFromLastStep = true;		///PIEPIEPIE delete these?
	drawMutex.unlock();
}

void ModelGL::setThreeD(bool in)
{
	threeD = in;
}
void ModelGL::setDrawing(bool in)
{
	drawing = in;
}
void ModelGL::drawCells()
{
	//PIEPIEPIE Investigate into line/plane collisions? 

	//Calculations for working out where the mouse is in terms of the grid
	windowHeight;
	windowWidth;
	//Null area is area on the screen that is not the graph
	int nullWidth;
	if (windowWidth > windowHeight)
		nullWidth = (windowWidth - windowHeight) / 2;
	else
		nullWidth = 0;

	//If the mouse is "on the grid" 
	if (mouseX > nullWidth && mouseX < windowWidth - nullWidth)
	{
		float gridSize = windowHeight;
		float trueX = (mouseX - nullWidth) / gridSize;
		float trueY = (mouseY) / gridSize;

		if (width != height && height != depth && width != depth)
		{
			if (shortest == width)
				trueY -= ((((simulator.getGrid()->getWidth() - simulator.getGrid()->getHeight()))*shortest) / 4);
			else if (shortest == height)
				trueX -= (((simulator.getGrid()->getHeight() - simulator.getGrid()->getWidth()))*shortest) / 4;
		}

		//Getting x-y co-ordinates in terms of an actual grid location
		int testX = (trueX / shortest) * 2;
		int testY = simulator.getGrid()->getHeight() - ((trueY / shortest) * 2);
		int testZ = 0;

		//One last validation check BECAUSE
		if (testX >= 0 && testX < simulator.getGrid()->getWidth() && testY >= 0 && testY < simulator.getGrid()->getHeight())
		{
			if (!simulator.getCAType())
			{
				testY = simulator.getGrid()->getHeight() - (testY + 1);
			}
			else if (simulator.getCAType() == 2 || simulator.getCAType() == 3)
				testZ = drawHideBuffer;

			if (mouseLeftDown)
				simulator.setCellID(testX, testY, testZ, selectedID);
			else if (mouseRightDown)
				simulator.setCellID(testX, testY, testZ, 0);
			else if (mouseMDown)
				simulator.setCellID(testX, testY, testZ, -99);
		}
	}
}

void ModelGL::drawHexCells()
{

}
void ModelGL::drawTriangleCells()
{

}

void ModelGL::moveCamera(float x, float y, float z)
{
	if (mouseLeftDown)
	{
		cameraDistance += z;
		cameraX -= (x / 160) * (cameraDistance / startDistance) / (float(windowHeight) / 298.0f);
		cameraY += (y / 160) * (cameraDistance / startDistance) / (float(windowHeight) / 298.0f);
	}
}

///////////////////////////////////////////////////////////////////////////////
// rotate the camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::rotateCamera(int x, int y)
{
	if (mouseRightDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
}

void ModelGL::scaleZoom(float scaleIn)
{
	cameraDistance /= scaleIn;
}

///////////////////////////////////////////////////////////////////////////////
// zoom the camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::zoomCamera(int delta)
{
	cameraDistance += (delta - mouseY) * 0.05f;
	mouseY = delta;
}

///////////////////////////////////////////////////////////////////////////////
// change background colour, the value should be between 0 and 1
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setBackgroundRed(float value)
{
	if (bgColor[0] != value)
	{
		bgColor[0] = value;
		bgFlag = true;
		calculateLineColour();
	}
}
void ModelGL::setBackgroundGreen(float value)
{
	if (bgColor[1] != value)
	{
		bgColor[1] = value;
		bgFlag = true;
		calculateLineColour();
	}
}
void ModelGL::setBackgroundBlue(float value)
{
	if (bgColor[2] != value)
	{
		bgColor[2] = value;
		bgFlag = true;
		calculateLineColour();
	}
}

void ModelGL::setCellRed(float value)
{
	if (colours[selectedID][0] != value)
	{
		colours[selectedID][0] = value;
		cellFlag = true;
	}
}
void ModelGL::setCellGreen(float value)
{
	if (colours[selectedID][1] != value)
	{
		colours[selectedID][1] = value;
		cellFlag = true;
	}
}
void ModelGL::setCellBlue(float value)
{
	if (colours[selectedID][2] != value)
	{
		colours[selectedID][2] = value;
		cellFlag = true;
	}
}
//Simple setters and junk with some validation for the hide buffer
void ModelGL::setDrawHideBuffer(int in)
{
	drawHideBuffer = in;
	if (drawHideBuffer > simulator.getGrid()->getDepth() - 1)
		drawHideBuffer = simulator.getGrid()->getDepth() - 1;
	else if (drawHideBuffer < 0)
		drawHideBuffer = 0;
}
void ModelGL::alterDrawHideBuffer(int in)
{
	drawHideBuffer += in;
	if (drawHideBuffer > simulator.getGrid()->getDepth() - 1)
		drawHideBuffer = simulator.getGrid()->getDepth() - 1;
	else if (drawHideBuffer < 0)
		drawHideBuffer = 0;
}
bool ModelGL::setTwoDimentionalTessellation(int in)
{
	bool returnVal = false;
	if (simulator.getTwoDimentionalTessellation() != in)
	{
		simulator.setTwoDimentionalTessellation(in);
		returnVal = true;
	}
	return returnVal;
}