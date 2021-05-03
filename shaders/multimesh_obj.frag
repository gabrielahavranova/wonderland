#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float color_intensity;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords) * color_intensity ;
}