#version 450 core

layout (location = 0) in vec2 vertPos;  // Position in attribute location 0
layout (location = 1) in vec2 vertVel;  // Velocity in attribute location 1
layout (location = 2) in int  vertexID; // Vertex ID (particle) or -1 (mouse) in attribute location 2

// Uniform inputs
uniform int  runSim;            // == 1 (run sim), == 0 (otherwise)
uniform vec2 mousePos;          // Mouse's position
uniform int  particlePointSize; // Particle's size

// Feedback transform outputs
out float posX, posY;   // Updated particle position
out float velX, velY;   // Updated particle velocity
out int vertID;         // Vertex ID - passed through

// Render output
out vec3 vertCol;   // Output color to the fragment shader

float gravity = 0.02;
float deltaT = 1.0/60.0;

vec2 clampPos(vec2 pos);
void calcMotion();

void main(void)
{
    float posZ;

    if(vertexID == -1)
    {
        vertCol = vec3(1.0, 1.0, 1.0);  // Mouse dot is white
        posZ = -1.0;                    // Mouse dot positioned in front
        gl_PointSize = 10;              // Mouse dot is larger
        posX = mousePos.x;   
        posY = mousePos.y;   
    }
    else
    {
        calcMotion();
        vertCol = vec3(posX+0.5, posY+0.5, 1.0);    // Particle dot is red
        posZ = 0.0;
        gl_PointSize = particlePointSize;                             // Particle dot behind mouse dot
    }
    vertID = vertexID;
    gl_Position = vec4(posX, posY, posZ, 1.0);
}

vec2 clampPos(vec2 pos)
{
    if(pos.x >  1.0)  pos.x -= 2.0;
    if(pos.x < -1.0)  pos.x += 2.0;
    if(pos.y >  1.0)  pos.x -= 2.0;
    if(pos.y < -1.0)  pos.x += 2.0;
    return pos;
}
void calcMotion()
{
    // vec2 displace = vec2(0.5, 0.5) - vertPos;
    vec2 displace = mousePos - vertPos;
    float distSq = max(displace.x * displace.x + displace.y * displace.y, 0.01);
    float dist = sqrt(distSq);
    vec2 accel = gravity * displace / (distSq * dist);
    vec2 newVel = vertVel + deltaT * accel;
    vec2 newPos = vertPos + deltaT * newVel;
    newPos = clampPos(newPos);
    posX = newPos.x;
    posY = newPos.y;
    velX = newVel.x;
    velY = newVel.y;
}