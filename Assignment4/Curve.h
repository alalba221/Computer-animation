#pragma once
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;
class Curve
{
public:
	Curve();
	~Curve();
	
	void init();
	void calculate_curve();

	static glm::mat4 quat_to_rotation_mat(const glm::quat m_quat);

//private:	
	static float Array[16];
	static glm::mat4 CoefMatrix;
	vec3 output_one_point(	float u,
							const mat4& CoefMatrix,
							const vec3& P0,
							const vec3& P1,
							const vec3& P2,
							const vec3& P3);
	int num_of_controlPoints;
public:
	
	vector<vec3> FillSegment(int Start, int End);


	


	float tau = 0.5; // Coefficient for catmull-rom spline
	int num_points_per_segment = 5;

	std::vector<glm::vec3> control_points_pos;
	std::vector<glm::vec3> curve_points_pos;
	std::vector<glm::dquat> control_points_quaternion;

};