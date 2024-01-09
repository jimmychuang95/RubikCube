#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "solve.h"
#include "random.h"

using namespace std;

template<class T>
class Point {
public:
	T x, y, z, c1, c2, c3, t1, t2;
	Point(T _x = 0, T _y = 0, T _z = 0, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f), T _t1 = 0, T _t2 = 3) {
		x = _x;
		y = _y;
		z = _z;
		c1 = color.x;
		c2 = color.y;
		c3 = color.z;
		t1 = _t1;
		t2 = _t2;
	}
};

class Cubito {
public:
	vector<Point<float>> vertices;
	float dim;
	unsigned int VAO, VBO;
	glm::vec3 center;

	const char* title_texture;
	unsigned int texture;
	//glm::mat4 model;
	float acc = 1.0f;
	glm::mat4 prevRot = glm::mat4(1.0f);

	Cubito(float _dim, glm::vec3 _pos, vector<glm::vec3> color) {			//Depende del tipo hacemos push a sus vertices
		title_texture = "../texture/allspark.jpg";
		dim = _dim;
		center = _pos;
		//FRONT
		vertices.push_back(Point<float>(-dim, -dim, dim, color[0], 0.0f, 0.0f));
		vertices.push_back(Point<float>(dim, -dim, dim, color[0], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, dim, color[0], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, dim, color[0], 1.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, dim, dim, color[0], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, dim, color[0], 0.0f, 0.0f));
		//UP
		vertices.push_back(Point<float>(-dim, dim, -dim, color[1], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, -dim, color[1], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, dim, color[1], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, dim, color[1], 1.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, dim, color[1], 0.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, -dim, color[1], 0.0f, 1.0f));
		//LEFT
		vertices.push_back(Point<float>(-dim, dim, dim, color[2], 1.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, -dim, color[2], 1.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[2], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[2], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, dim, color[2], 0.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, dim, color[2], 1.0f, 0.0f));
		//DOWN
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[3], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim, color[3], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, dim, color[3], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, -dim, dim, color[3], 1.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, -dim, dim, color[3], 0.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[3], 0.0f, 1.0f));
		//RIGHT
		vertices.push_back(Point<float>(dim, dim, dim, color[4], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, -dim, color[4], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim, color[4], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim, color[4], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, dim, color[4], 0.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, dim, color[4], 1.0f, 0.0f));
		//BACK
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[5], 0.0f, 0.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim, color[5], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, -dim, color[5], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, -dim, color[5], 1.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, dim, -dim, color[5], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[5], 0.0f, 0.0f));

	}
	void genBuffers();
	void load_create_texture(Shader*);
	void draw(Shader*, glm::vec3);
	void move(glm::vec3, Shader*, float);
	void breath(bool);
	void panting(bool, float, int, int);
	void expand();
	void twist(Shader*, glm::vec3, float);
	void rotateInTheFloor(Shader*, int, float);
	void vibrate(float);
	void deleteBuffers();
private:
	void updateBuffers();
	void move_vertices(glm::vec3, float);
};

class Rubik {
	GLFWwindow* window;
	Shader* shader;
	vector<Cubito> cubitos;
	float dim;
	unordered_map<char, vector<int>>  parts;
	bool inhala = true;

public:
	bool drawing = true;
	vector<Rubik*> brothers;
	int timesTwist;
	vector<string> shuffle;
	vector<string> solution;
	float degrees;
	Rubik(GLFWwindow* _window, Shader* _shader, float _dim = 0.2f) {
		window = _window;
		shader = _shader;
		timesTwist = 0;
		dim = _dim;
		degrees = -1.0f;
		float pos = dim + (dim / 8); // 正向位置
		float neg = -1 * pos;        // 負向位置
		glm::vec3 cubePositions[] = {
		   glm::vec3(neg,  pos, pos), // 前左上
		   glm::vec3(pos,  pos, pos), // 前右上
		   glm::vec3(neg,  neg, pos), // 前左下
		   glm::vec3(pos,  neg, pos), // 前右下

		   glm::vec3(neg,  pos, neg), // 背左上
		   glm::vec3(pos,  pos, neg), // 背右上
		   glm::vec3(neg,  neg, neg), // 背左下
		   glm::vec3(pos,  neg, neg)  // 背右下
		};


		glm::vec3 colors[] = {
			glm::vec3(0.0f,0.0f,0.0f),			//Negro
			glm::vec3(1.0f,1.0f,1.0f),			//Blanco
			glm::vec3(1.0f,0.5f,0.0f),			//Naranja  
			glm::vec3(1.0f,0.9f,0.0f),			//yellow!!  
			glm::vec3(1.0f,0.025f,0.25f),			//Rojo
			glm::vec3(0.18f,0.545f,0.34f),			//Verde 
			glm::vec3(0.15f,0.35f,1.0f)			//Azul
		};
		int assignColor[][6] = {
			// 前面的四個小方塊
			{4, 1, 5, 0, 0, 0}, // 前左上（白色）
			{4, 1, 0, 0, 6, 0}, // 前右上（白色, 右面藍色）
			{4, 0, 5, 3, 0, 0}, // 前左下（白色, 下面紅色）
			{4, 0, 0, 3, 6, 0}, // 前右下（白色, 下面紅色, 右面藍色）

			// 背面的四個小方塊
			{0, 1, 5, 0, 0, 2}, // 背左上（背面橙色）
			{0, 1, 0, 0, 6, 2}, // 背右上（背面橙色, 右面藍色）
			{0, 0, 5, 3, 0, 2}, // 背左下（背面橙色, 下面紅色）
			{0, 0, 0, 3, 6, 2}  // 背右下（背面橙色, 下面紅色, 右面藍色）
		};

		parts.insert({ 'F', vector<int>{0, 1, 2, 3} }); // 前面的小方塊
		parts.insert({ 'B', vector<int>{5, 4, 7, 6} }); // 背面的小方塊
		parts.insert({ 'L', vector<int>{4, 0, 6, 2} }); // 左面的小方塊
		parts.insert({ 'R', vector<int>{1, 5, 3, 7} }); // 右面的小方塊
		parts.insert({ 'U', vector<int>{4, 5, 0, 1} }); // 上面的小方塊
		parts.insert({ 'D', vector<int>{2, 3, 6, 7} }); // 下面的小方塊
		vector<glm::vec3> cubeColor;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 6; j++) {
				cubeColor.push_back(colors[assignColor[i][j]]);
			}
			cubitos.push_back(Cubito(dim, cubePositions[i], cubeColor));
			cubeColor.clear();
		}

		genBuffers();
		load_create_texture();
	}
	void draw();
	void updateCurrentPart(char);
	void updateParts(char);
	void fillShuffle(char);
	void move(char);
	void solve(vector<string> sol);
	void setSolve();
	void breath();
	void expand();
	void twist();
	void vibrate();
	void magnet();
	void deleteBuffers();
private:
	void genBuffers();
	void load_create_texture();
	void fall();
};