#include "Engine.h"
#include <GL/wglew.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include "../Objects/BuiltIn/Tunnel.h"
#include "../Objects/BuiltIn/Ground.h"
#include "../String/Strings.h"
#include "../Math/Matrix4.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "Debugging/DebugDraw.h"

static float rotZ = 0;
Engine* GH_ENGINE = nullptr;
Player* GH_PLAYER = nullptr;
const Input* GH_INPUT = nullptr;
int64_t GH_FRAME = 0;

LRESULT WINAPI StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  Engine* eng = (Engine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
  if (eng) {
    return eng->WindowProc(hWnd, uMsg, wParam, lParam);
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Engine::Engine() : hWnd(NULL), hDC(NULL), hRC(NULL) {
  GH_ENGINE = this;
  GH_INPUT = &input;
  isFullscreen = false;

  SetProcessDPIAware();
  CreateGLWindow();
  InitGLGameObjects();
  SetupInputs();

  player.reset(new Player);
  GH_PLAYER = player.get();

  LoadWorld();

  sky.reset(new Sky);
}

Engine::~Engine() {
  ClipCursor(NULL);
  wglMakeCurrent(NULL, NULL);
  ReleaseDC(hWnd, hDC);
  wglDeleteContext(hRC);
  DestroyWindow(hWnd);
}

int Engine::Run() {
    if (!hWnd || !hDC || !hRC) {
        return 1;
    }

    //Recieve events from this window
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

    //Setup the timer
    const int64_t ticks_per_step = timer.SecondsToTicks(GH_DELTATIME);
    int64_t cur_ticks = timer.GetTicks();
    GH_FRAME = 0;
    //Game loop
    MSG msg;
    GH_HIDE_MOUSE = true;
    lockCameraPos = false;
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            //Handle windows messages
            if (msg.message == WM_QUIT) {
                break;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            //Confine the cursor
            ConfineCursor();

            if (input.key_press['1']) {
                LoadWorld();
            }
            if (input.key_press['E']) {
                lockCameraPos = !lockCameraPos;
            }
            if (input.key_press['F']) {
                rotZ += 0.1f;
            }
            if (input.key_press['C']) {
                rotZ -= 0.1f;
            }

            if (input.key_press[VK_RETURN]) {
                GH_HIDE_MOUSE = !GH_HIDE_MOUSE;
            }

            //Used fixed time steps for updates
            const int64_t new_ticks = timer.GetTicks();
            for (int i = 0; cur_ticks < new_ticks && i < GH_MAX_STEPS; ++i) {
                Update();
                cur_ticks += ticks_per_step;
                GH_FRAME += 1;
                input.EndFrame();
            }
            cur_ticks = (cur_ticks < new_ticks ? new_ticks : cur_ticks);

            //Setup camera for rendering
            if (!lockCameraPos) {
                cam_mat = player->WorldToCam();
            }
            main_cam.worldView = cam_mat;
            main_cam.SetSize(iWidth, iHeight, GH_NEAR_MIN, GH_FAR);
            main_cam.UseViewport();

            //Render scene
            Render(main_cam, 0);
            SwapBuffers(hDC);

            //Sleep(10);
        }
    }
    DestroyGLGameObjects();
    return 0;
}

void Engine::LoadWorld() {
    //Clear out old scene
    vGameObjects.clear();
    player->Reset();
    player->useGravity = false;
    //std::shared_ptr<Tunnel> tunnel1(new Tunnel(Tunnel::NORMAL));
    //tunnel1->SetPosition(Vector3(-2.4f, 0, -1.8f));
    //tunnel1->SetScale(1, 1, 4.8f);
    //vGameObjects.push_back(tunnel1);
    //
    //std::shared_ptr<Tunnel> tunnel2(new Tunnel(Tunnel::NORMAL));
    //tunnel2->SetPosition(Vector3(2.4f, 0, 0));
    //tunnel2->SetScale(1, 1, 0.6f);
    //vGameObjects.push_back(tunnel2);

    std::shared_ptr<Ground> ground(new Ground());
    ground->SetScale(ground->scale * 1.2f);
    ground->doesDebugDraw = false;
    vGameObjects.push_back(ground);

    //std::shared_ptr<Tunnel> cube1(new Tunnel(Tunnel::NORMAL));
    //cube1->SetPosition(0.4f, 1.0f, 0.0f);
    //cube1->scale = Size(1, 1, 1);
    //vGameObjects.push_back(cube1);
    //std::shared_ptr<Tunnel> cube2(new Tunnel(Tunnel::NORMAL));
    //cube2->SetPosition(1.6f, 1.0f, 0.0f);
    //cube2->scale = Size(1, 1, 1);
    //vGameObjects.push_back(cube2);
    std::shared_ptr<Tunnel> cube3(new Tunnel(Tunnel::NORMAL));
    cube3->SetPosition(3.8f, 1.0f, 0.0f);
    cube3->scale = Size(1,1,1);
    vGameObjects.push_back(cube3);

    player->SetPosition(0, GH_PLAYER_HEIGHT, 5);

    vGameObjects.push_back(player);
}
void Engine::Update() {
    //Update
    for (size_t i = 0; i < vGameObjects.size(); ++i) {
        std::shared_ptr<GameObject>& obj1 = vGameObjects[i];

        obj1->Update();

        PhysicalGameObject* physObj1 = obj1->AsPhysicalGameObject();
        if (!physObj1)
            continue;
        if (!physObj1->useCollisions)
            continue;


        // physObj1 isnt null and it uses collision detection
        // Collision checking
        for (size_t a = 0; a < vGameObjects.size(); a++) {
            std::shared_ptr<GameObject>& obj2 = vGameObjects[a];
            if (obj1 == obj2)
                continue;

            PhysicalGameObject* physObj2 = obj2->AsPhysicalGameObject();
            if (!physObj2)
                continue;
            if (!physObj2->useCollisions)
                continue;

            // obj2 is another object, physobj2 isn't null and uses collision detection.
            // calculate collisions

            AxisAlignedBB aabb1 = physObj1->collider;
            AxisAlignedBB aabb2 = physObj2->collider;
            //Point diff = physObj1->PosDifference();
            //
            //bool isIntersectB = aabb1.IsAABBIntersectingAABB(aabb2);
            //
            //std::string isIntersect;
            //if (isIntersectB) isIntersect = "true";
            //else isIntersect = "false";
            //    
            //std::string xOffset = "X Intersection: " + std::to_string(aabb1.GetIntersectionAmountX(aabb2));
            //std::string yOffset = "Y Intersection: " + std::to_string(aabb1.GetIntersectionAmountY(aabb2));
            //std::string zOffset = "Z Intersection: " + std::to_string(aabb1.GetIntersectionAmountZ(aabb2));
            //
            //std::vector <std::string> lines;
            //lines.push_back(isIntersect);
            //lines.push_back(xOffset);
            //lines.push_back(yOffset);
            //lines.push_back(zOffset);
            //
            //DrawMultilineText(lines, 5, 5);

            if (aabb1.IsAABBIntersectingAABB(aabb2)) {
                Vector3 intersection = aabb1.GetIntersection(aabb2);
                //physObj1->SetVelocity(physObj1->velocity + (((-intersection) / physObj1->mass) * -physObj1->velocity));
            }
        }
    }
}

void Engine::Render(const Camera& cam, GLuint curFBO) {
    //Clear buffers
    if (GH_USE_SKY) {
        glClear(GL_DEPTH_BUFFER_BIT);
        sky->Draw(cam);
    }
    else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //Draw scene
    for (size_t i = 0; i < vGameObjects.size(); ++i) {
        std::shared_ptr<GameObject>& obj1 = vGameObjects[i];
        PhysicalGameObject* physObj1 = obj1->AsPhysicalGameObject();
        obj1->Draw(cam, curFBO);
        if (physObj1) {
            //DrawAABBOutline(cam, physObj1->collider);
        }
    }

    DrawXYZAxis(cam.projection, player->cam_ry, player->cam_rx, rotZ);
    std::string xyz = "Position:\n";
        xyz += 
            "   X: " + std::to_string(player->pos.x) + "\n" +
            "   Y: " + std::to_string(player->pos.y) + "\n" +
            "   Z: " + std::to_string(player->pos.z);
    DebugDrawString(xyz.c_str(), 5, 5);

#if 0
    //Debug draw outline
    for (size_t i = 0; i < vGameObjects.size(); ++i) {
        vGameObjects[i]->DebugDraw(cam);
    }

#endif
}

LRESULT Engine::WindowProc(HWND hCurWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  static PAINTSTRUCT ps;
  static BYTE lpb[256];
  static UINT dwSize = sizeof(lpb);

  switch (uMsg) {
  case WM_SYSCOMMAND:
    if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) {
      return 0;
    }
    break;

  case WM_PAINT:
    BeginPaint(hCurWnd, &ps);
    EndPaint(hCurWnd, &ps);
    return 0;

  case WM_SIZE:
    iWidth = LOWORD(lParam);
    iHeight = HIWORD(lParam);
    PostMessage(hCurWnd, WM_PAINT, 0, 0);
    return 0;

  case WM_KEYDOWN:
    //Ignore repeat keys
    if (lParam & 0x40000000) { return 0; }
    input.key[wParam & 0xFF] = true;
    input.key_press[wParam & 0xFF] = true;
    if (wParam == VK_ESCAPE) {
      PostQuitMessage(0);
    }
    return 0;

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN) {
      ToggleFullscreen();
      return 0;
    }
    break;

  case WM_KEYUP:
    input.key[wParam & 0xFF] = false;
    return 0;

  case WM_INPUT:
    dwSize = sizeof(lpb);
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
    input.UpdateRaw((const RAWINPUT*)lpb);
    break;

  case WM_CLOSE:
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hCurWnd, uMsg, wParam, lParam);
}

