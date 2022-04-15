#version 330
in  vec4 vpos;
out vec4 color;

void main(void) {
    vec3 ocol = vec3(0,0.3,1)*(0.9/length(vpos));
    color = vec4(ocol,1);
}