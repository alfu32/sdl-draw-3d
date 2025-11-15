#version 330

out vec4 FragColor;

in vec3 FragPos; 
in vec3 Normal; 
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

vec3 calculateShadow() {
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    float currentDepth = projCoords.z;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    
    float shadow = currentDepth > closestDepth ? 0.5 : 1.0; // adjust bias here if needed
    
    return vec3(shadow);
}

void main() {           
    vec3 lighting = calculateShadow();
    
    gl_FragColor = texture(diffuseTexture, TexCoords) * vec4(lighting, 1.0);
}
