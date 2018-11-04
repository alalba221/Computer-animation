#pragma once

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Curve.h"

class Bunny_Animation
{

public:
	float total_moving_time = 10;
	float t1 = 0.1;
	float t2 = 0.7;
	float current_time = 0;
private:
	glm::mat4 m_model_mat;
	Curve* m_animation_curve = nullptr;

	vector<glm::vec4> table_P2S;
	vector<glm::vec3> norm_table_US;
	vector<glm::vec4> CreatTable(const Curve* curve);

	vector<vec3> Convert_table_to_Norm_U2S(const vector<vec4>& table);
	
	// find the value of U given the arc length S. 
	// Since the arc length is monotonically increasing, 
	// a binary search is a efficient way to locate the closest 2 entries on either side of the given S. 
	// The two U values corresponding to these 2 arc entries can be used to linearly interpolate the target U value 
	vec2 table(const vector<vec3>& table_us,float s);

	float ease(float t);
	vec3 calcPoint(const vec2& u);
public:
	Bunny_Animation();
	~Bunny_Animation();

	void init();
	void init(Curve* animation_curve);

	void update(float delta_time);

	void reset();
	glm::mat4 get_model_mat() { return m_model_mat; };
};

