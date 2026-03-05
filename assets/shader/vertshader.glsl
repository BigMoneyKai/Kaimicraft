#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aRGB;
layout (location = 2) in vec2 aUV;

uniform mat4 mv;
uniform mat4 p;

out vec2 vUV;

void main(void) {
    gl_Position = p * mv * vec4(aPos, 1.0);
    vUV = aUV;
}
