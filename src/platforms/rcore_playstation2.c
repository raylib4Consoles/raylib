/**********************************************************************************************
*
*   rcore_playstation2 - Functions to manage window, graphics device and inputs
*
*   PLATFORM: PLAYSTATION2
*       - PlayStation 2
*
*   LIMITATIONS:
*       - Limitation 01
*       - Limitation 02
*
*   POSSIBLE IMPROVEMENTS:
*       - Improvement 01
*       - Improvement 02
*
*   ADDITIONAL NOTES:
*       - Port done by Antonio Jose Ramos Marquez aka bigboss @psxdev		
*
*   CONFIGURATION:
*       #define RCORE_PLATFORM_CUSTOM_FLAG
*           Custom flag for rcore on target platform -not used-
*
*   DEPENDENCIES:
*       PlayStation 2 ps2dev environment ps2sdk and toolchains - Provides C API to access PlayStation 2 homebrew functionality
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5) and contributors
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <stdio.h>
#include <string.h>
#include <libpad.h>
#include <GL/gl.h>
//#include <GL/glut.h>

/* ps2stuff */
#include <ps2s/core.h>
#include <ps2s/displayenv.h>
#include <ps2s/drawenv.h>
#include <ps2s/gs.h>
#include <ps2s/timer.h>

/* ps2gl */
#include <ps2gl/debug.h>
#include <ps2gl/displaycontext.h>
#include <ps2gl/drawcontext.h>
#include <ps2gl/glcontext.h>


static char padBuf[256] __attribute__((aligned(64)));
static char actAlign[6];
static int actuators;
struct padButtonStatus buttons;
int port;
int slot;
unsigned int paddata;
unsigned int old_pad = 0;
unsigned int new_pad;

// Button mapping table
static const struct
{
    uint16_t ps2Button;
    int raylibButton;
} buttonMap[] = {
    { PAD_SELECT, GAMEPAD_BUTTON_MIDDLE_LEFT },
    { PAD_START, GAMEPAD_BUTTON_MIDDLE_RIGHT },
    { PAD_CROSS, GAMEPAD_BUTTON_RIGHT_FACE_DOWN },
    { PAD_CIRCLE, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT },
    { PAD_SQUARE, GAMEPAD_BUTTON_RIGHT_FACE_LEFT },
    { PAD_TRIANGLE, GAMEPAD_BUTTON_RIGHT_FACE_UP },
    { PAD_L1, GAMEPAD_BUTTON_LEFT_TRIGGER_1 },
    { PAD_L2, GAMEPAD_BUTTON_LEFT_TRIGGER_2 },
    { PAD_R1, GAMEPAD_BUTTON_RIGHT_TRIGGER_1 },
    { PAD_R2, GAMEPAD_BUTTON_RIGHT_TRIGGER_2 },
    { PAD_UP, GAMEPAD_BUTTON_LEFT_FACE_UP },
    { PAD_DOWN, GAMEPAD_BUTTON_LEFT_FACE_DOWN },
    { PAD_LEFT, GAMEPAD_BUTTON_LEFT_FACE_LEFT },
    { PAD_RIGHT, GAMEPAD_BUTTON_LEFT_FACE_RIGHT },
    { PAD_L3, GAMEPAD_BUTTON_LEFT_THUMB },
    { PAD_R3 ,GAMEPAD_BUTTON_RIGHT_THUMB },
};

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct {
    // TODO: Define the platform specific variables required
    int version;
} PlatformData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern CoreData CORE;                   // Global CORE state context

static PlatformData platform = { 0 };   // Platform specific data

bool firstTime = true;

//----------------------------------------------------------------------------------
// Module Internal Functions Declaration
//----------------------------------------------------------------------------------
int InitPlatform(void);          // Initialize platform (graphics, inputs and more)
bool InitGraphicsDevice(void);   // Initialize graphics device


//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
// NOTE: Functions declaration is provided by raylib.h

//----------------------------------------------------------------------------------
// Module Functions Definition: Window and Graphics Device
//----------------------------------------------------------------------------------

// Check if application should close
bool WindowShouldClose(void)
{
    if (CORE.Window.ready) return CORE.Window.shouldClose;
    else return true;
}

// Toggle fullscreen mode
void ToggleFullscreen(void)
{
    TRACELOG(LOG_WARNING, "ToggleFullscreen() not available on target platform");
}

// Toggle borderless windowed mode
void ToggleBorderlessWindowed(void)
{
    TRACELOG(LOG_WARNING, "ToggleBorderlessWindowed() not available on target platform");
}

