#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <random>
#include <cmath>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Shape.h"

#define Quiz15
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60

GLuint vao, vbo[2], ebo, axesVAO, axesVBO;
void drawAxes();
void convertXY(int x, int y, float& fx, float& fy);
void UpdateBuffer();
void InitBuffer();
void InitializeData();
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKey(int key, int x, int y);

GLvoid Timer(int value);

float bGCr = 1.0, bGCg = 1.0, bGCb = 1.0;
int buffersize;
GLuint shaderPID;

void drawAxes()
{
	glm::vec3 axesVertices[] = {
		glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 시작점 (빨간색)
		glm::vec3(1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 끝점
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // y축 시작점 (녹색)
		glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  // y축 끝점
		glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), // z축 시작점 (파랑)
		glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),  // z축 끝점
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
uniform_int_distribution<int> randcube(0, 5);
uniform_int_distribution<int> randpyramid(0, 3);

static const int index = 10;

Shape cube[6];
Shape pyramid[4];
int drawidx = 0;
int drawidx_2 = -1;
int drawshape = 0;
#endif // Quiz13

#ifdef Quiz14
enum Animation
{
	NONE, ROTATE_X, ROTATE_Y
};

static const int index = 11;

Shape cube[6];
Shape pyramid[5];
Animation anim;
int drawshape = 0;
GLenum drawmode = GL_FILL;
bool depthtest = true;
#endif // Quiz14

#ifdef Quiz15
static const int index = 4;
Shape cube, cone, sphere, cylinder;

Shape CreateCube(float sideLength)
{
	glm::vec3 cubeCoords[MAX_POINTS];
	float halfSide = sideLength / 2.0f;
	int index = 0;

	// 육면체의 각 면을 정의 (삼각형 두 개씩, 시계 방향)

	// 앞면
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, halfSide);

	// 뒷면
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, -halfSide);

	// 왼쪽 면
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, halfSide);

	// 오른쪽 면
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, -halfSide);

	// 위쪽 면
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, halfSide, -halfSide);

	// 아래쪽 면
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, halfSide);
	cubeCoords[index++] = glm::vec3(-halfSide, -halfSide, -halfSide);
	cubeCoords[index++] = glm::vec3(halfSide, -halfSide, -halfSide);

	// Shape 객체 생성 (육면체의 각 면에 해당하는 좌표 포함)
	Shape cube(index, cubeCoords);
	return cube;
}

Shape CreateCone(float radius, float height, int segments)
{
	glm::vec3 coneCoords[MAX_POINTS];
	int index = 0;

	// 꼭짓점 설정 (원뿔의 위쪽 점)
	coneCoords[index++] = glm::vec3(0.0f, height, 0.0f);

	// 밑면 점들 (삼각형 팬으로 측면을 그리기 위해 추가)
	for (int i = 0; i <= segments; ++i)
	{
		float angle = 2.0f * M_PI * i / segments;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		coneCoords[index++] = glm::vec3(x, 0.0f, z);
	}

	// Shape 객체 생성 (측면의 삼각형 팬을 위한 점 포함)
	Shape cone(index, coneCoords);
	return cone;
}
#endif // Quiz15


void InitializeData()
{
#ifdef Quiz13
	glm::vec3 shapecoord[4];
	float rotation = -10.0f;

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, -0.5);
	shapecoord[3] = glm::vec3(-0.5, 0.5, -0.5);
	cube[0] = Shape(4, shapecoord);
	cube[0].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, -0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, -0.5, 0.5);
	cube[1] = Shape(4, shapecoord);
	cube[1].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(-0.5, 0.5, -0.5);
	shapecoord[2] = glm::vec3(-0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, -0.5, 0.5);
	cube[2] = Shape(4, shapecoord);
	cube[2].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, 0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(0.5, -0.5, 0.5);
	cube[3] = Shape(4, shapecoord);
	cube[3].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(-0.5, 0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, 0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, 0.5, 0.5);
	cube[4] = Shape(4, shapecoord);
	cube[4].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(-0.5, -0.5, 0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, 0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, 0.5, 0.5);
	cube[5] = Shape(4, shapecoord);
	cube[5].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.0, -0.5, 0.5);
	pyramid[0] = Shape(3, shapecoord);
	pyramid[0].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.0, 0.5, 0.0);
	pyramid[1] = Shape(3, shapecoord);
	pyramid[1].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.0, 0.5, 0.0);
	shapecoord[2] = glm::vec3(0.0, -0.5, 0.5);
	pyramid[2] = Shape(3, shapecoord);
	pyramid[2].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	shapecoord[0] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.0, -0.5, 0.5);
	shapecoord[2] = glm::vec3(0.0, 0.5, 0.0);
	pyramid[3] = Shape(3, shapecoord);
	pyramid[3].rotation = glm::radians(glm::vec3(rotation, rotation, 0.0));

	buffersize = 10;
