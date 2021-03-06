#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include "math_utils.h"

/*
 * Various parameters to tweak the result. Enjoy!
 */

/**
 * Geometry Generation
 */

/*
 * Radius of the sphere. Setting this to 1 would make it take up tho whole
 * screen's space.
 */
#define Radius 0.5

/*
 * Increase this number to reduce the elevation spacing between control points.
 */
#define NElevation 10

/*
 * Increase this number to reduce the azimuth spacing between control points.
 */
#define NAzimuth 10

/*
 * When the elevation step is at this distance of a pole or less, control points
 * are placed on the sphere without ever generating hills.
 */
#define RandomizationCutoff 3

/*
 * Increase this number to make hills rarer.
 */
#define HillRarity 5

/*
 * Smallest radius (relative to Radius) that can be generated in hills.
 */
#define MinRadiusFactor 0.5

/*
 * Greatest radius (relative to Radius) that can be generated in hills.
 */
#define MaxRadiusFactor 1.8

/**
 * Approximation pattern
 */

/*
 * You can change this matrix, for example to evaluate hermite spline instead.
 */
#define SplineMatrix BSplineMatrix

/**
 * Window parameters.
 */

/*
 * Width of the window in pixels.
 */
#define WindowWidth 640

/*
 * Height of the window in pixels
 */
#define WindowHeight 640

/**
 * Animation parameters
 */

/*
 * Rotation speed, in radians per frame.
 */
#define AngularSpeed (2*Pi)/120

/*
 * Rotation axis.
 */
#define RotX 1
#define RotY 1
#define RotZ 0

/**
 * Lighting & Colors (XYZ or RGB)
 */

#define LightPosition (1, 0, 1)

#define AmbientColor  (0.03, 0.03, 0.03)
#define DiffuseColor  (0.0, 0.0, 0.6)
#define SpecularColor (0.5, 0.5, 0.5)

#define Shininess 1

#endif
