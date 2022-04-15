#version 430 core

out vec4 FragColor;

uniform sampler2D myTextureSampler;
in vec2 UV;
uniform int background;

void main()
{
    if (background == 0) {
        float ht = texture( myTextureSampler, UV ).r*255.0*255.0f;

        vec3 color = vec3(1.0, 0.0, 1.0);

        if      (ht < 0  )   color = vec3(0.,       0.,        1.); //blue
        else if (ht < 500)   color = vec3(0.,       ht/500.0,    0.); //->green ht/500
        else if (ht < 1000)  color = vec3(ht/500-1, 1.,        0.); //->yellow
        else if (ht < 2000)  color = vec3(1.,       2.-ht/1000,0.); //->red
        else                 color = vec3(1.,       ht/2000-1 ,ht/2000-1);  //->white

        FragColor = vec4(color, 1.0);
    } else {
        FragColor = vec4(0.0, 1.0, 0.0, 0.7);
    }
}