#endif // Quiz13
#ifdef Quiz14
	glm::vec3 shapecoord[4];
	float rotationX = 30.0f;
	float rotationY = 30.0f;

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, -0.5);
	shapecoord[3] = glm::vec3(-0.5, 0.5, -0.5);
	cube[0] = Shape(4, shapecoord);

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, -0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, -0.5, 0.5);
	cube[1] = Shape(4, shapecoord);

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(-0.5, 0.5, -0.5);
	shapecoord[2] = glm::vec3(-0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, -0.5, 0.5);
	cube[2] = Shape(4, shapecoord);

	shapecoord[0] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, 0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(0.5, -0.5, 0.5);
	cube[3] = Shape(4, shapecoord);

	shapecoord[0] = glm::vec3(-0.5, 0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, 0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, 0.5, 0.5);
	cube[4] = Shape(4, shapecoord);

	shapecoord[0] = glm::vec3(-0.5, -0.5, 0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, 0.5);
	shapecoord[2] = glm::vec3(0.5, 0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, 0.5, 0.5);
	cube[5] = Shape(4, shapecoord);

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, -0.5, 0.5);
	shapecoord[3] = glm::vec3(-0.5, -0.5, 0.5);
	pyramid[0] = Shape(4, shapecoord);

	shapecoord[0] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[1] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.0, 0.5, 0.0);
	pyramid[1] = Shape(3, shapecoord);

	shapecoord[0] = glm::vec3(0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.5, -0.5, 0.5);
	shapecoord[1] = glm::vec3(0.0, 0.5, 0.0);
	pyramid[2] = Shape(3, shapecoord);

	shapecoord[0] = glm::vec3(0.5, -0.5, 0.5);
	shapecoord[1] = glm::vec3(-0.5, -0.5, 0.5);
	shapecoord[2] = glm::vec3(0.0, 0.5, 0.0);
	pyramid[3] = Shape(3, shapecoord);

	shapecoord[0] = glm::vec3(-0.5, -0.5, 0.5);
	shapecoord[1] = glm::vec3(-0.5, -0.5, -0.5);
	shapecoord[2] = glm::vec3(0.0, 0.5, 0.0);
	pyramid[4] = Shape(3, shapecoord);

	for (int i = 0; i < 5; ++i)
	{
		cube[i].rotation = glm::radians(glm::vec3(rotationX, rotationY, 0.0));
		pyramid[i].rotation = glm::radians(glm::vec3(rotationX, rotationY, 0.0));
	}
	cube[5].rotation = glm::radians(glm::vec3(rotationX, rotationY, 0.0));

	anim = NONE;
	buffersize = 11;
#endif // Quiz14
#ifdef Quiz15
	cube = CreateCube(0.3f);
	cube.translation = glm::vec3(-0.5f, 0.0f, 0.0f);
	cone = CreateCone(0.3f, 0.5f, 60);
	cone.translation = glm::vec3(0.5f, 0.0f, 0.0f);

	buffersize = 4;
#endif // Quiz15

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
	glutSpecialFunc(SpecialKey);
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
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f); //카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

#ifdef Quiz15
	cameraPos = glm::vec3(1.0f, 2.0f, 2.0f);
#endif // Quiz15

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform"); //뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 50.0f); //투영 공간 설정 : [-100.0, 100.0]
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(axesVAO);
	glm::mat4 axesTransform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(axesTransform));
	glDrawArrays(GL_LINES, 0, 6);


	glBindVertexArray(vao);

