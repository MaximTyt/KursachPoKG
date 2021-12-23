#ifndef MYSHADERS_H
#define MYSHADERS_H

#include <windows.h>

#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glext.h>
#include <glm/glm.hpp>
#include <string>


extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;

extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
extern PFNGLUNIFORM1FARBPROC glUniform1fARB;
extern PFNGLUNIFORM1IARBPROC glUniform1iARB;
extern PFNGLUNIFORM3FARBPROC glUniform3fARB;
extern PFNGLUNIFORM2FARBPROC glUniform2fARB;
extern PFNGLUNIFORM4FARBPROC glUniform4fARB;
extern PFNGLUNIFORM1IARBPROC glUniform1iARB;

extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;

extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	    
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

 
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;



void initShadersFunctions();
int loadShader(const char  *filename, char **shaderSrc, int *programLength);







//вызываем эту функцию в  перед рендером для настройки шейдеров

void initShadersFunctions();

class Shader
{
protected:


public:
	GLhandleARB program;
	GLhandleARB vertex;
	GLhandleARB fragment;

	std::string VshaderFileName;
	std::string FshaderFileName;
	unsigned int ID;


	Shader()
	{
		/*PFNGLCREATEPROGRAMPROC glCreateProgram;
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		ID = glCreateProgram();	*/	
	}

	~Shader()
	{

	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1iARB(glGetUniformLocation(program, name.c_str()), value);
	}

	//void setMat2(const std::string& name, const glm::mat2& mat) const
	//{
	//	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}
	//// ------------------------------------------------------------------------
	//void setMat3(const std::string& name, const glm::mat3& mat) const
	//{
	//	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}
	//// ------------------------------------------------------------------------
	//void setMat4(const std::string& name, const glm::mat4& mat) const
	//{
	//	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}

	void LoadShaderFromFile();


	void Compile();


	void UseShader();

	static void DontUseShaders();
};


//загружает код шейдера из файла
//************************************
// Parameter: char * filename   имя файла
// Parameter: char * * shaderSrc   указатель на массив символов, в который загрузится код шейдера. после того, как станет ненужным надо высвобадить память!
// Parameter: int * programLength  указатель на длину массива символов.
//************************************
int loadShader(const char  *filename, char **shaderSrc, int *programLength);


#endif // !MYSHADERS_H
