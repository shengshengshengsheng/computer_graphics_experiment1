#pragma once
#include "Angel.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
struct face {
	vec3 V;
	vec3 T;
	vec3 N;
};
class objLoader
{
private:
	int v_num = 0;
	int vn_num = 0;
	int f_num = 0;
	vec3 a;
	vec3 b;
	vec3 c;
public:
	vector<vec3> vArr;
	vector<vec3> vnArr;
	vector<face> f;
	bool loadobj(string filename);

};