#ifdef Quiz13
	if(drawshape == 1)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cube[drawidx].translation);
		model = glm::rotate(model, cube[drawidx].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cube[drawidx].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cube[drawidx].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cube[drawidx].scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

		UpdateBuffer();

		cube[drawidx].Draw(drawidx);

		if (drawidx_2 >= 0)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cube[drawidx_2].translation);
			model = glm::rotate(model, cube[drawidx_2].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cube[drawidx_2].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cube[drawidx_2].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, cube[drawidx_2].scaling);

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

			UpdateBuffer();

			cube[drawidx_2].Draw(drawidx_2);
		}
	}
	else if(drawshape == 2)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pyramid[drawidx].translation);
		model = glm::rotate(model, pyramid[drawidx].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, pyramid[drawidx].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, pyramid[drawidx].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, pyramid[drawidx].scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

		UpdateBuffer();

		pyramid[drawidx].Draw(drawidx + 6);

		if (drawidx_2 >= 0)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pyramid[drawidx_2].translation);
			model = glm::rotate(model, pyramid[drawidx_2].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, pyramid[drawidx_2].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, pyramid[drawidx_2].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, pyramid[drawidx_2].scaling);

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

			UpdateBuffer();

			pyramid[drawidx_2].Draw(drawidx_2 + 6);
		}
	}
#endif // Quiz13
#ifdef Quiz14
	if (drawshape == 1)
	{
		for (int i = 0; i < 6; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cube[i].translation);
			model = glm::rotate(model, cube[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cube[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cube[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, cube[i].scaling);

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

			UpdateBuffer();

			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
			cube[i].Draw(i);
		}
	}
	else if (drawshape == 2)
	{
		for (int i = 0; i < 5; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pyramid[i].translation);
			model = glm::rotate(model, pyramid[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, pyramid[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, pyramid[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, pyramid[i].scaling);

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

			UpdateBuffer();

			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
			pyramid[i].Draw(i + 6);
		}
	}

#endif // Quiz14
#ifdef Quiz15
	UpdateBuffer();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cube.translation);
	model = glm::rotate(model, cube.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, cube.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, cube.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, cube.scaling);

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	cube.Draw(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, cone.translation);
	model = glm::rotate(model, cone.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, cone.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, cone.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, cone.scaling);

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

	cone.Draw(1);
#endif // Quiz15

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
		drawidx_2 = -1;
		break;
	case '2':
		drawshape = 1;
		drawidx = 1;
		drawidx_2 = -1;
		break;
	case '3':
		drawshape = 1;
		drawidx = 2;
		drawidx_2 = -1;
		break;
	case '4':
		drawshape = 1;
		drawidx = 3;
		drawidx_2 = -1;
		break;
	case '5':
		drawshape = 1;
		drawidx = 4;
		drawidx_2 = -1;
		break;
	case '6':
		drawshape = 1;
		drawidx = 5;
		drawidx_2 = -1;
		break;
	case '7':
		drawshape = 2;
		drawidx = 0;
		drawidx_2 = -1;
		break;
	case '8':
		drawshape = 2;
		drawidx = 1;
		drawidx_2 = -1;
		break;
	case '9':
		drawshape = 2;
		drawidx = 2;
		drawidx_2 = -1;
		break;
	case '0':
		drawshape = 2;
		drawidx = 3;
		drawidx_2 = -1;
		break;
	case 'c':
		drawshape = 1;
		drawidx = randcube(gen);
		do
		{
			drawidx_2 = randcube(gen);
		} while (drawidx == drawidx_2);
		break;
	case 't':
		drawshape = 2;
		drawidx = randpyramid(gen);
		do
		{
			drawidx_2 = randpyramid(gen);
		} while (drawidx == drawidx_2);
		break;
	default:
		break;
	}
#endif // Quiz13
#ifdef Quiz14
	switch (key)
	{
	case 'c':
		drawshape = 1;
		break;
	case 'p':
		drawshape = 2;
		break;
	case 'h':
		if (depthtest)
		{
			glDisable(GL_DEPTH_TEST);
			depthtest = false;
		}
		else
		{
			glEnable(GL_DEPTH_TEST);
			depthtest = true;
		}
		break;
	case 'w':
		if (drawmode == GL_FILL)
		{
			drawmode = GL_LINE;
		}
		else if(drawmode == GL_LINE)
		{
			drawmode = GL_FILL;
		}
		break;
	case 'x':
		anim = ROTATE_X;
		break;
	case 'y':
		anim = ROTATE_Y;
		break;
	case 's':
		InitializeData();
		break;
	default:
		break;
	}
#endif // Quiz14

	if(key == 'q')
		glutLeaveMainLoop();

	glutPostRedisplay();
}

GLvoid SpecialKey(int key, int x, int y)
{
#ifdef Quiz14
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if(drawshape == 1)
		{
			for (int i = 0; i < 6; ++i)
				cube[i].translation.x += 0.05f;
		}
		else if (drawshape == 2)
		{
			for (int i = 0; i < 5; ++i)
				pyramid[i].translation.x += 0.05f;
		}
		break;
	case GLUT_KEY_LEFT:
		if(drawshape == 1)
		{
			for (int i = 0; i < 6; ++i)
				cube[i].translation.x -= 0.05f;
		}
		else if (drawshape == 2)
		{
			for (int i = 0; i < 5; ++i)
				pyramid[i].translation.x -= 0.05f;
		}
		break;
	case GLUT_KEY_UP:
		if(drawshape == 1)
		{
			for (int i = 0; i < 6; ++i)
				cube[i].translation.y += 0.05f;
		}
		else if (drawshape == 2)
		{
			for (int i = 0; i < 5; ++i)
				pyramid[i].translation.y += 0.05f;
		}
		break;
	case GLUT_KEY_DOWN:
		if(drawshape == 1)
		{
			for (int i = 0; i < 6; ++i)
				cube[i].translation.y -= 0.05f;
		}
		else if (drawshape == 2)
		{
			for (int i = 0; i < 5; ++i)
				pyramid[i].translation.y -= 0.05f;
		}
		break;
	default:
		break;
	}
#endif // Quiz14

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float fx = 0.0, fy = 0.0;
	convertXY(x, y, fx, fy);

}

