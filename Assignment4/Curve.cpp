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

	this->control_points_quaternion =
	{
		{ 0.991015,0.00883871,-0.102529,-0.085432 },
		{ 0.998588,-0.000207206,-0.052976,0.0039058 },
		{ 0.742373,0.186137,0.229859,0.601165 },
		{ -0.109644,0.356721,0.926794,-0.0422019 },
		{ 0.0248536,-0.036511,0.999024,-0.000908318 },
		{ 0.136048,-0.366425,0.918847,-0.0542544 },
		{ 0.767665,-0.192912,0.270156,-0.548171 },
		{ 0.999426,0.000371592,-0.0116842,-0.0317848 }
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
