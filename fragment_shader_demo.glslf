#version 410 core
in vec3 theColor;
in vec3 theNormal;
in vec3 pos;
out vec4 color;

uniform vec3 lightDirection;
uniform vec3 cameraLocation;
void main() {
    // color = vec4(1.0f, 0.5f, 0.0f, 1.0f);
    float intensity = dot(lightDirection, theNormal);
    vec4 intensity_vec = clamp(vec4(intensity, intensity, intensity, 1.0), 0.1, 1.0);

    // specular light
    vec3 vertexToEye = normalize(cameraLocation - pos);
    vec3 reflected = normalize(reflect(-lightDirection, theNormal));
    float specFactor = pow(clamp(dot(vertexToEye, reflected), 0.0, 1.0), 10);

    vec4 specColor = vec4(specFactor, specFactor, specFactor, 1.0);

    color = clamp(vec4(theColor, 1.0) * intensity_vec + specColor, 0.0, 1.0);
    // color = clamp(specColor, 0.0, 1.0);
    // color = vec4(theColor, 1.0);
}