// Set window state: maximized, if resizable
void MaximizeWindow(void)
{
    TRACELOG(LOG_WARNING, "MaximizeWindow() not available on target platform");
}

// Set window state: minimized
void MinimizeWindow(void)
{
    TRACELOG(LOG_WARNING, "MinimizeWindow() not available on target platform");
}

// Set window state: not minimized/maximized
void RestoreWindow(void)
{
    TRACELOG(LOG_WARNING, "RestoreWindow() not available on target platform");
}

// Set window configuration state using flags
void SetWindowState(unsigned int flags)
{
    TRACELOG(LOG_WARNING, "SetWindowState() not available on target platform");
}

// Clear window configuration state flags
void ClearWindowState(unsigned int flags)
{
    TRACELOG(LOG_WARNING, "ClearWindowState() not available on target platform");
}

// Set icon for window
void SetWindowIcon(Image image)
{
    TRACELOG(LOG_WARNING, "SetWindowIcon() not available on target platform");
}

// Set icon for window
void SetWindowIcons(Image *images, int count)
{
    TRACELOG(LOG_WARNING, "SetWindowIcons() not available on target platform");
}

// Set title for window
void SetWindowTitle(const char *title)
{
    CORE.Window.title = title;
}

// Set window position on screen (windowed mode)
void SetWindowPosition(int x, int y)
{
    TRACELOG(LOG_WARNING, "SetWindowPosition() not available on target platform");
}

// Set monitor for the current window
void SetWindowMonitor(int monitor)
{
    TRACELOG(LOG_WARNING, "SetWindowMonitor() not available on target platform");
}

// Set window minimum dimensions (FLAG_WINDOW_RESIZABLE)
void SetWindowMinSize(int width, int height)
{
    CORE.Window.screenMin.width = width;
    CORE.Window.screenMin.height = height;
}

// Set window maximum dimensions (FLAG_WINDOW_RESIZABLE)
void SetWindowMaxSize(int width, int height)
{
    CORE.Window.screenMax.width = width;
    CORE.Window.screenMax.height = height;
}

// Set window dimensions
void SetWindowSize(int width, int height)
{
    TRACELOG(LOG_WARNING, "SetWindowSize() not available on target platform");
}

// Set window opacity, value opacity is between 0.0 and 1.0
void SetWindowOpacity(float opacity)
{
    TRACELOG(LOG_WARNING, "SetWindowOpacity() not available on target platform");
}

// Set window focused
void SetWindowFocused(void)
{
    TRACELOG(LOG_WARNING, "SetWindowFocused() not available on target platform");
}

// Get native window handle
void *GetWindowHandle(void)
{
    TRACELOG(LOG_WARNING, "GetWindowHandle() not implemented on target platform");
    return NULL;
}

// Get number of monitors
int GetMonitorCount(void)
{
    TRACELOG(LOG_WARNING, "GetMonitorCount() not implemented on target platform");
    return 1;
}

// Get number of monitors
int GetCurrentMonitor(void)
{
    TRACELOG(LOG_WARNING, "GetCurrentMonitor() not implemented on target platform");
    return 0;
}

// Get selected monitor position
Vector2 GetMonitorPosition(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorPosition() not implemented on target platform");
    return (Vector2){ 0, 0 };
}

// Get selected monitor width (currently used by monitor)
int GetMonitorWidth(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorWidth() not implemented on target platform");
    return 0;
}

// Get selected monitor height (currently used by monitor)
int GetMonitorHeight(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorHeight() not implemented on target platform");
    return 0;
}

// Get selected monitor physical width in millimetres
int GetMonitorPhysicalWidth(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorPhysicalWidth() not implemented on target platform");
    return 0;
}

// Get selected monitor physical height in millimetres
int GetMonitorPhysicalHeight(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorPhysicalHeight() not implemented on target platform");
    return 0;
}

// Get selected monitor refresh rate
int GetMonitorRefreshRate(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorRefreshRate() not implemented on target platform");
    return 0;
}

// Get the human-readable, UTF-8 encoded name of the selected monitor
const char *GetMonitorName(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorName() not implemented on target platform");
    return "";
}

// Get window position XY on monitor
Vector2 GetWindowPosition(void)
{
    TRACELOG(LOG_WARNING, "GetWindowPosition() not implemented on target platform");
    return (Vector2){ 0, 0 };
}

