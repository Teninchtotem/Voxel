///////////////////////////////////////////////////////////////////////////////
// ControllerGL.cpp
// ================
// Derived Controller class for OpenGL window
// It is the controller of OpenGL rendering window. It initializes DC and RC,
// when WM_CREATE called, then, start new thread for OpenGL rendering loop.
//
// When this class is constructed, it gets the pointers to model and view
// components.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2006-07-09
// UPDATED: 2014-01-01
///////////////////////////////////////////////////////////////////////////////

#include <process.h>                                // for _beginthreadex()
#include "ControllerGL.h"
#include "Log.h"
#include "resource.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerGL::ControllerGL(ModelGL* model, ViewGL* view) : model(model), view(view), xLast(0), yLast(0),
															threadHandle(0), threadId(0), ctrlDown(false), 
															shiftDown(false), loopFlag(false)
{
	disco = 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CLOSE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::close()
{
	model->simulator.initViewController(NULL);

	loopFlag = false;

	::WaitForSingleObject(threadHandle, INFINITE);   // wait for rendering thread is terminated

    // close OpenGL Rendering Context (RC)
    view->closeContext(handle);

    ::DestroyWindow(handle);
    Win::log("OpenGL window is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::create()
{
    // create a OpenGL rendering context
    if(!view->createContext(handle, 32, 24, 8))
    {
        Win::log(L"[ERROR] Failed to create OpenGL rendering context from ControllerGL::create().");
        return -1;
    }

    // create a thread for OpenGL rendering
    // The params of _beginthreadex() are security, stackSize, functionPtr, argPtr, initFlag, threadId.
    threadHandle = (HANDLE)_beginthreadex(0, 0, (unsigned (__stdcall *)(void *))threadFunction, this, 0, &threadId);
    if(threadHandle)
    {
        loopFlag = true;
        Win::log(L"Created a rendering thread for OpenGL.");
    }
    else
    {
        Win::log(L"[ERROR] Failed to create rendering thread from ControllerGL::create().");
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_PAINT
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::paint()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::command(int id, int cmd, LPARAM msg)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// route to worker thread
// The function prototype is:
// unsigned int (__stdcall *)(void *)
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::threadFunction(void* param)
{
    ((ControllerGL*)param)->runThread();
}



///////////////////////////////////////////////////////////////////////////////
// rendering thread
// initialize OpenGL states and start rendering loop
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::runThread()
{
    // set the current RC in this thread
    ::wglMakeCurrent(view->getDC(), view->getRC());

    // initialize OpenGL states
    model->init();
    Win::log(L"Initialized OpenGL states.");

    // cofigure projection matrix
    RECT rect;
    ::GetClientRect(handle, &rect);
    model->resizeWindow(rect.right, rect.bottom);
    Win::log(L"Initialized OpenGL viewport and projection matrix.");

    // rendering loop
    Win::log(L"Entering OpenGL rendering thread...");
    while(loopFlag)
    {
		Sleep(1);                 // yield to other processes or threads
		model->draw();
		model->update();
		view->swapBuffers();
	}
	Win::log("ControllerGL: Terminating main loop");
	// Terminatie OpenGL context
    // terminate rendering thread
    ::wglMakeCurrent(0, 0);             // unset RC
    ::CloseHandle(threadHandle);
    Win::log(L"Exit OpenGL rendering thread.");
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonDown(WPARAM state, int x, int y)
{
	// update mouse position
	model->setMousePosition(x, y);

	if (state == MK_LBUTTON || state == 9) //9 is +CTRL
		model->setMouseLeft(true);

	SetFocus(handle);

	return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);
	model->setMouseLeft(false);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonDown(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);
    if(state == MK_RBUTTON || state == 10) //10 for +CRTL
    {
        model->setMouseRight(true);
    }

	SetFocus(handle);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    model->setMouseRight(false);

    return 0;
}


int ControllerGL::mButtonDown(WPARAM state, int x, int y)
{
	// update mouse position
	model->setMousePosition(x, y);

	if (state == MK_MBUTTON || state == 24) //24 is +CTRL
		model->setMouseM(true);

	SetFocus(handle);

	return 0;
}

int ControllerGL::mButtonUp(WPARAM state, int x, int y)
{
	// update mouse position
	model->setMousePosition(x, y);

	if (state == MK_MBUTTON || state == 24) //24 is +CTRL
		model->setMouseM(false);

	SetFocus(handle);

	return 0;
}
int ControllerGL::keyDown(int key, LPARAM lParam)
{
	if (key != 'd' && key != 'D' && key != 'i' && key != 'I' && key != 's' &&
		key != 'S' && key != 'c' && key != 'C' && key != 'o' && key != 'O')
		disco = 0;

	switch (key)
	{
	case 'd':
	case 'D':
		disco = 1;
		break;
	case 'i':
	case 'I':
		if (disco == 1)
			disco++;
		else
			disco = 0;
		break;
	case 's':
	case 'S':
		if (disco == 2)
			disco++;
		else
			disco = 0;

		if (ctrlDown)
		{
			if (!model->getAnimateFlag())
			{
				model->simulator.save();
				model->setDrawing(false);
				model->setTempCamera(false);
				ctrlDown = false;
			}
		}
		break;
	case 'c':
	case 'C':
		if (disco == 3)
			disco++;
		else
			disco = 0;
		break;
	case 'o':
	case 'O':
		if (disco == 4)
			disco++;
		else
			disco = 0;
		if (ctrlDown)
		{
			if (!model->getAnimateFlag())
			{
				model->simulator.open();
				model->setDrawing(false);
				model->setTempCamera(false);
				ctrlDown = false;
			}
		}
		break;
	case 'r':
	case 'R':
		model->resetCamera();
		break;
	case VK_CONTROL:
		model->setDrawing(true);
		model->setTempCamera(true);
		ctrlDown = true;
		break;
	case VK_SHIFT:
		shiftDown = true;
		break;
	case VK_F5:
	case VK_F6:
		if (!model->getAnimateFlag())
			model->simulator.save("TempQuick");
	case VK_F9:
		if (!model->getAnimateFlag())
		model->simulator.open("TempQuick");
		break;
	}
	if (disco == 5)
		model->toggleDisco();
	return 0;
}
int ControllerGL::keyUp(int key, LPARAM lParam)
{
	switch (key)
	{
	case VK_CONTROL:
		model->setDrawing(false);
		model->setTempCamera(false);
		ctrlDown = false;
		break;
	case VK_SHIFT:
		shiftDown = false;
		break;
	}
	return 0;
}

int ControllerGL::mouseWheel(int state, int d, int x, int y) 
{
	if (ctrlDown)
	{
		if (d == 1)
			model->alterDrawHideBuffer(1);
		else
			model->alterDrawHideBuffer(-1);
	}
	else
	{
		if (d == 1)
			model->scaleZoom(1.05);
		else
			model->scaleZoom(0.95);
	}
	return 0; 
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEMOVE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseMove(WPARAM state, int x, int y)
{
	if (state == MK_RBUTTON)
    {
        model->rotateCamera(x, y);
    }
	if (state == MK_LBUTTON)
    {
		model->moveCamera(xLast - x, yLast - y, 0);
    }
	if (state == 9 || state == 10 || state == 24)
	{
		model->setMousePosition(x,y);
	}

	xLast = x;
	yLast = y;

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::size(int w, int h, WPARAM wParam)
{
    model->resizeWindow(w, h);
    Win::log(L"OpenGL window is resized: %dx%d.", w, h);
    return 0;
}
