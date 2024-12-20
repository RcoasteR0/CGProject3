#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <random>
#include <cmath>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Shape.h"

#define Quiz18
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
enum Animation
{
	NONE, ROTATE_X, ROTATE_Y, REVOLVE_Y, SPIRAL, LINEAR, REVOLUTION_CURVE, REVOLUTION_STRAIGHT, REVOLUTION_WITHSCALE 
};

static const int index = 5;

Shape cube, cone, sphere, cylinder, spiral;
float scaleByzero;
Animation anim;
int rotatedir;
int select;
int progress;
GLenum drawmode = GL_FILL;
bool alter;

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

Shape CreateSphere(float radius, int latitudeSegments, int longitudeSegments)
{
	glm::vec3 sphereCoords[MAX_POINTS];

	int index = 0;

	// 위도와 경도를 따라 구 표면 점 생성
	for (int lat = 0; lat <= latitudeSegments; ++lat)
	{
		float theta = lat * M_PI / latitudeSegments;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int lon = 0; lon <= longitudeSegments; ++lon)
		{
			float phi = lon * 2.0f * M_PI / longitudeSegments;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			// x, y, z 좌표계로 변환하여 점 위치 설정
			float x = radius * cosPhi * sinTheta;
			float y = radius * cosTheta;
			float z = radius * sinPhi * sinTheta;

			sphereCoords[index++] = glm::vec3(x, y, z);
		}
	}

	// 생성된 좌표로 Shape 객체 생성
	Shape sphere(index, sphereCoords);
	return sphere;
}

Shape CreateCylinder(float radius, float height, int segments)
{
	glm::vec3 cylinderCoords[MAX_POINTS];
	int index = 0;

	// 하단 중심점
	cylinderCoords[index++] = glm::vec3(0.0f, 0.0f, 0.0f);

	// 하단 밑면 점들
	for (int i = 0; i <= segments; ++i)
	{
		float angle = 2.0f * M_PI * i / segments;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		cylinderCoords[index++] = glm::vec3(x, 0.0f, z);
	}

	// 상단 중심점
	cylinderCoords[index++] = glm::vec3(0.0f, height, 0.0f);

	// 상단 밑면 점들
	for (int i = 0; i <= segments; ++i)
	{
		float angle = 2.0f * M_PI * i / segments;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		cylinderCoords[index++] = glm::vec3(x, height, z);
	}

	// 측면 (GL_TRIANGLE_STRIP을 사용하여 원통 벽면 생성)
	for (int i = 0; i <= segments; ++i)
	{
		float angle = 2.0f * M_PI * i / segments;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		cylinderCoords[index++] = glm::vec3(x, 0.0f, z);          // 하단 점
		cylinderCoords[index++] = glm::vec3(x, height, z);        // 상단 점
	}

	// Shape 객체 생성 (원통의 점들을 포함)
	Shape cylinder(index, cylinderCoords);
	return cylinder;
}

Shape CreateSpiral(float radius, int segments)
{
	glm::vec3 spiralCoords[MAX_POINTS];
	int index = 0;
	float r = 0.0f;

	for (int i = 0; i <= segments; ++i)
	{
		float angle = 8.0 * M_PI * i / segments;
		float x = r * cos(angle);
		float z = r * sin(angle);
		spiralCoords[i] = glm::vec3(x, 0.0f, z);
		r += radius;
	}

	Shape spiral(segments, spiralCoords);
	return spiral;
}
#endif // Quiz15

#ifdef Quiz17
enum PyramidAnimation { NONE, OPEN, CLOSE, OPEN_EACH, CLOSE_EACH };

static const int index = 11;

Shape cube[6];
Shape pyramid[5];
float cube_transform[3];
float pyramid_transform[4];
int drawshape = 1;
GLenum drawmode = GL_FILL;
bool depthtest = true;
bool perspective = false;
bool rotationY = false;
bool cube_rotatetop;
bool cube_openfront;
bool cube_openside;
bool cube_openback;
PyramidAnimation pyramid_anim;