// Get window scale DPI factor for current monitor
Vector2 GetWindowScaleDPI(void)
{
    TRACELOG(LOG_WARNING, "GetWindowScaleDPI() not implemented on target platform");
    return (Vector2){ 1.0f, 1.0f };
}

// Set clipboard text content
void SetClipboardText(const char *text)
{
    TRACELOG(LOG_WARNING, "SetClipboardText() not implemented on target platform");
}

// Get clipboard text content
// NOTE: returned string is allocated and freed by GLFW
const char *GetClipboardText(void)
{
    TRACELOG(LOG_WARNING, "GetClipboardText() not implemented on target platform");
    return NULL;
}

// Show mouse cursor
void ShowCursor(void)
{
    CORE.Input.Mouse.cursorHidden = false;
}

// Hides mouse cursor
void HideCursor(void)
{
    CORE.Input.Mouse.cursorHidden = true;
}

// Enables cursor (unlock cursor)
void EnableCursor(void)
{
    // Set cursor position in the middle
    SetMousePosition(CORE.Window.screen.width/2, CORE.Window.screen.height/2);

    CORE.Input.Mouse.cursorHidden = false;
}

// Disables cursor (lock cursor)
void DisableCursor(void)
{
    // Set cursor position in the middle
    SetMousePosition(CORE.Window.screen.width/2, CORE.Window.screen.height/2);

    CORE.Input.Mouse.cursorHidden = true;
}

