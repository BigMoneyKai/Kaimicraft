#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main(void) {
    FragPos = vec3(m * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(m))) * aNormal;
    TexCoord = aUV;
    
    gl_Position = p * v * vec4(FragPos, 1.0);
}