void CreateCube(Shape cube[], float sideLength = 0.5f)
{
	glm::vec3 cubeCoords[6][MAX_POINTS];

	float halfSide = sideLength / 2.0f;
	int index = 0;

	// 육면체의 각 면을 정의 (삼각형 두 개씩, 시계 방향)

	// 앞면
	cubeCoords[0][index++] = glm::vec3(-halfSide, 0, halfSide);
	cubeCoords[0][index++] = glm::vec3(halfSide, 0, halfSide);
	cubeCoords[0][index++] = glm::vec3(halfSide, sideLength, halfSide);
	cubeCoords[0][index++] = glm::vec3(-halfSide, sideLength, halfSide);
	index = 0;

	// 뒷면
	cubeCoords[1][index++] = glm::vec3(-halfSide, 0, -halfSide);
	cubeCoords[1][index++] = glm::vec3(halfSide, 0, -halfSide);
	cubeCoords[1][index++] = glm::vec3(halfSide, sideLength, -halfSide);
	cubeCoords[1][index++] = glm::vec3(-halfSide, sideLength, -halfSide);
	index = 0;

	// 왼쪽 면
	cubeCoords[2][index++] = glm::vec3(-halfSide, 0, -halfSide);
	cubeCoords[2][index++] = glm::vec3(-halfSide, sideLength, -halfSide);
	cubeCoords[2][index++] = glm::vec3(-halfSide, sideLength, halfSide);
	cubeCoords[2][index++] = glm::vec3(-halfSide, 0, halfSide);
	index = 0;

	// 오른쪽 면
	cubeCoords[3][index++] = glm::vec3(halfSide, 0, -halfSide);
	cubeCoords[3][index++] = glm::vec3(halfSide, 0, halfSide);
	cubeCoords[3][index++] = glm::vec3(halfSide, sideLength, halfSide);
	cubeCoords[3][index++] = glm::vec3(halfSide, sideLength, -halfSide);
	index = 0;

	// 위쪽 면
	cubeCoords[4][index++] = glm::vec3(-halfSide, sideLength, halfSide);
	cubeCoords[4][index++] = glm::vec3(halfSide, sideLength, halfSide);
	cubeCoords[4][index++] = glm::vec3(halfSide, sideLength, -halfSide);
	cubeCoords[4][index++] = glm::vec3(-halfSide, sideLength, -halfSide);
	index = 0;

	// 아래쪽 면
	cubeCoords[5][index++] = glm::vec3(-halfSide, 0, halfSide);
	cubeCoords[5][index++] = glm::vec3(-halfSide, 0, -halfSide);
	cubeCoords[5][index++] = glm::vec3(halfSide, 0, -halfSide);
	cubeCoords[5][index++] = glm::vec3(halfSide, 0, halfSide);
	index = 0;

	// Shape 객체 생성 (육면체의 각 면에 해당하는 좌표 포함)
	for (int i = 0; i < 6; ++i)
	{
		cube[i] = Shape(4, cubeCoords[i]);
	}
}

void CreatePyramid(Shape pyramid[], float sideLength = 0.5f, float height = 0.5f)
{
	glm::vec3 pyramidCoords[5][MAX_POINTS];

	float halfSide = sideLength / 2.0f;
	int index = 0;

	// 아래쪽 면
	pyramidCoords[0][index++] = glm::vec3(-halfSide, 0, halfSide);
	pyramidCoords[0][index++] = glm::vec3(-halfSide, 0, -halfSide);
	pyramidCoords[0][index++] = glm::vec3(halfSide, 0, -halfSide);
	pyramidCoords[0][index++] = glm::vec3(halfSide, 0, halfSide);
	index = 0;

	// 앞면
	pyramidCoords[1][index++] = glm::vec3(-halfSide, 0, halfSide);
	pyramidCoords[1][index++] = glm::vec3(halfSide, 0, halfSide);
	pyramidCoords[1][index++] = glm::vec3(0, height, 0);
	index = 0;

	// 뒷면
	pyramidCoords[2][index++] = glm::vec3(-halfSide, 0, -halfSide);
	pyramidCoords[2][index++] = glm::vec3(halfSide, 0, -halfSide);
	pyramidCoords[2][index++] = glm::vec3(0, height, 0);
	index = 0;

	// 왼쪽 면
	pyramidCoords[3][index++] = glm::vec3(-halfSide, 0, halfSide);
	pyramidCoords[3][index++] = glm::vec3(-halfSide, 0, -halfSide);
	pyramidCoords[3][index++] = glm::vec3(0, height, 0);
	index = 0;

	// 오른쪽 면
	pyramidCoords[4][index++] = glm::vec3(halfSide, 0, -halfSide);
	pyramidCoords[4][index++] = glm::vec3(halfSide, 0, halfSide);
	pyramidCoords[4][index++] = glm::vec3(0, height, 0);
	index = 0;

	// Shape 객체 생성 (육면체의 각 면에 해당하는 좌표 포함)
	pyramid[0] = Shape(4, pyramidCoords[0]);
	for (int i = 1; i < 5; ++i)
	{
		pyramid[i] = Shape(3, pyramidCoords[i]);
	}
}
#endif // Quiz17

#ifdef Quiz18
static const int index = 13;
Shape middle;
Shape satelite_of_middle[3];
Shape satelite_of_satelite[3];
Shape route_satelite_of_middle[3];
Shape route_satelite_of_satelite[3];
GLenum drawmode = GL_FILL;
float angle = 0.0f;
glm::vec3 move_satelite(0.0f);
glm::vec3 rotate_satelite(0.0f);
bool depthtest = true;
bool perspective = false;

Shape CreateSphere(float radius, glm::vec3 middle = glm::vec3(0.0f), int latitudeSegments = glm::sqrt(MAX_POINTS), int longitudeSegments = glm::sqrt(MAX_POINTS))
{
	glm::vec3 sphereCoords[MAX_POINTS];

	int index = 0;

	// 위도와 경도를 따라 구 표면 점 생성
	for (int lat = 0; lat < latitudeSegments; ++lat)
	{
		float theta = lat * M_PI / latitudeSegments;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int lon = 0; lon < longitudeSegments; ++lon)
		{
			float phi = lon * 2.0f * M_PI / longitudeSegments;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			// x, y, z 좌표계로 변환하여 점 위치 설정
			float x = radius * cosPhi * sinTheta;
			float y = radius * cosTheta;
			float z = radius * sinPhi * sinTheta;

			sphereCoords[index++] = glm::vec3(x, y, z) + middle;
		}
	}

	// 생성된 좌표로 Shape 객체 생성
	Shape sphere(index, sphereCoords);
	return sphere;
}

