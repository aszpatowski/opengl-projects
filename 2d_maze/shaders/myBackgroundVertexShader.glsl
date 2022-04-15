#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
out vec4 vcolor;
layout(location = 0) uniform float color;
layout(location = 1) uniform float color2;
layout(location = 2) uniform float scale;

void main(void) {
    vec2 vertices[6] =      vec2[6](vec2(0.9*scale, -0.9*scale),
                                    vec2(-0.9*scale, -0.9*scale),
                                    vec2(0.9*scale, 0.9*scale),

                                    vec2(-0.9*scale, 0.9*scale),
                                    vec2(-0.9*scale, -0.9*scale),
                                    vec2(0.9*scale, 0.9*scale));
    vec4 colors[]    = vec4[6](vec4(color, 0.0, color2, 1.0),
                                vec4(0.0, color2, 0.0, 1.0),
                                vec4(color2, 0.0, color, 1.0),
                                vec4(color, 0.0, color2, 1.0),
                                vec4(0.0, color2, 0.0, 1.0),
                                vec4(color2, 0.0, color, 1.0));

    vcolor      = colors[gl_VertexID];
    gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0);
}