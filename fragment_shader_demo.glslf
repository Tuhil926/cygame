#version 410 core
in vec3 theColor;
out vec4 color;
void main() {
    // color = vec4(1.0f, 0.5f, 0.0f, 1.0f);
    color = vec4(theColor, 1.0);
}
