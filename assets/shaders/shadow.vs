#version 330 core
layout (location = 0) in vec3 aPosition;
// Add more layout locations as needed (e.g., for texture coordinates or normals)

uniform mat4 matModel;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix; // Matrix to transform positions to the light's view space

void main() {
    gl_Position = projection * view * matModel * vec4(aPosition, 1.0);
    // Pass additional data to the fragment shader as needed, such as texture coordinates or the transformed position for shadow calculation
}
