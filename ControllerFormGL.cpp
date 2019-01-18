///////////////////////////////////////////////////////////////////////////////
// ControllerFormGL.cpp
// ====================
// Derived Controller class for OpenGL dialog window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2006-07-09
// UPDATED: 2006-08-15
///////////////////////////////////////////////////////////////////////////////

#include <process.h>                                // for _beginthreadex()
#include "ControllerFormGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerFormGL::ControllerFormGL(ModelGL* model, ViewFormGL* view) : model(model), view(view)
{
	editingRules = false;
	advRuleScroll = 0;
	advancedRules = false;
	flag = false;
}

int ControllerFormGL::close()
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::create()
{
    // initialize all controls
    view->initControls(handle);
	drawMutex = model->getDrawMutex();
    // place the opengl form dialog in right place, bottom of the opengl rendering window
    //RECT rect = {0, 0, 4, 8};
    //::MapDialogRect(glDialog.getHandle(), &rect);
    //int width = MulDiv(300, 4, rect.right);
    //::SetWindowPos(handle, HWND_TOP, 0, 300, 300, 200, 0);

    return 0;
}

void ControllerFormGL::setMainHandle(HWND in)
{
	mainHandle = in;
}

void ControllerFormGL::disableRuleButtons()
{
	for (int i = OPTIONS_START; i < OPTIONS_END; i++)
	{
		EnableMenuItem(GetMenu(mainHandle), i, MF_DISABLED);
	}

	Win::CheckBox temp;

	view->disableInclusive(IDC_1D_SET_CENTER, IDC_1D_SET_CENTER);
	view->disableInclusive(IDC_RANDOMISE_GRID_BUTTON, IDC_LOOPING);
	view->disableInclusive(IDC_QUICK_SAVE, IDC_QUICK_LOAD);
	view->disable(IDC_SQUARE, END_2D);
	view->disableInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_ADJACENT_NEIGHBORS);

	for (int i = 0; i < 18; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1 + i);
		temp.disable();
	}
	for (int i = 0; i < 8; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1D_1 + i);
		temp.disable();
	}
	temp.set(handle, IDC_SET_1D_GRID);
	temp.disable();
	temp.set(handle, IDC_SET_2D_GRID);
	temp.disable();
	temp.set(handle, IDC_SET_2S_GRID);
	temp.disable();
	temp.set(handle, IDC_SET_3D_GRID);
	temp.disable();
}
void ControllerFormGL::disableThreeD()
{
	Win::CheckBox temp;
	temp.set(handle, IDC_TEXT_DEPTH);
	model->setThreeD(false);
	temp.disable();

	wchar_t * tempWidthChar;
	string widthText = "1";
	wstring widthTextTemp = L"";
	for (int i = 0; i < widthText.length(); i++)
		widthTextTemp += widthText[i];
	tempWidthChar = (wchar_t*)widthTextTemp.c_str();
	temp.setText(tempWidthChar);
}
void ControllerFormGL::enableThreeD()
{
	Win::CheckBox temp;
	temp.set(handle, IDC_TEXT_DEPTH);
	model->setThreeD(true);
	temp.enable();
}
void ControllerFormGL::setText(int ID, string text)
{
	Win::TextBox temp;
	temp.set(handle, ID);

	stringstream convert;
	wchar_t * tempWidthChar;
	convert << text;
	string widthText;
	wstring widthTextTemp = L"";
	convert >> widthText;
	for (int i = 0; i < widthText.length(); i++)
		widthTextTemp += widthText[i];
	tempWidthChar = (wchar_t*)widthTextTemp.c_str();

	temp.setText(tempWidthChar);
}
void ControllerFormGL::enableRuleButtons()
{
	for (int i = OPTIONS_START; i < OPTIONS_END; i++)
	{
		EnableMenuItem(GetMenu(mainHandle), i, MF_ENABLED);
	}

	Win::CheckBox temp;

	view->enableInclusive(IDC_1D_SET_CENTER, IDC_1D_SET_CENTER);
	view->enableInclusive(IDC_RANDOMISE_GRID_BUTTON, IDC_LOOPING);
	view->enableInclusive(IDC_QUICK_SAVE, IDC_QUICK_LOAD);
	view->enableInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_ADJACENT_NEIGHBORS);
	view->enable(IDC_SQUARE, END_2D);

	int rulesToEnable = 9;
	switch (model->simulator.getTwoDimentionalTessellation())
	{
	case 0: 
		if(model->simulator.getSquareNeighborType() == 0)
			rulesToEnable = 9; 
		else if (model->simulator.getSquareNeighborType() == 1)
			rulesToEnable = 5;
		break;
	case 1: rulesToEnable = 4; break;
	case 2: rulesToEnable = 7; break;
	default: rulesToEnable = 9; break;
	}
	for (int i = 0; i < rulesToEnable; i++)
	{
		temp.set(handle, IDC_BOX_DIE_1 + i);
		temp.enable();
	}
	for (int i = 0; i < rulesToEnable; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1 + i);
		temp.enable();
	}

	for (int i = 0; i < 8; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1D_1 + i);
		temp.enable();
	}
	temp.set(handle, IDC_SET_1D_GRID);
	temp.enable();
	temp.set(handle, IDC_SET_2D_GRID);
	temp.enable();
	temp.set(handle, IDC_SET_2S_GRID);
	temp.enable();
	temp.set(handle, IDC_SET_3D_GRID);
	temp.enable();
}

