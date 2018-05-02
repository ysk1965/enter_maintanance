#pragma once

void Keyboard(unsigned char key, int xx, int yy);
void SpecialKeyboard(int key, int xx, int yy);
void releaseKey(int key, int x, int y);

void MouseMove(int x, int y);
void Mouse(int button, int state, int x, int y);