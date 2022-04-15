#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 colorData;
layout (location = 2) in vec3 instanceOffset;
layout (location = 3) in float scale;

uniform mat4 MVP;

out vec3 normalVector;
out vec3 fragmentColor;
out vec3 fragmentPosition;

void main()
{
    normalVector = normalize(vertexPosition- instanceOffset);
    fragmentPosition = vertexPosition*scale + instanceOffset;
    fragmentColor = colorData;
    gl_Position = MVP * (vec4(vertexPosition,1) * scale + vec4(instanceOffset,1));
}