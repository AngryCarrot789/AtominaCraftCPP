#pragma once
#include "../../Collisions/AxisAlignedBB.h"
#include "GL/glew.h"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4.h"
#include "../../Rendering/Camera.h"

//void DrawVertex(Vector4 v) {
//	glVertex4f(v.x, v.y, v.z, v.w);
//}

void DrawVertex(Vector3 v) {
	glVertex3f(v.x, v.y, v.z);
}

void DrawAABB(const Camera& cam, AxisAlignedBB aabb) {
	glDepthFunc(GL_ALWAYS);
	glUseProgram(0);
	glColor3f(0.0f, 1.0f, 0.0f);

	//Point pos = aabb.GetCenter();
	//Size size = aabb.GetSize();
	//const Matrix4 mvp = cam.Matrix() * Matrix4::CreateLocalToWorld(pos, Euler(0, 0, 0), size);
	//
	//Vector4 v;
	//Vector3 vp;
	//
	//Vector4 v1 = mvp * Vector4(1, 1, 0, 1);
	//Vector4 v2 = mvp * Vector4(1, -1, 0, 1);
	//Vector4 v3 = mvp * Vector4(-1, -1, 0, 1);
	//Vector4 v4 = mvp * Vector4(-1, 1, 0, 1);
	//Vector4 v5 = mvp * Vector4(-1, 1, 1, 1);
	//Vector4 v6 = mvp * Vector4(-1, -1, 1, 1);
	//Vector4 v7 = mvp * Vector4(1, -1, 1, 1);
	//Vector4 v8 = mvp * Vector4(1, 1, 1, 1);
	
	const Matrix4 mat = cam.Matrix();
	
	Vector3 v;
	
	Vector3 v1 = mat.MultiplyPoint(Vector3(1, 1, 0));
	Vector3 v2 = mat.MultiplyPoint(Vector3(1, -1, 0));
	Vector3 v3 = mat.MultiplyPoint(Vector3(-1, -1, 0));
	Vector3 v4 = mat.MultiplyPoint(Vector3(-1, 1, 0));
	Vector3 v5 = mat.MultiplyPoint(Vector3(-1, 1, 1));
	Vector3 v6 = mat.MultiplyPoint(Vector3(-1, -1, 1));
	Vector3 v7 = mat.MultiplyPoint(Vector3(1, -1, 1));
	Vector3 v8 = mat.MultiplyPoint(Vector3(1, 1, 1));

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