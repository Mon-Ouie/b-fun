#ifndef _GL_SETUP_H_
#define _GL_SETUP_H_

#include <GL/glew.h>
#include "shaders.h"

/*
 * Utilities to start a window with an OpenGL context and an empty VBO and IBO.
 */

typedef struct {
  GLuint vao;
  GLuint vbo;
  GLuint ibo;

  bspline_program prog;
} gl_config;

void gl_setup(gl_config *conf);
void gl_teardown(gl_config *conf);

#endif
