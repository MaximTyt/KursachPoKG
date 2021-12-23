#include "Camera.h"
#include <mmsystem.h>
#include "main.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <GL/GLU.h>
#pragma comment(lib,"glu32.lib")
using namespace std;
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#define PUSH glPushMatrix()
#define POP glPopMatrix()
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


void ClientToOpenGL(int x, int y, double* ox, double* oy, double* oz)
{
	int viewPort[4];;
	double mMatrix[16], pMatrix[16];
	float z;

	glGetIntegerv(GL_VIEWPORT, viewPort);
	y = viewPort[3] - y - 1;

	glGetDoublev(GL_MODELVIEW_MATRIX, mMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, pMatrix);
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	gluUnProject(x, y, z, mMatrix, pMatrix, viewPort, ox, oy, oz);
}

void Cur_Height()
{	
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			int x = i + cur.x;
			int y = j + cur.y;
			if (IsCoordInMap(x, y))
			{
				if (cur.oper == moUp) map[x][y].z += 0.1;
				if (cur.oper == moDown) map[x][y].z -= 0.1;
			}
		}
	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++)
		{
			int x = i + cur.x;
			int y = j + cur.y;
			if (IsCoordInMap(x, y))
				CalcNormals(map[x][y], map[x + 1][y], map[x][y + 1], &mapNormal[x][y]);
		}
	for (int i = 0; i < plantCnt; i++)
		if ((plantMas[i].x > cur.x - 1) &&
			(plantMas[i].x < cur.x + 2) &&
			(plantMas[i].y > cur.y - 1) &&
			(plantMas[i].y < cur.y + 2))
			plantMas[i].z = Map_GetHeight(plantMas[i].x, plantMas[i].y);
	for (int i = 0; i < pumpCnt; i++)
		if ((pumpMas[i].x > cur.x - 1) &&
			(pumpMas[i].x < cur.x + 2) &&
			(pumpMas[i].y > cur.y - 1) &&
			(pumpMas[i].y < cur.y + 2))
			pumpMas[i].z = Map_GetHeight(pumpMas[i].x, pumpMas[i].y);
	for (int i = 0; i < keyCnt; i++)
		if ((keyMas[i].x > cur.x - 1) &&
			(keyMas[i].x < cur.x + 2) &&
			(keyMas[i].y > cur.y - 1) &&
			(keyMas[i].y < cur.y + 2))
			keyMas[i].z = Map_GetHeight(keyMas[i].x, keyMas[i].y)+1.7;
}


void Cur_Init()
{
	cur.oper = moUp;
}


void Cur_SetPos(int x, int y)
{
	cur.x = x;
	cur.y = y;
}

