#version 460 core
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 color;
uniform sampler2D image;

void main(){
    FragColor = vec4(color,1.0) * texture(image,TexCoords);
}