void ControllerFormGL::calculate1DRule()
{
	Win::CheckBox temp;
	int rule = 0;
	for (int i = 0; i < 8; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1D_8 - i);
		if (temp.isChecked())
			rule += pow(2, i);
	}
	model->simulator.getRuleGrid()->removeAllEquasions();
	model->simulator.getRuleGrid()->addOneDimentionalEquasion(rule, One_Dimentional_Adjacent_Neighbors, model->simulator.getLooping());

	stringstream convert;
	wchar_t * tempWidthChar;
	convert << rule;
	string widthText;
	wstring widthTextTemp = L"Rule No: ";
	convert >> widthText;
	for (int i = 0; i < widthText.length(); i++)
		widthTextTemp += widthText[i];
	tempWidthChar = (wchar_t*)widthTextTemp.c_str();
	view->text1D.setText(tempWidthChar);
}
void ControllerFormGL::calculate2DRules()
{
	Win::CheckBox temp;
	neighbors neighborType = Surrounding_Neighbors; //Surrounding by default
	if (model->simulator.getSquareNeighborType() == 1)	//Adjacent
		neighborType = Adjacent_Neighbors;

	for (int i = 0; i < 9; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1 + i);
		if (temp.isChecked()) 
			model->simulator.getRuleGrid()->addEquasion(-11 - i, neighborType, Equal_To, 0, 1, i, 1, model->simulator.getLooping());
	}
	for (int i = 0; i < 9; i++)
	{
		temp.set(handle, IDC_BOX_DIE_1 + i);
		if (temp.isChecked())
			model->simulator.getRuleGrid()->addEquasion(-1 - i, neighborType, Equal_To, 1, 1, i, 0, model->simulator.getLooping());
	}
}
void ControllerFormGL::calculate2DRulesHex()
{
	Win::CheckBox temp;
	for (int i = 0; i < 7; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1 + i);
		if (temp.isChecked())
			model->simulator.getRuleGrid()->addEquasion(-11 - i, Surrounding_Neighbors_Hex, Equal_To, 0, 1, i, 1, model->simulator.getLooping());
	}
	for (int i = 0; i < 7; i++)
	{
		temp.set(handle, IDC_BOX_DIE_1 + i);
		if (temp.isChecked())
			model->simulator.getRuleGrid()->addEquasion(-1 - i, Surrounding_Neighbors_Hex, Equal_To, 1, 1, i, 0, model->simulator.getLooping());
	}
}
void ControllerFormGL::calculate2DRulesTriangle()
{
	Win::CheckBox temp;
	for (int i = 0; i < 4; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1 + i);
		if (temp.isChecked())
			model->simulator.getRuleGrid()->addEquasion(-11 - i, Surrounding_Neighbors_Triangles, Equal_To, 0, 1, i, 1, model->simulator.getLooping());
	}
	for (int i = 0; i < 4; i++)
	{
		temp.set(handle, IDC_BOX_DIE_1 + i);
		if (temp.isChecked())
			model->simulator.getRuleGrid()->addEquasion(-1 - i, Surrounding_Neighbors_Triangles, Equal_To, 1, 1, i, 0, model->simulator.getLooping());
	}
}
void ControllerFormGL::calculate2DStepRules()
{
	Win::CheckBox temp;
	for (int i = 0; i < 9; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1 + i);
		if (temp.isChecked())
			model->simulator.getRuleGrid()->addTwoStepEquasion(-11 - i, Surrounding_Neighbors, Equal_To, 0, 1, i, 1, model->simulator.getLooping());
	}
	for (int i = 0; i < 9; i++)
	{
		temp.set(handle, IDC_BOX_DIE_1 + i);
		if (temp.isChecked())
			model->simulator.getRuleGrid()->addTwoStepEquasion(-1 - i, Surrounding_Neighbors, Equal_To, 1, 1, i, 0, model->simulator.getLooping());
	}
}
void ControllerFormGL::calculate3DRules()
{
	model->simulator.getRuleGrid()->addEquasion(-2, Surrounding_Neighbors_Three_D, Less_Than, 1, 1, 4, 0, model->simulator.getLooping());
	model->simulator.getRuleGrid()->addEquasion(-14, Surrounding_Neighbors_Three_D, Equal_To, 0, 1, 4, 1, model->simulator.getLooping());
	model->simulator.getRuleGrid()->addEquasion(-2, Surrounding_Neighbors_Three_D, Greater_Than, 1, 1, 4, 0, model->simulator.getLooping());
}
void ControllerFormGL::setInfo()
{
	stringstream convert;
	string totalSteps = "Total-Steps:";
	string tempSteps;
	convert << model->simulator.getSteps();
	convert >> tempSteps;
	totalSteps += tempSteps;
	setText(IDC_TOTAL_STEPS, totalSteps);

	convert.clear();
	totalSteps = "";
	convert << model->simulator.getStepsToDo();
	convert >> totalSteps;
	setText(IDC_SET_STEPS, totalSteps);
}
void ControllerFormGL::pauseSimualtion()
{
	if (flag)
	{
		(*drawMutex).lock();
		model->animate(false);
		view->animate(false);
		flag = false;
		enableRuleButtons();
		view->setGrid.enable();
		(*drawMutex).unlock();
	}
}