Shape CreateCircle(float radius, int segments = MAX_POINTS)
{
	glm::vec3 Coords[MAX_POINTS];

	int index = 0;

	for (int i = 0; i < segments; ++i)
	{
		Coords[index++] = glm::vec3(radius * glm::cos(glm::radians(float(i) / float(segments) * 360.0f)), 0.0f, radius * glm::sin(glm::radians(float(i) / float(segments) * 360.0f)));
	}

	Shape circle(index, Coords);
	return circle;
}

void TransformSatelite(Shape satelite)
{
	GLuint transformLoc = glGetUniformLocation(shaderProgramID, "modelTransform");

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, move_satelite);
	model = glm::rotate(model, rotate_satelite.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotate_satelite.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -move_satelite);

	model = glm::rotate(model, satelite.revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, satelite.revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, satelite.revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, satelite.translation);
	model = glm::rotate(model, satelite.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, satelite.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, satelite.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, satelite.scaling);

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
}

#endif // Quiz18


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
#endif // Quiz14
#ifdef Quiz15
	cube = CreateCube(0.3f);
	cube.translation = glm::vec3(-0.5f, 0.0f, 0.0f);
	cone = CreateCone(0.3f, 0.5f, 100);
	cone.translation = glm::vec3(0.5f, 0.0f, 0.0f);
	sphere = CreateSphere(0.3f, 20, 20);
	sphere.translation = glm::vec3(-0.5f, 0.0f, 0.0f);
	cylinder = CreateCylinder(0.3f, 0.5f, 100);
	cylinder.translation = glm::vec3(0.5f, 0.0f, 0.0f);
	spiral = CreateSpiral(0.002f, 499);

	scaleByzero = 1.0f;
	anim = NONE;
	rotatedir = 0;
	select = 3;
	progress = 0;
	alter = false;
#endif // Quiz15
#ifdef Quiz17
	CreateCube(cube);
	CreatePyramid(pyramid);

	for (int i = 0; i < 4; ++i)
	{
		pyramid_transform[i] = 0.0f;
	}
	cube_transform[0] = 0.0f;
	cube_transform[1] = 0.0f;
	cube_transform[2] = 0.0f;

	cube_rotatetop = false;
	cube_openfront = false;
	cube_openside = false;
	cube_openback = false;
	pyramid_anim = NONE;
#endif // Quzi17
#ifdef Quiz18
	middle = CreateSphere(0.3f);

	for (int i = 0; i < 3; ++i)
	{
		satelite_of_middle[i] = CreateSphere(0.1f);
		satelite_of_satelite[i] = CreateSphere(0.05f);
		route_satelite_of_satelite[i] = CreateCircle(0.25f);
	}	

	route_satelite_of_middle[0] = CreateCircle(0.75);
	route_satelite_of_middle[1] = CreateCircle(0.5);
	route_satelite_of_middle[2] = CreateCircle(0.5);

	route_satelite_of_middle[1].rotation.z = glm::radians(-45.0f);
	route_satelite_of_middle[1].scaling = glm::vec3(glm::sqrt(2));
	route_satelite_of_middle[2].rotation.z = glm::radians(45.0f);
	route_satelite_of_middle[2].scaling = glm::vec3(glm::sqrt(2));
#endif // Quiz18

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
#ifdef Quiz17
	cameraPos = glm::vec3(-1.0f, 2.0f, 2.0f);
#endif // Quiz17
#ifdef Quiz18
	cameraPos = glm::vec3(0.0f, 1.0f, 2.0f);
#endif // Quiz18

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform"); //뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 100.0f); //투영 공간 설정 : [-100.0, 100.0]

#ifdef Quiz17
	if (perspective)
	{
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	}
#endif // Quiz17
#ifdef Quiz18
	if (perspective)
	{
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	}
#endif // Quiz18

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(axesVAO);
	glm::mat4 axesTransform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(axesTransform));
	glDrawArrays(GL_LINES, 0, 6);

	UpdateBuffer();

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

		cube[drawidx].OldDraw(drawidx);

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

			cube[drawidx_2].OldDraw(drawidx_2);
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

		pyramid[drawidx].OldDraw(drawidx + 6);

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

			pyramid[drawidx_2].OldDraw(drawidx_2 + 6);
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
			cube[i].OldDraw(i);
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
			pyramid[i].OldDraw(i + 6);
		}
	}

