#include "gl_setup.h"
#include "shaders.h"
#include "parameters.h"

#include <stdlib.h>
#include <time.h>
#include <SDL.h>

void gl_setup(gl_config *conf) {
  srand(time(NULL));

  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(WindowWidth, WindowHeight, 32, SDL_OPENGL);
  SDL_WM_SetCaption("Fun with B-Splines", NULL);
  glewInit();

  glEnable(GL_DEPTH_TEST);

  create_program(&conf->prog);

  glGenVertexArrays(1, &conf->vao);
  glBindVertexArray(conf->vao);

  glGenBuffers(1, &conf->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, conf->vbo);

  glGenBuffers(1, &conf->ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, conf->ibo);

  enable_vertex();
}

void gl_teardown(gl_config *conf) {
  glDeleteBuffers(1, &conf->ibo);
  glDeleteBuffers(1, &conf->vbo);
  glDeleteVertexArrays(1, &conf->vao);
  delete_program(&conf->prog);
  SDL_Quit();
}
