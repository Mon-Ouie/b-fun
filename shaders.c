#include "shaders.h"
#include "misc.h"
#include "parameters.h"

#include <string.h>
#include <stdio.h>

/*
 * Applies transformation.
 *
 * This can be done here because B-Spline are invariant under affine
 * transformations.
 */
static const char *vertex_shader =
  "#version 400\n"
  "\n"
  "in vec3 position;\n"
  "out vec3 vector_position;\n"
  "uniform mat4 mvp;\n"
  "\n"
  "void main() {\n"
  "  vector_position = (mvp * vec4(position, 1)).xyz;\n"
  "}\n";

/*
 * Defines how each patch should be tessellated.
 */
static const char *tess_control_shader =
  "#version 400\n"
  "\n"
  "layout(vertices = 16) out;\n"
  "\n"
  "in vec3 vector_position[];\n"
  "out vec3 tc_position[];\n"
  "const float level = 16;\n"
  "\n"
  "void main() {\n"
  "  #define id gl_InvocationID\n"
  "  tc_position[id] = vector_position[id];"
  "  if (id == 0) {\n"
  "     gl_TessLevelInner[0] = level;\n"
  "     gl_TessLevelInner[1] = level;\n"
  "     gl_TessLevelOuter[0] = level;\n"
  "     gl_TessLevelOuter[1] = level;\n"
  "     gl_TessLevelOuter[2] = level;\n"
  "     gl_TessLevelOuter[3] = level;\n"
  "  }\n"
  "}\n";

/*
 * Evaluates B-splines along the tesselated surface.
 */
static const char *tess_eval_shader =
  "#version 400\n"
  "\n"
  "layout(quads) in;\n"
  "\n"
  "in vec3 tc_position[];\n"
  "out vec3 te_position;\n"
  "\n"
  "uniform mat4 B;\n"
  "uniform mat4 Bt;\n"
  "\n"
  "void main() {\n"
  "  float u = gl_TessCoord.x;\n"
  "  float v = gl_TessCoord.y;\n"
  "\n"
  "  mat4 px, py, pz;\n"
  "  for (int i = 0; i < 16; i++) {\n"
  "    px[i/4][i%4] = tc_position[i].x;\n"
  "    py[i/4][i%4] = tc_position[i].y;\n"
  "    pz[i/4][i%4] = tc_position[i].z;\n"
  "  }\n"
  "\n"
  "  mat4 cx = B * px * Bt;\n"
  "  mat4 cy = B * py * Bt;\n"
  "  mat4 cz = B * pz * Bt;\n"
  "\n"
  "  vec4 pu = vec4(u*u*u, u*u, u, 1);\n"
  "  vec4 pv = vec4(v*v*v, v*v, v, 1);\n"
  "\n"
  "  float x = dot(cx*pv, pu);\n"
  "  float y = dot(cy*pv, pu);\n"
  "  float z = dot(cz*pv, pu);\n"
  "\n"
  "  te_position = vec3(x,y,z);\n"
  "  gl_Position = vec4(te_position, 1);\n"
  "}\n";

/*
 * A geometry shader that computes normal vertices without changing the
 * tesselated data.
 */
static const char *geometry_shader =
  "#version 400\n"
  "\n"
  "layout(triangles) in;\n"
  "layout(triangle_strip, max_vertices=3) out;\n"
  "\n"
  "in vec3 te_position[3];\n"
  "out vec3 normal;\n"
  "\n"
  "void main() {\n"
  "  vec3 a = te_position[2] - te_position[0];\n"
  "  vec3 b = te_position[1] - te_position[0];\n"
  "  normal = normalize(cross(a,b));\n"
  "\n"
  "  for (int i = 0; i < 3; i++) {\n"
  "    gl_Position = gl_in[i].gl_Position;\n"
  "    EmitVertex();\n"
  "  }\n"
  "  EndPrimitive();\n"
  "}\n";

/*
 * Applies lighgting effects on the resulting geometry.
 * (Unlit spheres tend to look like circles...)
 */
