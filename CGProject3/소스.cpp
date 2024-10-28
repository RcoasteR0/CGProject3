#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <random>
#include <cmath>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Shader.h"

#define Quiz13
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60

using namespace std;

random_device rd;
mt19937 gen(rd());

GLuint vao, vbo[2], ebo, axesVAO, axesVBO;
void RandomColor(float& colorR, float& colorG, float& colorB);
void drawAxes();
void convertXY(int x, int y, float& fx, float& fy);
void UpdateBuffer();
void InitBuffer();
void InitializeData();
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);

float bGCr = 1.0, bGCg = 1.0, bGCb = 1.0;
GLuint shaderPID;

class Shape
{
public:
	glm::vec3 shapecoord[4];
	glm::vec3 shapecolor[4];
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scaling;
	int points;

	Shape()
	{
		for (int i = 0; i < 4; ++i)
		{
			shapecoord[i].x = 0.0f;
			shapecoord[i].y = 0.0f;
			shapecoord[i].z = 0.0f;
			shapecolor[i].r = 0.0f;
			shapecolor[i].g = 0.0f;
			shapecolor[i].b = 0.0f;
		}
		translation = glm::vec3(0.0);
		rotation = glm::vec3(0.0);
		scaling = glm::vec3(0.0);
		points = 0;
	}

	Shape(int state, glm::vec3 coord[])
	{
		glm::vec3 color;
		RandomColor(color.r, color.g, color.b);

		for (int i = 0; i < 4; ++i)
		{
			if (i > state)
			{
				shapecoord[i].x = 0.0f;
				shapecoord[i].y = 0.0f;
				shapecoord[i].z = 0.0f;
				shapecolor[i].r = 0.0f;
				shapecolor[i].g = 0.0f;
				shapecolor[i].b = 0.0f;
			}
			else
			{
				shapecoord[i] = coord[i];
				shapecolor[i] = color;
			}
		}
		translation = glm::vec3(0.0);
		rotation = glm::vec3(0.0);
		scaling = glm::vec3(1.0);
		points = state;
	}

	Shape(int state, glm::vec3 coord[], glm::vec3 color)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (i > state)
			{
				shapecoord[i].x = 0.0f;
				shapecoord[i].y = 0.0f;
				shapecoord[i].z = 0.0f;
				shapecolor[i].r = 0.0f;
				shapecolor[i].g = 0.0f;
				shapecolor[i].b = 0.0f;
			}
			else
			{
				shapecoord[i] = coord[i];
				shapecolor[i] = color;
			}
		}
		translation = glm::vec3(0.0);
		rotation = glm::vec3(0.0);
		scaling = glm::vec3(1.0);
		points = state;
	}

	~Shape() {}

	void Draw(int i)
	{
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, points);
	}
};

void RandomColor(float& colorR, float& colorG, float& colorB)
{
	uniform_real_distribution<float> random(0, 1);
	colorR = random(gen);
	colorG = random(gen);
	colorB = random(gen);
}

