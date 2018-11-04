#include "Curve.h"
#include "glm/ext.hpp"

float Curve::Array[16] = {-0.5,1.0,-0.5,0.0,
							1.5,-2.5,0.0,1.0,
							-1.5,2.0,0.5,0.0,
							0.5,-0.5,0.0,0.0};
mat4 Curve::CoefMatrix;
Curve::Curve()
{
	CoefMatrix = make_mat4(Array);
	
}

Curve::~Curve()
{
}

void Curve::init()
{
	this->control_points_pos = {
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 }
	};
	num_of_controlPoints = control_points_pos.size();
	calculate_curve();
}


vec3 Curve::output_one_point(	float u,
								const mat4& CoefMatrix,
								const vec3& P0,
								const vec3& P1,
								const vec3& P2,
								const vec3& P3) 
{
	vec3 result;
	vec4 Variable(u*u*u, u*u, u, 1);


	mat4x3 Geo(P0, P1, P2, P3); // 4 cols and 3 rows

	mat3x4 GeoInfo = transpose(Geo);

	result = Variable*CoefMatrix*GeoInfo;
	return result;
}
vector<vec3> Curve::FillSegment(int Start, int End) {

	int Prev = 0;
	int Post = 0;
	if (Start == 0) {
		
		Prev = num_of_controlPoints-1;
	}
	else {
		Prev = Start - 1;
	}

	if (End == num_of_controlPoints-1) {
		Post = 0;
	}
	else {
		Post = End + 1;
	}

	vector<vec3> result;
	for (int i = 0; i < num_points_per_segment; i++) {

		float u = (1.0 /(float) num_points_per_segment)*(i+1);
		vec3 Point = output_one_point(u, CoefMatrix, control_points_pos[Prev], control_points_pos[Start], control_points_pos[End], control_points_pos[Post]);
		result.push_back(Point);
		//cout << u << " ";
	}

	return result;
}

void Curve::calculate_curve()
{

/*	
	this->curve_points_pos = {
		
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 },
	};
*/
	int num_of_controlPoints = control_points_pos.size();
	
	for (int i = 0; i < num_of_controlPoints; i++) {
		vector<vec3> Points = FillSegment(i, (i + 1) % num_of_controlPoints);
		curve_points_pos.push_back(control_points_pos[i]);
		
		vector<vec3>::iterator it;
		it = curve_points_pos.end();
		curve_points_pos.insert(it, Points.begin(),Points.end());
	}

}
