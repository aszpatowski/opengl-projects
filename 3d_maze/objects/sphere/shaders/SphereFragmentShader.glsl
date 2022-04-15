#version 330 core
in vec3 vpos;
out vec3 color;
void main(){
    color = vec3((vpos.x  + 1) * 0.4, 0,(vpos.y  + 1) * 0.4);
}
