#version 410 core
in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 texture_coord;

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
    pos = vec3(modelToWorldMatrix*vec4(position, 1.0));
    textureCoord = texture_coord;
    // theNormal = vec3(1, 1, 1);
}
