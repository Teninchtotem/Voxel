///////////////////////////////////////////////////////////////////////////////
// ViewFormGL.cpp
// ==============
// View component of OpenGL dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-10
// UPDATED: 2006-08-15
///////////////////////////////////////////////////////////////////////////////

#include "ViewFormGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;

///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewFormGL::ViewFormGL()
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewFormGL::~ViewFormGL()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::initControls(HWND handle)
{
    // set all controls
    buttonAnimate.set(handle, IDC_ANIMATE);
	trackbarRed.set(handle, IDC_RED);
	trackbarGreen.set(handle, IDC_GREEN);
	trackbarBlue.set(handle, IDC_BLUE);
	trackbarRed2.set(handle, IDC_RED2);
	trackbarGreen2.set(handle, IDC_GREEN2);
	trackbarBlue2.set(handle, IDC_BLUE2);
	trackbarSpeed.set(handle, IDC_SPEED);

	boxLive1D1.set(handle, IDC_BOX_LIVE_1D_1);
	boxLive1D2.set(handle, IDC_BOX_LIVE_1D_2);
	boxLive1D3.set(handle, IDC_BOX_LIVE_1D_3);
	boxLive1D4.set(handle, IDC_BOX_LIVE_1D_4);
	boxLive1D5.set(handle, IDC_BOX_LIVE_1D_5);
	boxLive1D6.set(handle, IDC_BOX_LIVE_1D_6);
	boxLive1D7.set(handle, IDC_BOX_LIVE_1D_7);
	boxLive1D8.set(handle, IDC_BOX_LIVE_1D_8);
	text1D.set(handle, IDC_BOX_LIVE_TEXT);

	squareT.set(handle, IDC_SQUARE);
	triangleT.set(handle, IDC_TRIANGLE);
	hexagonT.set(handle, IDC_HEX);

	rules.set(handle, IDC_RULES_TEXT);
	ruleBox.set(handle, IDC_RULES_BOX);
	ruleBoxText.set(handle, IDC_CLEAR_ADV_RULE_ID_BOX);

	boxLive1.set(handle, IDC_BOX_LIVE_1);
	boxLive2.set(handle, IDC_BOX_LIVE_2);
	boxLive3.set(handle, IDC_BOX_LIVE_3);
	boxLive4.set(handle, IDC_BOX_LIVE_4);
	boxLive5.set(handle, IDC_BOX_LIVE_5);
	boxLive6.set(handle, IDC_BOX_LIVE_6);
	boxLive7.set(handle, IDC_BOX_LIVE_7);
	boxLive8.set(handle, IDC_BOX_LIVE_8);
	boxLive9.set(handle, IDC_BOX_LIVE_9);
	boxDie1.set(handle, IDC_BOX_DIE_1);
	boxDie2.set(handle, IDC_BOX_DIE_2);
	boxDie3.set(handle, IDC_BOX_DIE_3);
	boxDie4.set(handle, IDC_BOX_DIE_4);
	boxDie5.set(handle, IDC_BOX_DIE_5);
	boxDie6.set(handle, IDC_BOX_DIE_6);
	boxDie7.set(handle, IDC_BOX_DIE_7);
	boxDie8.set(handle, IDC_BOX_DIE_8);
	boxDie9.set(handle, IDC_BOX_DIE_9);
	width.set(handle, IDC_TEXT_WIDTH);
	height.set(handle, IDC_TEXT_HEIGHT);
	depth.set(handle, IDC_TEXT_DEPTH);
	setGrid.set(handle, IDC_SET_GRID);
	randomiseButton.set(handle, IDC_RANDOMISE_GRID_BUTTON);
	randomiseBar.set(handle, IDC_RANDOMISE_GRID_BAR);
	advBox1.set(handle, IDC_ADV_BOX_1);
	advBox2.set(handle, IDC_ADV_BOX_2);
	advBox3.set(handle, IDC_ADV_BOX_3);
	advBox4.set(handle, IDC_ADV_BOX_4);
	advBox5.set(handle, IDC_ADV_BOX_5);
	selectionBox.set(handle, IDC_ID_SELECT);
	selectionBox.setText(L"1");

    // initial state
	squareT.check();
	boxLive4.check();
	boxDie1.check();
	boxDie2.check();
	boxDie5.check();
	boxDie6.check();
	boxDie7.check();
	boxDie8.check();
	boxDie9.check();
	trackbarRed.setRange(0, 255);
	trackbarRed.setPos(0);
	trackbarGreen.setRange(0, 255);
	trackbarGreen.setPos(0);
	trackbarBlue.setRange(0, 255);
	trackbarBlue.setPos(0);

	trackbarRed2.setRange(0, 255);
	trackbarRed2.setPos(255);
	trackbarGreen2.setRange(0, 255);
	trackbarGreen2.setPos(255);
	trackbarBlue2.setRange(0, 255);
	trackbarBlue2.setPos(0);

	randomiseBar.setRange(0, 100);
	randomiseBar.setPos(30);

	trackbarSpeed.setRange(0, 100);
	trackbarSpeed.setPos(50);

	width.setText(L"100");
	height.setText(L"100");
	depth.setText(L"1");
	depth.disable();

	boxLive1D4.check();
	boxLive1D6.check();
	boxLive1D7.check();
	text1D.setText(L"Rule No: 22");

	Win::CheckBox temp;
	temp.set(handle, IDC_SURROUNDING_NEIGHBORS);
	temp.check();

	defaultHandle = handle;

	//setHidden(START_2D, END_2D);
	setHidden(START_RULE_EDIT, END_RULE_EDIT);
	setHidden(START_1D, END_1D);
	setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
}

