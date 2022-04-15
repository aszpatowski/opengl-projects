#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in float rotate;

out vec3 fColor;
vec2 rotate_vec2(vec2 points, float rotate)
{
    float s = sin(radians(rotate));
    float c = cos(radians(rotate));
    return vec2(points.x*c - points.y*s, points.x*s + points.y*c);
}
void main()
{
    fColor = aColor;
    gl_Position = vec4((rotate_vec2(aPos, rotate) + aOffset), 0.0, 1.0);
}