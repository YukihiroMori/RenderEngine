#pragma once

#include "Main.h"
#include "Camera.h"
#include "Trackball.h"
#include "debug.h"


class Window {
	GLFWwindow *const window;

	Trackball left_tb, right_tb;

	int m_width, m_height;

	Camera m_camera;

public:

	Window(const char *title = "Default Window", int width = 640, int height = 480);

	~Window();

	void swapBuffers();

	int shouldClose() const
	{
		return glfwWindowShouldClose(window) | glfwGetKey(window, GLFW_KEY_ESCAPE);
	}

	static void mouseButtonCallbackImpl(GLFWwindow* raw_window, int button, int action, int mods)
	{
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(raw_window));
		window->mouse(raw_window,button,action,mods);
	}

	void mouse(GLFWwindow* window, int button, int action, int mods);

	static void resizeCallbackImpl(GLFWwindow* raw_window, int width, int height)
	{
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(raw_window));
		window->resize(raw_window, width, height);
	}

	void resize(GLFWwindow* window, int width, int height);

	void setViewPort();

	int getWidth() {
		return m_width;
	}

	int getHeight() {
		return m_height;
	}

	Trackball getLeftTrack() {
		return left_tb;
	}

	Trackball getRightTrack() {
		return right_tb;
	}

	Camera getCamera() {
		return m_camera;
	}
};