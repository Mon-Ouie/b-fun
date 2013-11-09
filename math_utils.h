#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

#include <GL/glew.h>

#define Pi 3.141592653589793

typedef struct {
  GLfloat x, y, z;
} vertex;

/*
 * Configures the current VAO to use vertices that are stored in an array of
 * vertex objects.
 */
void enable_vertex();

/*
 * Converts coordinates from spherical to cartesian coordinates.
 *
 * The range for elevation is from -pi/2 to pi/2.
 */
vertex to_cartes(float r, float elevation, float azimuth);

/*
 * A random floating point number between 0 and 1.
 */
float randomf();

typedef struct {
  GLfloat data[16];
} mat4;

#define IdentityMatrix                          \
  ((mat4){                                      \
    {1, 0, 0, 0,                                \
     0, 1, 0, 0,                                \
     0, 0, 1, 0,                                \
     0, 0, 0, 1}                                \
  })

#define BSplineMatrix                           \
  ((mat4){                                      \
    {-1/6.0, 1/2.0, -1/2.0, 1/6.0,              \
     1/2.0, -1, 1/2.0, 0,                       \
     -1/2.0, 0, 1/2.0, 0,                       \
     1/6.0, 2/3.0, 1/6.0, 0}                    \
  })

#define mat4_at(m, x, y) ((m).data[(y)*4+(x)])

/*
 * In-place normalization of a vector.
 */
void vec3_normalize(float *x, float *y, float *z);

/*
 * Represents a rotation of a certain angle around the axis (x, y, z).
 */
mat4 rotation(float angle, float x, float y, float z);

#endif