void drawAxes()
{
	glm::vec3 axesVertices[] = {
		glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 시작점 (빨간색)
		glm::vec3(1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 끝점
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // y축 시작점 (녹색)
		glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)  // y축 끝점
	};
	glGenVertexArrays(1, &axesVAO);
	glGenBuffers(1, &axesVBO);

	glBindVertexArray(axesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);

	// 위치 속성
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 색상 속성
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

#ifdef Quiz13
Shape cube[6];
Shape pyramid[4];
Shape coordinate[2];
int drawidx = 0;
int drawshape = 0;
#endif // Quiz13

void InitializeData()
{
#ifdef Quiz13
	glm::vec3 cubecoord[4];
	float rotation = -10.0f;

	cubecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	cubecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	cubecoord[2] = glm::vec3(0.5, 0.5, -0.5);
	cubecoord[3] = glm::vec3(-0.5, 0.5, -0.5);
	cube[0] = Shape(4, cubecoord);
	cube[0].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	cubecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	cubecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	cubecoord[2] = glm::vec3(0.5, -0.5, 0.5);
	cubecoord[3] = glm::vec3(-0.5, -0.5, 0.5);
	cube[1] = Shape(4, cubecoord);
	cube[1].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	cubecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	cubecoord[1] = glm::vec3(-0.5, 0.5, -0.5);
	cubecoord[2] = glm::vec3(-0.5, 0.5, 0.5);
	cubecoord[3] = glm::vec3(-0.5, -0.5, 0.5);
	cube[2] = Shape(4, cubecoord);
	cube[2].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	cubecoord[0] = glm::vec3(0.5, -0.5, -0.5);
	cubecoord[1] = glm::vec3(0.5, 0.5, -0.5);
	cubecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	cubecoord[3] = glm::vec3(0.5, -0.5, 0.5);
	cube[3] = Shape(4, cubecoord);
	cube[3].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	cubecoord[0] = glm::vec3(-0.5, 0.5, -0.5);
	cubecoord[1] = glm::vec3(0.5, 0.5, -0.5);
	cubecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	cubecoord[3] = glm::vec3(-0.5, 0.5, 0.5);
	cube[4] = Shape(4, cubecoord);
	cube[4].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	cubecoord[0] = glm::vec3(-0.5, -0.5, 0.5);
	cubecoord[1] = glm::vec3(0.5, -0.5, 0.5);
	cubecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	cubecoord[3] = glm::vec3(-0.5, 0.5, 0.5);
	cube[5] = Shape(4, cubecoord);
	cube[5].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

#endif // Quiz13

}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Example");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	InitializeData();
	glEnable(GL_DEPTH_TEST);

	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutTimerFunc(1000 / FPS, Timer, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	glBindVertexArray(axesVAO);
	glm::mat4 axesTransform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(axesTransform));
	glDrawArrays(GL_LINES, 0, 4);

	glBindVertexArray(vao);

#ifdef Quiz13
	if(true)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cube[drawidx].translation);
		model = glm::rotate(model, cube[drawidx].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cube[drawidx].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cube[drawidx].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cube[drawidx].scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

		UpdateBuffer();

		cube[0].Draw(drawidx);
	}
#endif // Quiz13


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void convertXY(int x, int y, float& fx, float& fy)
{
	int w = WINDOW_WIDTH;
	int h = WINDOW_HEIGHT;
	fx = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	fy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
#ifdef Quiz13
	switch (key)
	{
	case '1':
		drawshape = 1;
		drawidx = 0;
		break;
	case '2':
		drawshape = 1;
		drawidx = 1;
		break;
	case '3':
		drawshape = 1;
		drawidx = 2;
		break;
	case '4':
		drawshape = 1;
		drawidx = 3;
		break;
	case '5':
		drawshape = 1;
		drawidx = 4;
		break;
	case '6':
		drawshape = 1;
		drawidx = 5;
		break;
	default:
		break;
	}
#endif // Quiz13

	if(key == 'q')
		glutLeaveMainLoop();

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float fx = 0.0, fy = 0.0;
	convertXY(x, y, fx, fy);

}

GLvoid Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, Timer, 1);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기


	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
#ifdef Quiz13
	glBufferData(GL_ARRAY_BUFFER, 10 * 4 * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
#endif // Quiz13

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//--- 변수 colors에서 버텍스 색상을 복사한다.
#ifdef Quiz13
	glBufferData(GL_ARRAY_BUFFER, 10 * 4 * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
#endif //Quiz13

	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
#ifdef Quiz13
	GLuint indices[10 * 4];
	for (int i = 0; i < 10 * 4; i++)
	{
		indices[i] = i % 4;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
#endif Quiz13
	drawAxes();
}

void UpdateBuffer()
{
#ifdef Quiz13
	for (int i = 0; i < 6; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, i * 4 * 3 * sizeof(GLfloat), 4 * 3 * sizeof(GLfloat), glm::value_ptr(cube[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, i * 4 * 3 * sizeof(GLfloat), 4 * 3 * sizeof(GLfloat), glm::value_ptr(cube[i].shapecolor[0]));
	}
#endif // Quiz13
}