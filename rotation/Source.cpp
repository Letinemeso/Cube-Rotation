#include <glew.h>
#include <glfw3.h>

#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <mat4x4.hpp>

#include <iostream>
#include <thread>
#include <chrono>

int main() {
	glfwInit();

	GLFWwindow* wind = glfwCreateWindow(400, 400, "airstream", 0, 0);
	glfwMakeContextCurrent(wind);

	glfwSetWindowSizeLimits(wind, 400, 400, 400, 400);

	glewInit();

	std::cout << "welcome to SQRP - \"small cube-rotating program\"!\n\nhere's what you can do:\n"
		<< "W/A/S/D - movement\nup/down arrows - increase/decrease size\n"
		<< "1/2 - auto rotation around different axis\n3 - manual rotation witn Q/E\n"
		<< "I/K - change rotation speed\n\n";

	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);

	const char* vsh =
		"#version 330 core\n"
		"layout (location = 0) in vec4 pos;\n"
		"layout (location = 1) in vec3 inColor;\n"
		"uniform mat4x4 transMatrix;\n"
		"uniform mat4x4 orthoMatrix;\n"
		"uniform mat4x4 sizeMatrix;\n"
		"uniform mat4x4 rotationMatrix;\n"
		"out vec3 color;\n"
		"void main(){\n"
		"color = inColor;\n"
		"gl_Position =  orthoMatrix * transMatrix * rotationMatrix * sizeMatrix * pos;\n"
		"}";

	const char* fsh =
		"#version 330 core\n"
		"in vec3 color;\n"
		"out vec4 outColor;\n"
		"void main(){\n"
		"outColor = vec4(color, 1.0f);\n"
		"}";

	unsigned int vShader;
	unsigned int fShader;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsh, 0);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsh, 0);
	glCompileShader(fShader);

	unsigned int prog;
	prog = glCreateProgram();

	glAttachShader(prog, vShader);
	glAttachShader(prog, fShader);

	glLinkProgram(prog);
	glUseProgram(prog);

	//////////////////////////////////////

	glm::mat4x4 ortho = glm::ortho(0.0f, 400.0f, 0.0f, 400.0f, 400.0f, 0.0f);

	glm::mat4x4 trans = glm::mat4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		200.0f, 200.0f, -200.0f, 1.0f
	);

	glm::mat4x4 size = glm::mat4x4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	glm::vec3 vec(1.0f, 1.0f, 1.0f);
	glm::mat4x4 rotation = glm::rotate(0.0f, vec);

	//CUBE COORDINATES
	float coordsCenterX = 0.0f;
	float coordsCenterY = 0.0f;
	float coordsCenterZ = 0.0f;
	float coords[] = {
		//FRONT SIDE
		coordsCenterX + 50.0f, coordsCenterY + 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY - 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY - 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY + 50.0f, coordsCenterZ + 50.0f, 1.0f,

		//BACK SIDE
		coordsCenterX - 50.0f, coordsCenterY + 50.0f, coordsCenterZ - 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY - 50.0f, coordsCenterZ - 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY - 50.0f, coordsCenterZ - 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY + 50.0f, coordsCenterZ - 50.0f, 1.0f,

		//TOP SIDE
		coordsCenterX + 50.0f, coordsCenterY + 50.0f, coordsCenterZ - 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY + 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY + 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY + 50.0f, coordsCenterZ - 50.0f, 1.0f,

		//BOTTOM SIDE
		coordsCenterX - 50.0f, coordsCenterY - 50.0f, coordsCenterZ - 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY - 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY - 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY - 50.0f, coordsCenterZ - 50.0f, 1.0f,

		//LEFT SIDE
		coordsCenterX - 50.0f, coordsCenterY + 50.0f, coordsCenterZ - 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY + 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY - 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX - 50.0f, coordsCenterY - 50.0f, coordsCenterZ - 50.0f, 1.0f,

		//RIGHT SIDE
		coordsCenterX + 50.0f, coordsCenterY - 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY + 50.0f, coordsCenterZ + 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY + 50.0f, coordsCenterZ - 50.0f, 1.0f,
		coordsCenterX + 50.0f, coordsCenterY - 50.0f, coordsCenterZ - 50.0f, 1.0f,
	};

	//CUBE COLORS
	float colors[] = {
		1.0f, 0.4f, 0.0f,	//ORANGE
		1.0f, 0.4f, 0.0f,
		1.0f, 0.4f, 0.0f,
		1.0f, 0.4f, 0.0f,

		1.0f, 0.0f, 0.0f,	//RED
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,	//YELLOW
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.8f, 0.0f, 0.8f,	//PURPLE
		0.8f, 0.0f, 0.8f,
		0.8f, 0.0f, 0.8f,
		0.8f, 0.0f, 0.8f,

		0.0f, 1.0f, 0.0f,	//GREEN
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.5f, 0.0f, 1.0f,	//BLUE DA BU DE DA BU DAI
		0.5f, 0.0f, 1.0f,
		0.5f, 0.0f, 1.0f,
		0.5f, 0.0f, 1.0f
	};

	//VERTICES SETUP
	unsigned int attArray;
	glGenVertexArrays(1, &attArray);
	glBindVertexArray(attArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	unsigned int coordBuffer;
	glGenBuffers(1, &coordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
	glBufferData(GL_ARRAY_BUFFER, 96 * sizeof(float), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	coordsCenterX = 200.0f;
	coordsCenterY = 200.0f;
	coordsCenterZ = -200.0f;

	unsigned int colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, /*72*/73 * sizeof(float), colors, GL_STATIC_DRAW);		//PROBLEM PROBLEM PROBLEM PROBLEM PROBLEM
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);					/*there should be buffer for 72 floats, but in that case last vertex doesn't draw and stays black.*/

	//UNIFORM SETUP
	unsigned int transLocation = glGetUniformLocation(prog, "transMatrix");
	glUniformMatrix4fv(transLocation, 1, GL_FALSE, &trans[0][0]);

	unsigned int orthoLocation = glGetUniformLocation(prog, "orthoMatrix");
	glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, &ortho[0][0]);

	unsigned int sizeLocation = glGetUniformLocation(prog, "sizeMatrix");
	glUniformMatrix4fv(sizeLocation, 1, GL_FALSE, &size[0][0]);

	unsigned int rotLocation = glGetUniformLocation(prog, "rotationMatrix");		//////
	glUniformMatrix4fv(rotLocation, 1, GL_FALSE, &rotation[0][0]);

	//STUFF
	float angle = 0.0f;
	float speed = 0.01f;
	int keys[GLFW_KEY_LAST];
	int keysForEE[GLFW_KEY_LAST];
	int mode = 1;
	float maxSpeed = 0.2f;

	int coinsedence = 0;
	int repeat[5] = { 0, 0, 0, 0, 0 };

	//MAIN CYCLE
	while (!glfwWindowShouldClose(wind)) {
		glClear(GL_COLOR_BUFFER_BIT);

		//MOVEMENT
		if (glfwGetKey(wind, GLFW_KEY_D) == 1) {
			if (coordsCenterX < 400.0f) {
				coordsCenterX += 3.0f;
				trans[3][0] += 3.0f;
				glUniformMatrix4fv(transLocation, 1, GL_FALSE, &trans[0][0]);
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_A) == 1) {
			if (coordsCenterX > 0.0f) {
				coordsCenterX -= 3.0f;
				trans[3][0] -= 3.0f;
				glUniformMatrix4fv(transLocation, 1, GL_FALSE, &trans[0][0]);
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_W) == 1) {
			if (coordsCenterY < 400.0f) {
				coordsCenterY += 3.0f;
				trans[3][1] += 3.0f;
				glUniformMatrix4fv(transLocation, 1, GL_FALSE, &trans[0][0]);
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_S) == 1) {
			if (coordsCenterY > 0.0f) {
				coordsCenterY -= 3.0f;
				trans[3][1] -= 3.0f;
				glUniformMatrix4fv(transLocation, 1, GL_FALSE, &trans[0][0]);
			}
		}

		//SIZE CONTROL
		if (glfwGetKey(wind, GLFW_KEY_UP) == 1) {
			if (size[0][0] < 2.39) {
				size[0][0] += 0.01f;
				size[1][1] += 0.01f;
				size[2][2] += 0.01f;

				glUniformMatrix4fv(sizeLocation, 1, GL_FALSE, &size[0][0]);
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_DOWN) == 1) {
			if (size[0][0] > 0.11f) {
				size[0][0] -= 0.01f;
				size[1][1] -= 0.01f;
				size[2][2] -= 0.01f;

				glUniformMatrix4fv(sizeLocation, 1, GL_FALSE, &size[0][0]);
			}
		}

		//MODE SELECTION
		if ((glfwGetKey(wind, GLFW_KEY_1)) && (!keys[GLFW_KEY_1])) {
			mode = 1;
			vec = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		if ((glfwGetKey(wind, GLFW_KEY_2)) && (!keys[GLFW_KEY_2])) {
			mode = 2;
			vec = glm::vec3(-1.0f, 1.0f, 1.0f);
		}
		if ((glfwGetKey(wind, GLFW_KEY_3)) && (!keys[GLFW_KEY_3])) {
			mode = 3;
		}

		//SPEED CHANGE
		if (glfwGetKey(wind, GLFW_KEY_I)) {
			if (speed <= maxSpeed) {
				speed += 0.001f;
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_K)) {
			if (speed > 0.011f) {
				speed -= 0.001f;
			}
		}

		//MODE-DEPENDING ACTION
		if ((mode == 1) || (mode == 2)) {
			angle += speed;
			if (angle > 6.28f) {
				angle = 0.0f;
			}

			rotation = glm::rotate(angle, vec);

			glUniformMatrix4fv(rotLocation, 1, GL_FALSE, &rotation[0][0]);
		}
		if (mode == 3) {
			if (glfwGetKey(wind, GLFW_KEY_E) == 1) {
				angle += speed;
				if (angle > 6.28f) {
					angle = 0.0f;
				}

				rotation = glm::rotate(angle, vec);

				glUniformMatrix4fv(rotLocation, 1, GL_FALSE, &rotation[0][0]);
			}
			if (glfwGetKey(wind, GLFW_KEY_Q) == 1) {
				angle -= speed;
				if (angle < 0.0f) {
					angle = 6.28f;
				}

				rotation = glm::rotate(angle, vec);

				glUniformMatrix4fv(rotLocation, 1, GL_FALSE, &rotation[0][0]);
			}
		}

		//SMALL EASTER EGG :D
		if (coinsedence != -1) {
			int onesCount = 0;
			for (int i = 0; i < GLFW_KEY_LAST; ++i) {
				keysForEE[i] = glfwGetKey(wind, i);
			}
			for (int i = 0; i < GLFW_KEY_LAST; ++i) {
				if (keysForEE[i] == 1) {
					++onesCount;
				}
			}

			if ((glfwGetKey(wind, GLFW_KEY_S)) && (!keys[GLFW_KEY_S])) {
				if (coinsedence == 0) {
					coinsedence = 1;
				}
				++repeat[0];
			}
			if ((glfwGetKey(wind, GLFW_KEY_N)) && (!keys[GLFW_KEY_N])) {
				if (coinsedence == 1) {
					coinsedence = 2;
				}
				++repeat[1];
			}
			if ((glfwGetKey(wind, GLFW_KEY_A)) && (!keys[GLFW_KEY_A])) {
				if (coinsedence == 2) {
					coinsedence = 3;
				}
				++repeat[2];
			}
			if ((glfwGetKey(wind, GLFW_KEY_R)) && (!keys[GLFW_KEY_R])) {
				if (coinsedence == 3) {
					coinsedence = 4;
				}
				++repeat[3];
			}
			if ((glfwGetKey(wind, GLFW_KEY_E)) && (!keys[GLFW_KEY_E])) {
				if (coinsedence == 4) {
					coinsedence = 5;
				}
				++repeat[4];
			}

			if (onesCount != 0) {
				if ((keysForEE[GLFW_KEY_S] == 1) || (keysForEE[GLFW_KEY_N] == 1) || (keysForEE[GLFW_KEY_A] == 1) || (keysForEE[GLFW_KEY_R] == 1) || (keysForEE[GLFW_KEY_E] == 1)) {
					/*if ((keysForEE[i] == GLFW_KEY_S) && (coinsedence == 0)) {
						break;
					}
					if ((keysForEE[i] == GLFW_KEY_N) && (coinsedence == 1)) {
						break;
					}
					if ((keysForEE[i] == GLFW_KEY_A) && (coinsedence == 2)) {
						break;
					}
					if ((keysForEE[i] == GLFW_KEY_R) && (coinsedence == 3)) {
						break;
					}
					if ((keysForEE[i] == GLFW_KEY_E) && (coinsedence == 4)) {
						break;
					}*/
					if ((keysForEE[GLFW_KEY_S] == 1) && (coinsedence == 1)) {
					}
					else if ((keysForEE[GLFW_KEY_N] == 1) && (coinsedence == 2)) {
					}
					else if ((keysForEE[GLFW_KEY_A] == 1) && (coinsedence == 3)) {
					}
					else if ((keysForEE[GLFW_KEY_R] == 1) && (coinsedence == 4)) {
					}
					else if ((keysForEE[GLFW_KEY_E] == 1) && (coinsedence == 5)) {
					}
					else {
						coinsedence = 0;
					}
				}
				else {
					coinsedence = 0;
				}

			}
			
			for (int i = 0; i < 5; ++i) {
				if (repeat[i] >= 2) {
					coinsedence = 0;
					for (int j = 0; j < 5; ++j) {
						repeat[j] = 0;
					}
					break;
				}else if (i >= 1) {
					if ((repeat[i] == 1) && (repeat[i - 1] == 0)) {
						coinsedence = 0;
						for (int j = 0; j < 5; ++j) {
							repeat[j] = 0;
						}
						break;
					}
				}
			}

			if (coinsedence == 5) {
				std::cout << "you found my easter egg!\nmax rotation speed is now nearly unlimited!\n\n";
				maxSpeed = 99999.0f;
				coinsedence = -1;
			}
		}
		
		for (int i = 0; i < GLFW_KEY_LAST; ++i) {
			keys[i] = glfwGetKey(wind, i);
		}

		glDrawArrays(GL_QUADS, 0, 24);

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));

		glfwPollEvents();
		glfwSwapBuffers(wind);
	}

	std::cout << (coinsedence != -1 ? "thanks for observing my small cube-rotating program!\nbtw, you missed my easter egg :)\n-Snare\n\n" : 
		"thanks for observing my small cube-rotating program!\n-.Snare\n\n");

	glfwTerminate();

	while(true){}

	return 0;
}