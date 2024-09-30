/**********************************************************************************************
*
*   rcore_vita - Functions to manage window, graphics device and inputs
*
*   PLATFORM: VITA
*       - PlayStation Vita
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
*       - Based in original port by Antonio Jose Ramos Marquez aka bigboss @psxdev	
*       - Log callback using network udp logs you must initialize debugNetInit included before call raylib InitWindow
*
*   CONFIGURATION:
*       - make PLATFORM=PLATFORM_VITA 
*
*   DEPENDENCIES:
*       VitaSDK - Provides C API to access Vita homebrew functionality
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
#include <vitasdk.h>
#include <psp2/types.h>
#include <pib.h>
#include <psp2/gxm.h>
#include <psp2/display.h>
#include <psp2/kernel/processmgr.h>
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct {
    // TODO: Define the platform specific variables required

    // Display data
    EGLDisplay device;                  // Native display device (physical screen connection)
    EGLSurface surface;                 // Surface to draw on, framebuffers (connected to context)
    EGLContext context;                 // Graphic context, mode in which drawing can be done
    EGLConfig config;                   // Graphic config
} PlatformData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern CoreData CORE;                   // Global CORE state context

static PlatformData platform = { 0 };   // Platform specific data

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
    eglSwapBuffers(platform.device, platform.surface);
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Misc
//----------------------------------------------------------------------------------

// Get elapsed time measure in seconds since InitTimer()
double GetTime(void)
{
    return (double)(sceKernelGetProcessTimeWide() - CORE.Time.base)*1e-6;
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

//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------
static void *net_memory = NULL;
static SceNetInAddr vita_addr;
struct SceNetSockaddrIn stSockAddr;
int debugnet_initialized=0;
int SocketFD;
#define NET_INIT_SIZE 1*1024*1024

/**
 * UDP printf for debugnet library,
 * use debugNetPrintf() instead unless necessary
 *
 * @par Example:
 * @code
 * debugNetUDPPrintf("This is a %s test\n", "real");
 * @endcode
 */
void debugNetUDPPrintf(const char* fmt, ...)
{
  char buffer[1024];
  va_list arg;
  va_start(arg, fmt);
  sceClibVsnprintf(buffer, sizeof(buffer), fmt, arg);
  va_end(arg);
  
  debugNetUDPSend(buffer);
}
/**
 * UDP Raw text send for debugnet library,
 * use debugNetPrintf() instead unless necessary
 *
 * @par Example:
 * @code
 * debugNetUDPSend("This is a test\n");
 * @endcode
 *
 * @param text - NULL-terminated buffer containing the raw text to send
 */
void debugNetUDPSend(const char *text)
{
    sceNetSend(SocketFD, text, strlen(text), 0);
}
/**
 * Init debugnet library 
 *
 * @par Example:
 * @code
 * #define LOGLEVEL 3  
 * int ret;
 * ret = debugNetInit("172.26.0.2", 18194, DEBUG);
 * @endcode
 *
 * @param serverIP - your pc/mac server ip
 * @param port - udp port server
 * @param level - DEBUG,ERROR,INFO or NONE 
 */
