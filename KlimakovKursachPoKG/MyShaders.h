#ifndef MYSHADERS_H
#define MYSHADERS_H

#include <windows.h>

#include <GLS3D/install/usr/include/GL/gl.h>
#include <GLS3D/install/usr/include/GL/glu.h>


#include <GL/glext.h>
#include <glm/glm.hpp>
#include <string>


extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB_ptr;
extern PFNGLGETHANDLEARBPROC glGetHandleARB_ptr;
extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB_ptr;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB_ptr;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB_ptr;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB_ptr;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB_ptr;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB_ptr;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB_ptr;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB_ptr;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB_ptr;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB_ptr;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB_ptr;
extern PFNGLUNIFORM1FARBPROC glUniform1fARB_ptr;
extern PFNGLUNIFORM1IARBPROC glUniform1iARB_ptr;
extern PFNGLUNIFORM3FARBPROC glUniform3fARB_ptr;
extern PFNGLUNIFORM2FARBPROC glUniform2fARB_ptr;
extern PFNGLUNIFORM4FARBPROC glUniform4fARB_ptr;
extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB_ptr;
extern PFNGLACTIVETEXTUREPROC glActiveTexture_ptr;
extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv_ptr;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv_ptr;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv_ptr;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation_ptr;
extern PFNGLCREATEPROGRAMPROC glCreateProgram_ptr;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays_ptr;
extern PFNGLGENBUFFERSPROC glGenBuffers_ptr;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray_ptr;
extern PFNGLBINDBUFFERARBPROC glBindBuffer_ptr;
extern PFNGLBUFFERDATAPROC glBufferData_ptr;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray_ptr;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer_ptr;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap_ptr;



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
		glUniform1iARB_ptr(glGetUniformLocation_ptr(program, name.c_str()), value);
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
