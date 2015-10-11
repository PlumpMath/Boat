#version 150

flat in vec3 triColor;
out vec4 outColor;

void main() {
   outColor = vec4(triColor, 1);
}