//
//  main.cpp
//  mandelbrot
//
//  Created by Keichi Takahashi on 12/03/09.
//  Copyright (c) 2012年 Osaka University. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <GLUT/glut.h>

#include "shader_utils.h"

static GLuint program;
static GLuint buffer;
static int levelValue = 80;

float vertex[] = {
    -1.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    -1.0, -1.0, 0.0,
    1.0, -1.0, 0.0,
};

GLuint vtxindex[] = {
    0,1,2,3
};

float start_color_const[] = { 1.0, 1.0, 1.0, 1.0 };
float end_color_const[] = { 0.2, 0.2, 0.7, 1.0 };
float center_point_const[] = {-0.5, 0.0, 0.0, 0.0};
int zoom_value = 7;
static int is_dragging = 0;
static int drag_start_x;
static int drag_start_y;

void screen_to_real(int x, int y, float* fx, float* fy);
void onKey(int key, int x, int y);
void onClick(int button, int state, int x, int y);
void onMove(int x, int y);
void force_redraw(int value);
int make_resources(void);
void render(void);

int make_resources()
{
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    
    return 1;
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(program);
    GLint level = glGetUniformLocation(program, "level");
    glUniform1i(level, levelValue);
    GLint start_color = glGetUniformLocation(program, "start_color");
    glUniform4fv(start_color, 1, start_color_const);
    GLint end_color = glGetUniformLocation(program, "end_color");
    glUniform4fv(end_color, 1, end_color_const);
    GLint center = glGetUniformLocation(program, "center");
    glUniform4fv(center, 1, center_point_const);
    GLint zoom = glGetUniformLocation(program, "zoom");
    glUniform1f(zoom, exp(zoom_value * log(1.1)));
    GLint aspect_ratio = glGetUniformLocation(program, "aspect_ratio");
    glUniform1f(aspect_ratio, 1200.0 / 900.0);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);    
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, static_cast<void*>(vtxindex));
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glutSwapBuffers();
}

void screen_to_real(int x, int y, float* fx, float* fy)
{
    *fx = exp(zoom_value * log(1.1)) * 2.0 / 1200 * x + center_point_const[0] - exp(zoom_value * log(1.1));
    *fy = exp(zoom_value * log(1.1)) * 2.0 / 900 * (900 - y) + center_point_const[1] - exp(zoom_value * log(1.1));
}

void onKey(int key, int x, int y)
{
    float fx, fy;
    screen_to_real(x, y, &fx, &fy);
    switch (key) {
        case GLUT_KEY_UP:
            center_point_const[0] = fx;
            center_point_const[1] = fy;
            zoom_value--;
            break;
        case GLUT_KEY_DOWN:
            center_point_const[0] = fx;
            center_point_const[1] = fy; 
            zoom_value++;

            break;
        default:
            break;
    }
}

void onClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            is_dragging = 1;
            drag_start_x = x;
            drag_start_y = y;
        } else  if (state == GLUT_UP) {
            is_dragging = 0;
        }
    }
}

void onMove(int x, int y)
{
    if (is_dragging) {
        float fx = (x - drag_start_x) / 600.0 * exp(zoom_value * log(1.1));
        float fy = (y - drag_start_y) / 450.0 * exp(zoom_value * log(1.1));
        
        drag_start_x = x;
        drag_start_y = y;
        
        center_point_const[0] += -fx;
        center_point_const[1] += fy;
    }
}

void force_redraw(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, force_redraw, 0);
}

int main (int argc, const char * argv[])
{
    glutInit(&argc, const_cast<char**>(argv));
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1200, 900);
    glutCreateWindow("Mandelbrot Renderer");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    glutDisplayFunc(render);
    glutTimerFunc(20, force_redraw, 0);
    glutSpecialFunc(onKey);
    glutMouseFunc(onClick);
    glutMotionFunc(onMove);
    
    load_shader("mandelbrot.vert", "mandelbrot.frag", &program);
    
    if (!make_resources()) {
        std::cout << "Failed to prepare resources" << std::endl;
        return 1;
    }
    
    glutMainLoop();
    return 0;
}

