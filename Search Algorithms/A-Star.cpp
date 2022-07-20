#include <iostream>
#include <vector>
#include <queue>
#include <math.h>

class Coordinates {
public:
	int row;
	int col;
	Coordinates(int r, int c) : row(r), col(c) {}
	Coordinates(const Coordinates& c) : row(c.row), col(c.col) {}

	bool operator==(const Coordinates& c) {
		return (this->row == c.row && this->col == c.col);
	}

	Coordinates operator+ (const Coordinates& c) {
		return (Coordinates({ this->row + c.row, this->col + c.col }));
	}
};

class Node {
public:
	Coordinates point;
	Node* parent;
	float g = 0.0;
	float h = 0.0;
	float f = 0.0;
	Node(Coordinates c, Node* p = nullptr, float g = 0.0, float h = 0.0, float f = 0.0) :
		point(c), parent(p), g(g), h(h), f(f) {}
	Node(const Node& c) : point(c.point), parent(c.parent), g(c.g), h(c.h), f(c.f){}
	bool operator< (const Node& c) {
		return this->f > c.f;
	}
};

class Search {
public:
	Coordinates start, goal;
	Search(Coordinates s, Coordinates g) : start(s), goal(g) {}
	void calculate_g(Node*& c);
	void calculate_h(Node*& c);
	void calculate_f(Node*& c);
	bool isValid(const Node& c, const std::vector<std::vector<bool>>& visited, const std::vector<std::vector<int>>& grid);
	void clear_Open_Queue(std::priority_queue<Node*>& q);
	void clear_Closed_Queue(std::queue<Node*>& q);
	std::vector<Coordinates> a_Star_Search(const std::vector<std::vector<int>>& grid);
};

void Search::calculate_g(Node*& c) {
	if (c->parent == nullptr)
		c->g = 0.0;
	else {
		int row1 = c->point.row;
		int row2 = c->parent->point.row;
		int col1 = c->point.col;
		int col2 = c->parent->point.col;
		// Manhattan Distance
		c->g = c->parent->g + abs(row2 - row1) + abs(col2 - col1);

		// Euclidean Distance
		//c->g = c->parent->g + sqrt(pow(row2 - row1, 2) + pow(col2 - col1, 2));
	}
}

void Search::calculate_h(Node*& c) {
	int row1 = c->point.row;
	int col1 = c->point.col;
	int row2 = goal.row;
	int col2 = goal.col;

	// Manhattan Distance
	c->h = abs(row2 - row1) + abs(col2 - col1);

	// Euclidean Distance
	//c->h = sqrt(pow(row2 - row1, 2) + pow(col2 - col1, 2));
}

void Search::calculate_f(Node*& c) {
	calculate_g(c);
	calculate_h(c);
	c->f = c->g + c->h;
}

bool Search::isValid(const Node& c, const std::vector<std::vector<bool>>& visited, const std::vector<std::vector<int>>& grid) {
	if (c.point.row < 0 || c.point.row >= grid.size() ||
		c.point.col < 0 || c.point.col >= grid[0].size() ||
		grid[c.point.row][c.point.col] == 1 ||
		visited[c.point.row][c.point.col] == true)
		return false;
	return true;
}

void Search::clear_Open_Queue(std::priority_queue<Node*>& q) {
	while (!q.empty()) {
		Node* node = q.top();
		q.pop();
		delete node;
	}
}

void Search::clear_Closed_Queue(std::queue<Node*>& q) {
	while (!q.empty()) {
		Node* node = q.front();
		q.pop();
		delete node;
	}
}

std::vector<Coordinates> Search::a_Star_Search(const std::vector<std::vector<int>>& grid) {
	std::vector<Coordinates> path;
	bool found = true;
	int steps = 0;

	int m = grid.size();
	int n = grid[0].size();
	
	std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));
	std::vector<Coordinates> dir = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	std::priority_queue<Node*> open_queue;
	std::queue<Node*> closed_queue;
	Node* current = nullptr;
	open_queue.push(new Node(start));
	while (!open_queue.empty()) {
		current = open_queue.top();
		open_queue.pop();
		closed_queue.push(current);

		steps++;
		
		if (current->point == goal) {
			found = true;
			break;
		}
		visited[current->point.row][current->point.col] = true;
		for (Coordinates point : dir) {
			Node neighbor(current->point + point);
			if (isValid(neighbor, visited, grid)) {
				Node* successor = new Node(neighbor);
				successor->parent = current;
				calculate_f(successor);
				open_queue.push(successor);
			}
		}
	}
	if (found) {
		std::cout << "Path found in " << steps << " steps." << std::endl;
		while (current != nullptr) {
			path.push_back(current->point);
			current = current->parent;
		}
	}
	clear_Open_Queue(open_queue);
	clear_Closed_Queue(closed_queue);
	reverse(path.begin(), path.end());
	return path;
}


void aStarSearch() {
	std::vector<std::vector<int>> grid =
	{
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 0, 0, 1, 1, 0, 1, 0, 1 },
		{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 1, 0, 1, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 0, 1, 0, 1 },
		{ 1, 1, 1, 0, 0, 1, 1, 0, 0, 1 }
	};

	Coordinates start(0, 1), goal(8, 8);
	Search astar(start, goal);
	std::vector<Coordinates> path;
	path = astar.a_Star_Search(grid);
	for (Coordinates point : path) {
		std::cout << "{" << point.row << "," << point.col << "} ";
	}
}

int main() {
	aStarSearch();
	return 0;
}