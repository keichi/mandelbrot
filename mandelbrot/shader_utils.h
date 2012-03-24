//
//  shader_utils.h
//  mandelbrot
//
//  Created by Keichi Takahashi on 12/03/17.
//  Copyright (c) 2012年 Osaka University. All rights reserved.
//

#ifndef mandelbrot_shader_utils_h
#define mandelbrot_shader_utils_h

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef _WIN32 || _WIN64
#include <gl/glew.h>
#include <gl/glut.h>
#endif

#define MAX_SHADER_LOG_SIZE		(1024)

int load_shader(char *vtxShdName, char *frgShdName, GLuint *lpProg);
int compile_shader(GLuint shader, char *file);
void print_shader_log(GLuint shader);
void print_program_log(GLuint program);

#endif
