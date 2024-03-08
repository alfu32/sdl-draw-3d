#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal; // Assuming you have normals
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 FragPosLightSpace;

void main() {
    gl_Position = proj * view * model * vec4(aPosition, 1.0);
    // Transform position to light space
    FragPosLightSpace = lightSpaceMatrix * model * vec4(aPosition, 1.0);
}
