//============================================================================
// Name        : glutcallbacks.h
// Author      : Christian Thurow
// Description : Rendering Engine Header
//============================================================================

#ifndef GLUTCALL_H
#define GLUTCALL_H
                                         
void Init(void);
void RegisterCallbacks(void);
void Display(void);
void Reshape(int width, int height);
void Idle(void);
void Mouse(int btn, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void MouseMotion(int x, int y);
void MouseMenu(int id);

void ResetCamera();

#endif
