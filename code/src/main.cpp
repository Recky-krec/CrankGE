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

#include "Window/Window.h"

#include "Utils/FileUtils.h"
#include "Utils/ResourceManager.h"
#include "Utils/GL_DebugUtils.h"

#include "Misc/vertex_data.h"
#include "Misc/core_runtime_functions.h"

#include "stb/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION


int main()
{
	if (!glfwInit())
		return -1;

    initialize_globals();

	Window window(width, height, "CrankGE");
	window.setActive();
	window.setMouseCursorLocked();

    initialize_glew();
    configure_opengl();

    // Set callbacks
	glfwSetScrollCallback(window.getWindowPtr(), scroll_callback);

    // Load resources
    Shader border_shader = ResourceManager::LoadShader("code/shaders/border.vs", "code/shaders/border.fs", nullptr, "border");
	Shader lamp_shader  = ResourceManager::LoadShader("code/shaders/lamp.vs", "code/shaders/lamp.fs", nullptr, "lamp");
	Shader model_shader = ResourceManager::LoadShader("code/shaders/model_loading.vs", "code/shaders/model_loading.fs", nullptr, "nanosuit_model");

    TIME(Model ourModel("assets/sponza/sponza.obj"));

    unsigned int box = load_texture("assets/earth.png");
    unsigned int red_window = load_texture("assets/blending_transparent_window.png");


	// Buffers for cube with position, normal and texcoords
	unsigned int cube_VAO;
	glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    VertexBuffer cube_VBO(cube_vertices, sizeof(cube_vertices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    // Buffers for cube with position
	unsigned int simple_cube_VAO;
	glGenVertexArrays(1, &simple_cube_VAO);
	glBindVertexArray(simple_cube_VAO);
	cube_VBO.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    // Buffers for quad plane with position
    unsigned int plane_VAO;
    VertexBuffer plane_VBO(plane_vertices, sizeof(plane_vertices));

    glGenVertexArrays(1, &plane_VAO);
    glBindVertexArray(plane_VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    while (window.isOpen())
	{
		glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        process_input(&window, deltaTime);

        // output fps: deltaTime * FPS = 1second
        //std::cout << 1/deltaTime << '\n';
        FPSsum += 1/deltaTime;
        FPSsum_count += 1;
        // debugging
        //std::cout << "(" << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << ")" <<std::endl;



        glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the building
        model_shader.enable();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down

		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), width / height, 0.1f, 100.0f);

        model_shader.SetMatrix4("model", model);
        model_shader.SetMatrix4("view", camera.getViewMatrix());
        model_shader.SetMatrix4("projection", projection);

        model_shader.SetVector3f("viewPos", camera.getPosition());
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
        model_shader.SetVector3f("spotLight.position", camera.getPosition());
        model_shader.SetVector3f("spotLight.direction", camera.mFront);
        model_shader.SetVector3f("spotLight.ambient", 0.01f, 0.01f, 0.01f);
		model_shader.SetVector3f("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        model_shader.SetVector3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
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
        ourModel.draw(model_shader);

        // -------------- ON THE FIELD POINT LIGHT -----------------
        border_shader.enable();

        glBindVertexArray(cube_VAO);

        for(int i=0; i<2; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, point_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));

            border_shader.SetMatrix4("model", model);
            border_shader.SetMatrix4("view", camera.getViewMatrix());
            border_shader.SetMatrix4("projection", projection);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // RED WINDOWS:
        glDisable(GL_CULL_FACE);
        glBindVertexArray(cube_VAO);
        lamp_shader.enable();

        std::map<float, glm::vec3> sortedWindowPositions;
        for(int i = 1; i < 4; i++)
        {
            float distance = glm::length(camera.getPosition() - plane_positions[i]);
            sortedWindowPositions[distance] = plane_positions[i];
        }

        for(std::map<float, glm::vec3>::reverse_iterator it = sortedWindowPositions.rbegin(); it != sortedWindowPositions.rend(); ++it)
        {
            model = glm::mat4();
            model = glm::translate(model, it->second);
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            lamp_shader.SetMatrix4("model", model);
            lamp_shader.SetMatrix4("view", camera.getViewMatrix());
            lamp_shader.SetMatrix4("projection", projection);
            lamp_shader.SetVector3f("glassColor", 1.0f, 1.0f, 1.0f);
            lamp_shader.SetInteger("texture_0", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, red_window);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glEnable(GL_CULL_FACE);



        // -------------- REFLECTIVE GROUND -----------------

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Only updates buffer stored stencil value with reference value if both depth and stencil tests pass.
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Test to be drawn: Always. Fragments should update the stencil buffer
        glStencilMask(0xFF);  // enable writing to the stencil buffer

        lamp_shader.enable();
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

        border_shader.enable();
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

        border_shader.enable();
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

        lamp_shader.enable();
        model = glm::mat4();
        model = glm::translate(model, newpos);
        model = glm::scale(model , glm::vec3(0.5f, -0.5f, 0.5f));
        lamp_shader.SetMatrix4("model", model);
        lamp_shader.SetMatrix4("view", camera.getViewMatrix());
        lamp_shader.SetMatrix4("projection", projection);
        lamp_shader.SetVector3f("glassColor", 0.5f, 0.5f, 0.5f);



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box);
        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_DEPTH_TEST);

        glStencilFunc(GL_EQUAL, 0, 0xFF);


        window.swapBuffers();
		glfwPollEvents();
	}

    std::cout << "Average FPS: " << FPSsum / FPSsum_count << std::endl;

	glfwTerminate();
	return 0;
}
