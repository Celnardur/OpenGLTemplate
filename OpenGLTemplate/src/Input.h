
#ifndef INPUT_H
#define INPUT_H

#include <GraphicsIncludes.h>
#include <vector>
#include <queue>
#include <string>

// This provides a structure for sending message data.
// "message" is the type of message provided by an enum.
// The rest are integer and double types that are used to send data with a message.
// For keyboard messages "iXData" is used to specify the key using GLFW key defs.
// For mouse and size messages "iXData" and "iYData" are the integer coordinates
// or size in pixels or screen coordinates depending on message.
struct Message
{
	int message;
	int iXData;
	int iYData;

	Message(int m = 0, int ix = -1, int iy = -1)
	{
		message = m;
		iXData = ix;
		iYData = iy;
	}
};

enum
{
	EMPTY,
	KEYDOWN,
	KEYUP,
	SHIFTKEYDOWN,
	SHIFTKEYUP,
	CTRLCOMMANDUP,
	CTRLCOMMANDDOWN,
	ALTCOMMANDUP,
	ALTCOMMANDDOWN,
	CHARINPUT,
	MOUSELEFTDOWN,
	MOUSELEFTUP,
	MOUSEMIDDLEDOWN,
	MOUSEMIDDLEUP,
	MOUSERIGHTDOWN,
	MOUSERIGHTUP,
	MOUSEOTHERDOWN,
	MOUSEOTHERUP,
	MOUSESCROLL,
	RESIZE,
	FRAMEBUFFERRESIZE,
	MINIMIZE,
	UNMINIMIZE,
	DESTROY
};

// This is a class to handle input from GLFW and create queues for it.
// It also wraps some functionality for it like key presses, text input,
// tracking mouse position, and converts mouse position to normalized coordinates.
class Input
{
public:
	static void init(GLFWwindow * window);
	static void update();

	// These handle the message queues
	static int createMessageQueue();
	static int getQueueSize(int iQueue) { return m_vecMessageQueues[iQueue].size(); }
	static bool getMessage(int iQueue, Message * message, bool bRemove);

	// These return weather or not a key or mouse button is held down
	// these use the GLFW enums for key or mouse button IDs
	static bool keyPressed(int key) { return m_bKeyPressed[key]; }
	static bool mouseButtonPressed(int iButton) { return m_bMouseButtonPressed[iButton]; }

	// Functions that help record text input
	static void recordText() { m_bRecordingText = true; }
	static void stopRecordingText() { m_bRecordingText = false; }
	static const std::string & getText() { return m_strTextBuffer; }
	static void clearText() { m_strTextBuffer.clear(); }

	// functions to get the client area size in pixels
	// pixel size is used for glViewport and projection matrices
	static int getPixelWidth() { return m_iPixelWidth; }
	static int getPixelHeight() { return m_iPixelHeight; }

	// functions to get the window size in screen Coordinates
	static int getScreenCoordWidth() { return m_iWidth; }
	static int getScreenCoordHeight() { return m_iHeight; }

	// functions to get the mouse position which is always in screen Coordinates
	static int getXMousePos() { return m_iXMousePos; }
	static int getYMousePos() { return m_iYMousePos; }
	static void getMousePosition(int * iXPos, int * iYPos)
	{
		*iXPos = m_iXMousePos;
		*iYPos = m_iYMousePos;
	}

private:
	// Callbacks for most of the input types.
	// These are used to put messages in the queues.
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void charCallback(GLFWwindow* window, unsigned int codepoint);
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void sizeCallback(GLFWwindow* window, int width, int height);
	static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void minimizeCallback(GLFWwindow* window, int iconified);
	static void closeCallback(GLFWwindow* window);

	static GLFWwindow * m_window;

	static std::vector<std::queue<Message>> m_vecMessageQueues;

	static std::vector<bool> m_bKeyPressed;
	static std::vector<bool> m_bMouseButtonPressed;

	static std::string m_strTextBuffer;
	static bool m_bRecordingText;

	static int m_iPixelWidth, m_iPixelHeight;
	static int m_iWidth, m_iHeight;
	static int m_iXMousePos, m_iYMousePos;
};

#endif