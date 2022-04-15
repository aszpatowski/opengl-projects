#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec4 pos;
layout(location = 1) uniform mat4x4 TSRmatrix;
layout(location = 2) uniform mat4x4 MVP;
layout(location = 3) uniform vec3 color;
out vec4 vpos;
void main(void) {
    vpos = MVP*(TSRmatrix*pos);
    gl_Position = vpos;
}
