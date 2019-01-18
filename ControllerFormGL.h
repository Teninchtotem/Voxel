///////////////////////////////////////////////////////////////////////////////
// ControllerFormGL.h
// ==================
// Derived Controller class for OpenGL dialog window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2006-07-09
// UPDATED: 2013-03-17
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_FORM_GL_H
#define WIN_CONTROLLER_FORM_GL_H

#include "Controller.h"
#include "ViewFormGL.h"
#include "FileManager.h"
#include <string.h>
#include "ModelGL.h"


namespace Win
{
    class ControllerFormGL : public Controller
    {
    public:
        ControllerFormGL(ModelGL* model, ViewFormGL* view);
        ~ControllerFormGL() {};

		int close();

		void prepareForFileDialog();
		void recoverFromFileDialog();
		void setMainHandle(HWND in);
		void calculate1DRule();
		void calculate2DRules();
		void calculate2DRulesTriangle();
		void calculate2DRulesHex();
		void calculate2DStepRules();
		void calculate3DRules();
		void calculateUIfromSimulator();
		void calculate1DRuleCheckBoxesFromSimulator();
		void calculate2DRuleCheckBoxesFromSimulator();
		void calculate3DRuleCheckBoxesFromSimulator();
		void calculateRuleText();
		void updateDimentionBoxes();
		void loadFile(string fileName = "");
		void saveFile(string fileName = "");
		void disableRuleButtons();
		void enableRuleButtons();
		void setText(int ID, string text);
		void setInfo();
		void disableThreeD();
		void enableThreeD();
		void setInfoFlag();
		void setCalculateUIFlag();
		void enterRuleMode();
		void exitRuleMode();
		void pauseSimualtion();
        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int create();                               // for WM_CREATE

        int hScroll(WPARAM wParam, LPARAM lParam);  // for WM_HSCROLL

	private:
		int convertWChar_TToInt(wchar_t *in);

		int advRuleScroll;
		bool flag;
		bool infoFlag;
		bool calculateUIFlag;
		bool advancedRules;
		bool editingRules;

		mutex *drawMutex;
		HWND mainHandle;
        ModelGL* model;                             // pointer to model component
        ViewFormGL* view;                           // pointer to view component
    };
}

#endif
