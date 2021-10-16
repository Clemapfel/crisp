#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //bool b = bool(texture2D(ourTexture, TexCoord).x);
    float value = texture(ourTexture, TexCoord).r;
    FragColor = vec4(value == 1);//texture2D(ourTexture, TexCoord); //vec4(float(b));
}
