#version 330 core
in vec3 vpos;
out vec4 color;
void main(){
    color = vec4((vpos.x  + 1) * 0.4, 0,(vpos.y  + 1) * 0.4,1.0);
}
