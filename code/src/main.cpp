#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"
#include "Graphics/Model.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBufferLayout.h"

#include "Window/Window.h"

#include "Utils/Files.h"
#include "Utils/ResourceManager.h"
#include "Utils/Debug.h"
#include "Utils/Log.h"

#include "Misc/vertex_data.h"
#include "Misc/core_runtime_functions.h"

#include "stb/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION


int main()
{
    if (!glfwInit())
        return -1;

    initialize_globals();
    crank::Log::Init();

    crank::Window window(width, height, "CrankGE");
    window.SetActive();
    window.SetMouseCursorLocked();

    initialize_glew();
    configure_opengl();

    // Set callbacks
    glfwSetScrollCallback(window.GetWindowPtr(), scroll_callback);

    // Load resources
    // shaders
    crank::Shader border_shader = crank::ResourceManager::LoadShader("code/shaders/border.vs", "code/shaders/border.fs", nullptr, "border");
    crank::Shader lamp_shader  = crank::ResourceManager::LoadShader("code/shaders/lamp.vs", "code/shaders/lamp.fs", nullptr, "lamp");
    crank::Shader model_shader = crank::ResourceManager::LoadShader("code/shaders/model_loading.vs", "code/shaders/model_loading.fs", nullptr, "nanosuit_model");
    crank::Shader screen_shader = crank::ResourceManager::LoadShader("code/shaders/framebuffers_screen.vs", "code/shaders/framebuffers_screen.fs", nullptr, "screen_shader");
    crank::Shader index_quad_shader = crank::ResourceManager::LoadShader("code/shaders/experimental/index_quad.vs", "code/shaders/experimental/index_quad.fs", nullptr, "index_quad");

    // model
    TIME(crank::Model ourModel("assets/sponza/sponza.obj"));

    // textures
    unsigned int box = crank::loadTexture("assets/earth.png");
    unsigned int red_window = crank::loadTexture("assets/blending_transparent_window.png");

    // Buffers for cube with position, normal and texcoords
    crank::VertexArray cubeVao;
    crank::VertexBuffer cubeVbo(cubeVertices, sizeof(cubeVertices));
    crank::VertexBufferLayout cLayout;
    cLayout.Push<float>(3);
    cLayout.Push<float>(3);
    cLayout.Push<float>(2);
    cubeVao.AddBuffer(cubeVbo, cLayout);
    cubeVao.Unbind();

    // Buffers for cube with position
    crank::VertexArray simpleCubeVao;
    cubeVbo.Bind();
    crank::VertexBufferLayout scLayout;
    scLayout.Push<float>(3);
    simpleCubeVao.AddBuffer(cubeVbo, scLayout);
    simpleCubeVao.Unbind();

    // Buffers for quad plane, horizontal (reflection plane)
    crank::VertexArray planeVao;
    crank::VertexBuffer planeVbo(planeVertices, sizeof(planeVertices));
    crank::VertexBufferLayout pLayout;
    pLayout.Push<float>(3);
    planeVao.AddBuffer(planeVbo, pLayout);
    planeVao.Unbind();

    // Buffers for screen quad
    crank::VertexArray screenQuadVao;
    crank::VertexBuffer screenQuadVbo(screenQuadVertices, sizeof(screenQuadVertices));
    crank::VertexBufferLayout sqLayout;
    sqLayout.Push<float>(2);
    sqLayout.Push<float>(2);
    screenQuadVao.AddBuffer(screenQuadVbo, sqLayout);
    screenQuadVao.Unbind();

    // Simple test quad
    crank::VertexArray testVao;
    crank::VertexBuffer testVbo(test, sizeof(test));
    crank::IndexBuffer testIbo(test_indices, 6);
    crank::VertexBufferLayout tLayout;
    tLayout.Push<float>(3);
    testVao.AddBuffer(testVbo, tLayout);
    testVao.Unbind();



    unsigned int fb_texture_rgb, fb_texture_depth, fb_texture_stencil, fb_texture_depth_stencil;
    glGenTextures(1, &fb_texture_rgb);
    glBindTexture(GL_TEXTURE_2D, fb_texture_rgb);
    // allocating memory for a texture with the dimensions of the screen size, ready to be used as a color buffer
    /* If you want to render your whole screen to a texture of a smaller or larger size you need to call glViewport
     * again (before rendering to your framebuffer) with the new dimensions of your texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // allocating memory for a texture to be used as a depth buffer
    glGenTextures(1, &fb_texture_depth);
    glBindTexture(GL_TEXTURE_2D, fb_texture_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    // allocating memory for a texture to be used as a stencil buffer
    glGenTextures(1, &fb_texture_stencil);
    glBindTexture(GL_TEXTURE_2D, fb_texture_stencil);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    // allocating memory for a texture that can be used both for depth and stencil
    glGenTextures(1, &fb_texture_depth_stencil);
    glBindTexture(GL_TEXTURE_2D, fb_texture_depth_stencil);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    // render buffer objects to be use within the framebuffer
    // advantages: already in opengl internal format, avoiding to texture conversions
    // thus making them faster to be written on or to have its data copied to other buffers
    // cons: write only, we can use glReadPixels() to read from the currently bound frame buffer but
    // not from the attachment itself
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    /* Since renderbuffer objects are generally write-only they are often used as depth and stencil attachments,
     * since most of the time we don't really need to read values from the depth and stencil buffers but still care about depth and stencil testing.
     * We need the depth and stencil values for testing,
     * but don't need to sample these values so a renderbuffer object suits this perfectly.
     * When we're not sampling from these buffers, a renderbuffer object is generally preferred since it's more optimized. */
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);


    // Framebuffer operations
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // GL_READ_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER

    glBindTexture(GL_TEXTURE_2D, fb_texture_rgb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_texture_rgb, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb_texture_depth, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fb_texture_stencil, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fb_texture_depth_stencil, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOGE << "FRAMEBUFFER: Framebuffer is not complete!";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    while (window.IsOpen())
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        process_input(&window, deltaTime);

        // output fps: deltaTime * FPS = 1second
        //std::cout << 1/deltaTime << '\n';
        FPSsum += 1/deltaTime;
        FPSsum_count += 1;
        // debugging
        //std::cout << "(" << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.GetPosition().z << ")" <<std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the building
        model_shader.Enable();
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), width / height, 0.1f, 100.0f);

        model_shader.SetMatrix4("model", model);
        model_shader.SetMatrix4("view", camera.GetViewMatrix());
        model_shader.SetMatrix4("projection", projection);

        model_shader.SetVector3f("viewPos", camera.GetPosition());
        model_shader.SetFloat("material.shininess", 32.0f);

        // Point light 0
        model_shader.SetVector3f("pointLights[0].position", point_light_positions[0]);
        model_shader.SetVector3f("pointLights[0].ambient", 0.02f, 0.02f, 0.02f);
        model_shader.SetVector3f("pointLights[0].diffuse", 0.15f, 0.15f, 0.15f);
        model_shader.SetVector3f("pointLights[0].specular", 0.80f, 0.80f, 0.80f);
        model_shader.SetFloat("pointLights[0].constant", 1.0f);
        model_shader.SetFloat("pointLights[0].linear", 0.09f);
        model_shader.SetFloat("pointLights[0].quadratic", 0.032f);

        // Point light 1
        model_shader.SetVector3f("pointLights[1].position", point_light_positions[1]);
        model_shader.SetVector3f("pointLights[1].ambient", 0.02f, 0.02f, 0.02f);
        model_shader.SetVector3f("pointLights[1].diffuse", 0.15f, 0.15f, 0.15f);
        model_shader.SetVector3f("pointLights[1].specular", 0.80f, 0.80f, 0.80f);
        model_shader.SetFloat("pointLights[1].constant", 1.0f);
        model_shader.SetFloat("pointLights[1].linear", 0.09f);
        model_shader.SetFloat("pointLights[1].quadratic", 0.032f);

        // Spotlight 0
        model_shader.SetVector3f("spotLight.position", camera.GetPosition());
        model_shader.SetVector3f("spotLight.direction", camera.Front);
        model_shader.SetVector3f("spotLight.ambient", 0.01f, 0.01f, 0.01f);
        model_shader.SetVector3f("spotLight.diffuse", 0.4, 0.4f, 0.4f);
        model_shader.SetVector3f("spotLight.specular", 0.9f, 0.9f, 0.9f);
        model_shader.SetFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
        model_shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        model_shader.SetFloat("spotLight.constant", 1.0f);
        model_shader.SetFloat("spotLight.linear", 0.022f);
        model_shader.SetFloat("spotLight.quadratic", 0.0019f);
        model_shader.SetInteger("spotLight.isLanternOn", isLanternOn);

        // Directional light
        model_shader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        model_shader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        model_shader.SetVector3f("dirLight.diffuse", 0.50f, 0.50f, 0.50f);
        model_shader.SetVector3f("dirLight.specular", 1.0f, 1.0f, 1.0f);

        // Draw call
        //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        ourModel.Draw(model_shader);

        // ----------------------- test quad -------------------------

        glDisable(GL_CULL_FACE);
        index_quad_shader.Enable();
        testVao.Bind();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.5, 1.5, 1.5));
        model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));
        index_quad_shader.SetMatrix4("model", model);
        index_quad_shader.SetMatrix4("view", camera.GetViewMatrix());
        index_quad_shader.SetMatrix4("projection", projection);
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);)
        glEnable(GL_CULL_FACE);


        // -------------- ON THE FIELD POINT LIGHT -----------------
        border_shader.Enable();

        cubeVao.Bind();

        for(int i=0; i<2; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, point_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));

            border_shader.SetMatrix4("model", model);
            border_shader.SetMatrix4("view", camera.GetViewMatrix());
            border_shader.SetMatrix4("projection", projection);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // RED WINDOWS:
        glDisable(GL_CULL_FACE);
        cubeVao.Bind();

        lamp_shader.Enable();

        std::map<float, glm::vec3> sortedWindowPositions;
        for(int i = 1; i < 4; i++)
        {
            float distance = glm::length(camera.GetPosition() - plane_positions[i]);
            sortedWindowPositions[distance] = plane_positions[i];
        }

        for(std::map<float, glm::vec3>::reverse_iterator it = sortedWindowPositions.rbegin(); it != sortedWindowPositions.rend(); ++it)
        {
            model = glm::mat4();
            model = glm::translate(model, it->second);
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            lamp_shader.SetMatrix4("model", model);
            lamp_shader.SetMatrix4("view", camera.GetViewMatrix());
            lamp_shader.SetMatrix4("projection", projection);
            lamp_shader.SetVector3f("glassColor", 1.0f, 1.0f, 1.0f);
            lamp_shader.SetInteger("texture_0", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, red_window);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glEnable(GL_CULL_FACE);



        // -------------- REFLECTIVE GROUND -----------------
/*
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Only updates buffer stored stencil value with reference value if both depth and stencil tests pass.
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Test to be drawn: Always. Fragments should update the stencil buffer
        glStencilMask(0xFF);  // enable writing to the stencil buffer

        lamp_shader.Enable();
        model = glm::mat4();
        model = glm::translate(model, cube_positions[0]);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lamp_shader.SetMatrix4("model", model);
        lamp_shader.SetMatrix4("view", camera.getViewMatrix());
        lamp_shader.SetMatrix4("projection", projection);
        lamp_shader.SetVector3f("glassColor", 1.0f, 1.0f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box);
        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //  we're only drawing parts of the containers that are not equal to 1
        glStencilMask(0x00);
        //glDisable(GL_DEPTH_TEST);

        border_shader.Enable();
        model = glm::mat4();
        model = glm::translate(model, cube_positions[0]);
        model = glm::scale(model, glm::vec3(0.52f, 0.52f, 0.52f));
        border_shader.SetMatrix4("model", model);
        border_shader.SetMatrix4("view", camera.getViewMatrix());
        border_shader.SetMatrix4("projection", projection);
        glBindVertexArray(simple_cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        // Plane
        glDisable(GL_CULL_FACE);

        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF); // Write to stencil buffer
        glDepthMask(GL_FALSE); // Don't write to depth buffer

        border_shader.Enable();
        model = glm::mat4();
        model = glm::translate(model, plane_positions[0]);
        border_shader.SetMatrix4("model", model);
        border_shader.SetMatrix4("view", camera.getViewMatrix());
        border_shader.SetMatrix4("projection", projection);

        glBindVertexArray(plane_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_CULL_FACE);


        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glDepthMask(GL_TRUE);
        glm::vec3 newpos(cube_positions[0].x, cube_positions[0].y - 0.5f, cube_positions[0].z);

        lamp_shader.Enable();
        model = glm::mat4();
        model = glm::translate(model, newpos);
        model = glm::scale(model , glm::vec3(0.5f, -0.5f, 0.5f));
        lamp_shader.SetMatrix4("model", model);
        lamp_shader.SetMatrix4("view", camera.GetViewMatrix());
        lamp_shader.SetMatrix4("projection", projection);
        lamp_shader.SetVector3f("glassColor", 0.5f, 0.5f, 0.5f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box);
        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);
        glStencilFunc(GL_EQUAL, 0, 0xFF);*/


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear( GL_COLOR_BUFFER_BIT );
        screenQuadVao.Bind();
        screen_shader.Enable();
        glActiveTexture(GL_TEXTURE0);
        screen_shader.SetInteger("texture1", 0);
        glBindTexture(GL_TEXTURE_2D, fb_texture_rgb);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        window.SwapBuffers();
        glfwPollEvents();
    }

    std::cout << "Average FPS: " << FPSsum / FPSsum_count << std::endl;


    crank::ResourceManager::Clear();
    crank::Log::Flush();

    glfwTerminate();
    return 0;
}
