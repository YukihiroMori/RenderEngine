#include "Window.h"

#define DEBUG 1

Window::Window(const char *title , int width , int height)
	: window(glfwCreateWindow(width, height, title, nullptr, nullptr))
	, m_width(width), m_height(height)
{

	if (window == NULL) {
		cerr << "Can't create GLFW window." << endl;
		exit(1);
	}

	glfwMakeContextCurrent(window);

	InitializeExtFuncs();

#if DEBUG
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glfwSetErrorCallback(error_callback);
#endif // DEBUG

	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, resizeCallbackImpl);
	glfwSetMouseButtonCallback(window, mouseButtonCallbackImpl);

	m_camera.setSize(width , height);

	left_tb.region(width, height);
	right_tb.region(width, height);
}

Window::~Window() {
	glfwDestroyWindow(window);
}

void Window::resize(GLFWwindow* window, int width, int height) {
	m_width = width;
	m_height = height;

	glfwSetWindowSize(window, width , height);

	glViewport(0, 0, width, height);

	m_camera.setSize(width, height);

	left_tb.region(width, height);
	right_tb.region(width, height);
}

void Window::setViewPort() {
	glViewport(0, 0, m_width, m_height);
}

void Window::swapBuffers(){
	glfwSwapBuffers(window);

	glfwPollEvents();

	glfwWaitEvents();

	double mx, my;
	glfwGetCursorPos(window, &mx, &my);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_RELEASE) left_tb.motion(mx, my);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_RELEASE) right_tb.motion(mx, my);
}

void Window::mouse(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action != GLFW_RELEASE)
		{
			left_tb.start(x, y);
		}
		else
		{
			left_tb.stop(x, y);
		}
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action != GLFW_RELEASE)
		{
			right_tb.start(x, y);
		}
		else
		{
			right_tb.stop(x, y);
		}
		break;
	default:
		break;
	}
}