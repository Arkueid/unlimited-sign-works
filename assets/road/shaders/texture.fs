#version 430

in vec2 TexCoord;
out vec4 FragColor;
//layout(binding=0) 
uniform sampler2D Tex0;
//layout( location = 0 ) out vec4 FragColor;

void main() 
{
    vec4 c0 = texture(Tex0, TexCoord );
    
    FragColor = c0;
}
