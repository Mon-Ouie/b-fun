#ifndef _TERRAIN_GEN_H_
#define _TERRAIN_GEN_H_

#include "math_utils.h"

/*
 * Functions to generate geometry used in this program.
 *
 * We create a sphere by placing control points with evenly spaced elevation and
 * azimuth. Each of these points has a 1/hill_rarity to be a "hill". That is,
 * its height will be randomly chosen between min_factor*r and max_factor*r.
 *
 * Notice hills are more likely to be closer when getting closer to the two
 * poles. In fact, control points that are very close to the pole can't be
 * hills, lest a smooth-looking transition be made impossible.
 */

/*
 * The amount of distinct vertices (i.e. the size of the VBO needed to contain
 * all of the geometry).
 */
#define sphere_vcount(n_elevation, n_azimuth) \
  (((n_elevation)+1)*(n_azimuth))

/*
 * The amount of distinct indices (i.e. the amount of vertices to be drawn).
 */
#define sphere_icount(n_elevation, n_azimuth) \
  (16*(n_elevation)*(n_azimuth))

void sphere(vertex *out,
            float r, int n_elevation, int n_azimuth,
            float min_factor, float max_factor);
void sphere_indices(GLushort *out,
                    int n_elevation, int n_azimuth);

#endif
