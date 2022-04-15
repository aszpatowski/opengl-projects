#version 430

layout(quads) in;
in vec3 tcPosition[];

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 MVP;


uniform sampler2D myTextureSampler;
uniform int background;
uniform int cartesian;
uniform int preteselation;
uniform vec3 ChunkPosition;

out vec2 UV;
out vec3 tePosition;
out vec2 tePatchDistance;
#define pi 3.14159265

void main()
{
    float hoff = 0.003;
    UV = ((1.0/preteselation) * (gl_TessCoord + vec3(gl_PrimitiveID/preteselation, gl_PrimitiveID%preteselation, 0.0)).xy);// + vec2(0.001, 0.0);

    float ht = 1.0;
    if (background == 0) {
        if (cartesian == 1) {
            ht = texture(myTextureSampler, UV*(1-2*hoff) + vec2(hoff)).r * 10.0 + 1.0;
        } else {
            ht = texture(myTextureSampler, UV*(1-2*hoff) + vec2(hoff)).r * 0.005 + 1.0;
        }
    }

    vec3 cart_pos = ChunkPosition + (1.0/preteselation) * (gl_TessCoord + vec3(gl_PrimitiveID/preteselation, gl_PrimitiveID%preteselation, ht));


    vec3 cart_norm = vec3(cart_pos[0], cart_pos[1], cart_pos[2]);
    if (background != 0) {
        cart_norm = vec3((cart_pos[0] * 360.0) - 360.0, (cart_pos[1] * 180.0) - 90.0, cart_pos[2] + 0.5);
    }

    if (cartesian == 1) {
        gl_Position = MVP * vec4(cart_norm, 1.0);
    } else {
        float earthradus = 5.0 * ht;

        if (background != 0) {
            earthradus = 4.98;
        }

        float hradus = earthradus*cos(pi/180.0 * cart_norm[1]);
        vec3 polar_pos = vec3(hradus * cos(pi/180.0 * cart_norm[0]), earthradus*sin(pi/180.0 * cart_norm[1]), hradus * sin(pi/180.0 * cart_norm[0]));

        gl_Position = MVP * vec4(polar_pos, 1.0);
    }
}