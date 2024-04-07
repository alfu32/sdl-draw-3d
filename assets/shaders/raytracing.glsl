#version 330

in vec2 fragCoord;

uniform vec2 resolution;
uniform vec3 lightDirection;
uniform int numCubes;
uniform vec3 cubePositions[1000];
uniform vec3 cubeColors[1000];

const float cubeSize = 1.0;

bool intersectCube(vec3 origin, vec3 direction, vec3 cubePos, out float t, out vec3 normal) {
    vec3 minCorner = cubePos - vec3(cubeSize * 0.5);
    vec3 maxCorner = cubePos + vec3(cubeSize * 0.5);

    vec3 invDir = 1.0 / direction;
    vec3 tMin = (minCorner - origin) * invDir;
    vec3 tMax = (maxCorner - origin) * invDir;

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);

    if (tNear > tFar || tFar < 0.0) {
        return false;
    }

    vec3 intersectionPoint = origin + direction * tNear;

    if (abs(intersectionPoint.x - minCorner.x) < 0.0001) {
        normal = vec3(-1.0, 0.0, 0.0); // Left face
    } else if (abs(intersectionPoint.x - maxCorner.x) < 0.0001) {
        normal = vec3(1.0, 0.0, 0.0); // Right face
    } else if (abs(intersectionPoint.y - minCorner.y) < 0.0001) {
        normal = vec3(0.0, -1.0, 0.0); // Bottom face
    } else if (abs(intersectionPoint.y - maxCorner.y) < 0.0001) {
        normal = vec3(0.0, 1.0, 0.0); // Top face
    } else if (abs(intersectionPoint.z - minCorner.z) < 0.0001) {
        normal = vec3(0.0, 0.0, -1.0); // Back face
    } else if (abs(intersectionPoint.z - maxCorner.z) < 0.0001) {
        normal = vec3(0.0, 0.0, 1.0); // Front face
    }

    t = tNear;
    return true;
}

bool intersectPlane(vec3 origin, vec3 direction, vec3 planePoint, vec3 planeNormal, out float t) {
    float denom = dot(planeNormal, direction);
    if (abs(denom) < 0.0001) {
        return false; // Ray is parallel to the plane
    }
    
    t = dot(planePoint - origin, planeNormal) / denom;
    
    return t >= 0.0;
}

bool inShadow(vec3 origin, vec3 direction) {
    for (int i = 0; i < numCubes; i++) {
        float t;
        vec3 normal;
        if (intersectCube(origin, direction, cubePositions[i], t, normal)) {
            return true; // Point is in shadow
        }
    }
    return false; // Point is not in shadow
}

float calculateLighting(vec3 normal, vec3 lightDirection) {
    float intensity = dot(normalize(normal), normalize(lightDirection));
    
    if (intensity <= 0.0) {
        return 0.0;
    }
    
    return intensity;
}

vec3 trace(vec3 origin, vec3 direction) {
    vec3 color = vec3(0.0);

    vec3 groundNormal = vec3(0.0, 1.0, 0.0);
    float tGround;
    bool groundHit = intersectPlane(origin, direction, vec3(0.0), groundNormal, tGround);

    if (groundHit) {
        vec3 groundPoint = origin + direction * tGround;
        bool shadowed = inShadow(groundPoint, lightDirection);
        color = shadowed ? vec3(0.0) : vec3(0.5);
    }

    for (int i = 0; i < numCubes; i++) {
        float t;
        vec3 normal;
        if (intersectCube(origin, direction, cubePositions[i], t, normal)) {
            float lighting = calculateLighting(normal, lightDirection);
            color += mix(color, cubeColors[i], lighting);
        }
    }

    return color;
}

void main() {
    vec2 uv = (fragCoord / resolution) * 2.0 - 1.0;
    vec3 rayDir = normalize(vec3(uv, -1.0));

    vec3 color = trace(vec3(0.0), rayDir);

    gl_FragColor = vec4(color, 1.0);
}