void Engine::CreateGLWindow() {
    WNDCLASSEX wc;
    hInstance = GetModuleHandle(NULL);
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)StaticWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = INSTANCE_CLASS;
    wc.hIconSm = NULL;

    if (!RegisterClassEx(&wc)) {
        MessageBoxEx(NULL, "RegisterClass() failed: Cannot register window class.", "Error", MB_OK, 0);
        return;
    }

    //Always start in windowed mode
    iWidth = GH_SCREEN_WIDTH;
    iHeight = GH_SCREEN_HEIGHT;

    //Create the window
    hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        INSTANCE_CLASS,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        GH_SCREEN_X,
        GH_SCREEN_Y,
        iWidth,
        iHeight,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hWnd == NULL) {
        MessageBoxEx(NULL, "CreateWindow() failed:  Cannot create a window.", "Error", MB_OK, 0);
        return;
    }

    hDC = GetDC(hWnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
        MessageBoxEx(NULL, "ChoosePixelFormat() failed: Cannot find a suitable pixel format.", "Error", MB_OK, 0);
        return;
    }

    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
        MessageBoxEx(NULL, "SetPixelFormat() failed: Cannot set format specified.", "Error", MB_OK, 0);
        return;
    }

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    if (GH_START_FULLSCREEN) {
        ToggleFullscreen();
    }
    ShowCursor(FALSE);

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
}

