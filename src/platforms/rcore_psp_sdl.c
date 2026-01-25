#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <pspkernel.h>
#include <pspdebug.h>

typedef struct {
  SDL_Window *window;
  SDL_GLContext glContext;
  SDL_GameController *gamepad[MAX_GAMEPADS];
} PlatformData;

static PlatformData platform = { 0 };

#define SCANCODE_MAPPED_NUM 232

static const KeyboardKey mapScancodeToKey[SCANCODE_MAPPED_NUM] = {
  KEY_NULL, 0, 0, 0,
  KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
  KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
  KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE, KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE, KEY_ZERO,
  KEY_ENTER, KEY_ESCAPE, KEY_BACKSPACE, KEY_TAB, KEY_SPACE,
  KEY_MINUS, KEY_EQUAL, KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET, KEY_BACKSLASH, 0,
  KEY_SEMICOLON, KEY_APOSTROPHE, KEY_GRAVE, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_CAPS_LOCK,
  KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
  KEY_PRINT_SCREEN, KEY_SCROLL_LOCK, KEY_PAUSE, KEY_INSERT, KEY_HOME, KEY_PAGE_UP,
  KEY_DELETE, KEY_END, KEY_PAGE_DOWN, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP,
  KEY_NUM_LOCK, KEY_KP_DIVIDE, KEY_KP_MULTIPLY, KEY_KP_SUBTRACT, KEY_KP_ADD, KEY_KP_ENTER,
  KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4, KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9, KEY_KP_0, KEY_KP_DECIMAL
};

static KeyboardKey ConvertScancodeToKey(SDL_Scancode scancode)
{
  if ((int)scancode >= 0 && (int)scancode < SCANCODE_MAPPED_NUM)
    return mapScancodeToKey[scancode];

  return KEY_NULL;
}

bool WindowShouldClose(void)
{
  if (CORE.Window.ready) return CORE.Window.shouldClose;
  else return true;
}

void ToggleFullscreen(void)
{
  TRACELOG(LOG_WARNING, "ToggleFullscreen() not supported on PSP");
}

void SetWindowTitle(const char *title)
{
  CORE.Window.title = title;
  if (platform.window) SDL_SetWindowTitle(platform.window, title);
}

void *GetWindowHandle(void)
{
  return platform.window;
}

void SetClipboardText(const char *text)
{
  TRACELOG(LOG_WARNING, "Clipboard not supported on PSP");
}

const char *GetClipboardText(void)
{
  return "";
}

double GetTime(void)
{
  return (double)SDL_GetTicks() / 1000.0;
}

static KeyboardKey ConvertJoystickButtonToKey(int button)
{
  switch (button)
  {
    case 0: return KEY_SPACE;
    case 1: return KEY_ESCAPE;
    case 2: return KEY_LEFT_SHIFT;
    case 3: return KEY_ENTER;
    case 4: return KEY_L;
    case 5: return KEY_R;
    case 6: return KEY_DOWN;
    case 7: return KEY_LEFT;
    case 8: return KEY_UP;
    case 9: return KEY_RIGHT;
    case 10: return KEY_TAB;
    case 11: return KEY_ESCAPE;
    default: return KEY_NULL;
  }
}

void PollInputEvents(void)
{
  CORE.Input.Keyboard.keyPressedQueueCount = 0;
  CORE.Input.Keyboard.charPressedQueueCount = 0;

  for (int i = 0; i < MAX_KEYBOARD_KEYS; i++)
  {
    CORE.Input.Keyboard.previousKeyState[i] = CORE.Input.Keyboard.currentKeyState[i];
    CORE.Input.Keyboard.keyRepeatInFrame[i] = 0;
  }

  SDL_Event event;
  while (SDL_PollEvent(&event))
  {    
    switch (event.type)
    {
      case SDL_QUIT:
        CORE.Window.shouldClose = true;
        break;
        
      case SDL_KEYDOWN:
      {
        KeyboardKey key = ConvertScancodeToKey(event.key.keysym.scancode);
        if (key != KEY_NULL)
        {
          CORE.Input.Keyboard.currentKeyState[key] = 1;
          if (!event.key.repeat &&
            CORE.Input.Keyboard.keyPressedQueueCount < MAX_KEY_PRESSED_QUEUE)
          {
            CORE.Input.Keyboard.keyPressedQueue[
              CORE.Input.Keyboard.keyPressedQueueCount++] = key;
          }
          if (event.key.repeat)
            CORE.Input.Keyboard.keyRepeatInFrame[key] = 1;
        }
      } break;
      
      case SDL_KEYUP:
      {
        KeyboardKey key = ConvertScancodeToKey(event.key.keysym.scancode);
        if (key != KEY_NULL) CORE.Input.Keyboard.currentKeyState[key] = 0;
      } break;
      
      case SDL_JOYBUTTONDOWN:
      {
        KeyboardKey key = ConvertJoystickButtonToKey(event.jbutton.button);
        if (key != KEY_NULL)
        {
          CORE.Input.Keyboard.currentKeyState[key] = 1;
          if (CORE.Input.Keyboard.keyPressedQueueCount < MAX_KEY_PRESSED_QUEUE)
          {
            CORE.Input.Keyboard.keyPressedQueue[
              CORE.Input.Keyboard.keyPressedQueueCount++] = key;
          }
        }
      } break;
      
      case SDL_JOYBUTTONUP:
      {
        KeyboardKey key = ConvertJoystickButtonToKey(event.jbutton.button);
        if (key != KEY_NULL) CORE.Input.Keyboard.currentKeyState[key] = 0;
      } break;
      
      case SDL_JOYAXISMOTION:
      {
        // todo
        // TraceLog(LOG_INFO, "Joystick axis: %d value: %d", event.jaxis.axis, event.jaxis.value);
      } break;
      
      default: break;
    }
  }
}

