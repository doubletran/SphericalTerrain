#include "tetra.h"

const double a[] = { 0.f, 0.f, 1.f };
const double b[] = { 0.f, 0.94281f, -0.333333f };
const double c[] = { 0.816497f, -0.471405f, -0.333333f };
const double d[] = { -0.816497f, -0.471405f, -0.333333f };
const char left = 'L';
const char top = 'T';
const char center = 'C'; 
const char right = 'R';
int scale = 1;
double land_height = 0.985f;

enum Biome {
	TUNDRA,
	ICE,
	HOT_DESERT, //Grassland
	COLD_DESERT,  //subtropical dessert
	HOT_FOREST, //tripical forest
	COLD_FOREST,
	HOT_RAIN_FOREST,
	COLD_RAIN_FOREST
};
GLfloat BiomeColor[8][3]{
	{0.8f, 0.8f, 0.8f},
	{1.f, 1.f, 1.f},
	{0.89f, 0.83f, 0.64f},
	{0.79, 0.73, 0.45 },
	{0.624, 0.651, 0.102 },
	{0.42, 0.67, 0.46 },
	{0.0f, 0.36, 0.19},
	{0.24, 0.42, 0.31 }

};
//x-axis: TEMPERATURE
int BIOME_MATRIX[3][3] = {
	{TUNDRA, COLD_DESERT, HOT_DESERT},
	{ICE, COLD_FOREST, HOT_FOREST},
	{ICE, COLD_RAIN_FOREST, HOT_RAIN_FOREST}
};
enum Debug {
	TEMP,
	FACE,
	ELEVATION
};
int NowDebug = TEMP;

QuadTree::QuadTree(const double variance, const double roughness_, const int depth) {
	Vertex *vA = new Vertex(a);
	Vertex* vB = new Vertex(b);
	Vertex* vC = new Vertex(c);
	Vertex* vD = new Vertex(d);
	roughness = roughness_;
	root = new Node(nullptr, NULL, vA, vA, vA);
	root->height_range = variance;

	root->ab = vD;
	root->bc = vB;
	root->ca = vC;
	root->left = new Node(root, 'L', vC, vB, vA);
	root->top = new Node(root, 'T', vA, vD, vC);
	root->center = new Node(root, 'C', vB, vC, vD);
	root->right = new Node(root, 'R', vD, vA, vB);
	subdivideTree(depth);

}

void QuadTree::subdivideTree(const int depth) {
	int i = 0;

	while (i < depth) {
		queue<Node*> q;
		q.push(root);
		Node* t = nullptr;
		while (!q.empty()) {
			t = q.front();
			
			q.pop();
			if (i == depth - 1) {
				maxheight = length(t->a->pos) > maxheight ? length(t->a->pos) : maxheight;
				minheight = length(t->a->pos) < minheight ? length(t->a->pos) : minheight;
				TriangleList.push_back(t);
			}
			if (t->left != nullptr) {
				q.push(t->center);
				q.push(t->top);
				q.push(t->right);
				q.push(t->left);
			}
			else {
				subdivideNode(t);
			}
		}
		i++;
	}
	printf("Height %f %f", maxheight, minheight);
}

void QuadTree::subdivideNode(Node* curr) {
	string bottom_path = curr->getNeighborPath('C');
	string left_path = curr->getNeighborPath('L');
	string right_path = curr->getNeighborPath('R');
	Node* right = getNode(right_path);
	Vertex* ab = right ? right->ab : nullptr;
	if (ab == nullptr ) {

		ab = new Vertex(curr->a->pos + curr->b->pos);
		normalize(ab->pos);
		double offset = length(curr->a->pos)+ length(curr->b->pos);
		offset /= 2.f;
		offset -= 1;
		ab->fractalDisplace(curr->height_range, offset);
		curr->ab = ab;
	}
	Node* bottom = getNode(bottom_path);
	Vertex* bc = bottom? bottom->bc:nullptr;
	if (bc == nullptr) {

		bc = new Vertex(curr->b->pos + curr->c->pos);
		double offset = (length(curr->b->pos) + length(curr->c->pos));
		normalize(bc->pos);
		offset /= 2.f;
		offset -= 1;
		bc->fractalDisplace(curr->height_range, offset);
		curr->bc = bc;
	}
	Node* left = getNode(left_path);
	Vertex* ca = left? left->ca: nullptr;
	if (ca == nullptr) {
		ca = new Vertex(curr->c->pos + curr->a->pos);
		double offset = length(curr->c->pos) + length(curr->a->pos);
		offset /= 2.f;
		normalize(ca->pos);
		offset -= 1;
		ca->fractalDisplace(curr->height_range, offset);
		curr->ca = ca;
	}
	curr->left = new Node(curr, 'L', ca, bc, curr->c);
	curr->right = new Node(curr, 'R', ab, curr->b, bc);
	curr->top = new Node(curr, 'T', curr->a, ab, ca);
	curr->center = new Node(curr, 'C', bc, ca, ab);
}