// Swap back buffer with front buffer (screen drawing)
void SwapScreenBuffer(void)
{
    pglEndGeometry();

    if (!firstTime)
        pglFinishRenderingGeometry(PGL_DONT_FORCE_IMMEDIATE_STOP);
    else
        firstTime = false;
    pglWaitForVSync();
    pglSwapBuffers();
    pglRenderGeometry();
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Misc
//----------------------------------------------------------------------------------

// Get elapsed time measure in seconds since InitTimer()
double GetTime(void)
{   
    double time=(double)(clock() - CORE.Time.base)*1e-6;
    return time;
}

// Open URL with default system browser (if available)
// NOTE: This function is only safe to use if you control the URL given.
// A user could craft a malicious string performing another action.
// Only call this function yourself not with user input or make sure to check the string yourself.
// Ref: https://github.com/raysan5/raylib/issues/686
void OpenURL(const char *url)
{
   // Security check to (partially) avoid malicious code on target platform
    if (strchr(url, '\'') != NULL) TRACELOG(LOG_WARNING, "SYSTEM: Provided URL could be potentially malicious, avoid [\'] character");
    else
    {
        // TODO:
    }
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Inputs
//----------------------------------------------------------------------------------

// Set internal gamepad mappings
int SetGamepadMappings(const char *mappings)
{
    TRACELOG(LOG_WARNING, "SetGamepadMappings() not implemented on target platform");
    return 0;
}

// Set mouse position XY
void SetMousePosition(int x, int y)
{
    CORE.Input.Mouse.currentPosition = (Vector2){ (float)x, (float)y };
    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
}

// Set mouse cursor
void SetMouseCursor(int cursor)
{
    TRACELOG(LOG_WARNING, "SetMouseCursor() not implemented on target platform");
}


// Register all input events
void PollInputEvents(void)
{
#if defined(SUPPORT_GESTURES_SYSTEM)
    // NOTE: Gestures update must be called every frame to reset gestures correctly
    // because ProcessGestureEvent() is just called on an event, not every frame
    UpdateGestures();
#endif

    // Reset keys/chars pressed registered
    CORE.Input.Keyboard.keyPressedQueueCount = 0;
    CORE.Input.Keyboard.charPressedQueueCount = 0;

    // Reset key repeats
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) CORE.Input.Keyboard.keyRepeatInFrame[i] = 0;

    // Reset last gamepad button/axis registered state
    CORE.Input.Gamepad.lastButtonPressed = 0; // GAMEPAD_BUTTON_UNKNOWN
    //CORE.Input.Gamepad.axisCount = 0;

    // Register previous touch states
    for (int i = 0; i < MAX_TOUCH_POINTS; i++) CORE.Input.Touch.previousTouchState[i] = CORE.Input.Touch.currentTouchState[i];


    //PlayStation 2 provisional



    int ret=padGetState(port, slot);
    while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) 
    {
        if(ret==PAD_STATE_DISCONN) 
        {
            TRACELOG(LOG_INFO,"Pad(%d, %d) is disconnected", port, slot);
        }
        ret=padGetState(port, slot);
    }
    CORE.Input.Gamepad.ready[port] = true;

    // Register previous gamepad button states
    for (int k = 0; k < MAX_GAMEPAD_BUTTONS; k++) 
        CORE.Input.Gamepad.previousButtonState[port][k] = CORE.Input.Gamepad.currentButtonState[port][k];


    ret = padRead(port, slot, &buttons); // port, slot, buttons

    if (ret != 0) 
    {
        paddata = 0xffff ^ buttons.btns;
        new_pad = paddata & ~old_pad;
        old_pad = paddata;
        //MapControls(port, buttons);
        for (int j = 0; j < sizeof(buttonMap) / sizeof(buttonMap[0]); j++) 
        {
            if (paddata & buttonMap[j].ps2Button)
            {
                CORE.Input.Gamepad.currentButtonState[port][buttonMap[j].raylibButton] = 1;
                CORE.Input.Gamepad.lastButtonPressed = buttonMap[j].raylibButton;
            } 
            else 
            {
                CORE.Input.Gamepad.currentButtonState[port][buttonMap[j].raylibButton] = 0;
            }
        }



    }

        

      /*  // Directions
        if(new_pad & PAD_LEFT) 
        {
            TRACELOG(LOG_INFO,"LEFT");
        }
        if(new_pad & PAD_DOWN) 
        {
            TRACELOG(LOG_INFO,"DOWN");
        }
        if(new_pad & PAD_RIGHT) 
        {
            TRACELOG(LOG_INFO,"RIGHT");
        }
        if(new_pad & PAD_UP) 
        {
            TRACELOG(LOG_INFO,"UP");
        }
        if(new_pad & PAD_START)
        {
            TRACELOG(LOG_INFO,"START");
        }
        if(new_pad & PAD_R3)
        {
            TRACELOG(LOG_INFO,"R3");
        }
        if(new_pad & PAD_L3)
        {
            TRACELOG(LOG_INFO,"L3");
        }
        if(new_pad & PAD_SELECT)
        {
            TRACELOG(LOG_INFO,"SELECT");
        }
        if(new_pad & PAD_SQUARE)
        {
            TRACELOG(LOG_INFO,"SQUARE");
        }
        if(new_pad & PAD_CROSS)
        {
            padEnterPressMode(port, slot);
            TRACELOG(LOG_INFO,"CROSS - Enter press mode");
        }
        if(new_pad & PAD_CIRCLE)
        {
            padExitPressMode(port, slot);
            TRACELOG(LOG_INFO,"CIRCLE - Exit press mode");
        }
        if(new_pad & PAD_TRIANGLE)
        {
            // Check for the reason below..
            TRACELOG(LOG_INFO,"TRIANGLE (press mode disabled, see code)");
        }
        if(new_pad & PAD_R1)
        {
            actAlign[0] = 1; // Start small engine
            padSetActDirect(port, slot, actAlign);
            TRACELOG(LOG_INFO,"R1 - Start little engine");
        }
        if(new_pad & PAD_L1)
        {
            actAlign[0] = 0; // Stop engine 0
            padSetActDirect(port, slot, actAlign);
            TRACELOG(LOG_INFO,"L1 - Stop little engine");
        }
        if(new_pad & PAD_R2)
        {
            TRACELOG(LOG_INFO,"R2");
        }
        if(new_pad & PAD_L2)
        {
            TRACELOG(LOG_INFO,"L2");
        }

        // Test the press mode
        if(buttons.triangle_p)
        {
            TRACELOG(LOG_INFO,"TRIANGLE %d", buttons.triangle_p);
        }
        // Start little engine if we move right analogue stick right
        if(buttons.rjoy_h > 0xf0)
        {
            // Stupid check to see if engine is already running,
            // just to prevent overloading the IOP with requests
            if (actAlign[0] == 0)
            {
                actAlign[0] = 1;
                padSetActDirect(port, slot, actAlign);
            }
        }
    }*/

    // Reset touch positions
    // TODO: It resets on target platform the mouse position and not filled again until a move-event,
    // so, if mouse is not moved it returns a (0, 0) position... this behaviour should be reviewed!
    //for (int i = 0; i < MAX_TOUCH_POINTS; i++) CORE.Input.Touch.position[i] = (Vector2){ 0, 0 };

    // Register previous keys states
    // NOTE: Android supports up to 260 keys
    //for (int i = 0; i < 260; i++)
    //{
    //    CORE.Input.Keyboard.previousKeyState[i] = CORE.Input.Keyboard.currentKeyState[i];
    //    CORE.Input.Keyboard.keyRepeatInFrame[i] = 0;
    //}

    // TODO: Poll input events for current plaform
}
    

