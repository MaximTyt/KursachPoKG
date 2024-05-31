#include "MyShaders.h"


PFNGLDELETEOBJECTARBPROC glDeleteObjectARB_ptr;
PFNGLGETHANDLEARBPROC glGetHandleARB_ptr;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB_ptr;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB_ptr;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB_ptr;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB_ptr;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB_ptr;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB_ptr;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB_ptr;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB_ptr;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB_ptr;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB_ptr;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB_ptr;
PFNGLUNIFORM1FARBPROC glUniform1fARB_ptr;
PFNGLUNIFORM1IARBPROC glUniform1iARB_ptr;
PFNGLUNIFORM3FARBPROC glUniform3fARB_ptr;
PFNGLUNIFORM2FARBPROC glUniform2fARB_ptr;
PFNGLUNIFORM4FARBPROC glUniform4fARB_ptr;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB_ptr;
PFNGLACTIVETEXTUREPROC glActiveTexture_ptr;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv_ptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv_ptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv_ptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation_ptr;
PFNGLCREATEPROGRAMPROC glCreateProgram_ptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays_ptr;
PFNGLGENBUFFERSPROC glGenBuffers_ptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray_ptr;
PFNGLBINDBUFFERARBPROC glBindBuffer_ptr;
PFNGLBUFFERDATAPROC glBufferData_ptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray_ptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer_ptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap_ptr;


int loadShader(const char *filename, char **shaderSrc, int *programLength)
{
	
		//открываем файл
		HANDLE file = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		LARGE_INTEGER size;

		//узнаем размер файла
		GetFileSizeEx(file, &size);

		*programLength = (int)size.LowPart;

		//выделяем память под буфер для считывания шейдера
		*shaderSrc = (char *)malloc(*programLength*sizeof(char));
		int sss = *programLength*sizeof(char);

		DWORD nBytesRead = 0;
		//считываем шейдер  в буфер
		ReadFile(file, *shaderSrc, *programLength, &nBytesRead, 0);
		CloseHandle(file);
		return 1;
	

}
bool init = false;
void initShadersFunctions()
{
	//наполняем делегаты адресами на функции
	glCreateShaderObjectARB_ptr = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glCreateProgramObjectARB_ptr = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB_ptr = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	glDetachObjectARB_ptr = (PFNGLDETACHOBJECTARBPROC)wglGetProcAddress("glDetachObjectARB");
	glDeleteObjectARB_ptr = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
	glShaderSourceARB_ptr = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB_ptr = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	glLinkProgramARB_ptr = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	glValidateProgramARB_ptr = (PFNGLVALIDATEPROGRAMARBPROC)wglGetProcAddress("glValidateProgramARB");
	glUseProgramObjectARB_ptr = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	glGetObjectParameterivARB_ptr = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
	glGetInfoLogARB_ptr = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");


	glUniform1fARB_ptr = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	glUniform1iARB_ptr = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	glUniform3fARB_ptr = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	glUniform4fARB_ptr = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
	glUniform2fARB_ptr = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
	glUniform1iARB_ptr = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");

	glUniformMatrix2fv_ptr = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv_ptr = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv_ptr = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetUniformLocation_ptr = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glCreateProgram_ptr = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");

	glGetUniformLocationARB_ptr = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	

	glActiveTexture_ptr = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	glGenVertexArrays_ptr = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glGenBuffers_ptr = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindVertexArray_ptr = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glBindBuffer_ptr = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBuffer");
	glBufferData_ptr = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glEnableVertexAttribArray_ptr = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer_ptr = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glGenerateMipmap_ptr = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");

	init = true;
}

void Shader::LoadShaderFromFile()
{
	if (!init)
		initShadersFunctions();

	glDeleteObjectARB_ptr(program);
	glDeleteObjectARB_ptr(vertex);
	glDeleteObjectARB_ptr(fragment);

	program = glCreateProgramObjectARB_ptr();
	vertex = glCreateShaderObjectARB_ptr(GL_VERTEX_SHADER_ARB);
	fragment = glCreateShaderObjectARB_ptr(GL_FRAGMENT_SHADER_ARB);


	char *shader1Text;
	char *shader2Text;
	int shader1TextSize;
	int shader2TextSize;

	loadShader(FshaderFileName.c_str(), &shader1Text, &shader1TextSize);
	loadShader(VshaderFileName.c_str(), &shader2Text, &shader2TextSize);

	const char *_f = shader1Text;
	const char *_v = shader2Text;

	glShaderSourceARB_ptr(fragment, 1, &_f, &shader1TextSize);
	glShaderSourceARB_ptr(vertex, 1, &_v, &shader2TextSize);


	free(shader1Text);
	free(shader2Text);
}

void Shader::Compile()
{
	
		
	
	   
	//компиляция фрагментного шейдера
	    glCompileShaderARB_ptr(fragment);

		{
			//Получение сообщения о ошибках компиляции
			int compiled = 0; int length = 0; int laux = 0;
			glGetObjectParameterivARB_ptr(fragment, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
			glGetObjectParameterivARB_ptr(fragment, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
			char *log = new char[length];
			glGetInfoLogARB_ptr(fragment, length, &laux, log);
			if (!compiled)
				MessageBox(0, log, "fragment shader error!", 0);
			delete log;
		}

		glCompileShaderARB_ptr(vertex);
		{
			int compiled = 0; int length = 0; int laux = 0;

			glGetObjectParameterivARB_ptr(vertex, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
			glGetObjectParameterivARB_ptr(vertex, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
			char *log = new char[length];
			glGetInfoLogARB_ptr(vertex, length, &laux, log);
			if (!compiled)
				MessageBox(0, log, "vertex shader error!", 0);
			delete log;
		}

		//привязывем шейдер к программному объекту
		glAttachObjectARB_ptr(program, fragment);
		glAttachObjectARB_ptr(program, vertex);
		
		//линкуем
		glLinkProgramARB_ptr(program);
		{
			int result = 0; int length = 0; int laux = 0;
			glGetObjectParameterivARB_ptr(program, GL_OBJECT_LINK_STATUS_ARB, &result);
			glGetObjectParameterivARB_ptr(program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
			char *log = new char[length];
			glGetInfoLogARB_ptr(program, length, &laux, log);
			if (!result)
				MessageBox(0, log, "fragment shader error!", 0);
			delete log;
		}
		

	
}

void Shader::UseShader()
{
	glUseProgramObjectARB_ptr(program);
}

void Shader::DontUseShaders()
{
	glUseProgramObjectARB_ptr(0);
}
