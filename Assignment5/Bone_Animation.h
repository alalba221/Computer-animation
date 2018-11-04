#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	
#include <glm/gtx/matrix_operation.hpp>
using namespace std;
using namespace glm;

class Node;
class Arc;

class Node{
public:
	vec3 scale_vector;
	Arc* arcPtr;
	mat4* obj;
	vec4 color;

	vec3 basePoint;
	vec3 topPoint;

	Node(vec3 & s):scale_vector(s) {
		obj = new mat4();
		vec4 v = vec4(scale_vector, 1.0);
		*obj = diagonal4x4(v);
	};
	~Node() { delete obj; };
};

class Arc {
public:
	Arc() {};
	~Arc() {};

	mat4 Trans2Origin = mat4();
	mat4 Lmatrix = mat4(); //for Link
	mat4 Amatrix = mat4(); // FOR ROTATION
	
	int theta_X=0;
	int theta_Y=0;
	int theta_Z=0;

	Node* nPtr;

};

class Bone_Animation
{
public:

	
	Bone_Animation();
	~Bone_Animation();

	void init();
	void update(float delta_time);
	void reset();
	void Traverse(Arc* head, mat4 matrix);

	Arc** arcs;
	Node** nodes;
	
	

public:

	// Here the head of each vector is the root bone
	std::vector<glm::vec3> scale_vector;
	std::vector<glm::vec3> rotation_degree_vector;
	std::vector<glm::vec4> colors;

	glm::vec3 root_position;

	stack<mat4> S;
	vector<mat4> Matrixes;
};