#endif // Quiz14
#ifdef Quiz15
	if (alter)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, sphere.revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, sphere.revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, sphere.revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleByzero));
		model = glm::translate(model, sphere.translation);
		model = glm::rotate(model, sphere.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, sphere.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, sphere.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, sphere.scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

		UpdateBuffer();

		glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		sphere.OldDraw(2);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, cylinder.revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cylinder.revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cylinder.revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleByzero));
		model = glm::translate(model, cylinder.translation);
		model = glm::rotate(model, cylinder.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cylinder.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cylinder.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cylinder.scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		UpdateBuffer();

		glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		cylinder.DrawCylinder(3);
	}
	else
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, cube.revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cube.revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cube.revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleByzero));
		model = glm::translate(model, cube.translation);
		model = glm::rotate(model, cube.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cube.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cube.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cube.scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

		UpdateBuffer();

		glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		cube.OldDraw(0);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, cone.revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cone.revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cone.revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleByzero));
		model = glm::translate(model, cone.translation);
		model = glm::rotate(model, cone.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cone.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cone.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cone.scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

		UpdateBuffer();

		glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		cone.OldDraw(1);
	}

	if (anim == SPIRAL)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, spiral.revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, spiral.revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, spiral.revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleByzero));
		model = glm::translate(model, spiral.translation);
		model = glm::rotate(model, spiral.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, spiral.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, spiral.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, spiral.scaling);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

		UpdateBuffer();

		spiral.DrawLineStrip(4);
	}
#endif // Quiz15
#ifdef Quiz17
	if (drawshape == 1)
	{
		for (int i = 0; i < 6; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, cube[i].revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cube[i].revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cube[i].revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, cube[i].translation);
			model = glm::rotate(model, cube[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cube[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cube[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, cube[i].scaling);

			switch (i)
			{
			case 0:
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.25f));
				model = glm::rotate(model, cube_transform[1], glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.25f));
				break;
			case 1:
				model = glm::translate(model, glm::vec3(0.0f, 0.25f, -0.25f));
				model = glm::scale(model, glm::vec3(cube_transform[2]));
				model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.25f));
				break;
			case 4:
				model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
				model = glm::rotate(model, cube_transform[0], glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
				break;
			default:
				break;
			}

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

			UpdateBuffer();

			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
			cube[i].OldDraw(i);
		}
	}
	else if (drawshape == 2)
	{
		for (int i = 0; i < 5; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, pyramid[i].revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, pyramid[i].revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, pyramid[i].revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, pyramid[i].scaling);
			model = glm::translate(model, pyramid[i].translation);
			model = glm::rotate(model, pyramid[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, pyramid[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, pyramid[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			switch (i)
			{
			case 1:
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.25f));
				model = glm::rotate(model, pyramid_transform[0], glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.25f));
				break;
			case 2:
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.25f));
				model = glm::rotate(model, -pyramid_transform[1], glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.25f));
				break;
			case 3:
				model = glm::translate(model, glm::vec3(-0.25f, 0.0f, 0.0f));
				model = glm::rotate(model, pyramid_transform[2], glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
				break;
			case 4:
				model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
				model = glm::rotate(model, -pyramid_transform[3], glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::translate(model, glm::vec3(-0.25f, 0.0f, 0.0f));
				break;
			default:
				break;
			}

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

			UpdateBuffer();

			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
			pyramid[i].OldDraw(i + 6);
		}
	}
#endif // Quiz17
#ifdef Quiz18
	glPolygonMode(GL_FRONT_AND_BACK, drawmode);

	middle.Draw(0);
	
	for (int i = 0; i < 3; ++i)
	{
		TransformSatelite(satelite_of_middle[i]);
		satelite_of_middle[i].Draw_Without_Transform(i + 1);
		TransformSatelite(satelite_of_satelite[i]);
		satelite_of_satelite[i].Draw_Without_Transform(i + 4);
		TransformSatelite(route_satelite_of_middle[i]);
		route_satelite_of_middle[i].Draw_Without_Transform(i + 7, GL_LINE_STRIP);
		TransformSatelite(route_satelite_of_satelite[i]);
		route_satelite_of_satelite[i].Draw_Without_Transform(i + 10, GL_LINE_STRIP);
	}
