#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform sampler2D textureDiffuse; // Diffuse texture
uniform sampler2D shadowMap; // Shadow map
uniform vec3 lightColor;
uniform vec3 objectColor;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // Transform to [0,1] range
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth from shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    float shadow = ShadowCalculation(FragPosLightSpace); // Determine shadow
    vec3 textureColor = texture(textureDiffuse, TexCoord).rgb;
    vec3 ambient = 0.3 * textureColor;
    vec3 lighting = (1.0 - shadow) * lightColor * textureColor; // Simple diffuse lighting
    FragColor = vec4(ambient + lighting, 1.0);
}
