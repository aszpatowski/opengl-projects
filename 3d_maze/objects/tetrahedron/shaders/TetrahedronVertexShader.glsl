#version 330
layout(location = 0) in vec4 pos;
layout(location = 1) in mat4 MVP;
out vec4 vcolor;
void main(void) {
    vec4 colors[] = vec4[3](vec4(1 - pos.x,pos.y, 1 - pos.z, 1),
                            vec4(pos.x, 1 - pos.y, pos.z, 1),
                            vec4(pos.x, pos.y, pos.z, 1));
    vcolor = colors[gl_VertexID%3];
    gl_Position = MVP*pos;
}
