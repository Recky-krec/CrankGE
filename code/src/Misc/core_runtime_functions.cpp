#include "core_runtime_functions.h"

void process_input(Window* window, float deltatime)
{
    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_M))
    {
        isLocked = !isLocked;
        window->setMouseCursorLocked(isLocked);
    }

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_Q))
        window->close();

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_L))
        window->setSize(1920, 1080);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_K))
        window->setSize(width, height);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_J))
        isEmitting = !isEmitting;

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_W))
        camera.processKeyboardInput(Camera::FORWARD, deltatime);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_S))
        camera.processKeyboardInput(Camera::BACKWARD, deltatime);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_A))
        camera.processKeyboardInput(Camera::LEFT, deltatime);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_D))
        camera.processKeyboardInput(Camera::RIGHT, deltatime);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_R))
        camera.processKeyboardInput(Camera::UP, deltatime);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_F))
        camera.processKeyboardInput(Camera::DOWN, deltatime);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_B))
        isLanternOn = !isLanternOn;

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_UP))
        camera.addSpeed(1.0f);

    if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_DOWN))
        camera.addSpeed(-1.0f);


    glfwGetCursorPos(window->getWindowPtr(), &xpos, &ypos);
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    camera.processMouseInput(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScrollInput(yoffset);
}

void initialize_glew()
{

    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

}

void configure_opengl()
{
    // Configuring OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // Passes if the fragment's depth value is less than the stored depth value.
    //glDepthMask(GL_FALSE); // makes the depth buffer read only

    glEnable(GL_STENCIL_TEST);
    //glStencilMask(0xFF); // each bit is written to the stencil buffer as is (performs an AND operation with the bit that is about to be written)
    //glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)

    // C¯result = C¯source ∗ Fsource + C¯destination ∗ Fdestination
    // source is the window, destination is whatever is on the color buffer
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); // setting options for each of the channels individually
    glBlendEquation(GL_FUNC_ADD);

    // Enabling Face Culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Tesselation
    //glPatchParameteri(GL_PATCH_VERTICES, 32); // nº of control points that will be used to construct each patch

    glViewport(0, 0, width, height);
}