void ControllerFormGL::setInfoFlag()
{
	infoFlag = true;
}
void ControllerFormGL::setCalculateUIFlag()
{
	calculateUIFlag = true;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::command(int id, int command, LPARAM msg)
{
	bool doSave = true;

	if (infoFlag)
	{
		setInfo(); 
		infoFlag = false;
	}
	if (calculateUIFlag)
	{
		calculateUIfromSimulator();
		calculateUIFlag = false;
	}

    switch(id)
	{
	case IDC_CLOSE_READY:
		model->simulator.initViewController(NULL);
		break;
	case IDC_BOX_LIVE_1D_1:
	case IDC_BOX_LIVE_1D_2:
	case IDC_BOX_LIVE_1D_3:
	case IDC_BOX_LIVE_1D_4:
	case IDC_BOX_LIVE_1D_5:
	case IDC_BOX_LIVE_1D_6:
	case IDC_BOX_LIVE_1D_7:
	case IDC_BOX_LIVE_1D_8:
		if (command == BN_CLICKED)
		{
			calculate1DRule();
		}
		break;
	case IDC_1D_SET_CENTER:
		if (command == BN_CLICKED)
		{
			pauseSimualtion();

			model->simulator.wipeGrid();

			int tempX = model->simulator.getGrid()->getWidth() / 2;
			int tempY = model->simulator.getGrid()->getHeight() / 2;
			int tempZ = model->simulator.getGrid()->getDepth() / 2;

			if (model->simulator.getCAType() == 0)
				tempY = 0;
			else if (model->simulator.getCAType() == 2)
				tempZ = 0;

			model->setDrawHideBuffer(0);
			model->simulator.setCellID(tempX, tempY, tempZ, 1);

		}; break;
	case IDC_BOX_DIE_1:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-1); };	break;
	case IDC_BOX_DIE_2:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-2); };	break;
	case IDC_BOX_DIE_3:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-3); };	break;
	case IDC_BOX_DIE_4:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-4); };	break;
	case IDC_BOX_DIE_5:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-5); };	break;
	case IDC_BOX_DIE_6:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-6); };	break;
	case IDC_BOX_DIE_7:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-7); };	break;
	case IDC_BOX_DIE_8:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-8); };	break;
	case IDC_BOX_DIE_9:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-9); };	break;
	case IDC_BOX_LIVE_1:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-11); };	break;
	case IDC_BOX_LIVE_2:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-12); };	break;
	case IDC_BOX_LIVE_3:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-13); };	break;
	case IDC_BOX_LIVE_4:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-14); };	break;
	case IDC_BOX_LIVE_5:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-15); };	break;
	case IDC_BOX_LIVE_6:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-16); };	break;
	case IDC_BOX_LIVE_7:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-17); };	break;
	case IDC_BOX_LIVE_8:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-18); };	break;
	case IDC_BOX_LIVE_9:
		if (command == BN_CLICKED){ model->simulator.simpleRuleToggle(-19); };	break;
	case IDC_GRID:
		if (command == BN_CLICKED){ model->toggleGrid(); };	break; 
	case IDC_LOOPING:
		if (command == BN_CLICKED){ model->simulator.toggleLooping(); };	break; 
	case IDC_EDIT_RULES_BUTTON:
		if (command == BN_CLICKED)
		{
			editingRules = !editingRules;
			if (editingRules)
			{
				enterRuleMode();
			}
			else
			{
				exitRuleMode();
			}
		}
		break;
    case IDC_ANIMATE:
        if(command == BN_CLICKED)
        {
            flag = !flag;
			if (flag)
			{
				disableRuleButtons();
				view->setGrid.disable();

				Win::EditBox tempTextBox;
				tempTextBox.set(handle, IDC_SET_STEPS);
				wchar_t *textBoxSteps = new wchar_t[tempTextBox.getTextLength() + 1];
				tempTextBox.getText(textBoxSteps, tempTextBox.getTextLength() + 1);
				int tempSteps = convertWChar_TToInt(textBoxSteps);
				if (tempSteps > 0)
					model->simulator.setStepsToDo(tempSteps);
			}
			else
			{
				(*drawMutex).lock();
				enableRuleButtons();
				view->setGrid.enable();
				(*drawMutex).unlock();
			}
			model->animate(flag);
			view->animate(flag);
        }
		break;
	case IDC_SYNC_DISPLAY:
		if (command == BN_CLICKED)
		{
			model->simulator.toggleSyncDisplay();
		}
		break; 
	case IDC_CLEAR_ADV_RULE_ID:
		if (command == BN_CLICKED)
		{
			wchar_t *textBox = new wchar_t[view->ruleBoxText.getTextLength() + 1];
			view->ruleBoxText.getText(textBox, view->ruleBoxText.getTextLength() + 1);
			int tempBox1 = convertWChar_TToInt(textBox);

			bool success = false;

			if (tempBox1 >= 0)
			{
				success = model->simulator.getRuleGrid()->removeEquasion(tempBox1);
				calculateRuleText();
			}
			if (!success)
			{
				view->ruleBoxText.setText(L"Err");
			}
			else
			{
				view->ruleBoxText.setText(L"");
			}
		}
		break;
	case IDC_ADD_ADV_RULE:
		if (command == BN_CLICKED)
		{
			wchar_t *textBoxAdv1 = new wchar_t[view->advBox1.getTextLength() + 1];
			view->advBox1.getText(textBoxAdv1, view->advBox1.getTextLength() + 1);
			int tempBox1 = convertWChar_TToInt(textBoxAdv1);

			wchar_t *textBoxAdv2 = new wchar_t[view->advBox2.getTextLength() + 1];
			view->advBox2.getText(textBoxAdv2, view->advBox2.getTextLength() + 1);
			int tempBox2 = convertWChar_TToInt(textBoxAdv2);

			bool symbolSuccess = true;
			symbols tempSymbol;
			wchar_t *textBoxAdv3 = new wchar_t[view->advBox3.getTextLength() + 1];
			view->advBox3.getText(textBoxAdv3, view->advBox3.getTextLength() + 1);
			switch (textBoxAdv3[0])
			{
			case '<':
				tempSymbol = Less_Than;
				break;
			case '=':
				tempSymbol = Equal_To;
				break;
			case '>':
				tempSymbol = Greater_Than;
				break;
			default:
				symbolSuccess = false;
			}

			wchar_t *textBoxAdv4 = new wchar_t[view->advBox4.getTextLength() + 1];
			view->advBox4.getText(textBoxAdv4, view->advBox4.getTextLength() + 1);
			int tempBox4 = convertWChar_TToInt(textBoxAdv4);

			wchar_t *textBoxAdv5 = new wchar_t[view->advBox5.getTextLength() + 1];
			view->advBox5.getText(textBoxAdv5, view->advBox5.getTextLength() + 1);
			int tempBox5 = convertWChar_TToInt(textBoxAdv5);

			int tempID = model->simulator.getRuleGrid()->getFirstFreeID();

			model->setSafeToDraw(false);
			if (!model->simulator.getInStep() && tempBox1 >= 0 && symbolSuccess &&
				tempBox2 >= 0 && tempBox4 >= 0 && tempBox5 >= 0)
			{
				//PIEPIEPIE ALSO LOOK INTO SELECTING THE ID TO DRAW AND ALTER
				neighbors neighborType;
				switch (model->simulator.getCAType())
				{
				case 1:	//Normal 2D
					switch (model->simulator.getTwoDimentionalTessellation())
					{
					case 0:	//Square
						neighborType = Surrounding_Neighbors; //Surrounding by default
						if (model->simulator.getSquareNeighborType() == 1)	//Adjacent
							neighborType = Adjacent_Neighbors;
						break;
					case 1:	//Triangle
						neighborType = Surrounding_Neighbors_Triangles;
						break;
					case 2:	//Hex
						neighborType = Surrounding_Neighbors_Hex;
						break;
					}
					model->simulator.getRuleGrid()->addEquasion(tempID, neighborType, tempSymbol,
						tempBox1, tempBox2, tempBox4, tempBox5, model->simulator.getLooping());
					break;
				case 2:
					//2D Step
					neighborType = Surrounding_Neighbors; //Surrounding by default
					if (model->simulator.getSquareNeighborType() == 1)	//Adjacent
						neighborType = Adjacent_Neighbors;
					model->simulator.getRuleGrid()->addTwoStepEquasion(tempID, neighborType,tempSymbol,
						tempBox1, tempBox2, tempBox4, tempBox5, model->simulator.getLooping());
					break;
				case 3:
					//3D rule
					model->simulator.getRuleGrid()->addEquasion(tempID, Surrounding_Neighbors_Three_D,
						tempSymbol, tempBox1, tempBox2, tempBox4, tempBox5, model->simulator.getLooping());
					break;
				}
				view->advBox1.setText(L"");
				view->advBox2.setText(L"");
				view->advBox3.setText(L"");
				view->advBox4.setText(L"");
				view->advBox5.setText(L"");
				calculateRuleText();
			}
			else
			{
				view->advBox1.setText(L"Err");
				view->advBox2.setText(L"or");
				view->advBox3.setText(L"try");
				view->advBox4.setText(L"Ag");
				view->advBox5.setText(L"ain");
			}
			model->setSafeToDraw(true);
		}
		break;
	case IDC_ADVANCED_CONTROLS:
		if (command == BN_CLICKED)
		{
			advancedRules = !advancedRules;
			model->simulator.setAdvanced(advancedRules);
			if (advancedRules)
			{
				if (model->simulator.getCAType() != 0)
				{
					pauseSimualtion();
					model->simulator.getRuleGrid()->removeAllEquasions();
					view->setHidden(START_1D, END_1D);
					view->setHidden(START_2D, END_2D_RULES);
					view->setHidden(START_3D, END_3D);
					view->setVisible(START_ADVANCED_RULES, END_ADVANCED_RULES);
				}
			}
			else
			{
				pauseSimualtion();

				view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);

				model->simulator.getRuleGrid()->removeAllEquasions();

				if (model->simulator.getCAType() == 0)
					calculate1DRule();
				else if (model->simulator.getCAType() == 1)
				{
					if (model->simulator.getTwoDimentionalTessellation() == 0)
						calculate2DRules();
					else if (model->simulator.getTwoDimentionalTessellation() == 1)
						calculate2DRulesTriangle();
					else if (model->simulator.getTwoDimentionalTessellation() == 2)
						calculate2DRulesHex();
				}
				else if (model->simulator.getCAType() == 2)
					calculate2DStepRules();
				else if (model->simulator.getCAType() == 3)
					calculate3DRules();

				calculateUIfromSimulator();
			}
		}
		break;
	case IDC_ID_SELECT_BUTTON:
		if (command == BN_CLICKED)
		{
			wchar_t *selectionTxt = new wchar_t[view->selectionBox.getTextLength() + 1];
			view->selectionBox.getText(selectionTxt, view->selectionBox.getTextLength() + 1);
			int tempSelection = convertWChar_TToInt(selectionTxt);
			if (tempSelection > 0)
			{
				model->setSelectedID(tempSelection);

				//Changing colour bars based on the IDs colour
				float* RGB = model->getIDcolourRGB(tempSelection);
				int R, G, B;
				R = RGB[0] * 255;
				G = RGB[1] * 255;
				B = RGB[2] * 255;
				view->updateTrackbars(R, G, B);
			}
			else
			{
				wchar_t * tempChar;
				stringstream convert;
				convert << model->getSelectedID();
				string tempText;
				wstring tempTextTemp;
				convert >> tempText;
				for (int i = 0; i < tempText.length(); i++)
					tempTextTemp += tempText[i];
				tempChar = (wchar_t*)tempTextTemp.c_str();
				view->selectionBox.setText(tempChar);
			}
		}
		break;
	case IDC_SINGLE_STEP:
		if (command == BN_CLICKED)
		{
			if (flag)
				pauseSimualtion();
			else
				model->simulator.mainLoop();
		}
		break;
	case IDC_SURROUNDING_NEIGHBORS:
		if (command == BN_CLICKED)
		{
			view->setSquareNeighborBox(0);
			model->simulator.setSquareNeighborType(0);			
		}
		break;
	case IDC_ADJACENT_NEIGHBORS:
		if (command == BN_CLICKED)
		{
			view->setSquareNeighborBox(1);
			model->simulator.setSquareNeighborType(1);
		}
		break;
	case IDC_CLEAR_ADV_RULE:
		if (command == BN_CLICKED)
		{
			pauseSimualtion();
			model->simulator.getRuleGrid()->removeAllEquasions();
			calculateRuleText();
		}
		break;
	case IDC_SET_1D_GRID:
		if (command == BN_CLICKED)
		{
			if (flag)
				pauseSimualtion();
			model->simulator.setCAType(0);
			model->setTwoDimentionalTessellation(0);
			calculate1DRule();
			model->setDrawHideBuffer(0);
			view->setHidden(START_2D, END_2D);
			view->setHidden(START_3D, END_3D);
			view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
			view->setVisible(START_1D, END_1D);
			disableThreeD();
			model->setDrawHideBuffer(0);
			model->calculateDrawInfo();
			model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
		}
		break;
	case IDC_SET_2D_GRID:
		if (command == BN_CLICKED)
		{
			if (flag)
				pauseSimualtion();
			view->setTBox(0);
			model->simulator.setCAType(1);
			model->setTwoDimentionalTessellation(0);
			model->simulator.getRuleGrid()->removeAllEquasions();
			view->setHidden(START_1D, END_1D);
			view->setHidden(START_3D, END_3D);
			view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
			if (!advancedRules)
			{
				view->setVisible(START_2D, END_2D);
				calculate2DRules();
			}
			else
			{
				view->setVisible(START_ADVANCED_RULES, END_ADVANCED_RULES);
			}
			view->setVisibleInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);
			view->setVisible(START_2D_TESSELATION, END_2D);
			disableThreeD();
			model->setDrawHideBuffer(0);
			model->calculateDrawInfo();
			model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
		}
		break;
	case IDC_SET_2S_GRID:
		if (command == BN_CLICKED)
		{
				if (flag)
					pauseSimualtion();
				model->simulator.setCAType(2);
				model->setTwoDimentionalTessellation(0);
				model->simulator.getRuleGrid()->removeAllEquasions();
				view->setHidden(START_1D, END_1D);
				view->setHidden(START_3D, END_3D);
				view->setHiddenInclusive(IDC_SQUARE, IDC_HEX);
				view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
				if (!advancedRules)
				{
					view->setVisible(START_2D, END_2D_STEP);
					calculate2DStepRules();
				}
				else
				{
					view->setVisible(START_ADVANCED_RULES, END_ADVANCED_RULES);
				}
				view->setVisibleInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);

				enableThreeD();
				model->setDrawHideBuffer(0);
				model->calculateDrawInfo();
				model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
		}
		break;
	case IDC_SET_3D_GRID:
		if (command == BN_CLICKED)
		{
			if (flag)
				pauseSimualtion();
			model->simulator.setCAType(3);
			model->setTwoDimentionalTessellation(0);
			model->simulator.getRuleGrid()->removeAllEquasions();
			view->setHidden(START_1D, END_1D);
			view->setHidden(START_2D, END_2D);
			view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
			if (!advancedRules)
			{
				view->setVisible(START_3D, END_3D);
				calculate3DRules();
			}
			else
			{
				view->setVisible(START_ADVANCED_RULES, END_ADVANCED_RULES);
			}
			enableThreeD();
			model->setDrawHideBuffer(0);
			model->calculateDrawInfo();
			model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
		}
		break;
	case IDC_SQUARE:
		if (command == BN_CLICKED)
		{
			view->setTBox(0);
			if (model->setTwoDimentionalTessellation(0))
			{
				if (flag)
					pauseSimualtion();
				view->setVisibleInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);
				model->simulator.getRuleGrid()->removeAllEquasions();
				model->calculateDrawInfo();
				if (!advancedRules)
					calculate2DRules();
				model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
			}
		}
		break;
	case IDC_TRIANGLE:
		if (command == BN_CLICKED)
		{
			view->setTBox(1);
			if (model->setTwoDimentionalTessellation(1))
			{
				if (flag)
					pauseSimualtion();
				view->setHiddenInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);
				model->simulator.getRuleGrid()->removeAllEquasions();
				if (!advancedRules)
					calculate2DRulesTriangle();
				model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
			}
		}
		break;
	case IDC_HEX:
		if (command == BN_CLICKED)
		{
			view->setTBox(2);
			if (model->setTwoDimentionalTessellation(2))
			{
				if (flag)
					pauseSimualtion();
				view->setHiddenInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);
				model->simulator.getRuleGrid()->removeAllEquasions();
				if (!advancedRules)
					calculate2DRulesHex();
				model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
			}
		}
		break;
	case IDC_ADV_SCROLL_UP:
		if (command == BN_CLICKED)
		{
			advRuleScroll--;
			if (advRuleScroll < 0)
				advRuleScroll = 0;
			calculateRuleText();
		}
		break;
	case IDC_ADV_SCROLL_DOWN:
		if (command == BN_CLICKED)
		{
			advRuleScroll++;
			if (advRuleScroll + 4 > model->simulator.getRuleGrid()->getRules().size())
			{
				advRuleScroll = model->simulator.getRuleGrid()->getRules().size() - 4;
			}
			if (advRuleScroll < 0)
				advRuleScroll = 0;
			calculateRuleText();
		}
		break;
	case IDC_RESET_CAMERA:
		if (command == BN_CLICKED)
		{
			model->resetCamera();
		}
		break;
	case IDC_RANDOMISE_GRID_BUTTON:
		if (command == BN_CLICKED)
		{
			if (flag)
				pauseSimualtion();
			int barPos = (int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0);

			int SharkIDs[2] = { 1, 2 };
			int SharkChances[2] = { 60 - (barPos / 5), 15 + (barPos / 5) };	//base is 50 and 25
			int SegregationIDs[2] = { 1, 2 };
			int SegregationChances[2] = { 65 - (barPos / 2), 15 + (barPos / 2) };	//base is 40 and 40

			if (model->simulator.getRuleGrid()->getRules().size() > 0)
			{
				switch (model->simulator.getRuleGrid()->getRules()[0]->getRandID())
				{
				case 1:
					model->simulator.arrayRandomise(2, SharkIDs, SharkChances);
					break;
				case 2:
					model->simulator.arrayRandomise(2, SegregationIDs, SegregationChances);
					break;
				default:
					model->simulator.simpleRandomise(barPos);
					break;
				}
			}
			model->setDrawHideBuffer(0);
		}
		break;
	case ID_ADV_SHARK:
		if (command == BN_CLICKED)
		{
			if (model->simulator.getGrid()->getDepth() > 1)
			{
				model->simulator.setGridSize(model->simulator.getGrid()->getWidth(), model->simulator.getGrid()->getHeight(), 1);
				model->calculateDrawInfo();
			}
			(*drawMutex).lock();
			view->setHidden(START_1D, END_1D);
			view->setHidden(START_2D, END_2D);
			view->setHidden(START_3D, END_3D);
			view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
			view->setHiddenInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);
			view->setVisibleInclusive(IDC_ID_SELECT, IDC_ID_SELECT_BUTTON);
			exitRuleMode();
			disableThreeD();
			model->simulator.getRuleGrid()->removeAllEquasions();
			model->simulator.getRuleGrid()->addSharkFish(model->simulator.getRuleGrid());
			model->simulator.getRuleGrid()->getRules()[0]->setLoopingGrid(model->simulator.getLooping());
			model->simulator.wipeGrid();
			int IDs[2] = { 1, 2 };
			int Chances[2] = { 50, 25 };
			model->simulator.arrayRandomise(2, IDs, Chances);
			(*drawMutex).unlock();
		}
		break;
	case IDC_CLEAR_GRID:
		if (command == BN_CLICKED)
		{
			pauseSimualtion();
			model->simulator.wipeGrid();
			model->setDrawHideBuffer(0);
		}
		break;
	case ID_ADV_SEGREGATION:
		if (command == BN_CLICKED)
		{
			if (model->simulator.getGrid()->getDepth() > 1)
			{
				model->simulator.setGridSize(model->simulator.getGrid()->getWidth(), model->simulator.getGrid()->getHeight(), 1);
				model->calculateDrawInfo();
			}
			(*drawMutex).lock();
			view->setHidden(START_1D, END_1D);
			view->setHidden(START_2D, END_2D);
			view->setHidden(START_3D, END_3D);
			view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
			view->setHiddenInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);
			view->setVisibleInclusive(IDC_ID_SELECT, IDC_ID_SELECT_BUTTON);
			exitRuleMode();
			disableThreeD();
			model->simulator.getRuleGrid()->removeAllEquasions();
			model->simulator.getRuleGrid()->addSegregation(model->simulator.getRuleGrid());
			model->simulator.getRuleGrid()->getRules()[0]->setLoopingGrid(model->simulator.getLooping());
			model->simulator.wipeGrid();
			int IDs[2] = { 1, 2 };
			int Chances[2] = { 40, 40 };
			model->simulator.arrayRandomise(2, IDs, Chances);
			(*drawMutex).unlock();
		}
		break;
	case IDC_SET_GRID:
		if (command == BN_CLICKED)
		{
			view->disableInclusive(IDC_ANIMATE, IDC_ANIMATE);
			view->disableInclusive(IDC_SINGLE_STEP, IDC_SINGLE_STEP);
			disableRuleButtons();

			model->setDrawHideBuffer(0);

			wchar_t *textBoxWidth = new wchar_t[view->width.getTextLength() + 1];
			view->width.getText(textBoxWidth, view->width.getTextLength() + 1);
			int tempWidth = convertWChar_TToInt(textBoxWidth);

			wchar_t *textBoxHeight = new wchar_t[view->height.getTextLength() + 1];
			view->height.getText(textBoxHeight, view->height.getTextLength() + 1);
			int tempHeight = convertWChar_TToInt(textBoxHeight);

			wchar_t *textBoxDepth = new wchar_t[view->depth.getTextLength() + 1];
			view->depth.getText(textBoxDepth, view->depth.getTextLength() + 1);
			int tempDepth = convertWChar_TToInt(textBoxDepth);

			model->setSafeToDraw(false);
			if (tempWidth > 0 && tempHeight > 0 && tempDepth > 0)
			{
				if (!model->simulator.getInStep())
				{
					model->simulator.setGridSize(tempWidth, tempHeight, tempDepth);
					model->simulator.simpleRandomise((int)::SendMessage(view->randomiseBar.getHandle(), TBM_GETPOS, 0, 0));
					model->calculateDrawInfo();
				}
			}
			model->setSafeToDraw(true);
			updateDimentionBoxes();

			enableRuleButtons();
			view->enableInclusive(IDC_ANIMATE, IDC_ANIMATE);
			view->enableInclusive(IDC_SINGLE_STEP, IDC_SINGLE_STEP);
		}
		break;
	case IDC_QUICK_SAVE:
		if (model->simulator.getRuleGrid()->getRules().size() > 0)
		{
			if (model->simulator.getRuleGrid()->getRules()[0]->getRandID() > 0)
			{
				doSave = false;
			}
		}
		if (doSave)
		{
			saveFile("TempQuick");
		}
		break;
	case IDC_QUICK_LOAD:
		loadFile("TempQuick");
		break;
	case ID_FILE_MENU_SAVE:
		saveFile();
		break;
	case ID_FILE_MENU_LOAD:
		loadFile();
		break;
    }

    return 0;
}
void ControllerFormGL::calculateRuleText()
{
	string tempBox;
	stringstream convert;
	convert << model->simulator.getRuleGrid()->getRules().size();
	convert >> tempBox;
	string ruleBoxText = "Adv Rules: ";
	ruleBoxText += tempBox;

	string ruleText = "";
	int startPoint = advRuleScroll;
	int endPoint = startPoint + 4;
	if (startPoint + 4 > model->simulator.getRuleGrid()->getRules().size())
	{
		advRuleScroll = model->simulator.getRuleGrid()->getRules().size();
		startPoint = model->simulator.getRuleGrid()->getRules().size() - 4;
		if (startPoint < 0)
			startPoint = 0;
		int endPoint = startPoint + 4;
	}
	for (int i = startPoint; i < endPoint && i < model->simulator.getRuleGrid()->getRules().size(); i++)
	{
		ruleText += model->simulator.getRuleGrid()->getRules()[i]->GetEquasionAsString();
		ruleText += "\n";
	}

	view->setRules(ruleBoxText,ruleText);
}
void ControllerFormGL::prepareForFileDialog()
{
	pauseSimualtion();
	EnableMenuItem(GetMenu(mainHandle), ID_FILE_MENU_SAVE, MF_DISABLED);
	EnableMenuItem(GetMenu(mainHandle), ID_FILE_MENU_LOAD, MF_DISABLED);
	view->disableInclusive(IDC_ANIMATE, IDC_ANIMATE);
	view->disableInclusive(IDC_SINGLE_STEP, IDC_SINGLE_STEP);
	model->setSafeToDraw(false);
}
void ControllerFormGL::recoverFromFileDialog()
{
	EnableMenuItem(GetMenu(mainHandle), ID_FILE_MENU_SAVE, MF_ENABLED);
	EnableMenuItem(GetMenu(mainHandle), ID_FILE_MENU_LOAD, MF_ENABLED);
	model->calculateDrawInfo();
	calculateUIfromSimulator();
	model->setSafeToDraw(true);
	view->enableInclusive(IDC_ANIMATE, IDC_ANIMATE);
	view->enableInclusive(IDC_SINGLE_STEP, IDC_SINGLE_STEP);
	calculateRuleText();
}
void ControllerFormGL::saveFile(string fileName)
{
	model->simulator.save(fileName);
}
void ControllerFormGL::loadFile(string fileName)
{
	model->simulator.open(fileName);
}
void ControllerFormGL::enterRuleMode()
{
	view->setHidden(START_GRID, END_GRID);
	view->setHiddenInclusive(IDC_1D_SET_CENTER, IDC_1D_SET_CENTER);
	view->setVisible(START_RULE_EDIT, END_RULE_EDIT);
	calculateRuleText();
}
void ControllerFormGL::exitRuleMode()
{
	model->setSelectedID(1);
	view->selectionBox.setText(L"1");
	view->setVisible(START_GRID, END_GRID);
	view->setVisibleInclusive(IDC_1D_SET_CENTER, IDC_1D_SET_CENTER);
	view->setHidden(START_RULE_EDIT, END_RULE_EDIT);
}
void ControllerFormGL::calculateUIfromSimulator()
{
	model->calculateDrawInfo();
	model->resetCamera();
	model->setDrawHideBuffer(0);
	model->setSelectedID(1);
	view->selectionBox.setText(L"1");
	editingRules = false;
	exitRuleMode();
	view->setHidden(START_1D, END_1D);
	view->setHidden(START_2D, END_2D);
	view->setHidden(START_3D, END_3D);
	view->setHidden(START_ADVANCED_RULES, END_ADVANCED_RULES);
	view->setHiddenInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);

	advancedRules = model->simulator.getAdvanced();

	Win::CheckBox temp;

	updateDimentionBoxes();
	temp.set(handle, IDC_LOOPING);
	if (model->simulator.getLooping())
		temp.check();
	else
		temp.uncheck();

	switch (model->simulator.getCAType())
	{
	case 0:
		disableThreeD();
		calculate1DRuleCheckBoxesFromSimulator();
		view->setVisible(START_1D, END_1D);
		break;
	case 1:
		disableThreeD();
		if (!model->simulator.getAdvanced())
		{
			calculate2DRuleCheckBoxesFromSimulator();
			view->setVisible(START_2D, END_2D);
		}
		else
		{
			view->setVisible(START_2D_NEIGHBORS, END_2D);
		}

		for (int i = 0; i < 3; i++)
		{
			temp.set(handle, IDC_SQUARE + i);
			temp.uncheck();
		}
		temp.set(handle, IDC_SQUARE + model->simulator.getTwoDimentionalTessellation());
		temp.check();
		if (model->simulator.getTwoDimentionalTessellation() != 0)
			view->setHiddenInclusive(IDC_SURROUNDING_NEIGHBORS, IDC_NEIGHBOR_BOX);
		break;
	case 2:
		enableThreeD();
		if (!model->simulator.getAdvanced())
		{
			calculate2DRuleCheckBoxesFromSimulator();
			view->setVisible(START_2D, END_2D_STEP);
		}
		else
		{
			view->setVisible(START_2D_NEIGHBORS, END_2D_STEP);
		}
		break;
	case 3:
		enableThreeD();
		if (!model->simulator.getAdvanced())
		{
			calculate3DRuleCheckBoxesFromSimulator();
			view->setVisible(START_3D, END_3D);
		}
		else
		{

		}
		break;
	}
	if (model->simulator.getAdvanced())
	{
		view->setVisible(START_ADVANCED_RULES, END_ADVANCED_RULES);
		CheckMenuItem(GetMenu(mainHandle), IDC_ADVANCED_CONTROLS, MF_CHECKED);

		//Other advanced stuff to load with advanced rules
		//PIEPIEPIE maybe a set of strings with equasions written on them?
	}
	else
	{
		CheckMenuItem(GetMenu(mainHandle), IDC_ADVANCED_CONTROLS, MF_UNCHECKED);
	}
	view->setSquareNeighborBox(model->simulator.getSquareNeighborType());
}
void ControllerFormGL::calculate1DRuleCheckBoxesFromSimulator()
{
	int tempEquasionID = model->simulator.getRuleGrid()->getRules()[0]->getID();
	Win::CheckBox temp;
	for (int i = 7; i >= 0; i--)
	{
		temp.set(handle, IDC_BOX_LIVE_1D_8 - i);
		temp.uncheck();
		if (tempEquasionID > pow(2, i) - 1)
		{
			temp.check();
			tempEquasionID -= pow(2, i);
		}
	}
}
void ControllerFormGL::calculate2DRuleCheckBoxesFromSimulator()
{
	model->simulator.getRuleGrid()->getRules().size();
	Win::CheckBox temp;
	for (int i = 0; i < 18; i++)
	{
		temp.set(handle, IDC_BOX_LIVE_1 + i);
		temp.uncheck();
	}
	for (int i = 0; i < model->simulator.getRuleGrid()->getRules().size(); i++)
	{
		int tempRuleID = model->simulator.getRuleGrid()->getRules()[i]->getID() * -1;
		if (tempRuleID > 10)
		{
			tempRuleID -= 11;
			temp.set(handle, IDC_BOX_LIVE_1 + tempRuleID);
			temp.check();
		}
		else
		{
			tempRuleID -= 1;
			temp.set(handle, IDC_BOX_DIE_1 + tempRuleID);
			temp.check();
		}
	}
}
void ControllerFormGL::calculate3DRuleCheckBoxesFromSimulator()
{

}
int ControllerFormGL::convertWChar_TToInt(wchar_t *in)
{
	int returnVal = -1;
	wstring tempTextTemp = in;
	string tempText = "";
	for (int i = 0; i < tempTextTemp.length(); i++)
		tempText += tempTextTemp[i];
	stringstream convert;
	convert << tempText;
	if (!(convert >> returnVal))
		returnVal = -1;
	return returnVal;
}

