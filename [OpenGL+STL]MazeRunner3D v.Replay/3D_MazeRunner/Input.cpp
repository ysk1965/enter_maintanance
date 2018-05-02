#include "Input.h"
#include "resource.h"

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void Keyboard(unsigned char key, int xx, int yy)
{

	if (key == 'h')
	{
		if (mainscore > 0)
		{
			if (hint == true)
			{
				hint = false;
			}
			else
			{
				hint = true;
			}
		}
		else
		{
			hint = false;
		}
	}

	if (key == 'e') {
		if (state == false) {
			state = true;
		}
		else {
			state = false;
		}
	}

	if (key == 'w')
	{
		if (pause == false) {
			pause = true;
		}
		else {
			pause = false;
		}
	}
	if (pause == true) {
		if (key == 's') {
			it--;
			x = (*it).first.first;
			z = (*it).first.second;
			deltaAngle = (*it).second.first;
			angle = (*it).second.second;
		}
		if (key == 'd') {
			it++;
			x = (*it).first.first;
			z = (*it).first.second;
			deltaAngle = (*it).second.first;
			angle = (*it).second.second;
		}
	}
}
void SpecialKeyboard(int key, int xx, int yy)
{
	if (play_bool == true)
	{
		if (key == GLUT_KEY_LEFT)
		{
			deltaAngle = -0.025f;
		}
		if (key == GLUT_KEY_RIGHT)
		{
			deltaAngle = 0.025f;
		}
		if (key == GLUT_KEY_UP)
		{
			deltaMove = 0.65 + speedup;
		}
		if (key == GLUT_KEY_DOWN)
		{
			deltaMove = -0.65 - speedup;
		}
	}
	glutSetWindow(mainWindow);
	glutPostRedisplay();
}

void releaseKey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		//replaySave(key::Left);
	case GLUT_KEY_RIGHT: deltaAngle = 0;
		//replaySave(key::Right);
		break;

	case GLUT_KEY_UP:
		//replaySave(key::Up);
	case GLUT_KEY_DOWN: deltaMove = 0;
		//replaySave(key::Down);
		break;
	}
}

// -----------------------------------
//             MOUSE
// -----------------------------------
void MouseMove(int x, int y) {
	if (xOrigin >= 0) {

		// 카메라 앵글
		deltaAngle = (x - xOrigin) * 0.001f;

		// 카메라 디렉션
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
		{
			angle += deltaAngle;
			deltaAngle = 0.0f;
			xOrigin = -1;
		}
		else
		{
			xOrigin = x;
		}
	}
}