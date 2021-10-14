#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform usampler2D ourTexture;

void main()
{
    //bool b = bool(texture2D(ourTexture, TexCoord).x);
    uint r = texture(ourTexture, TexCoord).r;
    FragColor = vec4(float(r));//texture2D(ourTexture, TexCoord); //vec4(float(b));
}
