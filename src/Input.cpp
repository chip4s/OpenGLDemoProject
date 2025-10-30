#include "Input.h"

Input::Input(GLFWwindow* w)
{
	window = w;
	glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));
	glfwSetKeyCallback(window, Input::key_Callback);
	glfwSetCursorPosCallback(window, Input::mouse_Callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Input::key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	inputs[key] = action;
    if (inputs[GLFW_KEY_ESCAPE])
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (inputs[GLFW_KEY_G])
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

    }
    if (inputs[GLFW_KEY_H])
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }
}
void Input::handle_CameraMovement(float dt)
{
    float cameraSpeed = 12.0 * dt;
    if (inputs[GLFW_KEY_LEFT_SHIFT])
    {
        cameraSpeed = 50.0 * dt;
    }
    else if (inputs[GLFW_KEY_LEFT_CONTROL])
    {
        cameraSpeed = 3.0 * dt;
    }
    else
    {
        cameraSpeed = 12.0 * dt;
    }
    if (inputs[GLFW_KEY_W])
    {
        cam.cameraPos += cameraSpeed * cam.cameraFront;
    }
    if (inputs[GLFW_KEY_S])
    {
        cam.cameraPos -= cameraSpeed * cam.cameraFront;
    }
    if (inputs[GLFW_KEY_A])
    {
        cam.cameraPos -= glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;
    }
    if (inputs[GLFW_KEY_D])
    {
        cam.cameraPos += glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;
    }
}
void Input::mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
    if (cam.firstMouse)
    {
        cam.lastX = xpos;
        cam.lastY = ypos;
        cam.firstMouse = false;
    }

    double xoffset = xpos - cam.lastX;
    double yoffset = cam.lastY - ypos;
    cam.lastX = xpos;
    cam.lastY = ypos;

    double sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam.yaw += xoffset;
    cam.pitch += yoffset;

    if (cam.pitch > 89.0f)
        cam.pitch = 89.0f;
    if (cam.pitch < -89.0f)
        cam.pitch = -89.0f;

    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
    direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    direction.y = sin(glm::radians(cam.pitch));
    direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.cameraFront = glm::normalize(direction);
}
glm::mat4 Input::CreateViewMat()
{
    return glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraFront, cam.cameraUp);
}
