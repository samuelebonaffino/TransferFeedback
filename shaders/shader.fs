#version 450 core

in vec3 vertCol;    // Color value coming from vertex shader (smoothed)
out vec4 FragColor; // Color that will be used for fragment shader output

void main(void)
{
    FragColor = vec4(vertCol, 1.0); // Add alpha channel
}