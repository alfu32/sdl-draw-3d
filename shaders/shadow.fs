#version 330 core
out vec4 FragColor;
in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;
uniform vec3 lightColor;
uniform vec3 objectColor;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // Perform shadow calculation, simplified version
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

void main() {
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 color = mix(objectColor, objectColor * lightColor, shadow);
    FragColor = vec4(color, 1.0);
}