void Cur_Show()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(2);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cur.mas);
	glColor3f(1, 1, 1);

	if (cur.oper == moUp) glColor3f(0.5, 1, 0.5);
	else if (cur.oper == moDown) glColor3f(1, 0.5, 0.5);
	int k = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			int x = i + cur.x;
			int y = j + cur.y;
			cur.mas[k].x = x;
			cur.mas[k].y = y;
			cur.mas[k].z = IsCoordInMap(x, y) ? map[x][y].z : 0;
			k++;
		}
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, curIndex);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void LoadTexture(const char* file_name, int* target)
{
	int width, height, cnt;
	unsigned char* data = stbi_load(file_name, &width, &height, &cnt, 0);
	glGenTextures(1, (GLuint*)target);
	glBindTexture(GL_TEXTURE_2D, *target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

BOOL IsCoordInMap(float x, float y)
{
	return (x >= 0) && (x < mapW) && (y >= 0) && (y < mapH);
}

float Map_GetHeight(float x, float y)
{
	if (!IsCoordInMap(x, y)) return 0;
	int cX = (int)x;
	int cY = (int)y;
	float h1 = ((1 - (x - cX)) * map[cX][cY].z + (x - cX) * map[cX + 1][cY].z);
	float h2 = ((1 - (x - cX)) * map[cX][cY + 1].z + (x - cX) * map[cX + 1][cY + 1].z);
	return ((1 - (y - cY)) * h1 + (y - cY) * h2);
}


void CalcNormals(TCell a, TCell b, TCell c, TCell* n)
{
	float wrki;
	TCell v1, v2;

	v1.x = a.x - b.x;
	v1.y = a.y - b.y;
	v1.z = a.z - b.z;
	v2.x = b.x - c.x;
	v2.y = b.y - c.y;
	v2.z = b.z - c.z;

	n->x = (v1.y * v2.z - v1.z * v2.y);
	n->y = (v1.z * v2.x - v1.x * v2.z);
	n->z = (v1.x * v2.y - v1.y * v2.x);
	wrki = sqrt(pow(n->x, 2) + pow(n->y, 2) + pow(n->z,2));
	n->x /= wrki;
	n->y /= wrki;
	n->z /= wrki;

}

void normal(double A[], double B[], double C[], double* tmp)
{
	double a[] = { 0,0,0 };
	double b[] = { 0,0,0 };
	double len;
	for (int i = 0; i < 3; i++)
	{
		a[i] = A[i] - B[i];
		b[i] = C[i] - B[i];
	}
	tmp[0] = a[1] * b[2] - b[1] * a[2];
	tmp[1] = -a[0] * b[2] + b[0] * a[2];
	tmp[2] = a[0] * b[1] - b[0] * a[1];
	len = sqrt(pow(tmp[0], 2) + pow(tmp[1], 2) + pow(tmp[2], 2));
	tmp[0] /= len;
	tmp[1] /= len;
	tmp[2] /= len;
}



void Map_CreateHill(int posX, int posY, int rad, int height)
{
	for(int i = posX-rad; i <= posX + rad; i++)
		for (int j = posY - rad; j <= posY + rad; j++)
		{
			if (IsCoordInMap(i, j))
			{
				float len = sqrt(pow(posX - i, 2) + pow(posY - j, 2));
				if (len < rad)
				{
					len = len / rad * PI_2;
					map[i][j].z += cos(len) * height;
				}
			}
		}
}



void Tree_Show(TObjGroup obj)
{
	glEnable(GL_TEXTURE_2D);
	double A[] = { 0,0,0 };
	double B[] = { 1,0,0 };
	double C[] = { 1,1,0 };
	double D[] = { 0,1,0 };
	double A1[] = { 0,0,1 };
	double B1[] = { 1,0,1 };
	double C1[] = { 1,1,1 };
	double D1[] = { 0,1,1 };
	double tmp[] = { 0,0,0 };
	glColor3f(0.7, 0.7, 0.7);
	glBindTexture(GL_TEXTURE_2D, obj.type);

	for (int i = 0; i < obj.itemsCnt; i++)
	{
		if (obj.items[i].type == 1)
		{
			PUSH;
			glTranslatef(obj.items[i].x, obj.items[i].y, obj.items[i].z);
			glBegin(GL_QUADS);			
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(A);
			glTexCoord2d(1, 0.5);
			glVertex3dv(B);
			glTexCoord2d(1, 0);
			glVertex3dv(C);
			glTexCoord2d(0.5, 0);
			glVertex3dv(D);

			/*normal(B, A, A1, tmp);*/
			glNormal3d(0,0,1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(A);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(B);
			glTexCoord2d(0.5, 0);
			glVertex3dv(B1);
			glTexCoord2d(0, 0);
			glVertex3dv(A1);

			/*normal(C, B, B1, tmp);*/
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(B);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(C);
			glTexCoord2d(0.5, 0);
			glVertex3dv(C1);
			glTexCoord2d(0, 0);
			glVertex3dv(B1);

			/*normal(D, C, C1, tmp);*/
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(C);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(D);
			glTexCoord2d(0.5, 0);
			glVertex3dv(D1);
			glTexCoord2d(0, 0);
			glVertex3dv(C1);

			/*normal(A, D, D1, tmp);*/
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(D);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(A);
			glTexCoord2d(0.5, 0);
			glVertex3dv(A1);
			glTexCoord2d(0, 0);
			glVertex3dv(D1);

			glNormal3d(0, 0, 1);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(A1);
			glTexCoord2d(1, 0.5);
			glVertex3dv(B1);
			glTexCoord2d(1, 0);
			glVertex3dv(C1);
			glTexCoord2d(0.5, 0);
			glVertex3dv(D1);
			glEnd();
			POP;
		}
		else
		{
			PUSH;
			glTranslatef(obj.items[i].x, obj.items[i].y, obj.items[i].z);
			glBegin(GL_QUADS);			
			glTexCoord2d(0, 1);
			glVertex3dv(A);
			glTexCoord2d(0.5, 1);
			glVertex3dv(B);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(C);
			glTexCoord2d(0, 0.5);
			glVertex3dv(D);

			/*normal(B, A, A1, tmp);
			glNormal3dv(tmp);*/
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 1);
			glVertex3dv(A);
			glTexCoord2d(0.5, 1);
			glVertex3dv(B);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(B1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(A1);

			/*normal(C, B, B1, tmp);
			glNormal3dv(tmp);*/
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 1);
			glVertex3dv(B);
			glTexCoord2d(0.5, 1);
			glVertex3dv(C);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(C1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(B1);

			/*normal(D, C, C1, tmp);
			glNormal3dv(tmp);*/
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 1);
			glVertex3dv(C);
			glTexCoord2d(0.5, 1);
			glVertex3dv(D);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(D1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(C1);

			/*normal(A, D, D1, tmp);
			glNormal3dv(tmp);*/
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 1);
			glVertex3dv(D);
			glTexCoord2d(0.5, 1);
			glVertex3dv(A);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(A1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(D1);

			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 1);
			glVertex3dv(A1);
			glTexCoord2d(0.5, 1);
			glVertex3dv(B1);
			glTexCoord2d(0.5, 0.5);
			glVertex3dv(C1);
			glTexCoord2d(0, 0.5);
			glVertex3dv(D1);
			glEnd();
			POP;
		}
	}
}


void Tree_Create(TObjGroup* obj, int type, float x, float y)
{
	obj->type = type;
	float z = Map_GetHeight(x + 0.5, y + 0.5) - 0.5;
	int logs = 6;
	int leafs = 66;

	obj->itemsCnt = logs + leafs;
	obj->items = (TObject*)malloc(sizeof(TObject) * obj->itemsCnt);
	for (int i = 0; i < logs; i++)
	{
		obj->items[i].type = 1;
		obj->items[i].x = x;
		obj->items[i].y = y;
		obj->items[i].z = z + i;
	}
	int pos = logs;
	for (int k = 0; k < 2; k++)
		for (int i = x - 2; i <= x + 2; i++)
			for (int j = y - 2; j <= y + 2; j++)
				if ((i != x) || (j != y))
				{
					obj->items[pos].type = 2;
					obj->items[pos].x = i;
					obj->items[pos].y = j;
					obj->items[pos].z = z + logs - 2 + k;
					pos++;
				}
	for (int k = 0; k < 2; k++)
		for (int i = x - 1; i <= x + 1; i++)
			for (int j = y - 1; j <= y + 1; j++)
			{
				obj->items[pos].type = 2;
				obj->items[pos].x = i;
				obj->items[pos].y = j;
				obj->items[pos].z = z + logs + k;
				pos++;
			}
}


void Map_Init()
{
	
	LoadTexture("textures/pole.png", &tex_pole);
	LoadTexture("textures/trava.png", &tex_trava);
	LoadTexture("textures/flower.png", &tex_flower);
	LoadTexture("textures/flower2.png", &tex_flower1);
	LoadTexture("textures/grib.png", &tex_grib);
	LoadTexture("textures/wood.png", &tex_wood);
	LoadTexture("textures/oak.png", &tex_oak);	
	LoadTexture("textures/Pumpkin.png", &tex_pumpkin);
	LoadTexture("textures/key.png", &colorMap);
	LoadTexture("textures/keyNmap.png", &normalMap);
	
	Ptero.loadTextureFromFile("textures//ptero.bmp");
	Ptero.bindTexture();


	keyD.loadTextureFromFile("textures//key.bmp");
	keyD.bindTexture();

	keyN.loadTextureFromFile("textures//keyNmap.bmp");
	keyN.bindTexture();


	s[0].VshaderFileName = "shaders\\complex.vert"; //имя файла вершинного шейдер
	s[0].FshaderFileName = "shaders\\normalmap.frag"; //имя файла фрагментного шейдера
	s[0].LoadShaderFromFile(); //загружаем шейдеры из файла
	s[0].Compile(); //компилируем

	s[1].VshaderFileName = "shaders\\v.vert"; //имя файла вершинного шейдер
	s[1].FshaderFileName = "shaders\\textureShader.frag"; //имя файла фрагментного шейдера
	s[1].LoadShaderFromFile(); //загружаем шейдеры из файла
	s[1].Compile(); //компилируем

	loadModel((char*)"models//Ptero.obj_m", &ptero);
	loadModel((char*)"models//key.obj_m", &key);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.99);

	for (int i = 0; i < mapW; i++)
		for (int j = 0; j < mapH; j++)
		{			
			map[i][j].x = i;
			map[i][j].y = j;
			map[i][j].z = (rand() % 10) * 0.01;

			mapUV[i][j].u = i;
			mapUV[i][j].v = j;
		}
	for (int i = 0; i < mapW-1; i++)
	{
		int pos = i * mapH;
		for (int j = 0; j < mapH-1; j++)
		{
			mapInd[i][j][0] = pos;
			mapInd[i][j][1] = pos + 1;
			mapInd[i][j][2] = pos + 1 + mapH;

			mapInd[i][j][3] = pos + 1 + mapH;
			mapInd[i][j][4] = pos + mapH;
			mapInd[i][j][5] = pos;

			pos++;
		}
	}

	for (int i = 0; i < 50; i++)
		Map_CreateHill(rand() % mapW, rand() % mapH, rand() % 50, rand() % 10); //генерируем холмики
	for (int i = 0; i < mapW - 1; i++)
		for (int j = 0; j < mapH - 1; j++)
			CalcNormals(map[i][j], map[i + 1][j], map[i][j + 1], &mapNormal[i][j]);
	int travaN = 20000;
	int gribN = 50;
	plantCnt = travaN + gribN;
	plantMas = (TObject*)realloc(plantMas, sizeof(*plantMas) * plantCnt);
	if (plantMas != NULL)
	{
		for (int i = 0; i < plantCnt; i++)
		{
			if (i < travaN)
			{
				plantMas[i].type = rand() % 10 != 0 ? tex_trava :
					(rand() % 2 == 0 ? tex_flower : tex_flower1);
				plantMas[i].scale = 0.7 + (rand() % 5) * 0.1;
			}
			else if (i < (travaN + gribN))
			{
				plantMas[i].type = tex_grib;
				plantMas[i].scale = 0.2 + (rand() % 10) * 0.01;
			}

			plantMas[i].x = rand() % mapW;
			plantMas[i].y = rand() % mapH;
			plantMas[i].z = Map_GetHeight(plantMas[i].x, plantMas[i].y);
		}
	}
	else
	{
		free(plantMas);
	}

	treeCnt = 150;
	tree = (TObjGroup*)realloc(tree, sizeof(*tree) * treeCnt);
	if (tree != NULL)
		for (int i = 0; i < treeCnt; i++)
		{
			tree[i].type = rand() % 2 == 0 ? tex_wood : tex_oak;
			Tree_Create(tree + i, tree[i].type, rand() % mapW, rand() % mapH);
		}
	else
	{
		free(tree);
	}


	pumpCnt = 80;
	pumpMas = (TObject*)realloc(pumpMas, sizeof(*pumpMas) * pumpCnt);
	if (pumpMas != NULL)
		for (int i = 0; i < pumpCnt; i++)
		{
			pumpMas[i].x = rand() % mapW;
			pumpMas[i].y = rand() % mapH;
			pumpMas[i].z = Map_GetHeight(pumpMas[i].x, pumpMas[i].y);
		}
	else
	{
		free(pumpMas);
	}

	keyCnt = 13;
	keyMas = (TObject*)realloc(keyMas, sizeof(*keyMas) * keyCnt);
	if (keyMas != NULL)
		for (int i = 0; i < keyCnt; i++)
		{
			keyMas[i].x = rand() % mapW;
			keyMas[i].y = rand() % mapH;
			keyMas[i].z = Map_GetHeight(keyMas[i].x, keyMas[i].y)+1.7;
		}
	else
	{
		free(keyMas);
	}
	
	Cur_Init();
	Cur_SetPos(2, 2);
}

void DrawQuad()
{
	double A[] = { 0,0 };
	double B[] = { 1,0 };
	double C[] = { 1,1 };
	double D[] = { 0,1 };
	glBegin(GL_QUADS);		
	glTexCoord2d(0, 0);
	glVertex2dv(A);
	glTexCoord2d(1, 0);
	glVertex2dv(B);
	glTexCoord2d(1, 1);
	glVertex2dv(C);
	glTexCoord2d(0, 1);
	glVertex2dv(D);
	glEnd();
}


void DrawQub()
{		
	double A[] = { 0,0,0 };
	double B[] = { 1,0,0 };
	double C[] = { 1,1,0 };
	double D[] = { 0,1,0 };
	double A1[] = { 0,0,1 };
	double B1[] = { 1,0,1 };
	double C1[] = { 1,1,1 };
	double D1[] = { 0,1,1 };
	double tmp[] = { 0,0,0 };
	glColor3f(0.7, 0.7, 0.7);
	PUSH;	
	glBegin(GL_QUADS);
	glTexCoord2d(0.5, 0.5);
	glVertex3dv(A);
	glTexCoord2d(1, 0.5);
	glVertex3dv(B);
	glTexCoord2d(1, 0);
	glVertex3dv(C);
	glTexCoord2d(0.5, 0);
	glVertex3dv(D);

	normal(B, A, A1, tmp);
	glNormal3dv(tmp);
	glTexCoord2d(0, 1);
	glVertex3dv(A);
	glTexCoord2d(0.5, 1);
	glVertex3dv(B);
	glTexCoord2d(0.5, 0.5);
	glVertex3dv(B1);
	glTexCoord2d(0, 0.5);
	glVertex3dv(A1);

	normal(C, B, B1, tmp);
	glNormal3dv(tmp);
	glTexCoord2d(0, 0.5);
	glVertex3dv(B);
	glTexCoord2d(0.5, 0.5);
	glVertex3dv(C);
	glTexCoord2d(0.5, 0);
	glVertex3dv(C1);
	glTexCoord2d(0, 0);
	glVertex3dv(B1);

	normal(D, C, C1, tmp);
	glNormal3dv(tmp);
	glTexCoord2d(0, 0.5);
	glVertex3dv(C);
	glTexCoord2d(0.5, 0.5);
	glVertex3dv(D);
	glTexCoord2d(0.5, 0);
	glVertex3dv(D1);
	glTexCoord2d(0, 0);
	glVertex3dv(C1);

	normal(A, D, D1, tmp);
	glNormal3dv(tmp);
	glTexCoord2d(0, 0.5);
	glVertex3dv(D);
	glTexCoord2d(0.5, 0.5);
	glVertex3dv(A);
	glTexCoord2d(0.5, 0);
	glVertex3dv(A1);
	glTexCoord2d(0, 0);
	glVertex3dv(D1);

	glNormal3d(0, 0, 1);
	glTexCoord2d(0.5, 0.5);
	glVertex3dv(A1);
	glTexCoord2d(1, 0.5);
	glVertex3dv(B1);
	glTexCoord2d(1, 0);
	glVertex3dv(C1);
	glTexCoord2d(0.5, 0);
	glVertex3dv(D1);
	glEnd();
	POP;	
	
}

void Map_Show()
{	
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_LIGHT0, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_LIGHT0, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_LIGHT0, GL_SPECULAR, spec);
	//размер блика
	glMaterialf(GL_LIGHT0, GL_SHININESS, sh);
#define zakat 40.0
	static float alfa = 0;
	alfa += 0.3;
	if (alfa > 180) alfa -= 360;

	float kcc = 1 - fabs(alfa) / 180;
	float k = 90 - fabs(alfa);
	k = zakat - fabs(k);
	k = k < 0 ? 0 : k / zakat;
	glClearColor(0.6* kcc, 0.8* kcc, 1* kcc, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	PUSH;
		PUSH;
		glRotatef(-camera.Xrot, 1, 0, 0);
		glRotatef(-camera.Zrot, 0, 0, 1);
		glRotatef(alfa, 0, 1, 0);
		glTranslatef(0, 0, 40);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glColor3d(1, 1-k*0.8, 1-k);
		DrawQuad();
		
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		POP;
	camera.Camera_Apply();
	PUSH;
		glRotatef(alfa, 0, 1, 0);
		GLfloat position[] = { 0,0,1,0 };
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		float mas[] = { 1 + k * 2,1,1,0 };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, mas);
		float clr = kcc * 0.15 + 0.05;
		float mas0[] = { clr,clr,clr,0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mas0);//параметры модели освещения; измененние фонового освещения	
	POP;
	glEnableClientState(GL_VERTEX_ARRAY); //терейн
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, map);
	glTexCoordPointer(2, GL_FLOAT, 0, mapUV);
	glColor3f(0.7, 0.7, 0.7);
	glNormalPointer(GL_FLOAT, 0, mapNormal);
	glBindTexture(GL_TEXTURE_2D, tex_pole);
	glDrawElements(GL_TRIANGLES, mapIndCnt, GL_UNSIGNED_INT, mapInd);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);//трава,цветы, грибы
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	glVertexPointer(3, GL_FLOAT, 0, plant);
	glTexCoordPointer(2, GL_FLOAT, 0, plantUV);
	glColor3f(0.7, 0.7, 0.7);
	glNormal3f(0, 0, 1);	
	for (int i = 0; i < plantCnt; i++)
	{
		glBindTexture(GL_TEXTURE_2D, plantMas[i].type);
		PUSH;
		glTranslatef(plantMas[i].x, plantMas[i].y, plantMas[i].z);
		glScalef(plantMas[i].scale, plantMas[i].scale, plantMas[i].scale);
		glDrawElements(GL_TRIANGLES, plantIndCnt, GL_UNSIGNED_INT, plantInd);
		POP;		
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	

	

	
		
	for (int i = 0; i < treeCnt; i++)//деревья
		Tree_Show(tree[i]);
	
	for (int i = 0; i < pumpCnt; i++)//тыквы
	{	
		PUSH;
		
		glTranslatef(pumpMas[i].x, pumpMas[i].y, pumpMas[i].z);
		glRotated(90, 0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex_pumpkin);
		DrawQub();
		POP;
	}

	PUSH;
	s[1].UseShader();
	location = glGetUniformLocationARB(s[1].program, "tex");	
	glUniform1iARB(location, 0);
	glRotatef(alfa*10, 1, 1, 1);
	glTranslated(mapW/2, mapH/2, 40);
	glScaled(0.1, 0.1, 0.1);
	/*glRotatef(-90, 0, 1, 0);*/
	Ptero.bindTexture();
	ptero.DrawObj();
	s[1].DontUseShaders();
	POP;

	for (int i = 0; i < keyCnt; i++)
	{
		
		PUSH;
		s[0].UseShader();
		s[0].setInt("colorMap", 0);
		s[0].setInt("normalMap", 1);
		location = glGetUniformLocation(s[0].program, "invRadius");
		glUniform1fARB(location, 0.0f);
		location = glGetUniformLocation(s[0].program, "alpha");
		glUniform1fARB(location, 1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		ang = fmod(ang + LEVITATION_SPEED, 360);
		//glTranslatef(keyMas[i].x, keyMas[i].y + 0.5f + (sin(ang * (PI / 180))) / 2, Map_GetHeight(keyMas[i].x, keyMas[i].y) + 1.7);
				
		glTranslatef(keyMas[i].x, keyMas[i].y, keyMas[i].z);
		glRotatef(ang, 0.0f, 1.0f, 0.0f);
		key.DrawObj();		
		s[0].DontUseShaders();
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		POP;
	}
	Shader::DontUseShaders();
	
	if(RedactorMode)
		Cur_Show();
	else
	{
		//for (int ii = 0; ii < 200; ii++)
		//{
		//	PUSH;
		//	/*s[1].UseShader();
		//	int l = glGetUniformLocationARB(s[1].program, "tex");
		//	glTranslated(camera.pos.x, camera.pos.y, camera.pos.z-3.7);
		//	glUniform1iARB(l, 0);

		//	Ptero.bindTexture();
		//	ptero.DrawObj();
		//	s[1].DontUseShaders();*/
		//	/*glTranslated(camera.pos.x, camera.pos.y, camera.pos.z-1.7);*/
		//	glTranslated(rand() % mapW, rand() % mapH, Map_GetHeight(rand() % mapW, rand() % mapH));
		//	DrawQub();
		//	POP;
		//}
	}
	POP;
}

void Game_Move()
{		
	if (!RedactorMode)
	{
		camera.Camera_MoveDirection(0.3);
		camera.Camera_AutoMoveByMouse(400, 300, 0.2);
		camera.pos.z = Map_GetHeight(camera.pos.x, camera.pos.y) + 2.7;
		
		/*if (GetAsyncKeyState(VK_SPACE) < 0)
		{
			camera.x += cos(0.1);
			camera.y += sin(0.1);
			camera.z = Map_GetHeight(camera.x, camera.y) + 2.7;			
		}*/
	}
	else
	{
		camera.Camera_MoveDirection(0.5);
		if (GetAsyncKeyState(VK_RBUTTON) < 0)
		{
			camera.Camera_AutoMoveByMouse(400, 300, 0.2);
		}		
		if (GetAsyncKeyState(VK_LBUTTON) < 0)
			Cur_Height();
		camera.pos.z = Map_GetHeight(camera.pos.x, camera.pos.y) + 10;
	}
} 

void WndResize(int x, int y)
{
	glViewport(0, 0, x, y);// размер области вывода
	float k = x / (float)y;
	float sz = 0.1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, 1000); // установка перспективной проекции
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



int imusic = 0;
const int numOfmusic = 3;
const char* music[numOfmusic] = { "breakout.wav", "sun.wav","sweet.wav" };
void Game_Init()
{
	SoundEngine->play2D(music[imusic], true);
	glEnable(GL_DEPTH_TEST);
	Map_Init();	
	RECT rct;
	GetClientRect(hwnd, &rct);
	WndResize(rct.right, rct.bottom);
}
void Game_Show()
{
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glClearColor(0.6, 0.8, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	PUSH;		
	camera.Camera_Apply();

	GLfloat position[] = { 1,0,2,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	Map_Show();
	POP;		
}

void ScrollMusic()
{
	if (GetAsyncKeyState('E')<0)
	{
		imusic++;
		SoundEngine->stopAllSounds();
		if (imusic == numOfmusic)
			imusic = 0;
		SoundEngine->play2D(music[imusic], true);
		if (imusic == 2)
			SoundEngine->setSoundVolume(0.5);
		else
			SoundEngine->setSoundVolume(0.8);
	}		
}

void Redactor()
{
	if (GetAsyncKeyState('Q') < 0)
	{
		RedactorMode = !RedactorMode;
	}
}


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	float theta = 0.0f;

	/* register window class */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "GLSample";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


	if (!RegisterClassEx(&wcex))
		return 0;

	/* create main window */
	hwnd = CreateWindowEx(0,
		"GLSample",
		"Kursach",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1920,
		1080,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);
	RECT rct;
	GetClientRect(hwnd, &rct);
	WndResize(rct.right, rct.bottom);
	Game_Init();

	/* program main loop */
	while (!bQuit)
	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/* OpenGL animation code goes here */
			
			Game_Move();
			Map_Show();			

			
			

			SwapBuffers(hDC);

			/*theta += 1.0f;*/
			Sleep(1);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	double x, y, z;
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:		
			PUSH;
			camera.Camera_Apply();
			ClientToOpenGL(LOWORD(lParam), HIWORD(lParam), &x, &y, &z);
			Cur_SetPos((int)x, (int)y);
			POP;		
		break;
	case WM_SIZE:
		WndResize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_SETCURSOR:		
			ShowCursor(FALSE);		
		break;

	case WM_RBUTTONDOWN:
		if(RedactorMode)
			SetCursorPos(400, 400);
		break;

	/*case WM_LBUTTONDOWN:
		Player_Shoot();
		break;*/
	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
			if (RedactorMode)
			{
		case '1':
			cur.oper = moUp;
			break;
		case '2':
			cur.oper = moDown;
			break;
			}		
		}
		ScrollMusic();
		Redactor();
		
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}