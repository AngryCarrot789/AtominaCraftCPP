#pragma once
#include "../Rendering/Camera.h"
#include "../Inputs/Input.h"
#include "../Objects/GameObject.h"
#include "../Objects/BuiltIn/Sky.h"
#include "../Objects/Player.h"
#include "../GameHeader.h"
#include "Timer.h"
#include "GL/glew.h"
#include <windows.h>
#include <memory>
#include <vector>

class Engine {
public:
  Engine();
  ~Engine();

  int Run();
  void Update();
  void Render(const Camera& cam, GLuint curFBO);
  void LoadWorld();

  LRESULT WindowProc(HWND hCurWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  const Player& GetPlayer() const { return *player; }

  HWND GetWindow() { return hWnd; }

private:
  void CreateGLWindow();
  void InitGLGameObjects();
  void DestroyGLGameObjects();
  void SetupInputs();
  void ConfineCursor();
  void ToggleFullscreen();

  HDC   hDC;           // device context
  HGLRC hRC;		   // opengl context
  HWND  hWnd;		   // window
  HINSTANCE hInstance; // process id

  LONG iWidth;         // window width
  LONG iHeight;        // window height
  bool isFullscreen;   // fullscreen state
  bool lockCameraPos;  // Locks the camera position to wherever it was. only really for debug

  Matrix4 cam_mat;
  Camera main_cam;
  Input input;
  Timer timer;

  std::vector<std::shared_ptr<GameObject>> vGameObjects;
  std::shared_ptr<Sky> sky;
  std::shared_ptr<Player> player;

  GLint occlusionCullingSupported;
};
