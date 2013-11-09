#include "gl_setup.h"
#include "parameters.h"
#include "terrain_gen.h"
#include "math_utils.h"

#include <SDL.h>
#include <stdbool.h>

/* Returns the amount of indices to draw. */
static int create_sphere();

int main(void) {
  gl_config conf;

  gl_setup(&conf);
  send_spline(&conf.prog, SplineMatrix);

  int icount = create_sphere();

  float angle = 0;
  bool rotating = true;

  bool running = true;
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) running = false;
      else if (ev.type == SDL_KEYDOWN) {
        switch (ev.key.keysym.sym) {
        case SDLK_SPACE:
          rotating = !rotating;
          break;
        default: break;
        }
      }
    }

    if (rotating) {
      angle += AngularSpeed;
      if (angle > 2*Pi) angle -= 2*Pi;
    }

    send_mvp(&conf.prog, rotation(angle, RotX, RotY, RotZ));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_PATCHES, icount, GL_UNSIGNED_SHORT, (void*)0);
    SDL_GL_SwapBuffers();
  }

  gl_teardown(&conf);
  return 0;
}

int create_sphere() {
  static const int icount = sphere_icount(NElevation, NAzimuth);

  vertex vertices[sphere_vcount(NElevation, NAzimuth)];
  GLushort indices[sphere_icount(NElevation, NAzimuth)];

  sphere(vertices, Radius, NElevation, NAzimuth,
         MinRadiusFactor, MaxRadiusFactor);
  sphere_indices(indices, NElevation, NAzimuth);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  return icount;
}