void ControllerFormGL::updateDimentionBoxes()
{
	stringstream convert;

	wchar_t * tempWidthChar;
	convert << model->simulator.getGrid()->getWidth();
	string widthText;
	wstring widthTextTemp;
	convert >> widthText;
	for (int i = 0; i < widthText.length(); i++)
		widthTextTemp += widthText[i];
	tempWidthChar = (wchar_t*)widthTextTemp.c_str();
	view->width.setText(tempWidthChar);

	convert.clear();
	wchar_t * tempHeightChar;
	convert << model->simulator.getGrid()->getHeight();
	string heightText;
	wstring heightTextTemp;
	convert >> heightText;
	for (int i = 0; i < heightText.length(); i++)
		heightTextTemp += heightText[i];
	tempHeightChar = (wchar_t*)heightTextTemp.c_str();
	view->height.setText(tempHeightChar);

	convert.clear();
	wchar_t * tempDepthChar;
	convert << model->simulator.getGrid()->getDepth();
	string depthText;
	wstring depthTextTemp;
	convert >> depthText;
	for (int i = 0; i < depthText.length(); i++)
		depthTextTemp += depthText[i];
	tempDepthChar = (wchar_t*)depthTextTemp.c_str();
	view->depth.setText(tempDepthChar);
}
///////////////////////////////////////////////////////////////////////////////
// handle horizontal scroll notification
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::hScroll(WPARAM wParam, LPARAM lParam)
{
    // check if the message comming from trackbar
    HWND trackbarHandle = (HWND)lParam;

    int position = HIWORD(wParam);              // current tick mark position
    if(trackbarHandle)
    {
        // get control ID
        int trackbarId = ::GetDlgCtrlID(trackbarHandle);

        switch(LOWORD(wParam))
        {
        case TB_THUMBTRACK:     // user dragged the slider
			view->updateTrackbars(trackbarHandle, position);
			if (trackbarId == IDC_RED)
				model->setBackgroundRed(position / 255.0f);
			else if (trackbarId == IDC_GREEN)
				model->setBackgroundGreen(position / 255.0f);
			else if (trackbarId == IDC_BLUE)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_RED2)
				model->setCellRed(position / 255.0f);
			else if (trackbarId == IDC_GREEN2)
				model->setCellGreen(position / 255.0f);
			else if (trackbarId == IDC_BLUE2)
				model->setCellBlue(position / 255.0f);
			else if (trackbarId == IDC_SPEED)
				model->setSpeed(position);
            break;

        case TB_THUMBPOSITION:  // by WM_LBUTTONUP
            break;

        case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
            break;

        case TB_LINEDOWN:       // by VK_LEFT, VK_UP
            break;

        case TB_TOP:            // by VK_HOME
            break;

        case TB_BOTTOM:         // by VK_END
            break;

        case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
            break;

        case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
            break;

        case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
            position = (int)::SendMessage(trackbarHandle, TBM_GETPOS, 0, 0);
            view->updateTrackbars(trackbarHandle, position);
            if(trackbarId == IDC_RED)
                model->setBackgroundRed(position / 255.0f);
            else if(trackbarId == IDC_GREEN)
				model->setBackgroundGreen(position / 255.0f);
			else if (trackbarId == IDC_BLUE)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_RED2)
				model->setCellRed(position / 255.0f);
			else if (trackbarId == IDC_GREEN2)
				model->setCellGreen(position / 255.0f);
			else if (trackbarId == IDC_BLUE2)
				model->setCellBlue(position / 255.0f);
			else if (trackbarId == IDC_SPEED)
				model->setSpeed(position);
            break;
        }
    }

    return 0;
}