void CustomLog(int msgType, const char *text, va_list args)
{
    char buffer[1024] = { 0 };

   
    vsnprintf(buffer,1024, text, args);
    buffer[1024-1] = 0;
    
    
    switch (msgType)
    {
        case LOG_TRACE: printf("[EE][TRACE]: %s\n",buffer); break;
        case LOG_DEBUG: printf("[EE][DEBUG]: %s\n",buffer); break;
        case LOG_INFO: printf("[EE][INFO]: %s\n",buffer); break;
        case LOG_WARNING: printf("[EE][WARNING]: %s\n",buffer); break;
        case LOG_ERROR: printf("[EE][ERROR]: %s\n",buffer); break;
        case LOG_FATAL: printf("[EE][FATAL]: %s\n",buffer); break;
        default: break;
    }
}

static void initGsMemoryForRaylib(bool pal)
{
    // frame and depth buffer
    pgl_slot_handle_t frame_slot_0, frame_slot_1, depth_slot;
    if(pal)
    {
        frame_slot_0 = pglAddGsMemSlot(0, 80, GS::kPsm32);
        frame_slot_1 = pglAddGsMemSlot(80, 80, GS::kPsm32);
        depth_slot   = pglAddGsMemSlot(160, 80, GS::kPsmz24);

    }
    else
    {
        frame_slot_0 = pglAddGsMemSlot(0, 70, GS::kPsm32);
        frame_slot_1 = pglAddGsMemSlot(70, 70, GS::kPsm32);
        depth_slot   = pglAddGsMemSlot(140, 70, GS::kPsmz24);
    }
    
    // lock these slots so that they aren't allocated by the memory manager
    pglLockGsMemSlot(frame_slot_0);
    pglLockGsMemSlot(frame_slot_1);
    pglLockGsMemSlot(depth_slot);

    // create gs memory area objects to use for frame and depth buffers
    pgl_area_handle_t frame_area_0, frame_area_1, depth_area;
    if(pal)
    {
        frame_area_0 = pglCreateGsMemArea(640, 256, GS::kPsm24);
        frame_area_1 = pglCreateGsMemArea(640, 256, GS::kPsm24);
        depth_area   = pglCreateGsMemArea(640, 256, GS::kPsmz24);

    }
    else
    {
        frame_area_0 = pglCreateGsMemArea(640, 224, GS::kPsm24);
        frame_area_1 = pglCreateGsMemArea(640, 224, GS::kPsm24);
        depth_area   = pglCreateGsMemArea(640, 224, GS::kPsmz24);
    }
    
    // bind the areas to the slots we created above
    pglBindGsMemAreaToSlot(frame_area_0, frame_slot_0);
    pglBindGsMemAreaToSlot(frame_area_1, frame_slot_1);
    pglBindGsMemAreaToSlot(depth_area, depth_slot);

    // draw to the new areas...
    pglSetDrawBuffers(PGL_INTERLACED, frame_area_0, frame_area_1, depth_area);
    // ...and display from them
    pglSetDisplayBuffers(PGL_INTERLACED, frame_area_0, frame_area_1);

    // 32 bit

    // a slot for fonts (probably)
    if(0)
    {
    pglAddGsMemSlot(210, 2, GS::kPsm8);

    // 64x32
    pglAddGsMemSlot(212, 1, GS::kPsm32);
    pglAddGsMemSlot(213, 1, GS::kPsm32);
    // 64x64
    pglAddGsMemSlot(214, 2, GS::kPsm32);
    pglAddGsMemSlot(216, 2, GS::kPsm32);
    pglAddGsMemSlot(218, 2, GS::kPsm32);
    pglAddGsMemSlot(220, 2, GS::kPsm32);
    // 128x128
    pglAddGsMemSlot(222, 8, GS::kPsm32);
    pglAddGsMemSlot(230, 8, GS::kPsm32);
    // 256x256
    pglAddGsMemSlot(238, 32, GS::kPsm32);
    pglAddGsMemSlot(270, 32, GS::kPsm32);
    // 512x256
    pglAddGsMemSlot(302, 64, GS::kPsm32);
    pglAddGsMemSlot(366, 64, GS::kPsm32);
    }
    //testing different values TODO
    if(pal)
    {
        pglAddGsMemSlot(240, 2, GS::kPsm8);
    
        // 64x32
        pglAddGsMemSlot(242, 1, GS::kPsm32);
        pglAddGsMemSlot(243, 1, GS::kPsm32);
        pglAddGsMemSlot(244, 1, GS::kPsm32);
        pglAddGsMemSlot(245, 1, GS::kPsm32);
        pglAddGsMemSlot(246, 1, GS::kPsm32);
        pglAddGsMemSlot(247, 1, GS::kPsm32);
        pglAddGsMemSlot(248, 1, GS::kPsm32);
        pglAddGsMemSlot(249, 1, GS::kPsm32);
    
        // 64x64
        pglAddGsMemSlot(250, 2, GS::kPsm32);
        pglAddGsMemSlot(252, 2, GS::kPsm32);
        pglAddGsMemSlot(254, 2, GS::kPsm32);
        pglAddGsMemSlot(256, 2, GS::kPsm32);
        pglAddGsMemSlot(258, 2, GS::kPsm32);
        pglAddGsMemSlot(260, 2, GS::kPsm32);
        pglAddGsMemSlot(262, 2, GS::kPsm32);
        pglAddGsMemSlot(264, 2, GS::kPsm32);
        // 128x128
        pglAddGsMemSlot(266, 8, GS::kPsm32);
        pglAddGsMemSlot(274, 8, GS::kPsm32);
        pglAddGsMemSlot(282, 8, GS::kPsm32);
        pglAddGsMemSlot(290, 8, GS::kPsm32);
        pglAddGsMemSlot(298, 8, GS::kPsm32);
        pglAddGsMemSlot(306, 8, GS::kPsm32);
    
        // 256x256
        pglAddGsMemSlot(314, 32, GS::kPsm32);
        pglAddGsMemSlot(346, 32, GS::kPsm32);
        // 512x256
        pglAddGsMemSlot(378, 64, GS::kPsm32);
        pglAddGsMemSlot(442, 64, GS::kPsm32);
    }
    else
    {
        pglAddGsMemSlot(210, 2, GS::kPsm8);

        // 64x32
        pglAddGsMemSlot(212, 1, GS::kPsm32);
        pglAddGsMemSlot(213, 1, GS::kPsm32);
        pglAddGsMemSlot(214, 1, GS::kPsm32);
        pglAddGsMemSlot(215, 1, GS::kPsm32);
        pglAddGsMemSlot(216, 1, GS::kPsm32);
        pglAddGsMemSlot(217, 1, GS::kPsm32);
        pglAddGsMemSlot(218, 1, GS::kPsm32);
        pglAddGsMemSlot(219, 1, GS::kPsm32);

        // 64x64
        pglAddGsMemSlot(220, 2, GS::kPsm32);
        pglAddGsMemSlot(222, 2, GS::kPsm32);
        pglAddGsMemSlot(224, 2, GS::kPsm32);
        pglAddGsMemSlot(226, 2, GS::kPsm32);
        pglAddGsMemSlot(228, 2, GS::kPsm32);
        pglAddGsMemSlot(230, 2, GS::kPsm32);
        pglAddGsMemSlot(232, 2, GS::kPsm32);
        pglAddGsMemSlot(234, 2, GS::kPsm32);
        // 128x128
        pglAddGsMemSlot(236, 8, GS::kPsm32);
        pglAddGsMemSlot(244, 8, GS::kPsm32);
        pglAddGsMemSlot(252, 8, GS::kPsm32);
        pglAddGsMemSlot(260, 8, GS::kPsm32);
        pglAddGsMemSlot(268, 8, GS::kPsm32);
        pglAddGsMemSlot(276, 8, GS::kPsm32);

        // 256x256
        pglAddGsMemSlot(284, 32, GS::kPsm32);
        pglAddGsMemSlot(316, 32, GS::kPsm32);
        // 512x256
        pglAddGsMemSlot(348, 64, GS::kPsm32);
        pglAddGsMemSlot(412, 64, GS::kPsm32);

    }
    //pglPrintGsMemAllocation();
}
//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------

