#version 330 core
layout (location = 0) in vec3 aPosition;

uniform mat4 lightSpaceMatrix; // Ensure this is declared
uniform mat4 matModel;

void main() {
    gl_Position = lightSpaceMatrix * matModel * vec4(aPosition, 1.0);
}