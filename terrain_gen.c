#include "terrain_gen.h"
#include "parameters.h"

#include <stdlib.h>

/*
 * Computes a random radius between min_factor*r and max_factor*r.
 */
static float random_radius(float r, float min_factor, float max_factor);

void sphere(vertex *out,
            float r, int n_elevation, int n_azimuth,
            float min_factor, float max_factor) {
  float delevation = Pi / n_elevation;
  float dazimuth   = 2 * Pi / n_azimuth;

  float elevation = -Pi / 2;
  float azimuth   = 0;

  for (int i = 0; i <= n_elevation; i++) {
    if (i < RandomizationCutoff || i > n_elevation-RandomizationCutoff) {
      for (int j = 0; j < n_azimuth; j++) {
        vertex data = to_cartes(r, elevation, azimuth);
        out[i *n_azimuth + j] = data;
        azimuth += dazimuth;
      }
      elevation += delevation;
    }
    else {
      for (int j = 0; j < n_azimuth; j++) {
        float local_r = r;
        if (rand()%HillRarity == 0)
          local_r = random_radius(r, min_factor, max_factor);

        out[i * n_azimuth + j] = to_cartes(local_r, elevation, azimuth);
        azimuth += dazimuth;
      }
      elevation += delevation;
    }
  }
}

void sphere_indices(GLushort *out,
                    int n_elevation, int n_azimuth) {
  int n = 0;

  for (int i = 0; i < n_elevation; i++) {
    for (int j = 0; j < n_azimuth; j++) {
      int prev_i  = i-1;
      int next_i  = i+1;
      int next_i2 = next_i + 1;

      int prev_j  = j == 0 ? n_azimuth - 1 : j - 1;
      int next_j  = j+1 == n_azimuth ? 0 : j + 1;
      int next_j2 = next_j+1 == n_azimuth ? 0 : next_j + 1;

      if (i == 0) {
        out[n++] = next_i*n_azimuth+((prev_j+n_azimuth/2)%n_azimuth);
        out[n++] = next_i*n_azimuth+((j+n_azimuth/2)%n_azimuth);
        out[n++] = next_i*n_azimuth+((next_j+n_azimuth/2)%n_azimuth);
        out[n++] = next_i*n_azimuth+((next_j2+n_azimuth/2)%n_azimuth);
      }
      else {
        out[n++] = prev_i*n_azimuth+prev_j;
        out[n++] = prev_i*n_azimuth+j;
        out[n++] = prev_i*n_azimuth+next_j;
        out[n++] = prev_i*n_azimuth+next_j2;
      }

      out[n++] = i*n_azimuth+prev_j;
      out[n++] = i*n_azimuth+j;
      out[n++] = i*n_azimuth+next_j;
      out[n++] = i*n_azimuth+next_j2;

      out[n++] = next_i*n_azimuth+prev_j;
      out[n++] = next_i*n_azimuth+j;
      out[n++] = next_i*n_azimuth+next_j;
      out[n++] = next_i*n_azimuth+next_j2;

      if (i == n_elevation - 1) {
        out[n++] = i*n_azimuth+((prev_j+n_azimuth/2)%n_azimuth);
        out[n++] = i*n_azimuth+((j+n_azimuth/2)%n_azimuth);
        out[n++] = i*n_azimuth+((next_j+n_azimuth/2)%n_azimuth);
        out[n++] = i*n_azimuth+((next_j2+n_azimuth/2)%n_azimuth);
      }
      else {
        out[n++] = next_i2*n_azimuth+prev_j;
        out[n++] = next_i2*n_azimuth+j;
        out[n++] = next_i2*n_azimuth+next_j;
        out[n++] = next_i2*n_azimuth+next_j2;
      }
    }
  }
}

static float random_radius(float r, float min_factor, float max_factor) {
  return (min_factor + (max_factor-min_factor)*randomf())*r;
}
