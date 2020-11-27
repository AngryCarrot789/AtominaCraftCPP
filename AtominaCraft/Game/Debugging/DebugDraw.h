#pragma once
#include "../../Rendering/Camera.h"
#include "../../Collisions/AxisAlignedBB.h"
#include "../../Math/Vector4.h"
#include "GL/glew.h"
#include <vector>
#include "../../Math/Quaternion.h"
static void DrawVertex(const Vector4& v) {
	glVertex4f(v.x, v.y, v.z, v.w);
}

static void DrawAABBOutline(const Camera& cam, AxisAlignedBB aabb) {
	glDepthFunc(GL_ALWAYS);
	glUseProgram(0);

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

	glColor3f(0.0f, 1.0f, 0.0f);
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
	DrawAABBOutline(cam, AxisAlignedBB::CreateAABBFromCenter(pos, scale));
}
static void DebugDrawString(const char* str, int x, int y) {
	HWND hwnd = FindWindowA(INSTANCE_CLASS, WINDOW_TITLE);
	HDC hDC = GetDC(hwnd);
	RECT r;
	GetClientRect(hwnd, &r);
	r.top += y;
	r.left += x;
	DrawTextA(hDC, str, -1, &r, DT_TOP | DT_LEFT);
	ReleaseDC(hwnd, hDC);
}
static void DrawMultilineText(std::vector<std::string> lines, int x, int y){
	std::string text;
	for (int i = 0; i < lines.size(); i++) {
		text += lines[i] + "\n";
	}
	DebugDrawString(text.c_str(), x, y);
}

static void DrawXYZAxis(const Matrix4& proj, float rotX, float rotY, float rotZ) {
	//Euler e = Euler(rotY * Maths::Cos(rotX), rotX, 0);
	Point pos = Point(0.0f, 0.0f, -1.0f);
	Size scale = Size(0.1f);
	Quaternion q = Quaternion::AxisAngles(-rotY, Axis::Up()) * Quaternion::AxisAngles(-rotX, Axis::Forward());
	//Quaternion q = Quaternion::RotateAxisZ(rot);
	//Matrix4 local = Matrix4::CreateWorldToLocal(Vector3(0.5f, 0.5f, 0.0f), e, Size(0.2f));
	//Matrix4 world = local;
	//Matrix4 worldView = Matrix4::CreateLocalToWorld(pos, rot, scale);
	Matrix4 worldView = Matrix4::CreateLocalToWorld(pos, Quaternion::ToEuler(q), scale);
	Matrix4 world = proj * worldView;
	Vector4 c  = world * Vector4( 0,  0,  0, 1);
	Vector4 xP = world * Vector4( 1.5f,  0.0f,  0.0f, 1);
	Vector4 yP = world * Vector4( 0.0f,  1.5f,  0.0f, 1);
	Vector4 zP = world * Vector4( 0.0f,  0.0f,  1.5f, 1);
	Vector4 xN = world * Vector4(-0.5f,  0.0f,  0.0f, 1);
	Vector4 yN = world * Vector4( 0.0f, -0.5f,  0.0f, 1);
	Vector4 zN = world * Vector4( 0.0f,  0.0f, -0.5f, 1);

	glDepthFunc(GL_ALWAYS);
	glUseProgram(0);
	
	// positives

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);
	DrawVertex(c);
	DrawVertex(xP);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 1.0f, 0.0f);
	DrawVertex(c);
	DrawVertex(yP);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 1.0f);
	DrawVertex(c);
	DrawVertex(zP);
	glEnd();

	// negatives

	glBegin(GL_LINE_LOOP);
	glColor3f(0.5f, 0.0f, 0.0f);
	DrawVertex(c);
	DrawVertex(xN);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.5f, 0.0f);
	DrawVertex(c);
	DrawVertex(yN);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.5f);
	DrawVertex(c);
	DrawVertex(zN);
	glEnd();

	Vector4 v1 = world * Vector4(1, 1, -1, 1);
	Vector4 v2 = world * Vector4(1, -1, -1, 1);
	Vector4 v3 = world * Vector4(-1, -1, -1, 1);
	Vector4 v4 = world * Vector4(-1, 1, -1, 1);
	Vector4 v5 = world * Vector4(-1, 1, 1, 1);
	Vector4 v6 = world * Vector4(-1, -1, 1, 1);
	Vector4 v7 = world * Vector4(1, -1, 1, 1);
	Vector4 v8 = world * Vector4(1, 1, 1, 1);
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.6f, 0.4f, 0.5f);
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