#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Mat4.h"
#include "Mat3.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "shaderLoader.h"
#include "TextureLoading.h"
#include "VertexArraysObject.h"
#include "VertexBufferObjects.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

int main() {
	//начало
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SkyBox",
		sf::Style::Titlebar | sf::Style::Close, settings);

	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

	glewExperimental = GL_TRUE;
	glEnable(GL_DEPTH_TEST);

	if (GLEW_OK != glewInit()) {
		std::cout << "Error:: glew not init =(" << std::endl;
		return -1;
	}
	//

	//камера
	Camera camera;
	//

	//шейдеры
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    Shader modelShader("shaders/model.vs", "shaders/model.fs");
	//

    //модели
    Model modelSign("objects/clean/Half-Dirty.obj");
    Model modelBomb("objects/bomb/Bomb.obj");
    //
	
	//вершины скайбокса
    float skyboxVertices[] = {       
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    //позиции моделей
    Vec3 positionModel[] = {
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(2.0f, 0.0f, 2.0f),
    };
	//

    //скайбокс
    VertexArraysObject VAO;
    VAO.Binding();
    VertexBufferObjects VBO(skyboxVertices, sizeof(skyboxVertices) / sizeof(float));
    VAO.VertexAttribPointer(0, 3, 3 * sizeof(float), (void*)0);
    //

	//для каждой грани куба картинка
	std::vector<std::string> skybox
	{
		"skybox/right.png",
		"skybox/left.png",
		"skybox/top.png",
		"skybox/bottom.png",
		"skybox/front.png",
		"skybox/back.png"
	};
    //

    //текстура для скайбокca
	TextureLoading textureSkyBox(skybox);
    //

	skyboxShader.Use();
	skyboxShader.SetInt("skybox", 0);


	bool isGo = true;
	while (true) {
		//обработка движений камеры
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGo = false;
				break;
			case sf::Event::KeyPressed:
				camera.ReadInput();
				break;
			case sf::Event::MouseMoved:
				camera.MouseInput(window);
				break;
			default:
				break;
			}
		}
		//

		//
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//

        	modelShader.Use();
        	Mat4 view(camera.GetViewMatrix());
        	Mat4 projection(camera.GetProjectionMatrix());
        	modelShader.SetMat4("projection", projection);
        	modelShader.SetMat4("view", view);

        	//рендеринг моделей
        	Mat4 model(1.0f);
        	model = model.Translate(positionModel[0]);
        	modelShader.SetMat4("model", model);
        	modelSign.Draw(modelShader);


        	Mat4 model1(1.0f);
        	model1 = model1.Translate(positionModel[1]);
        	model1 = model1.Scale(Vec3(2.0f));
        	modelShader.SetMat4("model", model1);
        	modelBomb.Draw(modelShader);
        	//

        	//скайбокс
        	glDepthFunc(GL_LEQUAL);
        	skyboxShader.Use();
        	view = camera.GetViewSkyBox();
        	skyboxShader.SetMat4("view", view);
        	skyboxShader.SetMat4("projection", projection);

        	VAO.Binding();
        	textureSkyBox.Binding(0);
        	glDrawArrays(GL_TRIANGLES, 0, 36);
        	VAO.Binding(0);
        	glDepthFunc(GL_LESS);
        	//

		window.display();
	}

	window.close();
	return 0;
}
