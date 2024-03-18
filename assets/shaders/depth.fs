#version 330 core
void main() {
    // gl_FragCoord.z contains the depth value in the light's perspective
    // No need to explicitly write anything to the output for depth map
    // The depth value is automatically stored in the framebuffer's depth attachment
    gl_FragDepth = gl_FragCoord.z;
}
