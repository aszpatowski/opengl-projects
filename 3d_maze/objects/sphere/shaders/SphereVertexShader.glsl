#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 0) uniform vec4 pos;
layout(location = 1) uniform vec4 scale;
layout(location = 2)uniform mat4 MVP;
out vec3 vpos;
void main(){
    gl_Position =  MVP * (vec4(vertexPosition_modelspace,1) * scale + pos) ;
    vpos = vertexPosition_modelspace;
}