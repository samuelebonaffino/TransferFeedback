#include "lib/glad.h"
#include "classes/shader.h"
#include "classes/particle.h"
#include "classes/myGLFW.h"

#include <iostream>
#include <cmath>
#include <vector>   
#include <GLFW/glfw3.h>

#define NP  1000000

int windowW = 1920;
int windowH = 1080;
bool firstTimeRendering = true;

// We make two each of the VAO, VBO and TF objects. 
// This is so we can alternate between them when rendering
unsigned int VAOs[2];  // Vertex Array Object - holds info about an array of vertex data;
unsigned int VBOs[2];  // Vertex Buffer Object - holds an array of data
unsigned int TFs[2];  // Tranform Feedback - container for transform feedback
int swapIndex = 0;

// We create one shader program: it consists of a vertex shader and a fragment shader
const unsigned int vertPos_loc = 0;    // Corresponds to "location = 0" in the verter shader definition
const unsigned int vertVel_loc = 1;    // Corresponds to "location = 1" in the verter shader definition
const unsigned int vertID_loc = 2; // Corresponds to "location = 2" in the verter shader definition

// Locations of uniform variables in the current shader program
unsigned int shader;
unsigned int mousePos_loc;
unsigned int runSim_loc;
unsigned int particlePointSize_loc;

// GL prototypes
void cursor_pos_callback(GLFWwindow* window, double x, double y);
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void setup_callbacks(GLFWwindow* window);
// My prototypes
std::vector<float> createParticleVertices(int numParticles);
void mySetupGeometries();
void mySetupSceneData();
void myRenderScene();

int main(void)
{
    // Init GLFW (Uses OpenGL Core 4.5.0)
    MyGLFW myglfw(4, 5, windowW, windowH, "Particles with TF");

    setup_callbacks(myglfw.getWindow());
    window_size_callback(myglfw.getWindow(), windowW, windowH);

    mySetupGeometries();
    mySetupSceneData();

    // FPS variables
    double previousTime = glfwGetTime();
    int frameCount = 0;

    // Render loop
    while(!glfwWindowShouldClose(myglfw.getWindow()))
    {
        myRenderScene();

        // GLFW: swap buffers and poll IO events (e.g. keys pressed, released, mouse moved, etc.)
        glfwSwapBuffers(myglfw.getWindow());
        glfwPollEvents();
        // Sets framerate
        glfwWaitEventsTimeout(1.0/60.0);

        // FPS
        double currentTime = glfwGetTime();
        frameCount++;
        // If a second has passed.
        if (currentTime - previousTime >= 1.0)
        {
            // Display the frame count here any way you want.
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            previousTime = currentTime;
        }
    }

    // GLFW: terminate, clearing all previously allocated GLFW resources
    myglfw.terminate();
    return 0;
}

// This function is called whenever the mouse position moves
void cursor_pos_callback(GLFWwindow* window, double x, double y)
{

    // Get mouse position, scaled to range [-1,1]x[-1,1]
    float dotX = (2.0f*(float)x / (float)(windowW - 1)) - 1.0f;
    float dotY = 1.0f - (2.0f*(float)y / (float)(windowH - 1));

    glUniform2f(mousePos_loc, dotX, dotY);
}
void window_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);		// Draw into entire window
    windowW = width;
    windowH = height;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	if (action == GLFW_RELEASE) {
		return;			// Ignore key up (key release) events
	}
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_X) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    else
        return;
    glUniform1i(runSim_loc, 0);
}
void setup_callbacks(GLFWwindow* window) 
{
	// Set callback function for resizing the window
	glfwSetFramebufferSizeCallback(window, window_size_callback);

	// Set callback for key up/down/repeat events
	glfwSetKeyCallback(window, key_callback);

	// Set callbacks for mouse movement (cursor position) and mouse botton up/down events.
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	// glfwSetMouseButtonCallback(window, mouse_button_callback);
}
std::vector<float> createParticleVertices(int numParticles)
{
    // Ogni particella ha 3 variabili
    std::vector<float> pVertices(numParticles * 5);

    for(uint i = 0; i < numParticles * 5; i++)
    {
        Particle p(i);
        pVertices[i++] = p.pos[0];
        pVertices[i++] = p.pos[1];
        pVertices[i++] = p.vel[0];
        pVertices[i++] = p.vel[1];
        pVertices[i++] = p.ID;
    }
    return pVertices;
}
void mySetupGeometries()
{
    // Create VAOs, VBOs and TFs
    std::vector<float> pVertices = createParticleVertices(NP * 5);

    glGenVertexArrays(2, &VAOs[0]);
    glGenBuffers(2, &VBOs[0]);
    glGenTransformFeedbacks(2, &TFs[0]);

    for(int i = 0; i < 2; i++)
    {
        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        // Load data into VBOs[0], just allocate buffer size for VBOs[1]
        // glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices[0]) * pVertices.size(), i==0 ? pVertices.data() : (void*)0, GL_DYNAMIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices[0]) * pVertices.size(), i==0 ? pVertices.data() : (void*)0, GL_DYNAMIC_DRAW);

        // Position
        glVertexAttribPointer(vertPos_loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Vel
        glVertexAttribPointer(vertVel_loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // ID
        glVertexAttribPointer(vertID_loc, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Let the two transform feedback buffers point to the VBO buffers
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFs[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBOs[0]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFs[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBOs[1]);

    // Just for safety, unbind all the buffers
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void mySetupSceneData()
{
    const char* feedbackValues[5] = {"posX", "posY", "velX", "velY", "vertID"};
    Shader s("shaders/shader.vs", "shaders/shader.fs", feedbackValues);
    shader = s.ID;

    runSim_loc = glGetUniformLocation(shader, "runSim");
    mousePos_loc = glGetUniformLocation(shader, "mousePos");
    particlePointSize_loc = glGetUniformLocation(shader, "particlePointSize");

    glUseProgram(shader);
    glUniform1i(runSim_loc, 0);
    glUniform2f(mousePos_loc, 0.0f, 0.0f);
    glUniform1i(particlePointSize_loc, 0.01f);
}
void myRenderScene()
{
    // Clear rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program
    glUseProgram(shader);

    // Render using VAO and VBO swapIndex
    glBindVertexArray(VAOs[swapIndex]);

    // Transform feedback goes into the other VBO
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFs[1-swapIndex]);
    glBeginTransformFeedback(GL_POINTS);

    // Debug
    // glDrawArrays(GL_POINTS, 0, NP);

    if(firstTimeRendering)
    {
        glDrawArrays(GL_POINTS, 0, NP);
        firstTimeRendering = !firstTimeRendering;
    }
    else
        glDrawTransformFeedback(GL_POINTS, TFs[swapIndex]);
    glEndTransformFeedback();

    swapIndex = 1 - swapIndex;

    // Unbind for safety (optional)
    glBindVertexArray(0);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}