void ViewFormGL::setHidden(int startID, int endID)
{
	for (int i = startID; i < endID; i++)
	{
		genericSet.set(defaultHandle, i);
		genericSet.hide();
	}
}
void ViewFormGL::setHiddenInclusive(int startID, int endID)
{
	setHidden(startID, endID + 1);
}
void ViewFormGL::setVisible(int startID, int endID)
{
	for (int i = startID; i < endID; i++)
	{
		genericSet.set(defaultHandle, i);
		genericSet.show();
	}
}
void ViewFormGL::disable(int startID, int endID)
{
	for (int i = startID; i < endID; i++)
	{
		genericSet.set(defaultHandle, i);
		genericSet.disable();
	}
}
void ViewFormGL::disableInclusive(int startID, int endID)
{
	disable(startID, endID + 1);
}
void ViewFormGL::enable(int startID, int endID)
{
	for (int i = startID; i < endID; i++)
	{
		genericSet.set(defaultHandle, i);
		genericSet.enable();
	}
}
void ViewFormGL::enableInclusive(int startID, int endID)
{
	enable(startID, endID + 1);
}
void ViewFormGL::setVisibleInclusive(int startID, int endID)
{
	setVisible(startID, endID + 1);
}
void ViewFormGL::setTBox(int in)
{
	switch (in)
	{
	case 0:
		squareT.check();
		triangleT.uncheck();
		hexagonT.uncheck();
		boxLive5.enable();
		boxLive6.enable();
		boxLive7.enable();
		boxLive8.enable();
		boxLive9.enable();
		boxDie5.enable();
		boxDie6.enable();
		boxDie7.enable();
		boxDie8.enable();
		boxDie9.enable();
		break;
	case 1:
		squareT.uncheck();
		triangleT.check();
		hexagonT.uncheck();
		boxLive5.disable();
		boxLive6.disable();
		boxLive7.disable();
		boxLive8.disable();
		boxLive9.disable();
		boxDie5.disable();
		boxDie6.disable();
		boxDie7.disable();
		boxDie8.disable();
		boxDie9.disable();
		break;
	case 2:
		squareT.uncheck();
		triangleT.uncheck();
		hexagonT.check();
		boxLive5.enable();
		boxLive6.enable();
		boxLive7.enable();
		boxLive8.disable();
		boxLive9.disable();
		boxDie5.enable();
		boxDie6.enable();
		boxDie7.enable();
		boxDie8.disable();
		boxDie9.disable();
		break;
	default:
		squareT.check();
		triangleT.uncheck();
		hexagonT.uncheck();
		boxLive5.enable();
		boxLive6.enable();
		boxLive7.enable();
		boxLive8.enable();
		boxLive9.enable();
		boxDie5.enable();
		boxDie6.enable();
		boxDie7.enable();
		boxDie8.enable();
		boxDie9.enable();
		break;
	}
}
void ViewFormGL::setSquareNeighborBox(int in)
{
	Win::CheckBox temp;
	temp.set(defaultHandle, IDC_SURROUNDING_NEIGHBORS);
	temp.uncheck();
	temp.set(defaultHandle, IDC_ADJACENT_NEIGHBORS);
	temp.uncheck();
	switch (in)
	{
	case 0:
		temp.set(defaultHandle, IDC_SURROUNDING_NEIGHBORS);
		temp.check();
		boxLive6.enable();
		boxLive7.enable();
		boxLive8.enable();
		boxLive9.enable();
		boxDie6.enable();
		boxDie7.enable();
		boxDie8.enable();
		boxDie9.enable();
		break;
	case 1:
		temp.set(defaultHandle, IDC_ADJACENT_NEIGHBORS);
		temp.check();
		boxLive6.disable();
		boxLive7.disable();
		boxLive8.disable();
		boxLive9.disable();
		boxDie6.disable();
		boxDie7.disable();
		boxDie8.disable();
		boxDie9.disable();
		break;
	default:
		temp.set(defaultHandle, IDC_SURROUNDING_NEIGHBORS);
		temp.check();
		boxLive6.enable();
		boxLive7.enable();
		boxLive8.enable();
		boxLive9.enable();
		boxDie6.enable();
		boxDie7.enable();
		boxDie8.enable();
		boxDie9.enable();
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// update caption of animate button
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::animate(bool flag)
{
    if(flag)
        buttonAnimate.setText(L"Stop");
    else
        buttonAnimate.setText(L"Go");
}

void ViewFormGL::setRules(std::string inBox, std::string in)
{
	wchar_t * tempWChar;
	std::wstring widthTextTemp;
	for (int i = 0; i < in.length(); i++)
		widthTextTemp += in[i];
	tempWChar = (wchar_t*)widthTextTemp.c_str();
	rules.setText(tempWChar);

	wchar_t * tempWCharBox;
	std::wstring widthTextTempBox;
	for (int i = 0; i < inBox.length(); i++)
		widthTextTempBox += inBox[i];
	tempWCharBox = (wchar_t*)widthTextTempBox.c_str();
	ruleBox.setText(tempWCharBox);
}


///////////////////////////////////////////////////////////////////////////////
// update trackbars
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::updateTrackbars(HWND handle, int position)
{
	if (handle == trackbarRed.getHandle())
	{
		trackbarRed.setPos(position);
	}
	else if (handle == trackbarGreen.getHandle())
	{
		trackbarGreen.setPos(position);
	}
	else if (handle == trackbarBlue.getHandle())
	{
		trackbarBlue.setPos(position);
	}
	else if (handle == trackbarSpeed.getHandle())
	{
		trackbarSpeed.setPos(position);
	}
}
void ViewFormGL::updateTrackbars(int R, int G, int B)
{
		trackbarRed2.setPos(R);	
		trackbarGreen2.setPos(G);	
		trackbarBlue2.setPos(B);	
}