int debugNetInit(const char *serverIp, int port, int level)
{
    int ret=0;
    SceNetInitParam initparam;
    SceNetCtlInfo info;
    
    if(debugnet_initialized)
    {
        return debugnet_initialized;
    }
 
    
    //debugNetSetLogLevel(level);
    
    
    if (sceSysmoduleIsLoaded(SCE_SYSMODULE_NET) != SCE_SYSMODULE_LOADED)
    ret=sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    
    if (ret >=0) 
    {
        
    
    /*net initialazation code from xerpi at https://github.com/xerpi/FTPVita/blob/master/ftp.c*/
    /* Init Net */
    if (sceNetShowNetstat() == SCE_NET_ERROR_ENOTINIT) {
        net_memory = malloc(NET_INIT_SIZE);

        initparam.memory = net_memory;
        initparam.size = NET_INIT_SIZE;
        initparam.flags = 0;

        ret = sceNetInit(&initparam);
        //printf("sceNetInit(): 0x%08X\n", ret);
    } else {
        //printf("Net is already initialized.\n");
    }

    /* Init NetCtl */
    ret = sceNetCtlInit();
    //printf("sceNetCtlInit(): 0x%08X\n", ret);
   

    /* Get IP address */
    ret = sceNetCtlInetGetInfo(SCE_NETCTL_INFO_GET_IP_CONFIG, &info);
    //printf("sceNetCtlInetGetInfo(): 0x%08X\n", ret);


    /* Save the IP of PSVita to a global variable */
    sceNetInetPton(SCE_NET_AF_INET, info.ip_address, &vita_addr);
    
    /* Create datagram udp socket*/
    SocketFD = sceNetSocket("debugnet_socket",
        SCE_NET_AF_INET , SCE_NET_SOCK_DGRAM, SCE_NET_IPPROTO_UDP);
   
    memset(&stSockAddr, 0, sizeof stSockAddr);
    
    
    /*Populate SceNetSockaddrIn structure values*/
    stSockAddr.sin_family = SCE_NET_AF_INET;
    stSockAddr.sin_port = sceNetHtons(port);
    sceNetInetPton(SCE_NET_AF_INET, serverIp, &stSockAddr.sin_addr);

    /*Connect socket to server*/
    sceNetConnect(SocketFD, (struct SceNetSockaddr *)&stSockAddr, sizeof stSockAddr);

    /*Show log on pc/mac side*/
    debugNetUDPPrintf("[VITA][INFO]:debugnet initialized\n");
    debugNetUDPPrintf("[VITA][INFO]:Copyright (C) 2010,2020 Antonio Jose Ramos Marquez aka bigboss @psxdev\n");
    debugNetUDPPrintf("[VITA][INFO]:ready to have a lot of fun...\n");

    /*library debugnet initialized*/
    debugnet_initialized = 1;
    }

    return debugnet_initialized;
}
void CustomLog(int msgType, const char *text, va_list args)
{
    char buffer[2048] = { 0 };


    vsnprintf(buffer,2048, text, args);
    buffer[2048-1] = 0;


    switch (msgType)
    {
        case LOG_TRACE: debugNetUDPPrintf("[VITA][TRACE]%s\n",buffer); break;
        case LOG_DEBUG: debugNetUDPPrintf("[VITA][DEBUG]%s\n",buffer); break;
        case LOG_INFO: debugNetUDPPrintf("[VITA][INFO]%s\n",buffer); break;
        case LOG_WARNING: debugNetUDPPrintf("[VITA][WARNING]%s\n",buffer); break;
        case LOG_ERROR: debugNetUDPPrintf("[VITA][ERROR]%s\n",buffer); break;
        case LOG_FATAL: debugNetUDPPrintf("[VITA][FATAL]%s\n",buffer); break;
        default: break;
    }
}
// Initialize platform: graphics, inputs and more
int InitPlatform(void)
{

    TRACELOG(LOG_INFO, "PLATFORM: calling pibInit with PIB_SHACCCG|PIB_GET_PROC_ADDR_CORE");
    int pib_ret=pibInit(PIB_SHACCCG|PIB_GET_PROC_ADDR_CORE );
    if(pib_ret!=0)
    {
        TRACELOG(LOG_FATAL, "PLATFORM: calling pibInit return %d",pib_ret);
        CORE.Window.ready = false;
        return -1;
    }


    CORE.Window.fullscreen = true;
    CORE.Window.flags |= FLAG_FULLSCREEN_MODE;

    CORE.Window.display.width = CORE.Window.screen.width;            // User desired width
    CORE.Window.display.height = CORE.Window.screen.height;          // User desired height
    CORE.Window.render.width = CORE.Window.screen.width;
    CORE.Window.render.height = CORE.Window.screen.height;
    CORE.Window.eventWaiting = false;
    CORE.Window.screenScale = MatrixIdentity();     // No draw scaling required by default
    CORE.Window.currentFbo.width = CORE.Window.screen.width;
    CORE.Window.currentFbo.height = CORE.Window.screen.height;
    CORE.Input.Mouse.currentPosition.x = (float)CORE.Window.screen.width/2.0f;
    CORE.Input.Mouse.currentPosition.y = (float)CORE.Window.screen.height/2.0f;
    CORE.Input.Mouse.scale = (Vector2){ 1.0f, 1.0f };


    EGLint samples = 0;
    EGLint sampleBuffer = 0;
    if (CORE.Window.flags & FLAG_MSAA_4X_HINT)
    {
        samples = 4;
        sampleBuffer = 1;
        TRACELOG(LOG_INFO, "PLATFORM: Trying to enable MSAA x4");
    }

 	const EGLint framebufferAttribs[] = {
        //EGL_CONFIG_ID, 2,                         // You can always provide a configuration id. The one displayed here is Configuration 2
        EGL_RED_SIZE, 8,                            // These four are always 8
        EGL_GREEN_SIZE, 8,                          //
        EGL_BLUE_SIZE, 8,                           //
        EGL_ALPHA_SIZE, 8,                          //
        EGL_DEPTH_SIZE, 32,                         // Depth is either 32 or 0
        EGL_STENCIL_SIZE, 8,                        //  Stencil Size is either 8 or 0
        EGL_SURFACE_TYPE, 5,                        // This is ALWAYS 5
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,    // Always EGL_OPENGL_ES2_BIT or 0x4
        EGL_SAMPLE_BUFFERS, sampleBuffer,    // Activate MSAA
        EGL_SAMPLES, samples,       // 4x Antialiasing if activated (Free on MALI GPUs)
        EGL_NONE
    };

    const EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLint numConfigs = 0;

    // Get an EGL device connection
    platform.device = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (platform.device == EGL_NO_DISPLAY)
    {
        TRACELOG(LOG_WARNING, "PLATFORM: Failed to initialize EGL device");
        return -1;
    }

    // Initialize the EGL device connection
    if (eglInitialize(platform.device, NULL, NULL) == EGL_FALSE)
    {
        // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
        TRACELOG(LOG_WARNING, "PLATFORM: Failed to initialize EGL device");
        return -1;
    }

    eglChooseConfig(platform.device, framebufferAttribs, &platform.config, 1, &numConfigs);

    // Set rendering API
    int ret=eglBindAPI(EGL_OPENGL_ES_API);
    if(!ret)
    {
        ret=eglGetError();
        TRACELOG(LOG_ERROR,"PLATFORM:  eglBindAPI failed: 0x%08X",ret);
        return -1;
    }
    TRACELOG(LOG_INFO,"PLATFORM: eglBindAPI success.");

	// Get an appropriate EGL framebuffer configuration
    ret=eglChooseConfig(platform.device, framebufferAttribs, &platform.config, 1, &numConfigs);
    if(!ret)
    {
        ret=eglGetError();
        TRACELOG(LOG_ERROR,"PLATFORM: eglChooseConfig failed: 0x%08X",ret);
        return -1;
    }
    if (numConfigs!=1)
    {
        TRACELOG(LOG_ERROR,"PLATFORM: No available configuration found.");
        return -1;
    }
    TRACELOG(LOG_INFO,"PLATFORM: eglChooseConfig success.");
    // Create an EGL rendering context
    platform.context = eglCreateContext(platform.device, platform.config, EGL_NO_CONTEXT, contextAttribs);
    if (platform.context == EGL_NO_CONTEXT)
    {
        TRACELOG(LOG_WARNING, "PLATFORM: Failed to create EGL context");
        return -1;
    }

    // Create an EGL window surface
    //---------------------------------------------------------------------------------
    EGLint displayFormat = 0;

    // EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is guaranteed to be accepted by ANativeWindow_setBuffersGeometry()
    // As soon as we picked a EGLConfig, we can safely reconfigure the ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID
    eglGetConfigAttrib(platform.device, platform.config, EGL_NATIVE_VISUAL_ID, &displayFormat);

    // At this point we need to manage render size vs screen size
    // NOTE: This function use and modify global module variables:
    //  -> CORE.Window.screen.width/CORE.Window.screen.height
    //  -> CORE.Window.render.width/CORE.Window.render.height
    //  -> CORE.Window.screenScale
    SetupFramebuffer(CORE.Window.display.width, CORE.Window.display.height);

    //ANativeWindow_setBuffersGeometry(platform.app->window, CORE.Window.render.width, CORE.Window.render.height, displayFormat);
    //ANativeWindow_setBuffersGeometry(platform.app->window, 0, 0, displayFormat);       // Force use of native display size

    platform.surface = eglCreateWindowSurface(platform.device, platform.config, VITA_WINDOW_960X544, NULL);
    if(platform.surface==EGL_NO_SURFACE)
    {
        ret=eglGetError();
        TRACELOG(LOG_ERROR,"PLATFORM: eglCreateWindowSurface failed: 0x%08X",ret);
        return -1;
    }
    TRACELOG(LOG_INFO,"PLATFORM: eglCreateWindowSurface success.");

    // There must be at least one frame displayed before the buffers are swapped
    //eglSwapInterval(platform.device, 1);

    if (eglMakeCurrent(platform.device, platform.surface, platform.surface, platform.context) == EGL_FALSE)
    {
        ret=eglGetError();
        TRACELOG(LOG_ERROR,"PLATFORM: Failed to attach EGL rendering context to EGL surface 0x%08X",ret);
        return -1;
    }
    else
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
    rlLoadExtensions(eglGetProcAddress);
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