void Engine::InitGLGameObjects() {
  //Initialize extensions
  glewInit();

  //Basic global variables
  glClearColor(0.6f, 0.9f, 1.0f, 1.0f);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);

  //Check GL functionality
  glGetQueryiv(GL_SAMPLES_PASSED_ARB, GL_QUERY_COUNTER_BITS_ARB, &occlusionCullingSupported);

  //Attempt to enalbe vsync (if failure then oh well)
  wglSwapIntervalEXT(1);
}

void Engine::DestroyGLGameObjects() {
  vGameObjects.clear();
}

void Engine::SetupInputs() {
    static const int HID_USAGE_PAGE_GENERIC = 0x01;
    static const int HID_USAGE_GENERIC_MOUSE = 0x02;
    static const int HID_USAGE_GENERIC_JOYSTICK = 0x04;
    static const int HID_USAGE_GENERIC_GAMEPAD = 0x05;

    RAWINPUTDEVICE Rid[3];

    //Mouse
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hWnd;

    //Joystick
    Rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[1].usUsage = HID_USAGE_GENERIC_JOYSTICK;
    Rid[1].dwFlags = 0;
    Rid[1].hwndTarget = 0;

    //Gamepad
    Rid[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[2].usUsage = HID_USAGE_GENERIC_GAMEPAD;
    Rid[2].dwFlags = 0;
    Rid[2].hwndTarget = 0;

    RegisterRawInputDevices(Rid, 3, sizeof(Rid[0]));
}

void Engine::ConfineCursor() {
    if (GH_HIDE_MOUSE) {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        SetCursorPos((rect.right + rect.left) / 2, (rect.top + rect.bottom) / 2);
    }
}

void Engine::ToggleFullscreen() {
    isFullscreen = !isFullscreen;
    if (isFullscreen) {
        iWidth = GetSystemMetrics(SM_CXSCREEN);
        iHeight = GetSystemMetrics(SM_CYSCREEN);
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0,
                     iWidth, iHeight, SWP_SHOWWINDOW);
    }
    else {
        iWidth = GH_SCREEN_WIDTH;
        iHeight = GH_SCREEN_HEIGHT;
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
        SetWindowPos(hWnd, HWND_TOP, GH_SCREEN_X, GH_SCREEN_Y, iWidth, iHeight, SWP_SHOWWINDOW);
    }
}