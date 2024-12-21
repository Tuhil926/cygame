#version 410 core
in vec3 theColor;
in vec3 theNormal;
out vec4 color;

uniform vec3 lightDirection;
void main() {
    // color = vec4(1.0f, 0.5f, 0.0f, 1.0f);
    float intensity = dot(lightDirection, theNormal);
    vec4 intensity_vec = clamp(vec4(intensity, intensity, intensity, 1.0), 0.1, 1.0);
    color = vec4(theColor, 1.0) * intensity_vec;
    // color = vec4(theColor, 1.0);
}
