///////////////////////////////////////////////////////////////////////////////
// ViewFormGL.h
// ============
// View component of OpenGL dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-10
// UPDATED: 2006-08-15
///////////////////////////////////////////////////////////////////////////////

#ifndef VIEW_FORM_GL_H
#define VIEW_FORM_GL_H

#include <windows.h>
#include <string>
#include "Controls.h"

namespace Win
{
    class ViewFormGL
    {
    public:
        ViewFormGL();
        ~ViewFormGL();

        void initControls(HWND handle);         // init all controls

        // update controls on the form
        void animate(bool flag);
        void updateTrackbars(HWND handle, int position);
		void updateTrackbars(int R, int G, int B);

		void setRules(std::string inBox, std::string in);
		void setHidden(int startID, int endID);
		void setHiddenInclusive(int startID, int endID);
		void setVisible(int startID, int endID);
		void setVisibleInclusive(int startID, int endID);
		void disable(int startID, int endID);
		void disableInclusive(int startID, int endID);
		void enable(int startID, int endID);
		void enableInclusive(int startID, int endID);
		void setSquareNeighborBox(int in);
		void setTBox(int in);

		Win::Button genericSet;

		Win::CheckBox boxLive1D1;
		Win::CheckBox boxLive1D2;
		Win::CheckBox boxLive1D3;
		Win::CheckBox boxLive1D4;
		Win::CheckBox boxLive1D5;
		Win::CheckBox boxLive1D6;
		Win::CheckBox boxLive1D7;
		Win::CheckBox boxLive1D8;

		Win::TextBox rules;
		Win::TextBox ruleBox;
		Win::EditBox ruleBoxText;

		Win::TextBox text1D;

		Win::CheckBox boxLive1;
		Win::CheckBox boxLive2;
		Win::CheckBox boxLive3;
		Win::CheckBox boxLive4;
		Win::CheckBox boxLive5;
		Win::CheckBox boxLive6;
		Win::CheckBox boxLive7;
		Win::CheckBox boxLive8;
		Win::CheckBox boxLive9;
		Win::CheckBox boxDie1;
		Win::CheckBox boxDie2;
		Win::CheckBox boxDie3;
		Win::CheckBox boxDie4;
		Win::CheckBox boxDie5;
		Win::CheckBox boxDie6;
		Win::CheckBox boxDie7;
		Win::CheckBox boxDie8;
		Win::CheckBox boxDie9;

		Win::CheckBox squareT;
		Win::CheckBox triangleT;
		Win::CheckBox hexagonT;

		Win::EditBox width;
		Win::EditBox height;
		Win::EditBox depth;

		Win::EditBox advBox1;
		Win::EditBox advBox2;
		Win::EditBox advBox3;
		Win::EditBox advBox4;
		Win::EditBox advBox5;
		Win::EditBox selectionBox;

		Win::Button setGrid;
		Win::Button randomiseButton;

		Win::Trackbar randomiseBar;

    protected:

    private:
        // controls
        Win::Button buttonAnimate;
		Win::Trackbar trackbarRed;
		Win::Trackbar trackbarGreen;
		Win::Trackbar trackbarBlue;
		Win::Trackbar trackbarRed2;
		Win::Trackbar trackbarGreen2;
		Win::Trackbar trackbarBlue2;
		Win::Trackbar trackbarSpeed;

		HWND defaultHandle;
    };
}

#endif
