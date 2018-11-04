#include "Bone_Animation.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

Bone_Animation::Bone_Animation()
{
	
	root_position = { 2.0f,0.0f,2.0f };

	scale_vector =
	{
		{ 1.0f,1.0f,1.0f },
		{ 0.5f,4.0f,0.5f },
		{ 0.5f,3.0f,0.5f },
		{ 0.5f,2.0f,0.5f }
	};

	rotation_degree_vector =
	{
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f }
	};

/*
	rotation_degree_vector =
	{
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,-90.0f,0.0f },
		{ 90.0f,90.0f,0.0f },
		{ 0.0f,90.0f,0.0f }
	};
	*/
	colors =
	{
		{ 0.7f,0.0f,0.0f,1.0f },
		{ 0.7f,0.7f,0.0f,1.0f },
		{ 0.7f,0.0f,0.7f,1.0f },
		{ 0.0f,0.7f,0.7f,1.0f }
	};


	arcs = new Arc* [4];
	nodes = new Node*[4];
	for (int i = 0; i < 4; i++) {
		arcs[i] = new Arc();
		nodes[i] = new Node(scale_vector[i]);
	}
}


Bone_Animation::~Bone_Animation()
{
	for (int i = 0; i < 4; i++) {
		delete[] arcs[i];
		delete[] nodes[i];
 	}
	delete[] arcs;
	delete[] nodes;
}

void Bone_Animation::init()
{

}

void Bone_Animation::update(float delta_time)
{
	///init all nodes and joints
	Matrixes.clear();
	for (int i = 0; i < 4; i++) {

		arcs[i]->theta_Y = rotation_degree_vector[i][0];
		arcs[i]->theta_Z = rotation_degree_vector[i][1];
		arcs[i]->theta_X = rotation_degree_vector[i][2];

		// move to origin
		arcs[i]->Trans2Origin =
			translate(mat4(), vec3(0, scale_vector[i][1] / 2, 0.0f));
		// link matrix
		if (i == 0)
			arcs[i]->Lmatrix = translate(mat4(), root_position);
		else
			arcs[i]->Lmatrix =
			translate(mat4(), vec3(0, scale_vector[i - 1][1] / 2, 0.0f));
		// rotate matrix
		if (i == 0) {
			arcs[i]->Amatrix = mat4(1);
		}
		else {
			double x = radians((float)arcs[i]->theta_X);

			double y = radians((float)arcs[i]->theta_Y);
			double z = radians((float)arcs[i]->theta_Z);

			glm::mat4 transformX = glm::eulerAngleX(x);
			glm::mat4 transformY = glm::eulerAngleY(y);
			glm::mat4 transformZ = glm::eulerAngleZ(z);
			arcs[i]->Amatrix =
				transformX * transformZ * transformY;
			
		}


		nodes[i]->color = colors[i];
	}
	///Build Tree
	for (int i = 0; i < 4; i++) {
		arcs[i]->nPtr = nodes[i];
		if (i < 3)
			nodes[i]->arcPtr = arcs[i + 1];
		else
			nodes[i]->arcPtr = nullptr;
	}

	
	mat4 matrix = mat4(1);
	Traverse(arcs[0], matrix);
}

void Bone_Animation::reset()
{
	rotation_degree_vector = { vec3(0),vec3(0),vec3(0),vec3(0) };
}

void Bone_Animation::Traverse(Arc* head, mat4 matrix) {
	if (head == nullptr)
		return;
	matrix = matrix * head->Lmatrix;
	matrix = matrix * head->Amatrix;
	matrix = matrix * head->Trans2Origin;
	// get node and transform data
	
	Node* nodePtr = head->nPtr;
	S.push(matrix);
	matrix = matrix * (*(nodePtr->obj));
	//cout << to_string(matrix)<< endl;
	//draw(aData);
	Matrixes.push_back(matrix);
	matrix = S.top();
	S.pop();
		// process children

	Arc* nextArcPtr = nodePtr->arcPtr;
	Traverse(nextArcPtr, matrix);

	

}
