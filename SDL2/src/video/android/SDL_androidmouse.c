/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_ANDROID

#include "SDL_androidmouse.h"

#include "SDL_events.h"
#include "../../events/SDL_mouse_c.h"

#include "../../core/android/SDL_android.h"

#include <android/log.h>
#define LOG_TAG "SDL_android"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2
#define ACTION_MOVE_REL 3
#define ACTION_HOVER_MOVE 7
#define ACTION_SCROLL 8
#define BUTTON_PRIMARY 1
#define BUTTON_SECONDARY 2
#define BUTTON_TERTIARY 4
#define BUTTON_BACK 8
#define BUTTON_FORWARD 16

static Uint8 SDLButton;

static void (*showMouseCallback)(int) = NULL;
static void (*mouseMoveCallback)(float, float) = NULL;


void SDL_ShowMouseCallBack(void (*pt2Func)(int))
{
	showMouseCallback = pt2Func;
}

void SDL_MouseMoveCallBack(void (*pt2Func)(float, float))
{
	mouseMoveCallback = pt2Func;
}

static SDL_Cursor *CreateCursor(SDL_Surface * surface, int hot_x, int hot_y)
{
	SDL_Cursor *cursor = NULL;
	cursor = SDL_calloc(1, sizeof(*cursor));
	return cursor;
}

static void MoveCursor (struct SDL_Cursor * cursor)
{
 	SDL_Mouse *mouse = SDL_GetMouse();
 	SDL_Window *focusWindow = SDL_GetKeyboardFocus();

	LOGI("MoveCursor %d %d", mouse->x, mouse->y);
	if(mouseMoveCallback && focusWindow)
	{
		//mouseMoveCallback((float)mouse->x / (float)focusWindow->w, (float)mouse->y / (float)focusWindow->h);
		mouseMoveCallback((float)mouse->x / (float)focusWindow->w, (float)mouse->y / (float)focusWindow->h);
	}
}

static void ShowCursor (struct SDL_Cursor * cursor)
{
	LOGI("ShowCursor");
	if(showMouseCallback)
	{
		showMouseCallback(cursor!=NULL);
	}
}

void
Android_InitMouse(void)
{
    SDLButton = 0;

  	SDL_Mouse *mouse = SDL_GetMouse();

	mouse->CreateCursor = CreateCursor;
	mouse->ShowCursor = ShowCursor;
	mouse->MoveCursor = MoveCursor;
	//mouse->FreeCursor = RPI_FreeCursor;
	//mouse->WarpMouse = RPI_WarpMouse;
	//mouse->WarpMouseGlobal = RPI_WarpMouseGlobal;
}

void Android_OnMouse( int androidButton, int action, float x, float y) {
    if (!Android_Window) {
        return;
    }

    switch(action) {
        case ACTION_DOWN:
            // Determine which button originated the event, and store it for ACTION_UP
            SDLButton = SDL_BUTTON_LEFT;
            if (androidButton == BUTTON_SECONDARY) {
                SDLButton = SDL_BUTTON_RIGHT;
            } else if (androidButton == BUTTON_TERTIARY) {
                SDLButton = SDL_BUTTON_MIDDLE;
            } else if (androidButton == BUTTON_FORWARD) {
                SDLButton = SDL_BUTTON_X1;
            } else if (androidButton == BUTTON_BACK) {
                SDLButton = SDL_BUTTON_X2;
            }
            //SDL_SendMouseMotion(Android_Window, 0, 0, x, y);
            SDL_SendMouseButton(Android_Window, 0, SDL_PRESSED,  SDLButton);
            break;

        case ACTION_UP:
            // Android won't give us the button that originated the ACTION_DOWN event, so we'll
            // assume it's the one we stored
            //SDL_SendMouseMotion(Android_Window, 0, 0, x, y);
            SDL_SendMouseButton(Android_Window, 0, SDL_RELEASED, SDLButton);
            break;

        case ACTION_MOVE:
        case ACTION_HOVER_MOVE:
            SDL_SendMouseMotion(Android_Window, 0, 0, x, y);
            break;
        case ACTION_MOVE_REL:
            SDL_SendMouseMotion(Android_Window, 0, 1, x, y);
            break;
        case ACTION_SCROLL:
            SDL_SendMouseWheel(Android_Window, 0, x, y, SDL_MOUSEWHEEL_NORMAL);
            break;

        default:
            break;
    }
}

#endif /* SDL_VIDEO_DRIVER_ANDROID */

/* vi: set ts=4 sw=4 expandtab: */

