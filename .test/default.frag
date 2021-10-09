#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    bool b = bool(texture2D(ourTexture, TexCoord).x);
    FragColor = vec4(float(b));
}
