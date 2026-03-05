#version 330

in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTexture1;

void main(void) {
    FragColor = texture(uTexture1, vUV);
}
