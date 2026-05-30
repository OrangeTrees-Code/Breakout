#version 460 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;
uniform vec2 offset[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool chaos;
uniform bool shake;
uniform bool confuse;

void main(){
    FragColor = vec4(0.0,0.0,0.0,0.0);
    vec3 samples[9];
    if(chaos || shake){
        for(int i = 0;i < 9;++i){
            samples[i] = texture(scene,TexCoords.st + offset[i]).rgb;
        }
    }
    if(chaos){
        for(int i = 0;i < 9;++i){
            FragColor += vec4(samples[i] * edge_kernel[i],0.0);
        }
        FragColor.a = 1.0;
    }
    else if(confuse){
        FragColor = vec4(1.0 - texture(scene,TexCoords).rgb,1.0);
    }
    else if(shake){
        for(int i = 0;i < 9;++i){
            FragColor += vec4(samples[i] * blur_kernel[i],0.0);
        }
        FragColor.a = 1.0;
    }
    else{
        FragColor = texture(scene,TexCoords);
    }
}