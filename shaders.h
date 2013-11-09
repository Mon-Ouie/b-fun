#ifndef _SHADERS_H_
#define _SHADERS_H_

#include <GL/glew.h>
#include "math_utils.h"

/*
 * Utility functions that allow the creation of a shader program that
 *   1) Tesslates a surface based on 16 control points.
 *   2) Evaluates a spline surface on the tesselated points.
 *   3) Computes normals for the created geometry.
 *   4) Applies lighting effect.
 *
 * The following uniforms are used in the process:
 *   - a 4*4 matrix representing transformations applied to the scene.
 *   - a 4*4 matricial representation of the spline. The ith column will be
 *     multiplied by the parameter u^(3-i).
 *
 * Rendering using this program should be done with 16-vertex patches. Each
 * vertex is made of a single attribute bound to location 0: its position, as a
 * vector of three floats (x, y, z).
 *
 * Creating the shader program will also set GL_PATCH_VERTICES to 16 and enable
 * its use automatically.
 */

typedef struct {
  GLuint vs;
  GLuint tcs;
  GLuint tes;
  GLuint gs;
  GLuint fs;
  GLuint prog;

  struct {
    GLint mvp;
    GLint spline;
    GLint transpose_spline;
  } uniforms;
} bspline_program;

void create_program(bspline_program *prog);
void delete_program(bspline_program *prog);

/*
 * Both functions assume prog is the currently bound program.
 */
void send_mvp(bspline_program *prog, mat4 matrix);
void send_spline(bspline_program *prog, mat4 matrix);

#endif
