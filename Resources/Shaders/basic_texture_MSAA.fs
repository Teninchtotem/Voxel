#version 330

uniform sampler2D texture;

in vec2 texCoord;

layout (location=0) out vec4 fragColour;

void main(void) {

    vec3 texColor = texture2D(texture, texCoord).rgb;
    vec4 col;
    col.r = texColor.r;
    col.g = texColor.g;
    col.b = texColor.b;
    col.a = 1;
    fragColour = col;;
}
