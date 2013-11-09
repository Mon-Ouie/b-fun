#include "math_utils.h"
#include <math.h>
#include <stdlib.h>

void enable_vertex() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void*)offsetof(vertex, x));
  glEnableVertexAttribArray(0);
}

vertex to_cartes(float r, float elevation, float azimuth) {
  GLfloat x = r * cosf(elevation) * cosf(azimuth);
  GLfloat y = r * cosf(elevation) * sinf(azimuth);
  GLfloat z = r * sinf(elevation);

  return (vertex){x, y, z};
}

float randomf() {
  return (float)rand()/(float)RAND_MAX;
}

void vec3_normalize(float *x, float *y, float *z) {
  float norm = sqrtf((*x) * (*x) + (*y) * (*y) + (*z) * (*z));

  *x = *x / norm;
  *y = *y / norm;
  *z = *z / norm;
}

mat4 rotation(float angle, float x, float y, float z) {
  vec3_normalize(&x, &y, &z);

  float c = cosf(angle);
  float s = sinf(angle);

  mat4 matrix = IdentityMatrix;

  mat4_at(matrix, 0, 0) = x * x * (1 - c) + c;
  mat4_at(matrix, 1, 0) = x * y * (1 - c) - z * s;
  mat4_at(matrix, 2, 0) = x * z * (1 - c) + y * s;

  mat4_at(matrix, 0, 1) = y * x * (1 - c) + z * s;
  mat4_at(matrix, 1, 1) = y * y * (1 - c) + c;
  mat4_at(matrix, 2, 1) = y * z * (1 - c) - x * s;

  mat4_at(matrix, 0, 2) = z * x * (1 - c) - y * s;
  mat4_at(matrix, 1, 2) = z * y * (1 - c) + x * s;
  mat4_at(matrix, 2, 2) = z * z * (1 - c) + c;

  return matrix;
}
