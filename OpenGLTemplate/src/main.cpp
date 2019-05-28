#include <iostream>
#include <chrono>
#include <thread>
#include "Input.h"
#include "CoreGraphics/Mesh.h"
#include "CoreGraphics/Shader.h"
#include "CoreGraphics/BufferMap.h"

using std::cout;
using std::endl;

// sleeps the current thread for dSleepTime seconds.
inline void sleep(double dSleepTime)
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(int(dSleepTime * 1.0e9)));
}

GLFWwindow * init(const std::string & strWindowTitle, int iWidth, int iHeight);

int main()
{
    // width and height of the window in pixels
	int iWidth = 1280;
	int iHeight = 720;

	// array for fps counter
	double pdFrameTimes[30];
	int iIndex = 0;

	GLFWwindow * pWindow = init("OpenGL Framework", iWidth, iHeight);
	Input::init(pWindow);
	// the ID for the message queue associated with main
	int idMessageQueue = Input::createMessageQueue();

	// Everything between the arrows is not part of the basic framework and is for testing.
	// If there is anything in these blocks they can be deleted.
	// Stuff in these should be deleted before merging (unless it's a fast-forward merge).
	// Initialize graphics here
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	struct vtPoint
	{
		float x, y;
	};
	VertexData<vtPoint> vdPoint({ 2 });
	vdPoint.vertices = {
		{ 0.5f,  0.5f},  // top right
		{ 0.5f, -0.5f},  // bottom right
		{-0.5f, -0.5f},  // bottom left
		{-0.5f,  0.5f}   // top left
	};
	vdPoint.vuiIndices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	Mesh msPoint;
	msPoint.create<vtPoint>(vdPoint);
	msPoint.setColor(1.0f, 0.5f, 0.2f);

	struct vtColor
	{
		float x, y;
		float r, g, b;
	};
	VertexData<vtColor> vdColor({ 2, 3 });
	vdColor.vertices = {
		{ 0.5f,  0.5f,    1.0f, 0.0f, 0.0f},  // top right
		{ 0.5f, -0.5f,    0.0f, 1.0f, 0.0f},  // bottom right
		{-0.5f, -0.5f,    0.0f, 0.0f, 1.0f},  // bottom left
		{-0.5f,  0.5f,    1.0f, 1.0f, 0.0f}   // top left
	};
	vdColor.vuiIndices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	Mesh msColor;
	msColor.create<vtColor>(vdColor);

	struct vtTexture
	{
		float x, y;
		float j, k;
	};
	VertexData<vtTexture> vdTexture({ 2, 0, 2 });
	vdTexture.vertices = {
		{ 0.5f,  0.5f,     1.0f, 1.0f},  // top right
		{ 0.5f, -0.5f,     1.0f, 0.0f},  // bottom right
		{-0.5f, -0.5f,     0.0f, 0.0f},  // bottom left
		{-0.5f,  0.5f,     0.0f, 1.0f}   // top left
	};
	vdTexture.vuiIndices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	Texture txImage("Resources/container.jpg");

	Mesh msTexture;
	msTexture.create<vtTexture>(vdTexture, txImage);

	struct Vertex
	{
		float x, y;
		float r, g, b;
		float j, k;
	};
	VertexData<Vertex> vdColorTexture({ 2, 3, 2 });
	vdColorTexture.vertices = {
		{ 0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f},  // top right
		{ 0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f},  // bottom right
		{-0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f},  // bottom left
		{-0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f}   // top left
	};
	vdColorTexture.vuiIndices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	Mesh msColorTexture;
	msColorTexture.create<Vertex>(vdColorTexture, txImage);

	VertexData<vtTexture> copy = vdTexture;
	Mesh msCopy;
	msCopy.create<vtTexture>(copy, txImage);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	// initialize timer variables
	double dStart, dEnd, dFrameTime = 1.0 / 60.0;
	int nFramesPerSecond = 60;
	double dPeriod = 1.0 / (nFramesPerSecond + 4);

	Message msg = {};
	while (!glfwWindowShouldClose(pWindow))
	{
		dStart = glfwGetTime();

		if (iIndex < 30)
		{
			pdFrameTimes[iIndex] = 1.0 / dFrameTime;
			iIndex++;
		}
		else
		{
			double sum = 0;
			for (int i = 0; i < 30; i++)
				sum += pdFrameTimes[i];
			double avg = sum / 30.0;
			glfwSetWindowTitle(pWindow, ("FPS: " + std::to_string(avg)).c_str());
			iIndex = 0;
		}

		// halts program if OpenGL error is encountered and prints it
		int flag = glGetError();
		if (flag != 0)
		{
			std::cout << flag << std::endl;
			while (true);
		}

        // input
		Input::update();
		// handle non-queue input here
		// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

		while(Input::getMessage(idMessageQueue, &msg, true))
		{
			// handle input from queue in here
			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        }

        // Rendering commands go in here
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		msPoint.render();
        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // swap buffer to show screen
		glfwSwapBuffers(pWindow);

		dEnd = glfwGetTime();
		sleep(dPeriod - (dEnd - dStart));
		dFrameTime = glfwGetTime() - dStart;
    }

    glfwTerminate();

    // Clean up
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	msColor.destroy();
	msTexture.destroy();
	msColorTexture.destroy();
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

// This function initializes the window
GLFWwindow * init(const std::string & strWindowTitle, int iWidth, int iHeight)
{
	// Initial setup of GLFW. Sets OpenGL version to 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// fixes issue on OS X
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// creates window and checks for fail
	GLFWwindow * pWindow = glfwCreateWindow(iWidth, iHeight, strWindowTitle.c_str(), NULL, NULL);
	if (pWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// This line sets it so all commands will refer to this window.
	glfwMakeContextCurrent(pWindow);

	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// sets the viewport to the right initial size
	glViewport(0, 0, iWidth, iHeight);

	//glEnable(GL_DEPTH_TEST);

	return pWindow;
}

