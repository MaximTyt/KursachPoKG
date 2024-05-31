#pragma once
#include <windows.h>
#include <GL/GL.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
using namespace glm;
HWND hwnd;
#define PI 3.1415926535
#define PI_2 1.57079632679489661923

class Object
{
public:
	vec3 pos;
};

class Light : public Object
{
public:	
	virtual void SetUpLight() = 0;
};

class Camera : public Object
{
public:

	vec3 lookPoint;
	vec3 normal;

	Camera()
	{

	}


	virtual void SetUpCamera()
	{

	}
};

class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = vec3(1, 1, 1);
	}
	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.x, pos.y, pos.z, 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}

} light;



class CustomCamera: public Camera
{
	
public:	
	float Xrot, Zrot;
	CustomCamera()
	{
		pos=vec3(0, 0, 1.7);
		Xrot = 70;
		Zrot = -40;
	}
	void Camera_Apply()
	{
		glRotatef(-Xrot, 1, 0, 0);
		glRotatef(-Zrot, 0, 0, 1);
		glTranslatef(-pos.x,-pos.y,-pos.z);
	}

	void Camera_Rotation(float xAngle, float zAngle)
	{
		Zrot += zAngle;
		if (Zrot < 0) Zrot += 360;
		if (Zrot > 360) Zrot -= 360;
		Xrot += xAngle;
		if (Xrot < 0) Xrot = 0;
		if (Xrot > 180) Xrot = 180;
	}

	void Camera_AutoMoveByMouse(int centreX, int centreY, float speed)
	{
		if (GetForegroundWindow() != hwnd) return;
		POINT cur;
		static POINT base = { centreX,centreY };
		GetCursorPos(&cur);
		Camera_Rotation((base.y - cur.y) * speed, (base.x - cur.x) * speed);
		SetCursorPos(base.x, base.y);
	}
	void Camera_MoveDirection(float ispeed)
	{
		if (GetForegroundWindow() != hwnd) return;

		float ugol = -Zrot / 180 * PI;
		float speed = 0;
		float upspedd = 0;
		if (GetAsyncKeyState('W') < 0) 
		{ 
			speed = ispeed;
			if (GetAsyncKeyState(VK_LSHIFT) < 0) speed = 2*ispeed;
		}
		if (GetAsyncKeyState('S') < 0) 
		{
			speed = -ispeed;
			if (GetAsyncKeyState(VK_LSHIFT) < 0) speed = -2 * ispeed;
		}
		if (GetAsyncKeyState('A') < 0)
		{
			speed = ispeed; ugol -= PI * 0.5;
			if (GetAsyncKeyState(VK_LSHIFT) < 0) speed = 2 * ispeed;
		}
		if (GetAsyncKeyState('D') < 0)
		{
			speed = ispeed; ugol += PI * 0.5;
			if (GetAsyncKeyState(VK_LSHIFT) < 0) speed = 2 * ispeed;
		}		
		if (speed != 0)
		{
			pos.x += sin(ugol) * speed;
			pos.y += cos(ugol) * speed;
		}	
	
	}

	mat4 GetViewMatrix()
	{
		return lookAt(pos,lookPoint, normal);
	}
}camera;