static const char *fragment_shader =
  "#version 400\n"
  "\n"
  "in vec3 normal;\n"
  "\n"
  "const vec3 light = normalize(vec3 " Stringify(LightPosition) ");\n"
  "const vec3 eye = vec3(0, 0, 1);"
  "\n"
  "const vec3 ambient  = vec3" Stringify(AmbientColor)  ";\n"
  "const vec3 diffuse  = vec3" Stringify(DiffuseColor)  ";\n"
  "const vec3 specular = vec3" Stringify(SpecularColor) ";\n"
  "\n"
  "const float shininess = " Stringify(Shininess) ";\n"
  "\n"
  "out vec4 frag_color;\n"
  "\n"
  "void main() {\n"
  "  vec3 n = normalize(normal);\n"
  "  vec3 s = normalize(light + eye);"
  "  frag_color = vec4(ambient + abs(dot(light,n))*diffuse\n"
  "                    + abs(dot(s,n))*shininess*specular, 1);\n"
  "}\n";

/*
 * Compiles a single shader and prints errors if it fails.
 */
static GLuint create_shader(GLenum mode, const char *name,
                            const char *src);

/*
 * Fills in the program's uniform location structure.
 */
static void fetch_uniforms(bspline_program *prog);

static void check_link_errors(GLuint prog);

void create_program(bspline_program *prog) {
  prog->vs = create_shader(GL_VERTEX_SHADER, "vertex shader", vertex_shader);
  prog->tcs = create_shader(GL_TESS_CONTROL_SHADER, "tess control",
                            tess_control_shader);
  prog->tes = create_shader(GL_TESS_EVALUATION_SHADER, "tess eval",
                            tess_eval_shader);
  prog->gs = create_shader(GL_GEOMETRY_SHADER, "geometry shader",
                           geometry_shader);
  prog->fs = create_shader(GL_FRAGMENT_SHADER, "fragment shader",
                           fragment_shader);

  prog->prog = glCreateProgram();
  glAttachShader(prog->prog, prog->vs);
  glAttachShader(prog->prog, prog->tcs);
  glAttachShader(prog->prog, prog->tes);
  glAttachShader(prog->prog, prog->gs);
  glAttachShader(prog->prog, prog->fs);
  glBindFragDataLocation(prog->prog, 0, "frag_color");
  glBindAttribLocation(prog->prog, 0, "position");
  glLinkProgram(prog->prog);

  check_link_errors(prog->prog);
  glUseProgram(prog->prog);

  glPatchParameteri(GL_PATCH_VERTICES, 16);

  fetch_uniforms(prog);
}

void delete_program(bspline_program *prog) {
  glDeleteShader(prog->vs);
  glDeleteShader(prog->tcs);
  glDeleteShader(prog->tes);
  glDeleteShader(prog->gs);
  glDeleteShader(prog->fs);

  glDeleteProgram(prog->prog);
}

void send_mvp(bspline_program *prog, mat4 matrix) {
  glUniformMatrix4fv(prog->uniforms.mvp, 1, GL_TRUE, matrix.data);
}

void send_spline(bspline_program *prog, mat4 matrix) {
  glUniformMatrix4fv(prog->uniforms.spline, 1, GL_TRUE, matrix.data);
  glUniformMatrix4fv(prog->uniforms.transpose_spline, 1, GL_FALSE, matrix.data);
}

static GLuint create_shader(GLenum mode, const char *name,
                            const char *src) {
  GLuint ret = glCreateShader(mode);
  GLint length = strlen(src);
  glShaderSource(ret, 1, &src, &length);

  int status;
  char error[256];
  glGetShaderiv(ret, GL_COMPILE_STATUS, &status);
  glGetShaderInfoLog(ret, sizeof(error), 0, error);
  if (status)
    fprintf(stderr, "%s: compilation error: %s\n", name, error);

  glCompileShader(ret);
  return ret;
}

static void fetch_uniforms(bspline_program *prog) {
  prog->uniforms.mvp = glGetUniformLocation(prog->prog, "mvp");
  prog->uniforms.spline = glGetUniformLocation(prog->prog, "B");
  prog->uniforms.transpose_spline = glGetUniformLocation(prog->prog, "Bt");
}

static void check_link_errors(GLuint prog) {
  int status;
  char error[256];
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  glGetProgramInfoLog(prog, sizeof(error), 0, error);
  if (!status)
    fprintf(stderr, "Linking error: %s\n", error);
}
