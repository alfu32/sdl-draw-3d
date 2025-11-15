#version 330

in vec3 vertexPos;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec3 FragPos; 
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 lightView;
uniform mat4 lightProjection;

void main() {
    gl_Position = projection * modelview * vec4(vertexPos, 1.0);
    FragPos = vec3(modelview * vec4(vertexPos, 1.0));
    Normal = mat3(transpose(inverse(modelview))) * vertexNormal;
    TexCoords = vertexUV;
    FragPosLightSpace = lightProjection * lightView * vec4(FragPos, 1.0);
}
