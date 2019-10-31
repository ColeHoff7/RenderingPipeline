#pragma once

#include "BasicIncludesAndDefines.h"

#include "Scene.h"


/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted.
*/
static void RenderSceneCB();

/**
* Called when the window is first opened and
* whenever the window is moved or resized. 
* 
* @param width of the window in pixels
* @param height of the window in pixels
*/
static void ResizeCB(int width, int height);

/**
* Calls a method of the the OpenGLApplicationBase class
* @param Key The generated ASCII character for the key whose press triggered the callback.
* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed.
* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void KeyboardCB(unsigned char key, int x, int y);

/**
* Responds to special  key presses.
* @param key - GLUT_KEY_* constant for the special key pressed.
* @param x The x - coordinate of the mouse relative to the window at the time the key is pressed.
* @param y The y - coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void SpecialKeysCB(int key, int x, int y);

/**
* Registered as an idle function.
*/
static void IdleCB();


static void mainMenuCB(int value);
