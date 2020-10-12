#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Cameras/FPS_Camera.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
FPS_Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Final Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	// ------------------------------------
	Shader ourShader("Shaders/BasicVertexShader.shader", "Shaders/BasicFragmentShader.shader");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	float singleCubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.5f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 0.5f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 0.5f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.5f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.5f,
	 0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.5f,
	 0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.5f,
	 0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.5f,
	-0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.5f,
	-0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.5f,

	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f
	};
	/*unsigned int indices[] = {
	   0, 1, 2, // first triangle
	};*/

	glm::vec3 rollingCubeLocation(-25.0f, 0.0f, -5.0f);
	bool rollingCubeFlag = false;

	glm::vec3 crushingCubesLocations[] = {
		glm::vec3(0.0f, 10.0f, -2.5f),
		glm::vec3(0.0f, -10.0f, -2.5f)
	};
	bool crushingCubesFlag = true;
	int aftermathDistance = rand() % 8 + 1;
	int numOfCrushes = 0;
	bool awayFromCrushFlag = true;
	bool towardsCrushFlag = false;

	glm::vec3 rotatingCubeLocations[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	bool rotatingCubesFlag = true;
	

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(singleCubeVertices), singleCubeVertices, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ourShader.use();
		// model view projection
		// ---------------------
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// render boxes
		// ------------
		glBindVertexArray(VAO);
		// ------------------- Rolling Cube Scene -------------------------
		if (!rollingCubeFlag) {
			glm::vec3 prevLocation(rollingCubeLocation.x, 0.0f, -5.0f);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, prevLocation);
			if (rollingCubeLocation.x < 25.0f) {
				model = glm::rotate(model, (float)glfwGetTime() * -glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				rollingCubeLocation.x += 0.015f;
			}
			else {
				rollingCubeFlag = true;
				crushingCubesFlag = false;
			}
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// ------------------ Crushing Cubes Scene ------------------------
		if (!crushingCubesFlag) {
			if (!towardsCrushFlag) {
				glm::vec3 prevLocation1(0.0f, crushingCubesLocations[0].y, -2.5f);
				glm::vec3 prevLocation2(0.0f, crushingCubesLocations[1].y, -2.5f);
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, prevLocation1);
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				model = glm::mat4(1.0f);
				model = glm::translate(model, prevLocation2);
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				if (crushingCubesLocations[0].y > 0.5f) {
					crushingCubesLocations[0].y -= 0.012f;
					crushingCubesLocations[1].y += 0.012f;
				}
				else {
					towardsCrushFlag = true;
					awayFromCrushFlag = false;
					numOfCrushes++;
					aftermathDistance = rand() % 4 + 1;
				}
			}
			if (!awayFromCrushFlag) {
				if (crushingCubesLocations[0].y < aftermathDistance) {
					crushingCubesLocations[0].y += 0.015f;
					crushingCubesLocations[1].y -= 0.015f;
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, crushingCubesLocations[0]);
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
					model = glm::mat4(1.0f);
					model = glm::translate(model, crushingCubesLocations[1]);
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				else {
					towardsCrushFlag = false;
					awayFromCrushFlag = true;
				}
			}
			if (numOfCrushes == 7) {
				crushingCubesFlag = true;
				rotatingCubesFlag = false;
			}
		}
		// ---------------------- Rotating Cubes Scene ---------------
		if (!rotatingCubesFlag) {
			for (int i = 0; i < 10; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, rotatingCubeLocations[i]);
				if (i%3==0)
					model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				else if (i%2==0)
					model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				else if (i%5==0)
					model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				else
					model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed, this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
