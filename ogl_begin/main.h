#pragma once
#include <Windows.h>
#include <GL/GL.h>
#include <irrKlang/irrKlang.h>
#include "MyShaders.h"
#include "ObjLoader.h"
#include "Texture.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")
ISoundEngine* SoundEngine = createIrrKlangDevice();
float kube[] = { 0,0,0, 0,1,0, 1,1,0, 1,0,0, 0,0,1, 0,1,1, 1,1,1, 1,0,1 };
GLuint kubeInd[] = { 0,1,2, 2,3,0, 4,5,6, 6,7,4, 3,2,5, 6,7,3, 0,1,5,
					5,4,0, 1,2,6, 6,5,1, 0,3,7, 7,4,0 };
BOOL showMask = FALSE;

typedef struct {
	float r, g, b;
}TColor;
typedef struct {
	float x, y, z;
}TCell;
typedef struct {
	float u, v;
}TUV;
typedef struct {
	float x, y, z;
	int type;
	float scale;
}TObject;

typedef struct {
	TObject *items;
	int itemsCnt;
	int type;
}TObjGroup;

#define mapW 150
#define mapH 150
TCell map[mapW][mapH];
TCell mapNormal[mapW][mapH];
TUV mapUV[mapW][mapH];
GLuint mapInd[mapW - 1][mapH - 1][6];
int mapIndCnt = sizeof(mapInd) / sizeof(GLuint);

int tex_pole, tex_trava, tex_flower, tex_flower1, tex_grib, tex_wood,tex_oak, tex, tex_pumpkin, colorMap, normalMap;

float plant[] = { -0.5,0,0, 0.5,0,0, 0.5,0,1, -0.5,0,1,
0,-0.5,0, 0,0.5,0, 0,0.5,1, 0,-0.5,1 };

float plantUV[] = { 0,1, 1,1, 1,0, 0,0,  0,1, 1,1, 1,0, 0,0 };

GLint plantInd[] = { 0,1,2, 2,3,0, 4,5,6, 6,7,4 };
int plantIndCnt= sizeof(plantInd) / sizeof(GLuint);

TObject* plantMas = NULL;
int plantCnt = 0;

TObject* pumpMas = NULL;
int pumpCnt = 0;


TObjGroup* tree = NULL;
int treeCnt = 0;

TObject* keyMas = NULL;
int keyCnt = 0;

bool RedactorMode = false;

typedef enum MapOper {moUp,moDown} TMapOper;

struct {
	int x, y;
	TMapOper oper;
	TCell mas[4];
}cur;
GLuint curIndex[4] = { 0,1,3,2 };

BOOL IsCoordInMap(float x, float y);
float Map_GetHeight(float x, float y);
void CalcNormals(TCell a, TCell b, TCell c, TCell* n);

Shader s[10];
ObjFile ptero, key;
Texture Ptero, keyD,keyN;
void DrawQub();

float ang = 0.0f;

#define LEVITATION_SPEED 2.0f
int location;