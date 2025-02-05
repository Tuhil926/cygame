#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texture_coord;

uniform mat4 fullTransformMatrix;
uniform mat4 rotationMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 theColor;
out vec3 theNormal;
out vec3 pos;
out vec2 textureCoord;
void main()
{
    vec4 v = vec4(position, 1.0);
    vec4 newPosition = fullTransformMatrix*v;
    gl_Position = newPosition;
    theColor = color;
    theNormal = vec3(rotationMatrix * vec4(normal, 0.0));
    pos = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
    textureCoord = texture_coord;
    // theNormal = vec3(1, 1, 1);
}
