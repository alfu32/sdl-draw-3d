#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal; // Normal might be used for lighting calculations
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix; // Matrix to transform positions to light's view space

out vec2 TexCoord;
out vec4 FragPosLightSpace;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
    FragPosLightSpace = lightSpaceMatrix * model * vec4(aPosition, 1.0); // Transform position to light's view space
}
