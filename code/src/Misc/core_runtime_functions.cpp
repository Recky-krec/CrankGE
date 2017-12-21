#include "core_runtime_functions.h"

void process_input(crank::Window* window, float deltatime)
{
    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_M))
    {
        isLocked = !isLocked;
        window->SetMouseCursorLocked(isLocked);
    }

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_Q))
        window->Close();

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_J))
        isEmitting = !isEmitting;

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_W))
        camera.ProcessKeyboardInput(crank::Camera::FORWARD, deltatime);

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_S))
        camera.ProcessKeyboardInput(crank::Camera::BACKWARD, deltatime);

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_A))
        camera.ProcessKeyboardInput(crank::Camera::LEFT, deltatime);

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_D))
        camera.ProcessKeyboardInput(crank::Camera::RIGHT, deltatime);

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_R))
        camera.ProcessKeyboardInput(crank::Camera::UP, deltatime);

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_F))
        camera.ProcessKeyboardInput(crank::Camera::DOWN, deltatime);

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_B))
        isLanternOn = !isLanternOn;

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_UP))
        camera.AddSpeed(1.0f);

    if (glfwGetKey(window->GetWindowPtr(), GLFW_KEY_DOWN))
        camera.AddSpeed(-1.0f);


    glfwGetCursorPos(window->GetWindowPtr(), &xpos, &ypos);
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseInput(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScrollInput(yoffset);
}

void initialize_glew()
{

    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

}

void configure_opengl()
{
    // DEPTH TESTING
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // Passes if the fragment's depth value is less than the stored depth value.
    //glDepthMask(GL_FALSE); // makes the depth buffer read only

    // STENCIL TESTING
    glEnable(GL_STENCIL_TEST);
    //glStencilMask(0xFF); // each bit is written to the stencil buffer as is (performs an AND operation with the bit that is about to be written)
    //glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)

    // BLENDING
    // (Source is the one in the front, destination is whatever is on the color buffer)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); // Sets option for both RGB and alpha channels individually
    glBlendEquation(GL_FUNC_ADD); // C¯result = C¯source ∗ Fsource + C¯destination ∗ Fdestination

    // FACE CULLING
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); // Counter clock wise faces are considered the front faces.
    glCullFace(GL_BACK); // The face on the back is the one which is thrown away.

    // TESSELATION
    //glPatchParameteri(GL_PATCH_VERTICES, 32); // nº of control points that will be used to construct each patch

    // VIEWPORT
    glViewport(0, 0, width, height);
}
