#pragma once
#include "GL/glew.h"
#include "../../GameHeader.h"
#include "../../Collisions/AxisAlignedBB.h"
#include "../../Rendering/Camera.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4.h"
#include <Windows.h>

class DebugDrawing {
public:
	static void DrawVertex(const Vector4& v) {
		glVertex4f(v.x, v.y, v.z, v.w);
	}

	static void DrawAABB(const Camera& cam, AxisAlignedBB aabb) {
		glDepthFunc(GL_ALWAYS);
		glUseProgram(0);
		glColor3f(0.0f, 1.0f, 0.0f);

		Point pos = aabb.GetCenter();
		Size size = aabb.GetSize();
		const Matrix4 mvp = cam.Matrix() * Matrix4::CreateLocalToWorld(pos, Euler(0, 0, 0), size);

		Vector4 v;

		Vector4 v1 = mvp * Vector4(1, 1, -1, 1);
		Vector4 v2 = mvp * Vector4(1, -1, -1, 1);
		Vector4 v3 = mvp * Vector4(-1, -1, -1, 1);
		Vector4 v4 = mvp * Vector4(-1, 1, -1, 1);
		Vector4 v5 = mvp * Vector4(-1, 1, 1, 1);
		Vector4 v6 = mvp * Vector4(-1, -1, 1, 1);
		Vector4 v7 = mvp * Vector4(1, -1, 1, 1);
		Vector4 v8 = mvp * Vector4(1, 1, 1, 1);

		const Matrix4 mat = cam.Matrix();

		glBegin(GL_LINE_LOOP);
		DrawVertex(v1);
		DrawVertex(v2);
		DrawVertex(v3);
		DrawVertex(v4);
		glEnd();

		glBegin(GL_LINE_LOOP);
		DrawVertex(v4);
		DrawVertex(v5);
		DrawVertex(v6);
		DrawVertex(v3);
		glEnd();

		glBegin(GL_LINE_LOOP);
		DrawVertex(v6);
		DrawVertex(v5);
		DrawVertex(v8);
		DrawVertex(v7);
		glEnd();

		glBegin(GL_LINE_LOOP);
		DrawVertex(v8);
		DrawVertex(v7);
		DrawVertex(v2);
		DrawVertex(v1);
		glEnd();

		glDepthFunc(GL_LESS);
	}

	static void DebugDrawCube(const Camera& cam, Point pos, Size scale) {
		DrawAABB(cam, AxisAlignedBB::CreateAABBFromCenter(pos, scale));
	}

	static void DebugDrawString(const char* str) {
		HWND hwnd = FindWindowA(INSTANCE_CLASS, WINDOW_TITLE);
		HDC hDC = GetDC(hwnd);
		RECT r;
		GetClientRect(hwnd, &r);
		DrawTextA(hDC, str, -1, &r, DT_TOP | DT_LEFT | DT_NOCLIP);
		ReleaseDC(hwnd, hDC);
	}
};