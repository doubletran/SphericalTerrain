#pragma once
#include <queue>
#include <string>
#include <algorithm>
#include "icVector.H"
using namespace std;
static double roughness;
struct Vertex {
	icVector3 pos;
	Vertex(const icVector3 & a) {
		pos = a;
	}
	Vertex(double x_, double y_, double z_) {
		pos = icVector3(x_, y_, z_);

	};
	Vertex(const double pos_[3]) {
		pos = icVector3(pos_);
	}
	void fractalDisplace(const double height, const double offset) {
		double min = -height / 2;
		double max = height / 2;
		double displace = min + (max - min) * rand() / RAND_MAX;
		displace += offset;
		pos = pos * (displace + 1);
	}
};
const char opposite_type_lookup[2][2] = { {'L', 'R'}, {'T', 'C'} };
const char adj_rule_lookup[3][2][2] =
{
	{ {'L', 'R'}, {'T', 'C'} }, //Bottom Edge
	{ {'C', 'R'}, {'L', 'T'} }, //Left Edge
	{ {'L', 'C'}, {'T', 'R'} }	//Right Edge

};
class Node {
public:
	Vertex *a, *b, *c;
	Vertex *ab, *bc, *ca;
	Node* parent;
	Node* left, * top, * center, * right;
	char node_type;
	double height_range;
	string quad_path = "";
	Node(Node* parent_, char type_, Vertex *a_, Vertex *b_, Vertex *c_) {
		parent = parent_;
		node_type = type_;
		a = a_;
		b = b_;
		c = c_;
		left = nullptr;
		right = nullptr;
		top = nullptr;
		ab = nullptr;
		bc = nullptr;
		ca = nullptr;
		center = nullptr;
		if (parent != nullptr) {
			height_range = parent->height_range* roughness;
			quad_path = parent->quad_path;
			quad_path.push_back(type_);
		}
	}
	char swap(const char type, const char lookup[2][2]) {
		for (int i = 0; i < 2; i++) {
			if (type== lookup[i][0]) {
				return lookup[i][1];
			}
			else if (type == lookup[i][1]) {
				return lookup[i][0];
			}
		}


	}
	string getNeighborPath(char neighbor_type) {
		string neighbor_path = quad_path;
		char opposite_type = swap(neighbor_type, opposite_type_lookup);
		int common_ancestor_index = quad_path.find_last_of(opposite_type);
		int last_center_index = quad_path.find_last_of('C');
		common_ancestor_index = last_center_index > common_ancestor_index ? last_center_index : common_ancestor_index;
		if (common_ancestor_index < 0) {
			return "";
		}
		for (int i = common_ancestor_index; i < neighbor_path.length(); i++) {
			char curr_opposite_type;
			if (neighbor_type == 'B') {
				curr_opposite_type = swap(quad_path[i], adj_rule_lookup[0]);
			}
			else if (neighbor_type == 'L') {
				curr_opposite_type = swap(quad_path[i], adj_rule_lookup[1]);
			}
			if (neighbor_type == 'R') {
				curr_opposite_type = swap(quad_path[i], adj_rule_lookup[2]);
			}
			neighbor_path[i] = curr_opposite_type;
			
		}
		return neighbor_path;

	}


};
class QuadTree {
public:
	Node* root;

	QuadTree(const double variance, const double roughness, const int);
	void subdivideTree(const int depth);
	void subdivideNode(Node*);
	Node* getNode(string path);

};