int waitPadReady(int port, int slot)
{
    int state;
    int lastState;
    char stateString[16];

    state = padGetState(port, slot);
    lastState = -1;
    while((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
        if (state != lastState) {
            padStateInt2String(state, stateString);
            TRACELOG(LOG_INFO,"Please wait, pad(%d,%d) is in state %s",
                       port, slot, stateString);
        }
        lastState = state;
        state=padGetState(port, slot);
    }
    // Were the pad ever 'out of sync'?
    if (lastState != -1) {
        TRACELOG(LOG_INFO,"Pad OK!");
    }
    return 0;
}

int initializePad(int port, int slot)
{

    int ret;
    int modes;
    int i;

    waitPadReady(port, slot);

    // How many different modes can this device operate in?
    // i.e. get # entrys in the modetable
    modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
    TRACELOG(LOG_INFO,"The device has %d modes", modes);

    
    TRACELOG(LOG_INFO,"It is currently using mode %d",padInfoMode(port, slot, PAD_MODECURID, 0));

    // If modes == 0, this is not a Dual shock controller
    // (it has no actuator engines)
    if (modes == 0) {
        TRACELOG(LOG_INFO,"This is a digital controller?");
        return 1;
    }

    // Verify that the controller has a DUAL SHOCK mode
    i = 0;
    do {
        if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
            break;
        i++;
    } while (i < modes);
    if (i >= modes) {
        TRACELOG(LOG_INFO,"This is no Dual Shock controller");
        return 1;
    }

    // If ExId != 0x0 => This controller has actuator engines
    // This check should always pass if the Dual Shock test above passed
    ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
    if (ret == 0) {
        TRACELOG(LOG_INFO,"This is no Dual Shock controller??");
        return 1;
    }

    TRACELOG(LOG_INFO,"Enabling dual shock functions\n");

    // When using MMODE_LOCK, user cant change mode with Select button
    padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

    waitPadReady(port, slot);
    TRACELOG(LOG_INFO,"infoPressMode: %d", padInfoPressMode(port, slot));

    waitPadReady(port, slot);
    TRACELOG(LOG_INFO,"enterPressMode: %d", padEnterPressMode(port, slot));

    waitPadReady(port, slot);
    actuators = padInfoAct(port, slot, -1, 0);
    TRACELOG(LOG_INFO,"# of actuators: %d",actuators);

    if (actuators != 0) 
    {
        actAlign[0] = 0;   // Enable small engine
        actAlign[1] = 1;   // Enable big engine
        actAlign[2] = 0xff;
        actAlign[3] = 0xff;
        actAlign[4] = 0xff;
        actAlign[5] = 0xff;

        waitPadReady(port, slot);
        TRACELOG(LOG_INFO,"padSetActAlign: %d",padSetActAlign(port, slot, actAlign));
    }
    else 
    {
        TRACELOG(LOG_INFO,"Did not find any actuators.");
    }

    waitPadReady(port, slot);

    return 1;
}
// Initialize platform: graphics, inputs and more
int InitPlatform(void)
{
    SetTraceLogCallback(CustomLog);

    SifInitRpc(0);
    int ret=SifLoadModule("rom0:SIO2MAN", 0, 0);

    if (ret < 0) {
        TRACELOG(LOG_ERROR,"sifLoadModule sio2man pad failed: %d", ret);
        return -1;
    }


    ret = SifLoadModule("rom0:PADMAN", 0, NULL);
    if (ret < 0) {
        TRACELOG(LOG_ERROR,"sifLoadModule pad failed: %d", ret);
        return -1;
    }

    padInit(0);

    port = 0; // 0 -> Connector 1, 1 -> Connector 2
    slot = 0; // Always zero if not using multitap

    if((ret = padPortOpen(port, slot, padBuf)) == 0)
    {
        TRACELOG(LOG_ERROR,"padOpenPort failed: %d", ret);
        return -1;
    }
    
    if(!initializePad(port, slot)) 
    {
        TRACELOG(LOG_ERROR,"pad initalization failed!");
        return -1;
    }
    
    TRACELOG(LOG_INFO, "Initializing raylib %s", RAYLIB_VERSION);
    TRACELOG(LOG_INFO, "Platform backend: PLAYSTATION2");
    TRACELOG(LOG_INFO, "PLATFORM: PlayStation 2 init");

    if (!pglHasLibraryBeenInitted()) {
        // reset the machine
        //      sceDevVif0Reset();
        //      sceDevVu0Reset();
        //      sceDmaReset(1);
        //      sceGsResetPath();

        // Reset the GIF. OSDSYS leaves PATH3 busy, that ends up having
        // our PATH1/2 transfers ignored by the GIF.
        *GIF::Registers::ctrl = 1;

        //      sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
        //SetGsCrt(1 /* interlaced */, 2 /* ntsc */, 1 /* frame */);
        SetGsCrt(1 /* interlaced */,2 /* ntsc */, 1 /* frame */);


        TRACELOG(LOG_INFO,"ps2gl library has not been initialized by the user; using default values.");
        //int immBufferVertexSize = 64 * 1024;
        int immBufferVertexSize = 128 * 1024;

        pglInit(immBufferVertexSize, 1000);
    }

    // does gs memory need to be initialized?

    if (!pglHasGsMemBeenInitted()) {
        TRACELOG(LOG_INFO,"GS memory has not been allocated by the user; using default values.");
        //pal true ntsc false
        initGsMemoryForRaylib(false);
    }


   


    CORE.Window.fullscreen = true;
    CORE.Window.flags |= FLAG_FULLSCREEN_MODE;

    CORE.Window.display.width = 640;//CORE.Window.screen.width;            // User desired width
    CORE.Window.display.height = 448;//CORE.Window.screen.height;          // User desired height
    CORE.Window.screen.width=640;
    CORE.Window.screen.height=448;
    CORE.Window.render.width = CORE.Window.screen.width;
    CORE.Window.render.height = CORE.Window.screen.height;
    CORE.Window.eventWaiting = false;
    CORE.Window.screenScale = MatrixIdentity();     // No draw scaling required by default
    CORE.Window.currentFbo.width = CORE.Window.screen.width;
    CORE.Window.currentFbo.height = CORE.Window.screen.height;
    CORE.Input.Mouse.currentPosition.x = (float)CORE.Window.screen.width/2.0f;
    CORE.Input.Mouse.currentPosition.y = (float)CORE.Window.screen.height/2.0f;
    CORE.Input.Mouse.scale = (Vector2){ 1.0f, 1.0f };


    

    // At this point we need to manage render size vs screen size
    // NOTE: This function use and modify global module variables:
    //  -> CORE.Window.screen.width/CORE.Window.screen.height
    //  -> CORE.Window.render.width/CORE.Window.render.height
    //  -> CORE.Window.screenScale
    SetupFramebuffer(CORE.Window.display.width, CORE.Window.display.height);

    //ANativeWindow_setBuffersGeometry(platform.app->window, CORE.Window.render.width, CORE.Window.render.height, displayFormat);
    //ANativeWindow_setBuffersGeometry(platform.app->window, 0, 0, displayFormat);       // Force use of native display size

    
    {
        CORE.Window.render.width = CORE.Window.screen.width;
        CORE.Window.render.height = CORE.Window.screen.height;
        CORE.Window.currentFbo.width = CORE.Window.render.width;
        CORE.Window.currentFbo.height = CORE.Window.render.height;

        TRACELOG(LOG_INFO, "PLATFORM: Device initialized successfully");
        TRACELOG(LOG_INFO, "    > Display size: %i x %i", CORE.Window.display.width, CORE.Window.display.height);
        TRACELOG(LOG_INFO, "    > Screen size:  %i x %i", CORE.Window.screen.width, CORE.Window.screen.height);
        TRACELOG(LOG_INFO, "    > Render size:  %i x %i", CORE.Window.render.width, CORE.Window.render.height);
        TRACELOG(LOG_INFO, "    > Viewport offsets: %i, %i", CORE.Window.renderOffset.x, CORE.Window.renderOffset.y);
    }

    // Load OpenGL extensions
    // NOTE: GL procedures address loader is required to load extensions
    //rlLoadExtensions(eglGetProcAddress);
    //const char *gl_exts = (char *) glGetString(GL_EXTENSIONS);
    //TRACELOG(LOG_INFO,"PLATFORM: GL_VENDOR:   \"%s\"", glGetString(GL_VENDOR));
    //TRACELOG(LOG_INFO,"PLATFORM: GL_VERSION:  \"%s\"", glGetString(GL_VERSION));
    //TRACELOG(LOG_INFO,"PLATFORM: GL_RENDERER: \"%s\"", glGetString(GL_RENDERER));
    //TRACELOG(LOG_INFO,"PLATFORM: SL_VERSION:  \"%s\"", glGetString(GL_SHADING_LANGUAGE_VERSION));
    CORE.Window.ready = true;

    // Initialize hi-res timer
    InitTimer();

    // Initialize base path for storage
    CORE.Storage.basePath = GetWorkingDirectory();
    TRACELOG(LOG_INFO, "PLATFORM: Initialized");

    return 0;
}

// Close platform
void ClosePlatform(void)
{
    // TODO: De-initialize graphics, inputs and more
}

// EOF
