//
//  main.cpp
//  mandelbrot
//
//  Created by Keichi Takahashi on 12/03/09.
//  Copyright (c) 2012年 Osaka University. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <GLUT/glut.h>

static GLuint program;
static GLuint buffer;
static int levelValue = 80;

float vertex[] =
{
    -1.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    -1.0, -1.0, 0.0,
    1.0, -1.0, 0.0,
};

GLuint vtxindex[]={
    0,1,2,3
};

float start_color_const[] = { 0.0, 0.0, 1.0, 1.0 };
float end_color_const[] = { 1.0, 1.0, 1.0, 1.0 };

static int make_resources(void)
{
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    
    return 1;
}

static void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(program);
    GLint level = glGetUniformLocation(program, "level");
    glUniform1i(level, levelValue);
    GLint start_color = glGetUniformLocation(program, "start_color");
    glUniform4fv(start_color, 1, start_color_const);
    GLint end_color = glGetUniformLocation(program, "end_color");
    glUniform4fv(end_color, 1, end_color_const);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);    
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)vtxindex);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glutSwapBuffers();
}

static int loadShader(GLuint shader, char *file);
static void printShaderInfoLog(GLuint shader);
static void printProgramInfoLog(GLuint program);
int FUTL_LoadShader(char *vtxShdName, char *frgShdName, GLuint *lpProg);

/* シェーダコンパイルのログバッファ */
#define MAX_SHADER_LOG_SIZE		(1024)
char s_logBuffer[MAX_SHADER_LOG_SIZE]; 

/*
 * シェーダプログラムのロード
 */
int FUTL_LoadShader(
                    char *vtxShdName,
                    char *frgShdName,
                    GLuint *lpProg
                    )
{
	GLuint vtxShader;
	GLuint frgShader;
	GLuint prog;
	GLint linked;
    
	/* シェーダオブジェクトの作成 */
	vtxShader = glCreateShader(GL_VERTEX_SHADER);
	frgShader = glCreateShader(GL_FRAGMENT_SHADER);
    
	/* バーテックスシェーダのロードとコンパイル */
	if (loadShader(vtxShader, vtxShdName) < 0)
	{
		return -1;
	}
    
	/* フラグメントシェーダのロードとコンパイル */
	if (loadShader(frgShader, frgShdName) < 0)
	{
		return -1;
	}
    
	/* プログラムオブジェクトの作成 */
	prog = glCreateProgram();
    
	/* シェーダオブジェクトのシェーダプログラムへの登録 */
	glAttachShader(prog, vtxShader);
	glAttachShader(prog, frgShader);
    
	/* シェーダオブジェクトの削除 */
	glDeleteShader(vtxShader);
	glDeleteShader(frgShader);
    
	/* シェーダプログラムのリンク */
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	printProgramInfoLog(prog);
	if (linked == GL_FALSE)
	{
        std::cerr << "Link error of " 
        << vtxShdName <<  " & " << frgShdName << " !!" << std::endl;
		return -1;
	} else {
        std::cout << "Link successful: " << vtxShdName <<  " & " << frgShdName << std::endl;
    }
    
	*lpProg = prog;
    
	return 0;
}


/*
 * シェーダープログラムをロードし、コンパイル
 */
static int loadShader(
                      GLuint shader, 
                      char *name
                      )
{
	GLint compiled;
    
    std::ifstream inputFile2(name);
	if (!inputFile2.is_open())
	{
        std::cout << name << "does not exist!" << std::endl;
		return -1;
	} else {
        std::cout << "Successfully loaded shader file " << name << std::endl;
    }
    
    std::istreambuf_iterator<char> fdataBegin(inputFile2);
    std::istreambuf_iterator<char> fdataEnd;
    
    std::string fileData2(fdataBegin,fdataEnd);
    const char * file = fileData2.c_str();
    
	glShaderSource(shader, 1, &file, NULL);
    
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(shader);		/* コンパイルログの出力 */
	if (compiled == GL_FALSE)
	{
        std::cout << "Compile error in " << name << std::endl;
		return -1;
	} else {
        std::cout << "Successfully compiled shader " << name << std::endl;
    }
    
	return 0;
}

/* シェーダコンパイルエラーの出力 */
static void printShaderInfoLog(GLuint shader)
{
	int logSize;
	int length;
    
	/* ログの長さは、最後のNULL文字も含む */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);
    
	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
        std::cout << "Shader Info Log " << s_logBuffer << std::endl;
	}
}


/* シェーダリンクエラーの出力 */
static void printProgramInfoLog(GLuint program)
{
	int logSize;
	int length;
    
	/* ログの長さは、最後のNULL文字も含む */
	glGetProgramiv(program, GL_INFO_LOG_LENGTH , &logSize);
    
	if (logSize > 1)
	{
		glGetProgramInfoLog(program, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		fprintf(stderr, "Program Info Log\n%s\n", s_logBuffer);
	}
}

void onKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            levelValue += 10;
            break;
        case GLUT_KEY_DOWN:
            levelValue -= 10;
            break;
        default:
            break;
    }
}

int main (int argc, const char * argv[])
{
    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1200, 900);
    glutCreateWindow("Hello OpenGL");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(&render);
    glutIdleFunc(glutPostRedisplay);
    glutSpecialFunc(onKey);
    
    FUTL_LoadShader("test_vertex.glsl", "test_fragment.glsl", &program);
    
    if (!make_resources()) {
        fprintf(stderr, "Failed to load resources\n");
        return 1;
    }
    
    glutMainLoop();
    return 0;
}

