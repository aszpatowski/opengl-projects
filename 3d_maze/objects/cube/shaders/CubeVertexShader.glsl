#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec4 pos;
layout(location = 1) uniform mat4x4 TSRmatrix;
layout(location = 2) uniform mat4x4 MVP;
layout(location = 3) uniform vec3 color;
out vec4 vcolor;
void main(void) {
    vec4 colors[] = vec4[3](vec4(1 - color.x, color.y, 1 - color.z, 1),
    vec4(color.x, 1 - color.y, color.z, 1),
    vec4(1-0.2*color.x, color.y, 1-0.2*color.z, 1));
    /*vec4 colors[] = vec4[3](vec4(1 - pos.x,pos.y, 1 - pos.z, 1),
                            vec4(1 - pos.x, 1 - pos.y, pos.z, 1),
                            vec4(pos.x, pos.y, pos.z, 1));*/
    vcolor = colors[gl_VertexID%3];
    //vcolor = vec4(1.0,0.0,0.0,1.0);
    gl_Position = MVP*(TSRmatrix*pos);
}
