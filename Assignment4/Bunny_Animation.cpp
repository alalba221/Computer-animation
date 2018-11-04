#include "Bunny_Animation.h"
#include "glm/ext.hpp"
using namespace std;
using namespace glm;

Bunny_Animation::Bunny_Animation()
{
	this->m_model_mat = glm::mat4(1.0f);
}


Bunny_Animation::~Bunny_Animation()
{
}

void Bunny_Animation::init()
{
	reset();
}

vector<glm::vec4> Bunny_Animation::CreatTable(const Curve* curve) {
	vector<vec4> result;

	vector<vec3> positions = curve->curve_points_pos;
	vector<vec3>::iterator it;
	
	float arcLength = 0;
	for (it = positions.begin(); it != positions.end(); it++) {

		vec4 entry;
		if (it == positions.begin()) {
			arcLength = 0;
		}
		else {
			arcLength = arcLength + length((*it) - (*(it - 1)));
			//cout << to_string(*(it-1))<< endl;
		}
		entry = vec4(*it, arcLength);
		result.push_back(entry);
	}
	return result;
}

vector<vec3> Bunny_Animation::Convert_table_to_Norm_U2S(const vector<vec4>& table) {
	vector<vec3> result;
	vector<vec4>::const_iterator it;
	float length = table[table.size()-1][3];
	float index = 0;
	int segment = 0;
	float u_val = 0;
	for (it = table.begin(); it != table.end(); it++,index++) {
		segment = index / (m_animation_curve->num_points_per_segment + 1);
		u_val = (index - segment*(m_animation_curve->num_points_per_segment + 1)) / (m_animation_curve->num_points_per_segment + 1);
		vec3 entry = vec3(segment,u_val, (*it)[3]/length);
		result.push_back(entry);
	}

	return result;
}


vec2 Bunny_Animation::table(const vector<vec3>& table_us,float s) {

	int start = 0;
	int end = table_us.size() - 1;
	vec2 result;
	
	while (start + 1 < end) {
		int mid = (start + end) / 2;
		
		if (table_us[mid][2] == s) {
			end = mid;
			start = mid;
		} 
		else if (table_us[mid][2] < s) {
			start = mid;
		}
		else if (table_us[mid][2] > s) {
			end = mid;
		}
	}

	if (table_us[start][2] == s) {
		end = start;
	}
	else if (table_us[end][2] == s) {
		start = end;
	}
	else { // For target s isn't in the table
		result =vec2 (table_us[end][0], table_us[start][1] + (s - table_us[start][2]) / (table_us[end][2] - table_us[start][2])*(table_us[end][1] - table_us[start][1]));
		return result;
	}
	result = vec2(table_us[start][0],table_us[start][1]);
	return result;
}

// Point(Location) and Orientation
vec3 Bunny_Animation::calcLocation(const vec2& u) {

	int Start = u[0];
	int End = Start + 1;
	int Prev = Start - 1;
	int Post = End + 1;
	
	if (Start == 0) {

		Prev = m_animation_curve->num_of_controlPoints - 1;
	}
	else if (Start == m_animation_curve->num_of_controlPoints - 1) {
		End = 0;
		Post = End + 1;
	}

	if (End == m_animation_curve->num_of_controlPoints - 1) {
		Post = 0;
	}

	//float u_val = (1.0 / (float)m_animation_curve->num_points_per_segment)*(i + 1);
	vec3 Point = m_animation_curve->output_one_point(u[1], m_animation_curve->CoefMatrix, m_animation_curve->control_points_pos[Prev], m_animation_curve->control_points_pos[Start], m_animation_curve->control_points_pos[End], m_animation_curve->control_points_pos[Post]);
	return Point;
}

mat4 Bunny_Animation::calcOrientation(const vec2& u) {
	mat4 result;
	int Start = u[0];
	int End = Start + 1;

	if (Start == m_animation_curve->num_of_controlPoints - 1) {
		End = 0;
	}
	
	double double_u = (double) u[1];
	
	glm::dquat interpolatedquat = mix(m_animation_curve->control_points_quaternion[Start], m_animation_curve->control_points_quaternion[End], double_u);
	result = toMat4(interpolatedquat);

	//cout << result[2][1]<< endl;
	return result;
}


void Bunny_Animation::init(Curve* animation_curve)
{
	m_animation_curve = animation_curve;

	table_P2S = CreatTable(m_animation_curve);
	norm_table_US = Convert_table_to_Norm_U2S(table_P2S);
	reset();
}



float Bunny_Animation::ease(float t) {

	float v0 = 1 / (0.5*t1 + 0.5 - 0.5*t2 + t2 - t1);
	float v = 0;
	float s;
	
	if (t < 0) {
		s = 0;
	}
	else if (t <= t1 && t >= 0) {
		v = (v0*t) / t1;
		s = v*t*0.5;
	}
	else if (t > t1 && t <= t2) {
		v = v0;
		s = 0.5*v0*t1 + v*(t - t1);
	}
	else if (t > t2 && t <= 1) {
		v = v0*(1.0 - (t - t2) / (1 - t2));
		s = 1-v*(1-t)*0.5;
	}
	else if (t > 1) {
		s = 1.0;
	}

	return s;
}
void Bunny_Animation::update(float delta_time)
{
	//float current_frame = glfwGetTime();
	m_model_mat = glm::mat4(1.0f);

	if (isMoving) {
		current_time = current_time + delta_time;
	}
	else {
		current_time = 0;
	}

	float norm_time = current_time / total_moving_time;

	float s = ease(norm_time);
	vec2 u = table(norm_table_US,s);
	vec3 Point = calcLocation(u);

	mat4 Rotation = calcOrientation(u);

	mat4 Affine = Rotation;

	Affine[3][0] = Point[0];
	Affine[3][1] = Point[1];
	Affine[3][2] = Point[2];

	//cout << to_string(Affine[0])<< endl;
	m_model_mat = Affine*m_model_mat;
	//m_model_mat = glm::translate(m_model_mat, Point);


}

void Bunny_Animation::reset()
{
	m_model_mat = glm::mat4(1.0f);
	if (m_animation_curve != nullptr && m_animation_curve->control_points_pos.size() > 0)
	{
		m_model_mat = glm::translate(m_model_mat, m_animation_curve->control_points_pos[0]);
	}
}
