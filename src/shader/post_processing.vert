#version 460 core
layout(location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform bool chaos;
uniform bool shake;
uniform bool confuse;
uniform float time;

void main(){
    gl_Position = vec4(vertex.xy,0.0,1.0);
    vec2 texCoords = vertex.zw;

    if(chaos){
        const float strength = 0.3;
        TexCoords = vec2(texCoords.x + sin(time) * strength,texCoords.y + cos(time) * strength);
    }
    else if(confuse){
        TexCoords = vec2((1.0 - texCoords.x),(1.0 - texCoords.y));
    }
    else{
        TexCoords = texCoords;
    }
    if(shake){
        float strength = 0.02;
        gl_Position.x += cos(time * 80.0) * strength;
        gl_Position.y += sin(time * 95.0) * strength;
    }
}