#endif // Quiz18

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
#ifdef Quiz15
	switch (key)
	{
	case '1':
		select = 1;
		break;
	case '2':
		select = 2;
		break;
	case '3':
		select = 3;
		break;
	case 'x':
		if (anim == ROTATE_X && rotatedir == 1)
		{
			anim = NONE;
			break;
		}

		anim = ROTATE_X;
		rotatedir = 1;
		break;
	case 'X':
		if (anim == ROTATE_X && rotatedir == -1)
		{
			anim = NONE;
			break;
		}

		anim = ROTATE_X;
		rotatedir = -1;
		break;
	case 'y':
		if (anim == ROTATE_Y && rotatedir == 1)
		{
			anim = NONE;
			break;
		}

		anim = ROTATE_Y;
		rotatedir = 1;
		break;
	case 'Y':
		if (anim == ROTATE_Y && rotatedir == -1)
		{
			anim = NONE;
			break;
		}

		anim = ROTATE_Y;
		rotatedir = -1;
		break;
	case 'r':
		if (anim == REVOLVE_Y && rotatedir == 1)
		{
			anim = NONE;
			break;
		}

		anim = REVOLVE_Y;
		rotatedir = 1;
		break;
	case 'R':
		if (anim == REVOLVE_Y && rotatedir == -1)
		{
			anim = NONE;
			break;
		}

		anim = REVOLVE_Y;
		rotatedir = -1;
		break;
	case '6':
	{
		bool temp = alter;
		InitializeData();
		alter = temp;
		anim = SPIRAL;
		cube.translation = glm::vec3(0.0f);
		cone.translation = glm::vec3(0.0f);
		sphere.translation = glm::vec3(0.0f);
		cylinder.translation = glm::vec3(0.0f);
	}
	break;
	case '7':
	{
		bool temp = alter;
		InitializeData();
		alter = temp;
		anim = LINEAR;
	}
	break;
	case '8':
	{
		bool temp = alter;
		InitializeData();
		alter = temp;
		anim = REVOLUTION_CURVE;
	}
	break;
	case '9':
	{
		bool temp = alter;
		InitializeData();
		alter = temp;
		anim = REVOLUTION_STRAIGHT;
	}
	break;
	case '0':
	{
		bool temp = alter;
		InitializeData();
		alter = temp;
		anim = REVOLUTION_WITHSCALE;
	}
	break;
	case 'c':
		if (alter)
			alter = false;
		else
			alter = true;
		break;
	case 'w':
		if (drawmode == GL_FILL)
		{
			drawmode = GL_LINE;
		}
		else if (drawmode == GL_LINE)
		{
			drawmode = GL_FILL;
		}
		break;
	case '+':
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.scaling += glm::vec3(0.05f);
			if (select == 2 || select == 3)
				cylinder.scaling += glm::vec3(0.05f);
		}
		else
		{
			if (select == 1 || select == 3)
				cube.scaling += glm::vec3(0.05f);
			if (select == 2 || select == 3)
				cone.scaling += glm::vec3(0.05f);
		}
		break;
	case '-':
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.scaling -= glm::vec3(0.05f);
			if (select == 2 || select == 3)
				cylinder.scaling -= glm::vec3(0.05f);
		}
		else
		{
			if (select == 1 || select == 3)
				cube.scaling -= glm::vec3(0.05f);
			if (select == 2 || select == 3)
				cone.scaling -= glm::vec3(0.05f);
		}
		break;
	case ']':
		scaleByzero += 0.05f;
		break;
	case '[':
		scaleByzero -= 0.05f;
		break;
	case 's':
		InitializeData();
		break;
	default:
		break;
	}
#endif // Quiz15
#ifdef Quiz17
	switch (key)
	{
	case '1':
		drawshape = 1;
		break;
	case '2':
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
	case 'p':
		if (perspective)
		{
			perspective = false;
		}
		else
		{
			perspective = true;
		}
		break;
	case 'y':
		if (rotationY)
		{
			rotationY = false;
		}
		else
		{
			rotationY = true;
		}
		break;
	case 't':
		if (cube_rotatetop)
		{
			cube_rotatetop = false;
		}
		else
		{
			cube_rotatetop = true;
		}
		break;
	case 'f':
		if (cube_openfront)
		{
			cube_openfront = false;
		}
		else
		{
			cube_openfront = true;
		}
		break;
	case 's':
		if (cube_openside)
		{
			cube_openside = false;
		}
		else
		{
			cube_openside = true;
		}
		break;
	case 'd':
		if (cube_openback)
		{
			cube_openback = false;
		}
		else
		{
			cube_openback = true;
		}
		break;
	case 'o':
		if (pyramid_anim == OPEN)
		{
			pyramid_anim = CLOSE;
		}
		else
		{
			pyramid_anim = OPEN;
		}
		break;
	case 'r':
		if (pyramid_anim == OPEN_EACH)
		{
			pyramid_anim = CLOSE_EACH;
		}
		else
		{
			pyramid_anim = OPEN_EACH;
		}
		break;
	case '`':
		InitializeData();
		break;
	default:
		break;
	}
#endif // Quiz17
#ifdef Quiz18
	switch (key)
	{
	case 'p':
		perspective = false;
		break;
	case 'P':
		perspective = true;
		break;
	case 'm':
		drawmode = GL_FILL;
		break;
	case 'M':
		drawmode = GL_LINE;
		break;
	case 'w':
		move_satelite.y += 0.05f;
		middle.translation.y += 0.05f;
		for (int i = 0; i < 3; ++i)
			route_satelite_of_middle[i].translation.y += 0.05f;
		break;
	case 's':
		move_satelite.y -= 0.05f;
		middle.translation.y -= 0.05f;
		for (int i = 0; i < 3; ++i)
			route_satelite_of_middle[i].translation.y -= 0.05f;
		break;
	case 'a':
		move_satelite.x -= 0.05f;
		middle.translation.x -= 0.05f;
		for (int i = 0; i < 3; ++i)
			route_satelite_of_middle[i].translation.x -= 0.05f;
		break;
	case 'd':
		move_satelite.x += 0.05f;
		middle.translation.x += 0.05f;
		for (int i = 0; i < 3; ++i)
			route_satelite_of_middle[i].translation.x += 0.05f;
		break;
	case '-':
		move_satelite.z -= 0.05f;
		middle.translation.z -= 0.05f;
		for (int i = 0; i < 3; ++i)
			route_satelite_of_middle[i].translation.z -= 0.05f;
		break;
	case '+':
		move_satelite.z += 0.05f;
		middle.translation.z += 0.05f;
		for (int i = 0; i < 3; ++i)
			route_satelite_of_middle[i].translation.z += 0.05f;
		break;
	case 'y':
		rotate_satelite.y += glm::radians(5.0f);
		break;
	case 'Y':
		rotate_satelite.y -= glm::radians(5.0f);
		break;
	case 'z':
		rotate_satelite.z += glm::radians(5.0f);
		break;
	case 'Z':
		rotate_satelite.z -= glm::radians(5.0f);
		break;
	default:
		break;
	}
