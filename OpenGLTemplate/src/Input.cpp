#include "Input.h"

GLFWwindow * Input::m_window;
std::vector<bool> Input::m_bKeyPressed;
std::vector<bool> Input::m_bMouseButtonPressed;
std::vector<std::queue<Message>> Input::m_vecMessageQueues;
std::string Input::m_strTextBuffer;
bool Input::m_bRecordingText;
int Input::m_iPixelWidth, Input::m_iPixelHeight;
int Input::m_iWidth, Input::m_iHeight;
int Input::m_iXMousePos, Input::m_iYMousePos;

void Input::init(GLFWwindow* window)
{
	m_window = window;
	m_bKeyPressed = std::vector<bool>(GLFW_KEY_LAST + 1, false);
	m_bMouseButtonPressed = std::vector<bool>(GLFW_MOUSE_BUTTON_LAST + 1, false);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCharCallback(window, charCallback);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetWindowSizeCallback(window, sizeCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetWindowIconifyCallback(window, minimizeCallback);
	glfwSetWindowCloseCallback(window, closeCallback);
	m_vecMessageQueues.push_back(std::queue<Message>());
	glfwGetWindowSize(window, &m_iWidth, &m_iHeight);
	glfwGetFramebufferSize(window, &m_iPixelWidth, &m_iPixelHeight);
}


void Input::update()
{
	glfwPollEvents();
	Message msg{};
	while (getMessage(0, &msg, true))
	{
		if(msg.message == KEYDOWN && msg.iXData == GLFW_KEY_BACKSPACE)
		{
			m_strTextBuffer.pop_back();
		}
	}
}


int Input::createMessageQueue()
{
	m_vecMessageQueues.push_back(std::queue<Message>());
	return m_vecMessageQueues.size() - 1;
}

bool Input::getMessage(int iQueue, Message* message, bool bRemove)
{
	if (m_vecMessageQueues[iQueue].empty())
		return false;

	*message = m_vecMessageQueues[iQueue].front();
	if (bRemove)
		m_vecMessageQueues[iQueue].pop();

	return true;
}


inline void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int iKeyDownFlag, iKeyUpFlag;

	switch(mods)
	{
	case GLFW_MOD_SHIFT:
		iKeyDownFlag = SHIFTKEYDOWN;
		iKeyUpFlag = SHIFTKEYUP;
		break;
	case GLFW_MOD_CONTROL:
		iKeyDownFlag = CTRLCOMMANDDOWN;
		iKeyUpFlag = CTRLCOMMANDUP;
		break;
	case GLFW_MOD_ALT:
		iKeyDownFlag = ALTCOMMANDDOWN;
		iKeyUpFlag = ALTCOMMANDUP;
		break;
	default:
		iKeyDownFlag = KEYDOWN;
		iKeyUpFlag = KEYUP;
	}

	if (action == GLFW_PRESS)
	{
		m_bKeyPressed[key] = true;
		for (auto queue : m_vecMessageQueues)
		{
			queue.push(Message{ iKeyDownFlag, key, mods });
		}
	}
	else if (action == GLFW_RELEASE)
	{
		m_bKeyPressed[key] = false;
		for (auto queue : m_vecMessageQueues)
		{
			queue.push(Message{ iKeyUpFlag, key, mods });
		}
	}
}

void Input::charCallback(GLFWwindow* window, unsigned int codepoint)
{
	for (auto queue : m_vecMessageQueues)
	{
		queue.push(Message{ CHARINPUT, int(codepoint) });
	}

	if (m_bRecordingText)
		m_strTextBuffer += char(codepoint);
}

void Input::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	for (auto queue : m_vecMessageQueues)
	{
		queue.push(Message{ FRAMEBUFFERRESIZE,  width, height});
	}
	m_iPixelWidth = width;
	m_iPixelHeight = height;
}

void Input::sizeCallback(GLFWwindow* window, int width, int height)
{
	for (auto queue : m_vecMessageQueues)
	{
		queue.push(Message{ RESIZE,  width, height });
	}
	m_iWidth = width;
	m_iHeight = height;
}


void Input::mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_iXMousePos = xpos;
	m_iYMousePos = ypos;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	int iButton;
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		iButton = MOUSELEFTDOWN;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		iButton = MOUSEMIDDLEDOWN;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		iButton = MOUSERIGHTDOWN;
		break;
	default:
		iButton = MOUSEOTHERDOWN;
		break;
	}

	if(action == GLFW_PRESS)
	{
		m_bMouseButtonPressed[button] = true;
		for (auto queue : m_vecMessageQueues)
		{
			queue.push(Message{ iButton,  m_iXMousePos, m_iYMousePos });
		}
	}
	else if (action == GLFW_RELEASE)
	{
		m_bMouseButtonPressed[button] = false;
		for (auto queue : m_vecMessageQueues)
		{
			queue.push(Message{ iButton + 1,  m_iXMousePos, m_iYMousePos });
		}
	}

}

void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	for (auto queue : m_vecMessageQueues)
	{
		queue.push(Message{ MOUSESCROLL,  int(xoffset), int(yoffset) });
	}
}

void Input::minimizeCallback(GLFWwindow* window, int iconified)
{
	int iMessage;
	if (iconified)
		iMessage = MINIMIZE;
	else
		iMessage = UNMINIMIZE;

	for (auto queue : m_vecMessageQueues)
	{
		queue.push(Message{ iMessage });
	}
}


void Input::closeCallback(GLFWwindow* window)
{
	for (auto queue : m_vecMessageQueues)
	{
		queue.push(Message{ DESTROY });
	}
}
