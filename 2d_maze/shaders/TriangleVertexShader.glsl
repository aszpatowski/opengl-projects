#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) uniform vec2 pos[3];
layout(location = 3) uniform vec3 color;
out vec4 vcolor;
void main(void) {
    vec4 colors[] = vec4[3](vec4(1 - color.x,1 - color.y,color.z,1),
    vec4(1 - color.x,1 - color.y,color.z,1),
    vec4(color.x,color.y,color.z,1));
    vec2 p = pos[gl_VertexID];
    vcolor = colors[gl_VertexID];
    gl_Position = vec4(p, 0.5, 1.0);
}