#endif // Quiz18

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
#ifdef Quiz15
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.translation.x += 0.1f;
			if (select == 2 || select == 3)
				cylinder.translation.x += 0.1f;
		}
		else
		{
			if (select == 1 || select == 3)
				cube.translation.x += 0.1f;
			if (select == 2 || select == 3)
				cone.translation.x += 0.1f;
		}
		break;
	case GLUT_KEY_LEFT:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.translation.x -= 0.1f;
			if (select == 2 || select == 3)
				cylinder.translation.x -= 0.1f;
		}
		else
		{
			if (select == 1 || select == 3)
				cube.translation.x -= 0.1f;
			if (select == 2 || select == 3)
				cone.translation.x -= 0.1f;
		}
		break;
	case GLUT_KEY_DOWN:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.translation.z += 0.1f;
			if (select == 2 || select == 3)
				cylinder.translation.z += 0.1f;
		}
		else
		{
			if (select == 1 || select == 3)
				cube.translation.z += 0.1f;
			if (select == 2 || select == 3)
				cone.translation.z += 0.1f;
		}
		break;
	case GLUT_KEY_UP:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.translation.z -= 0.1f;
			if (select == 2 || select == 3)
				cylinder.translation.z -= 0.1f;
		}
		else
		{
			if (select == 1 || select == 3)
				cube.translation.z -= 0.1f;
			if (select == 2 || select == 3)
				cone.translation.z -= 0.1f;
		}
		break;
	case GLUT_KEY_SHIFT_L:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.translation.y += 0.1f;
			if (select == 2 || select == 3)
				cylinder.translation.y += 0.1f;
		}
		else
		{
			if (select == 1 || select == 3)
				cube.translation.y += 0.1f;
			if (select == 2 || select == 3)
				cone.translation.y += 0.1f;
		}
		break;
	case GLUT_KEY_CTRL_L:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.translation.y -= 0.1f;
			if (select == 2 || select == 3)
				cylinder.translation.y -= 0.1f;
		}
		else
		{
			if (select == 1 || select == 3)
				cube.translation.y -= 0.1f;
			if (select == 2 || select == 3)
				cone.translation.y -= 0.1f;
		}
		break;
	default:
		break;
	}
