#version 330
layout(location = 0) in vec4 pos;
layout(location = 1) in mat4 MVP;
out vec4 vcolor;
void main(void) {
    vec4 colors[] = vec4[3](vec4(1.0,0.6, 0, 1),
    vec4(0.6, 1.0, 0.3, 1),
    vec4(1.0, 0.6, 0.6, 1));
    vcolor = colors[gl_VertexID%3];
    gl_Position = MVP*pos;
}