Node* QuadTree::getNode(string path) {
	Node* node = nullptr;
	for (auto subpath : path) {
		if (node == nullptr) {
			node = root;
		}
		switch (subpath)
		{
			case 'L': 
				node = node->left;
				break;
			case 'R':
				node = node->right;
				break;
			case 'C':
				node = node->center;
				break;
			case 'T':
				node = node->top;
				break;
		default:
			break;
		}
	}
	return node;

}
void QuadTree::display() {
	int i = 0;
	int min_temp = -10;
	int max_temp = 30;
	for (auto& curr : TriangleList) {
		double height = length((curr->a->pos + curr->b->pos + curr->c->pos) * (1.f / 3.f));
		if (NowDebug == FACE) {
			if (curr->node_type == 'L') {
				SetMaterial(1.f, 0.f, 0.f, 1.f);
			}
			else if (curr->node_type == 'R') {
				SetMaterial(0.f, 1.f, 0.f, 1.f);
			}
			else if (curr->node_type == 'C') {
				SetMaterial(0.f, 0.f, 1.f, 1.f);
			}
			else if (curr->node_type == 'T') {
				SetMaterial(1.f, 1.f, 1.f, 1.f);
			}
		}
		else if (NowDebug == TEMP) {
			double temp;
			if (abs(curr->a->pos.x) * 90 < 30) {
				temp = max_temp;
			}
			else if (abs(curr->a->pos.x) * 90 < 60) {
				temp = 15;
			}
			else {
				temp = 0;
			}
	

			if (height > land_height) {
				temp = temp - ((height - land_height) * 650) + 25.f;
				/*
				if (temp < 0) {
					SetMaterial(1.0f, 1.0f, 1.0f, 1.f);
				}
				else if (temp < 10) {
					SetMaterial(0.5f, 0.5f, 0.5f, 1.f);
				}
				else if (temp < 20) {
					SetMaterial(1.f, 1.f, 0.f, 1.f);
				}
				else {
					SetMaterial(0.f, 1.f, 0.f, 1.f);
				}
				*/
				int temp_index = ((temp - min_temp) * 3 / (max_temp - min_temp));
				int prec_index = ((maxheight - height) * 3 / (maxheight - land_height));
				temp_index = max({ temp_index, 0 });
				temp_index = min({ temp_index, 2 });
				//prec_index = prec_index + (2 * rand() / RAND_MAX - 1);
				prec_index = max({ prec_index, 0 });
				prec_index = min({ prec_index, 2 });
				int biome = BIOME_MATRIX[prec_index][temp_index];
				SetMaterial(BiomeColor[biome][0], BiomeColor[biome][1], BiomeColor[biome][2], 1.f);
			}
			else {
				SetMaterial(0.f, 0.f, 1.f, 1.f);
			}
		}
		else if (NowDebug == ELEVATION) {
			double color = (height - minheight) / (maxheight - minheight);
			SetMaterial(color, color, color, 1.f);
		}
		glBegin(GL_TRIANGLES);
		glVertex3f(curr->a->pos.x * scale, curr->a->pos.y * scale, curr->a->pos.z * scale);
		glVertex3f(curr->b->pos.x * scale, curr->b->pos.y * scale, curr->b->pos.z * scale);
		glVertex3f(curr->c->pos.x * scale, curr->c->pos.y * scale, curr->c->pos.z * scale);
		glEnd();
	}
}