#endif // Quiz15

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
#ifdef Quiz15
	switch (anim)
	{
	case ROTATE_X:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.rotation.x += glm::radians(1.0f * rotatedir);
			if (select == 2 || select == 3)
				cylinder.rotation.x += glm::radians(1.0f * rotatedir);
		}
		else
		{
			if(select == 1 || select == 3)
				cube.rotation.x += glm::radians(1.0f * rotatedir);
			if (select == 2 || select == 3)
				cone.rotation.x += glm::radians(1.0f * rotatedir);
		}
		break;
	case ROTATE_Y:
		if (alter)
		{
			if (select == 1 || select == 3)
				sphere.rotation.y += glm::radians(1.0f * rotatedir);
			if (select == 2 || select == 3)
				cylinder.rotation.y += glm::radians(1.0f * rotatedir);
		}
		else
		{
			if (select == 1 || select == 3)
				cube.rotation.y += glm::radians(1.0f * rotatedir);
			if (select == 2 || select == 3)
				cone.rotation.y += glm::radians(1.0f * rotatedir);
		}
		break;
	case REVOLVE_Y:
		if (alter)
		{
			sphere.revolution.y += glm::radians(1.0f * rotatedir);
			cylinder.revolution.y += glm::radians(1.0f * rotatedir);
		}
		else
		{
			cube.revolution.y += glm::radians(1.0f * rotatedir);
			cone.revolution.y += glm::radians(1.0f * rotatedir);
		}
		break;
	case SPIRAL:
	{
		float angle = 8.0 * M_PI * progress / 100.0f;
		float x = progress / 100.0f * cos(angle);
		float z = progress / 100.0f * sin(angle);

		if (alter)
		{
			sphere.translation = glm::vec3(x, 0.0f, z);
			cylinder.translation = glm::vec3(-x, 0.0f, -z);
		}
		else
		{
			cube.translation = glm::vec3(x, 0.0f, z);
			cone.translation = glm::vec3(-x, 0.0f, -z);
		}

		if (progress < 100)
			++progress;
	}	
		break;
	case LINEAR:
	{
		float t = progress / 100.0f;
		float x = (1.0f - t) * 0.5f + t * -0.5f;

		if (alter)
		{
			sphere.translation = glm::vec3(x, 0.0f, 0.0f);
			cylinder.translation = glm::vec3(-x, 0.0f, 0.0f);
		}
		else
		{
			cube.translation = glm::vec3(x, 0.0f, 0.0f);
			cone.translation = glm::vec3(-x, 0.0f, 0.0f);
		}

		if (progress < 100)
			++progress;
	}
		break;
	case REVOLUTION_CURVE:
	{
		float t = progress / 100.0f;
		float rad = glm::radians(180.0f * t);

		if (alter)
		{
			sphere.revolution.y = rad;
			cylinder.revolution.y = rad;
		}
		else
		{
			cube.revolution.y = rad;
			cone.revolution.y = rad;
		}

		if (progress < 100)
			++progress;
	}
		break;
	case REVOLUTION_STRAIGHT:
	{
		float t = progress / 50.0f;
		float x, y;

		if (t < 1.0f)
		{
			x = (1 - t) * -0.5f;
			y = t * -0.5f;
		}
		else if (t >= 1.0f)
		{
			t -= 1.0f;
			x = t * 0.5f;
			y = (1 - t) * -0.5f;
		}

		if (alter)
		{
			sphere.translation = glm::vec3(x, y, 0.0f);
			cylinder.translation = glm::vec3(-x, -y, 0.0f);
		}
		else
		{
			cube.translation = glm::vec3(x, y, 0.0f);
			cone.translation = glm::vec3(-x, -y, 0.0f);
		}

		if (progress < 100)
			++progress;
	}
		break;
	case REVOLUTION_WITHSCALE:
	{
		float t = progress / 100.0f;
		float rad = glm::radians(180.0f * t);

		if (alter)
		{
			sphere.rotation.y = rad;
			cylinder.rotation.y = rad;
			sphere.revolution.y = rad;
			cylinder.revolution.y = rad;
			sphere.scaling = glm::vec3(t);
			cylinder.scaling = glm::vec3(1.0 - t);
		}
		else
		{
			cube.rotation.y = rad;
			cone.rotation.y = rad;
			cube.revolution.y = rad;
			cone.revolution.y = rad;
			cube.scaling = glm::vec3(t);
			cone.scaling = glm::vec3(1.0 - t);
		}

		if (progress < 100)
			++progress;
	}
		break;
	default:
		break;
	}
