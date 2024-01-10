#pragma once
#include "Cubito.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/gtx/string_cast.hpp>
#include <Windows.h>

void Cubito::genBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	//vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return;
}

void Cubito::load_create_texture(Shader* ourShader) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(title_texture, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader->use();
	ourShader->setInt("texture1", 0);
	
	return;
}

void Cubito::updateBuffers() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	return;
}

void Cubito::draw(Shader* ourShader, glm::vec3 pivot = glm::vec3(0.0f, 0.0f, 0.0f)) {

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, center);
	ourShader->setMat4("model", model);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	return;
}

void Cubito::move_vertices(glm::vec3 pivot, float degrees) {
	glm::vec4 vertex;
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	for (int i = 0; i < vertices.size(); i++)
	{
		vertex = glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0f);

		vertex = glm::rotate(rotMatrix, glm::radians(degrees), pivot) * vertex;

		vertices[i].x = vertex.x;
		vertices[i].y = vertex.y;
		vertices[i].z = vertex.z;
	}
	updateBuffers();

	return;
}

void Cubito::move(glm::vec3 pivot, Shader* ourShader, float degrees) {

	glm::mat4 model = glm::mat4(1.0f);

	//Rotamos los centros
	glm::mat4 rot = glm::mat4(1.0f);
	rot = glm::rotate(rot, glm::radians(degrees), pivot);
	glm::vec4 r = rot * glm::vec4(center, 1.0f);
	center = r;

	model = glm::translate(model, center);
	move_vertices(pivot, degrees);
	ourShader->setMat4("model", model);


	glDrawArrays(GL_TRIANGLES, 0, 36);

	return;
}

void Cubito::panting(bool inhala, float growth, int range, int speed) {
	glm::vec3 v = center;
	if (inhala) v /= range + speed;
	else v /= -1 * (range + speed);
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);

	center = transform * glm::vec4(center, 1.0f);


	glm::vec4 vertex;
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	for (int k = 0; k < vertices.size(); k++)
	{
		vertex = glm::vec4(vertices[k].x, vertices[k].y, vertices[k].z, 1.0f);

		vertex = glm::scale(scaleMatrix, glm::vec3(1.0f + growth, 1.0f + growth, 1.0f + growth)) * vertex;

		vertices[k].x = vertex.x;
		vertices[k].y = vertex.y;
		vertices[k].z = vertex.z;
	}
	updateBuffers();

	return;
}

void Cubito::twist(Shader* shader, glm::vec3 randomPivot, float growth) {
	glm::vec3 v = center;
	v /= 2000;
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);

	center = transform * glm::vec4(center, 1.0f);


	glm::vec4 vertex;
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	for (int k = 0; k < vertices.size(); k++)
	{
		vertex = glm::vec4(vertices[k].x, vertices[k].y, vertices[k].z, 1.0f);

		vertex = glm::scale(scaleMatrix, glm::vec3(1.0f + growth, 1.0f + growth, 1.0f + growth)) * vertex;

		vertices[k].x = vertex.x;
		vertices[k].y = vertex.y;
		vertices[k].z = vertex.z;
	}


	glm::mat4 model = glm::mat4(1.0f);

	//Rotamos los centros
	glm::mat4 rot = glm::mat4(1.0f);
	rot = glm::rotate(rot, glm::radians(2.0f), randomPivot);
	glm::vec4 r = rot * glm::vec4(center, 1.0f);
	center = r;

	model = glm::translate(model, center);

	move_vertices(randomPivot, 2.0f);

	shader->setMat4("model", model);

	return;
}

void Cubito::rotateInTheFloor(Shader* shader, int distance, float randDirection = 0.0f) {
	glm::vec3 v = center;
	v.y = 0.0f;
	if (randDirection != 0.0f) {
		if (v.x < 0) v.x -= randDirection;
		else v.x += randDirection;
		if (v.z < 0) v.z -= randDirection;
		else v.z += randDirection;
	}
	v /= distance;
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);
	center = transform * glm::vec4(center, 1.0f);

	//Rotamos los centros
	glm::vec3 pivot = center;
	pivot.y = 0.0f;
	float temp90deg = pivot.x;
	pivot.x = pivot.z;
	pivot.z = -1 * pivot.x;

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 rot = glm::mat4(1.0f);
	float magnitudeRotation = ((rand() % 100) / 100);
	rot = glm::rotate(rot, glm::radians(magnitudeRotation), pivot);
	glm::vec4 r = rot * glm::vec4(center, 1.0f);
	center = r;

	model = glm::translate(model,center);

	move_vertices(pivot, 2.0f);

	shader->setMat4("model", model);

	return;
}

