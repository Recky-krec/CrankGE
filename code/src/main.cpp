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
#include "Buffers/RenderBuffer.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/TextureBuffer.h"

#include "Window/Window.h"

#include "Utils/Files.h"
#include "Utils/ResourceManager.h"
#include "Utils/Debug.h"
#include "Utils/Log.h"

#include "Misc/Vector.h"
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
    crank::Shader borderShader = crank::ResourceManager::LoadShader("code/shaders/border.vs", "code/shaders/border.fs", nullptr, "border");
    crank::Shader lamp_shader  = crank::ResourceManager::LoadShader("code/shaders/lamp.vs", "code/shaders/lamp.fs", nullptr, "lamp");
    crank::Shader model_shader = crank::ResourceManager::LoadShader("code/shaders/model_loading.vs", "code/shaders/model_loading.fs", nullptr, "nanosuit_model");
    crank::Shader screen_shader = crank::ResourceManager::LoadShader("code/shaders/framebuffers_screen.vs", "code/shaders/framebuffers_screen.fs", nullptr, "screen_shader");
    crank::Shader index_quad_shader = crank::ResourceManager::LoadShader("code/shaders/experimental/index_quad.vs", "code/shaders/experimental/index_quad.fs", nullptr, "index_quad");
    crank::Shader skyboxShader = crank::ResourceManager::LoadShader("code/shaders/skybox.vs", "code/shaders/skybox.fs", nullptr, "skybox");
    crank::Shader reflectiveCube = crank::ResourceManager::LoadShader("code/shaders/reflectiveCube.vs", "code/shaders/reflectiveCube.fs", nullptr, "reflectiveCube");
    crank::Shader reflectiveModelShader = crank::ResourceManager::LoadShader("code/shaders/reflectiveModel.vs", "code/shaders/reflectiveModel.fs", nullptr, "reflectiveModel");
    // model
    //TIME(crank::Model ourModel("assets/sponza/sponza.obj"));
    TIME(crank::Model nanosuit("assets/nanosuit/nanosuit.obj"));

    // textures
    unsigned int box = crank::loadTexture("assets/earth.png");
    unsigned int red_window = crank::loadTexture("assets/blending_transparent_window.png");

    std::vector<std::string> faces
    {
        "assets/skybox/lake/right.jpg",
        "assets/skybox/lake/left.jpg",
        "assets/skybox/lake/top.jpg",
        "assets/skybox/lake/bottom.jpg",
        "assets/skybox/lake/back.jpg",
        "assets/skybox/lake/front.jpg",
    };
    unsigned int cubeMap = crank::loadCubemap(faces);

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

    // Skybox
    crank::VertexArray skyboxVao;
    crank::VertexBuffer skyboxVbo(skyboxVertices, sizeof(skyboxVertices));
    crank::VertexBufferLayout sbLayout;
    sbLayout.Push<float>(3);
    skyboxVao.AddBuffer(skyboxVbo, sbLayout);
    skyboxVao.Unbind();

    // Reflection Cube
    crank::VertexArray reflectiveCubeVao;
    crank::VertexBuffer reflectiveCubeVbo(normalCubeVertices, sizeof(normalCubeVertices));
    crank::VertexBufferLayout rcLayout;
    rcLayout.Push<float>(3); // position
    rcLayout.Push<float>(3); // normals

    reflectiveCubeVao.AddBuffer(reflectiveCubeVbo, rcLayout);
    reflectiveCubeVao.Unbind();


    // Framebuffer operations
    crank::RenderBuffer rbo;
    rbo.Storage(width, height);
    rbo.Unbind();

    crank::TextureBuffer tbo(crank::TextureBuffer::COLOR);
    tbo.Storage(width, height);
    tbo.Unbind();

    // Framebuffer operations
    crank::FrameBuffer fbo(crank::FrameBuffer::READ_DRAW);
    fbo.Attach(rbo);
    fbo.Attach(tbo);
    fbo.Unbind();

    if(!fbo.Complete())
        LOGE << "FRAMEBUFFER: Framebuffer is not complete!";

    while (window.IsOpen())
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        process_input(&window, deltaTime);

        FPSsum += 1/deltaTime;
        FPSsum_count += 1;
        // debugging
        //std::cout << "(" << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.GetPosition().z << ")" <<std::endl;


        fbo.Bind();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_CULL_FACE);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the building

        glm::mat4 model;
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), width / height, 0.1f, 100.0f);

        // --- Mansion model ---
        /*model_shader.Enable();

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down


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
        model_shader.SetVector3f("dirLight.direction", -0.2f, -1.0f, 0.3f);
        model_shader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        model_shader.SetVector3f("dirLight.diffuse", 0.50f, 0.50f, 0.50f);
        model_shader.SetVector3f("dirLight.specular", 1.0f, 1.0f, 1.0f);

        // Draw call
        ourModel.Draw(model_shader);*/

        // --- Nanosuit ---
        glDisable(GL_BLEND);

        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0, 1.0, 1.0));
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

        reflectiveModelShader.Enable();
        reflectiveModelShader.SetMatrix4("model", model);
        reflectiveModelShader.SetMatrix4("view", camera.GetViewMatrix());
        reflectiveModelShader.SetMatrix4("projection", projection);
        reflectiveModelShader.SetVector3f("viewPos", camera.GetPosition());
        reflectiveModelShader.SetFloat("material.shininess", 32.0f);

        // Point light 0
        reflectiveModelShader.SetVector3f("pointLights[0].position", point_light_positions[0]);
        reflectiveModelShader.SetVector3f("pointLights[0].ambient", 0.02f, 0.02f, 0.02f);
        reflectiveModelShader.SetVector3f("pointLights[0].diffuse", 0.15f, 0.15f, 0.15f);
        reflectiveModelShader.SetVector3f("pointLights[0].specular", 0.80f, 0.80f, 0.80f);
        reflectiveModelShader.SetFloat("pointLights[0].constant", 1.0f);
        reflectiveModelShader.SetFloat("pointLights[0].linear", 0.09f);
        reflectiveModelShader.SetFloat("pointLights[0].quadratic", 0.032f);

        // Point light 1
        reflectiveModelShader.SetVector3f("pointLights[1].position", point_light_positions[1]);
        reflectiveModelShader.SetVector3f("pointLights[1].ambient", 0.02f, 0.02f, 0.02f);
        reflectiveModelShader.SetVector3f("pointLights[1].diffuse", 0.15f, 0.15f, 0.15f);
        reflectiveModelShader.SetVector3f("pointLights[1].specular", 0.80f, 0.80f, 0.80f);
        reflectiveModelShader.SetFloat("pointLights[1].constant", 1.0f);
        reflectiveModelShader.SetFloat("pointLights[1].linear", 0.09f);
        reflectiveModelShader.SetFloat("pointLights[1].quadratic", 0.032f);

        // Spotlight 0
        reflectiveModelShader.SetVector3f("spotLight.position", camera.GetPosition());
        reflectiveModelShader.SetVector3f("spotLight.direction", camera.Front);
        reflectiveModelShader.SetVector3f("spotLight.ambient", 0.01f, 0.01f, 0.01f);
        reflectiveModelShader.SetVector3f("spotLight.diffuse", 0.4, 0.4f, 0.4f);
        reflectiveModelShader.SetVector3f("spotLight.specular", 0.9f, 0.9f, 0.9f);
        reflectiveModelShader.SetFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
        reflectiveModelShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        reflectiveModelShader.SetFloat("spotLight.constant", 1.0f);
        reflectiveModelShader.SetFloat("spotLight.linear", 0.022f);
        reflectiveModelShader.SetFloat("spotLight.quadratic", 0.0019f);
        reflectiveModelShader.SetInteger("spotLight.isLanternOn", isLanternOn);

        // Directional light
        reflectiveModelShader.SetVector3f("dirLight.direction", -0.2f, -1.0f, 0.3f);
        reflectiveModelShader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        reflectiveModelShader.SetVector3f("dirLight.diffuse", 0.50f, 0.50f, 0.50f);
        reflectiveModelShader.SetVector3f("dirLight.specular", 1.0f, 1.0f, 1.0f);

        // Draw call
        glActiveTexture(GL_TEXTURE3);
        reflectiveModelShader.SetInteger("skybox", 3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

        nanosuit.Draw(reflectiveModelShader);


        // -------------- ON THE FIELD POINT LIGHT -----------------
        borderShader.Enable();

        cubeVao.Bind();

        for(int i=0; i<2; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, point_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));

            borderShader.SetMatrix4("model", model);
            borderShader.SetMatrix4("view", camera.GetViewMatrix());
            borderShader.SetMatrix4("projection", projection);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // RED WINDOWS:
        glEnable(GL_BLEND);
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

        // ---- Reflective Cube ---
        glDisable(GL_CULL_FACE);
        reflectiveCube.Enable();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(5.0, 10.0, 0.0));
        reflectiveCube.SetMatrix4("model", model);
        reflectiveCube.SetMatrix4("view", camera.GetViewMatrix());
        reflectiveCube.SetMatrix4("projection", projection);
        reflectiveCube.SetVector3f("u_cameraPos", camera.GetPosition());
        reflectiveCubeVao.Bind();
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // ------------------------




        // -------------- REFLECTIVE GROUND -----------------

        // First normal cube
        /*glEnable(GL_STENCIL_TEST);
        glClear(GL_STENCIL_BUFFER_BIT);

        glStencilMask(0xFF);  // enable writing to the stencil buffer
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Test to be drawn: Always. Fragments should update the stencil buffer
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Only updates buffer stored stencil value with reference value if both depth and stencil tests pass.

        lamp_shader.Enable();
        model = glm::mat4();
        model = glm::translate(model, cube_positions[0]);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lamp_shader.SetMatrix4("model", model);
        lamp_shader.SetMatrix4("view", camera.GetViewMatrix());
        lamp_shader.SetMatrix4("projection", projection);
        lamp_shader.SetVector3f("glassColor", 1.0f, 1.0f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box);
        cubeVao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glStencilMask(0x00);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //  we're only drawing parts of the containers that are not equal to 1
        //glDisable(GL_DEPTH_TEST);

        border_shader.Enable();
        model = glm::mat4();
        model = glm::translate(model, cube_positions[0]);
        model = glm::scale(model, glm::vec3(0.52f, 0.52f, 0.52f));
        border_shader.SetMatrix4("model", model);
        border_shader.SetMatrix4("view", camera.GetViewMatrix());
        border_shader.SetMatrix4("projection", projection);

        cubeVao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);*/



        /*glStencilMask(0xFF);
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
        border_shader.SetMatrix4("view", camera.GetViewMatrix());
        border_shader.SetMatrix4("projection", projection);

        planeVao.Bind();
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
        cubeVao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);
        glStencilFunc(GL_EQUAL, 0, 0xFF);*/

        glEnable(GL_DEPTH_TEST);
        skyboxShader.Enable();

        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // cuts out the translation properties of the matrix
        skyboxShader.SetMatrix4("view", view);
        skyboxShader.SetMatrix4("projection", projection);

        skyboxVao.Bind();
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        fbo.Unbind();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear( GL_COLOR_BUFFER_BIT );
        screenQuadVao.Bind();
        screen_shader.Enable();

        tbo.Bind();
        glActiveTexture(GL_TEXTURE0);
        screen_shader.SetInteger("texture1", 0);
        glBindTexture(GL_TEXTURE_2D, tbo.Id());
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