#endif // Quiz15
#ifdef Quiz17
	switch (drawshape)
	{
	case 1:
		if (rotationY)
		{
			for (int i = 0; i < 6; ++i)
			{
				cube[i].rotation.y += glm::radians(1.0f);
			}
		}

		if (cube_rotatetop)
		{
			cube_transform[0] += glm::radians(1.0f);
		}

		if (cube_openfront && cube_transform[1] < glm::radians(90.0f))
		{
			cube_transform[1] += glm::radians(1.0f);
		}
		else if (!cube_openfront && cube_transform[1] > glm::radians(0.0f))
		{
			cube_transform[1] -= glm::radians(1.0f);
		}

		if (cube_openside && cube[2].translation.y < 0.5f)
		{
			cube[2].translation.y += 0.025f;
			cube[3].translation.y += 0.025f;
		}
		else if (!cube_openside && cube[2].translation.y > 0.0f)
		{
			cube[2].translation.y -= 0.025f;
			cube[3].translation.y -= 0.025f;
		}

		if (cube_openback && cube_transform[2] > 0.0f)
		{
			cube_transform[2] -= 0.05f;
		}
		else if (!cube_openback && cube_transform[2] < 1.0f)
		{
			cube_transform[2] += 0.05f;
		}
		break;
	case 2:
		if (rotationY)
		{
			for (int i = 0; i < 5; ++i)
			{
				pyramid[i].rotation.y += glm::radians(1.0f);
			}
		}

		switch (pyramid_anim)
		{
		case OPEN:
			for (int i = 0; i < 4; ++i)
			{
				if (pyramid_transform[i] < glm::radians(180.0f + (90.0f - 63.4f) * 2))
					pyramid_transform[i] += glm::radians(1.0f);
			}
			break;
		case CLOSE:
			for (int i = 0; i < 4; ++i)
			{
				if (pyramid_transform[i] > glm::radians(0.0f))
					pyramid_transform[i] -= glm::radians(1.0f);
			}
			break;
		case OPEN_EACH:
			if (pyramid_transform[0] < glm::radians(180.0f - 63.4f))
			{
				pyramid_transform[0] += glm::radians(1.0f);
			}
			else if (pyramid_transform[1] < glm::radians(180.0f - 63.4f))
			{
				pyramid_transform[1] += glm::radians(1.0f);
			}
			else if (pyramid_transform[2] < glm::radians(180.0f - 63.4f))
			{
				pyramid_transform[2] += glm::radians(1.0f);
			}
			else if (pyramid_transform[3] < glm::radians(180.0f - 63.4f))
			{
				pyramid_transform[3] += glm::radians(1.0f);
			}
			break;
		case CLOSE_EACH:
			if (pyramid_transform[3] > glm::radians(0.0f))
			{
				pyramid_transform[3] -= glm::radians(1.0f);
			}
			else if (pyramid_transform[2] > glm::radians(0.0f))
			{
				pyramid_transform[2] -= glm::radians(1.0f);
			}
			else if (pyramid_transform[1] > glm::radians(0.0f))
			{
				pyramid_transform[1] -= glm::radians(1.0f);
			}
			else if (pyramid_transform[0] > glm::radians(0.0f))
			{
				pyramid_transform[0] -= glm::radians(1.0f);
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

#endif // Quiz17
#ifdef Quiz18
	satelite_of_middle[0].translation = glm::vec3(0.75f * glm::cos(glm::radians(angle)), 0.0f, 0.75f * glm::sin(glm::radians(angle)));
	satelite_of_middle[1].translation = glm::vec3(0.5f * glm::cos(glm::radians(angle + 90.0f)), -0.5f * glm::cos(glm::radians(angle + 90.0f)), 0.75f * glm::sin(glm::radians(angle + 90.0f)));
	satelite_of_middle[2].translation = glm::vec3(-0.5f * glm::cos(glm::radians(angle + 180.0f)), -0.5f * glm::cos(glm::radians(angle + 180.0f)), 0.75f * glm::sin(glm::radians(angle + 180.0f)));

	satelite_of_satelite[0].translation = glm::vec3(0.75f * glm::cos(glm::radians(angle)) + 0.25f * glm::cos(glm::radians(angle)), 0.0f, 0.75f * glm::sin(glm::radians(angle)) + 0.25f * glm::sin(glm::radians(angle)));
	satelite_of_satelite[1].translation = glm::vec3(0.5f * glm::cos(glm::radians(angle + 90.0f)) + 0.25f * glm::cos(glm::radians(angle + 90.0f)), -0.5f * glm::cos(glm::radians(angle + 90.0f)), 0.75f * glm::sin(glm::radians(angle + 90.0f)) + 0.25f * glm::sin(glm::radians(angle + 90.0f)));
	satelite_of_satelite[2].translation = glm::vec3(-0.5f * glm::cos(glm::radians(angle + 180.0f)) + 0.25f * glm::cos(glm::radians(angle + 180.0f)), -0.5f * glm::cos(glm::radians(angle + 180.0f)), 0.75f * glm::sin(glm::radians(angle + 180.0f)) + 0.25f * glm::sin(glm::radians(angle + 180.0f)));
	route_satelite_of_satelite[0].translation = glm::vec3(0.75f * glm::cos(glm::radians(angle)), 0.0f, 0.75f * glm::sin(glm::radians(angle)));
	route_satelite_of_satelite[1].translation = glm::vec3(0.5f * glm::cos(glm::radians(angle + 90.0f)), -0.5f * glm::cos(glm::radians(angle + 90.0f)), 0.75f * glm::sin(glm::radians(angle + 90.0f)));
	route_satelite_of_satelite[2].translation = glm::vec3(-0.5f * glm::cos(glm::radians(angle + 180.0f)), -0.5f * glm::cos(glm::radians(angle + 180.0f)), 0.75f * glm::sin(glm::radians(angle + 180.0f)));

	for (int i = 0; i < 3; ++i)
	{
		satelite_of_middle[i].translation += move_satelite;
		satelite_of_satelite[i].translation += move_satelite;
		route_satelite_of_satelite[i].translation += move_satelite;
	}

	angle += 1.0f;
	if (angle >= 360.0f)
		angle = 0.0f;
#endif // Quiz18

	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, Timer, 1);
}



/*
=========동작이 의도한대로 되지 않을때 확인할 것=========

1. 버퍼 크기를 할당했는가
2. 버퍼와 인덱스의 간격을 적정하게 주었는가
3. 접근하려는 버퍼의 인덱스가 적절한가

=========자주 실수하는 부분이니까 반드시 확인=========
*/



void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기


	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//--- 변수 colors에서 버텍스 색상을 복사한다.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(sphere.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(sphere.shapecolor[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cylinder.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(cylinder.shapecolor[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 4 * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(spiral.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 4 * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(spiral.shapecolor[0]));
#endif // Quiz15
#ifdef Quiz17
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

#endif // Quiz17
#ifdef Quiz18
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(middle.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(middle.shapecolor[0]));

	for (int i = 0; i < 3; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 1) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(satelite_of_middle[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 1) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(satelite_of_middle[i].shapecolor[0]));
	}

	for (int i = 0; i < 3; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 4) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(satelite_of_satelite[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 4) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(satelite_of_satelite[i].shapecolor[0]));
	}

	for (int i = 0; i < 3; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 7) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(route_satelite_of_middle[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 7) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(route_satelite_of_middle[i].shapecolor[0]));
	}

	for (int i = 0; i < 3; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 10) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(route_satelite_of_satelite[i].shapecoord[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, (i + 10) * MAX_POINTS * 3 * sizeof(GLfloat), MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(route_satelite_of_satelite[i].shapecolor[0]));
	}

#endif // Quiz17

}