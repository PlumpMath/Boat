#version 150

flat in vec3 color;
//flat varying vec3 color;
out vec4 outColor;

void main() {
   outColor = vec4(color, 1);
}