GLvoid Timer(int value)
{
#ifdef Quiz14
	switch (anim)
	{
	case ROTATE_X:
		if (drawshape == 1)
		{
			for (int i = 0; i < 6; ++i)
				cube[i].rotation.x += glm::radians(1.0f);
		}
		else if (drawshape == 2)
		{
			for (int i = 0; i < 5; ++i)
				pyramid[i].rotation.x += glm::radians(1.0f);
		}
		break;
	case ROTATE_Y:
		if (drawshape == 1)
		{
			for (int i = 0; i < 6; ++i)
				cube[i].rotation.y += glm::radians(1.0f);
		}
		else if (drawshape == 2)
		{
			for (int i = 0; i < 5; ++i)
				pyramid[i].rotation.y += glm::radians(1.0f);
		}
		break;
	default:
		break;
	}
#endif // Quiz14

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
	glBufferData(GL_ARRAY_BUFFER, buffersize * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//--- 변수 colors에서 버텍스 색상을 복사한다.
	glBufferData(GL_ARRAY_BUFFER, buffersize * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	GLuint indices[index * MAX_POINTS];
	for (int i = 0; i < buffersize * MAX_POINTS; i++)
	{
		indices[i] = i % MAX_POINTS;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	drawAxes();
}

void UpdateBuffer()
{
#ifdef Quiz13
	for (int i = 0; i < 6; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, i * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cube[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, i * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cube[i].shapecolor[0]));
	}
	for (int i = 0; i < 4; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 6) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(pyramid[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 6) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(pyramid[i].shapecolor[0]));
	}
#endif // Quiz13
#ifdef Quiz14
	for (int i = 0; i < 6; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, i * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cube[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, i * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cube[i].shapecolor[0]));
	}
	for (int i = 0; i < 5; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 6) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(pyramid[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 6) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(pyramid[i].shapecolor[0]));
	}
#endif // Quiz14
#ifdef Quiz15
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cube.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cube.shapecolor[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cone.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cone.shapecolor[0]));
#endif // Quiz15

}