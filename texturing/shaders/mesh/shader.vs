#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 uvs;
layout(location = 2) in vec3 normal;
layout(location = 0) uniform vec4 pos;
layout(location = 1) uniform vec4 scale;
layout(location = 2) uniform vec3 light;

layout(location = 3)uniform mat4 MVP;

out float vcol;
out vec2 UV;

void main(){
    vec4 verPos = (vec4(vertexPosition,1) * scale + pos);
    gl_Position =  MVP * verPos;

    vec3 L = light - verPos.xyz; //should be light, not vec3(0,5,0)
    L = L/L.length();
    vcol = (length(dot(normal,L))/length(normal)/length(L));

    UV = uvs;
}