int InitPlatform(void)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
  {
    TRACELOG(LOG_FATAL, "SDL: Initialization failed");
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  platform.window = SDL_CreateWindow(
    CORE.Window.title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    CORE.Window.screen.width,
    CORE.Window.screen.height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );

  platform.glContext = SDL_GL_CreateContext(platform.window);

  if (!platform.window || !platform.glContext)
  {
    TRACELOG(LOG_FATAL, "PSP: Failed to create window or GL context");
    return -1;
  }


  int numJoysticks = SDL_NumJoysticks();
  TRACELOG(LOG_INFO, "Number of joysticks detected: %d", numJoysticks);

  if (numJoysticks > 0)
  {
    SDL_Joystick *joystick = SDL_JoystickOpen(0);
    if (joystick)
    {
      TRACELOG(LOG_INFO, "Joystick 0 opened: %s", SDL_JoystickName(joystick));
      TRACELOG(LOG_INFO, "Joystick buttons: %d", SDL_JoystickNumButtons(joystick));
      TRACELOG(LOG_INFO, "Joystick axes: %d", SDL_JoystickNumAxes(joystick));
    }
    else
    {
      TRACELOG(LOG_WARNING, "Failed to open joystick 0");
    }
  }

  CORE.Window.ready = true;

  CORE.Window.display.width = 480;
  CORE.Window.display.height = 272;
  CORE.Window.render = CORE.Window.screen;
  CORE.Window.currentFbo = CORE.Window.render;

  rlLoadExtensions(SDL_GL_GetProcAddress);

  CORE.Time.previous = GetTime();

  TRACELOG(LOG_INFO, "PLATFORM: PSP (SDL2) initialized");

  return 0;
}

void ClosePlatform(void)
{
  SDL_GL_DeleteContext(platform.glContext);
  SDL_DestroyWindow(platform.window);
  SDL_Quit();
}

void SwapScreenBuffer(void)
{
  SDL_GL_SwapWindow(platform.window);
}

Vector2 GetWindowScaleDPI(void)
{
  return (Vector2){ 1.0f, 1.0f };
}


void ShowCursor(void)
{
  CORE.Input.Mouse.cursorHidden = false;
}

void HideCursor(void)
{
  CORE.Input.Mouse.cursorHidden = true;
}

void EnableCursor(void)
{
  CORE.Input.Mouse.cursorHidden = false;
}

void DisableCursor(void)
{
  CORE.Input.Mouse.cursorHidden = true;
}

void SetMousePosition(int x, int y)
{
  CORE.Input.Mouse.currentPosition = (Vector2){ (float)x, (float)y };
  CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
}

void SetMouseCursor(int cursor)
{
  // Not supported
}

int SetGamepadMappings(const char *mappings)
{
  TRACELOG(LOG_WARNING, "SetGamepadMappings() not supported on PSP");
  return 0;
}


int GetMonitorCount(void) { return 1; }
int GetCurrentMonitor(void) { return 0; }

int GetMonitorWidth(int monitor)
{
  return CORE.Window.display.width;
}

int GetMonitorHeight(int monitor)
{
  return CORE.Window.display.height;
}

int GetMonitorRefreshRate(int monitor)
{
  return 60;
}

const char *GetMonitorName(int monitor)
{
  return "PSP Screen";
}

Vector2 GetMonitorPosition(int monitor)
{
  return (Vector2){ 0, 0 };
}

int GetMonitorPhysicalWidth(int monitor) { return 0; }
int GetMonitorPhysicalHeight(int monitor) { return 0; }

void ToggleBorderlessWindowed(void) {}
void MaximizeWindow(void) {}
void MinimizeWindow(void) {}
void RestoreWindow(void) {}
void SetWindowState(unsigned int flags) {}
void ClearWindowState(unsigned int flags) {}
void SetWindowIcon(Image image) {}
void SetWindowIcons(Image *images, int count) {}
void SetWindowPosition(int x, int y) {}
void SetWindowMonitor(int monitor) {}
void SetWindowMinSize(int width, int height) {}
void SetWindowMaxSize(int width, int height) {}
void SetWindowSize(int width, int height) {}
void SetWindowOpacity(float opacity) {}
void SetWindowFocused(void) {}
Vector2 GetWindowPosition(void) { return (Vector2){ 0, 0 }; }


void glHint(GLenum target, GLenum mode)
{
  (void)target;
  (void)mode;
}

void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
{
  (void)target;
  (void)level;
  (void)format;
  (void)type;
  (void)pixels;
}