void Cubito::deleteBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return;
}


void Rubik::genBuffers() {
	for (int i = 0; i < cubitos.size(); i++) {
		cubitos[i].genBuffers();
	}

	return;
}

void Rubik::load_create_texture() {
	for (int i = 0; i < cubitos.size(); i++) {
		cubitos[i].load_create_texture(shader);
	}

	return;
}

void Rubik::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < cubitos.size(); i++)
	{
		cubitos[i].draw(shader);
	}
	for (int i = 0; i < brothers.size(); i++) {
		if (brothers[i]->drawing) {
			for (int j = 0; j < brothers[i]->cubitos.size(); j++)
			{
				brothers[i]->cubitos[j].draw(shader);
			}

		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();

	return;
}

void Rubik::updateCurrentPart(char p) {             //Actualizamos las posiciones de los cubitos de la capa actual
	vector<int>* pv = &(parts.find(p)->second);
	vector<int> temp = (*pv);

	for (int i = 0; i < 3; i++) {
		(*pv)[i] = temp[6 - (i * 3)];                  //0,1,2
		(*pv)[i + 3] = temp[10 - ((i + 1) * 3)];     //3,4,5
		(*pv)[i + 6] = temp[i + 12 - ((i + 1) * 4)];   //6,7,8
	}

	return;
}

void Rubik::updateParts(char movedChoosen) {  //Sides
	vector<int>* pMoved = &(parts.find(movedChoosen)->second);         //Side moved
	vector<char> updateSides;   //Sides to update R,D,L,U,F,B
	vector<vector<int>> updateIndex; //Index to update divided in parts of a side
	switch (movedChoosen)
	{
	case 'F':
		updateSides = vector<char>{ 'U','R','D','L' };
		updateIndex = vector<vector<int>>{ {6,7,8}, {0,3,6}, {0,1,2}, {2,5,8} };
		break;
	case 'B':
		updateSides = vector<char>{ 'U','L','D','R' };       //Actualizar desde B
		updateIndex = vector<vector<int>>{ {2,1,0}, {0,3,6}, {8,7,6}, {2,5,8} };
		break;
	case 'L':
		updateSides = vector<char>{ 'U','F','D','B' };
		updateIndex = vector<vector<int>>{ {0,3,6}, {0,3,6}, {6,3,0}, {2,5,8} };
		break;
	case 'R':
		updateSides = vector<char>{ 'U','B','D','F' };
		updateIndex = vector<vector<int>>{ {8,5,2}, {0,3,6}, {2,5,8}, {2,5,8} };
		break;
	case 'D':
		updateSides = vector<char>{ 'F','R','B','L' };
		updateIndex = vector<vector<int>>{ {6,7,8}, {6,7,8}, {8,7,6}, {8,7,6} };
		break;
	case 'U':
		updateSides = vector<char>{ 'B','R','F','L' };
		updateIndex = vector<vector<int>>{ {2,1,0}, {2,1,0}, {0,1,2}, {0,1,2} };
		break;
	default:
		break;
	}
	updateCurrentPart(movedChoosen);
	vector<vector<int>> sortedIndex = { {0,1,2},{2,5,8},{6,7,8},{0,3,6} }; //Orden de los indices que actualizaremos
	vector<int>* side_updating;          //Side updating
	for (int i = 0; i < 4; i++) {
		side_updating = &(parts.find(updateSides[i])->second);
		for (int j = 0; j < 3; j++) {
			(*side_updating)[updateIndex[i][j]] = (*pMoved)[sortedIndex[i][j]];
		}
	}

	return;
}

void Rubik::fillShuffle(char sideMove) {
	if (degrees == -1) {
		shuffle.push_back(string(1, sideMove));
		cout << sideMove << " ";
	}
	else {
		string s(1, sideMove);
		s.push_back('\'');
		shuffle.push_back(s);
		cout << s << " ";
	}

	return;
}

void Rubik::move(char sideMove) {
	vector<int>* pv = &(parts.find(sideMove)->second);
	vector<bool> moviendo(26, false);
	for (int k = 0; k < 90; k++) {
		for (int j = 0; j < pv->size(); j++) {
			cubitos[(*pv)[j]].move(cubitos[(*pv)[4]].center, shader, degrees);      //[4] es el pivot
			//cubitos[(*pv)[j]].draw(ourShader,cubitos[(*pv)[4]].center);      //Pivot
			moviendo[(*pv)[j]] = true;
		}
		draw();
	}

	return;
}

void Rubik::solve(vector<string> sol) {
	solution = sol;
	char sideMoved;
	degrees = -1.0f;
	for (int i = 0; i < solution.size(); i++) {
		if (i > 1) {
			std::cout << "\nSOLUTION: ";
			for (int j = i; j < solution.size(); j++) std::cout << solution[j] << " ";
		}
		if (solution[i].size() == 1) {
			sideMoved = solution[i].c_str()[0];
			move(sideMoved);
			updateParts(sideMoved);
		}
		else if (solution[i].size() == 2) {
			if (solution[i][1] == '\'') {
				degrees = 1.0f;
				sideMoved = solution[i].c_str()[0];
				move(sideMoved);
				updateParts(sideMoved);
				updateParts(sideMoved);
				updateParts(sideMoved);
				degrees = -1.0f;
			}
			else {
				sideMoved = solution[i].c_str()[0];
				move(sideMoved);
				updateParts(sideMoved);
				move(sideMoved);
				updateParts(sideMoved);
			}
		}
	}
	std::cout << "\n\nSHUFFLE: ";

	return;
}

void Rubik::setSolve() {
	vector<string> move = get_solution(to_cube_not(shuffle));
	cout << "\nSOLUTION: ";
	for (int i = 0; i < move.size(); i++) cout << move[i] << " ";
	solve(move);
	shuffle.clear();
	"\nSHUFFLE: ";

	return;
}

void Rubik::twist() {
	int idx_pivots[6] = { 4,21,12,13,16,10 };
	glm::vec3 randomPivot = cubitos[idx_pivots[rand() % 6]].center;
	float growth = 0.0005;
	for (int i = 0; i < 90; i++) {
		for (int j = 0; j < cubitos.size(); j++) {
			cubitos[j].twist(shader, randomPivot, growth);
		}
		draw();
	}

	return;
}

void Rubik::deleteBuffers() {
	for (int i = 0; i < cubitos.size(); i++)
	{
		cubitos[i].deleteBuffers();
	}

	return;
}

void kRubik::genBuffers() {
	for (int i = 0; i < cubitos.size(); i++){
		cubitos[i].genBuffers();
	}

	return;
}

void kRubik::load_create_texture() {
	for (int i = 0; i < cubitos.size(); i++) {
		cubitos[i].load_create_texture(shader);
	}
	
	return;
}

void kRubik::draw() {			
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < cubitos.size(); i++)
	{
		cubitos[i].draw(shader);
	}
	for (int i = 0; i < brothers.size(); i++) {
		if (brothers[i]->drawing) {
			for (int j = 0; j < brothers[i]->cubitos.size(); j++)
			{
				brothers[i]->cubitos[j].draw(shader);
			}

		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();

	return;
}

void kRubik::updateCurrentPart(char p) {             //Actualizamos las posiciones de los cubitos de la capa actual
	vector<int>* pv = &(parts.find(p)->second);
	vector<int> temp = (*pv);
//here
	(*pv)[1] = temp[0];
	(*pv)[3] = temp[1];
	(*pv)[0] = temp[2];
	(*pv)[2] = temp[3];

	return;
}

void kRubik::updateParts(char movedChoosen) {

	vector<int>* pMoved = &(parts.find(movedChoosen)->second); // Side moved
	vector<char> updateSides; // Sides to update R,D,L,U,F,B
	vector<vector<int>> updateIndex; // Index to update divided in parts of a side

	switch (movedChoosen) {
	case 'F':
		updateSides = vector<char>{ 'U', 'R', 'D', 'L' };
		updateIndex = vector<vector<int>>{ {2, 3}, {0, 2}, {0, 1}, {1, 3} };
		break;
	case 'B':
		updateSides = vector<char>{ 'U', 'L', 'D', 'R' };
		updateIndex = vector<vector<int>>{ {1, 0}, {0, 2}, {3, 2}, {1, 3} };
		break;
	case 'L':
		updateSides = vector<char>{ 'U', 'F', 'D', 'B' };
		updateIndex = vector<vector<int>>{ {0, 2}, {0, 2}, {2, 0}, {1, 3} };
		break;
	case 'R':
		updateSides = vector<char>{ 'U', 'B', 'D', 'F' };
		updateIndex = vector<vector<int>>{ {3, 1}, {0, 2}, {1, 3}, {1, 3} };
		break;
	case 'D':
		updateSides = vector<char>{ 'F', 'R', 'B', 'L' };
		updateIndex = vector<vector<int>>{ {2, 3}, {2, 3}, {3, 2}, {3, 2} };
		break;
	case 'U':
		updateSides = vector<char>{ 'B', 'R', 'F', 'L' };
		updateIndex = vector<vector<int>>{ {1, 0}, {1, 0}, {0, 1}, {0, 1} };
		break;
	default:
		break;
	}

	updateCurrentPart(movedChoosen);
	vector<vector<int>> sortedIndex = { {0, 1}, {1, 3}, {2, 3}, {0, 2} }; // Order of indices to be updated
	vector<int>* side_updating; // Side updating

	for (int i = 0; i < 4; i++) {
		side_updating = &(parts.find(updateSides[i])->second);
		for (int j = 0; j < 2; j++) {
			(*side_updating)[updateIndex[i][j]] = (*pMoved)[sortedIndex[i][j]];
		}
	}

	return;
}

void kRubik::fillShuffle(char sideMove) {
	if (degrees == -0.9f) {
		shuffle.push_back(string(1, sideMove));
		cout << sideMove << " ";
	}
	else {
		string s(1, sideMove);
		s.push_back('\'');
		shuffle.push_back(s);
		cout << s << " ";
	}
	
	return;
}

void kRubik::getRotateCenters() {
	vector<char> sides = { 'F','B','L','R','U','D' };
	for (int i = 0; i < sides.size(); i++) {
		glm::vec3 center = (cubitos[parts.find(sides[i])->second[3]].center + cubitos[parts.find(sides[i])->second[0]].center) / 2.0f;
		centers.insert({ sides[i], center });
	}
}

void kRubik::move(char sideMove) {

	vector<int>* pv = &(parts.find(sideMove)->second);
	vector<bool> moviendo(8, false);
	
	glm::vec3 center = centers.find(sideMove)->second;

	for (int k = 0; k < 100; k++) {

		for (int j = 0; j < 4; j++) {

			cubitos[(*pv)[j]].move(center, shader, degrees);

			//cubitos[(*pv)[j]].draw(ourShader,cubitos[(*pv)[4]].center);      //Pivot
			moviendo[(*pv)[j]] = true;
		}


		draw();
	}

	return;
}

void kRubik::solve(vector<string> sol) {
	solution = sol;
	char sideMoved;
	degrees = -0.9f;
	for (int i = 0; i < solution.size(); i++) {
		if (i > 1) {
			std::cout << "\nSOLUTION: ";
			for (int j = i; j < solution.size(); j++) std::cout << solution[j] << " ";
		}
		if (solution[i].size() == 1) {
			sideMoved = solution[i].c_str()[0];
			move(sideMoved);
			updateParts(sideMoved);
		}
		else if (solution[i].size() == 2) {
			if (solution[i][1] == '\'') {
				degrees = 0.9f;
				sideMoved = solution[i].c_str()[0];
				move(sideMoved);
				updateParts(sideMoved);
				updateParts(sideMoved);
				updateParts(sideMoved);
				degrees = -0.9f;
			}
			else {
				sideMoved = solution[i].c_str()[0];
				move(sideMoved);
				updateParts(sideMoved);
				move(sideMoved);
				updateParts(sideMoved);
			}
		}
	}
	std::cout << "\n\nSHUFFLE: ";

	return;
}

void kRubik::setSolve() {
	vector<string> move = get_solution(to_cube_not(shuffle));
	cout << "\nSOLUTION: ";
	for (int i = 0; i < move.size(); i++) cout << move[i] << " ";
	solve(move);
	shuffle.clear();
	"\nSHUFFLE: ";

	return;
}

void kRubik::twist() {
	int idx_pivots[6] = { 4,21,12,13,16,10 };
	glm::vec3 randomPivot = cubitos[idx_pivots[rand() % 6]].center;
	float growth = 0.0005;
	for (int i = 0; i < 90; i++) {
		for (int j = 0; j < cubitos.size(); j++) {
			cubitos[j].twist(shader,randomPivot,growth);
		}
		draw();
	}

	return;
}


void kRubik::deleteBuffers() {
	for (int i = 0; i < cubitos.size(); i++)
	{
		cubitos[i].deleteBuffers();
	}

	return;
}