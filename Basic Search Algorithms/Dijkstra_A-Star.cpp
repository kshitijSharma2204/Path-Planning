#include <iostream>
#include <vector>
#include <math.h>
#include <queue>

class Coordinates {
public:
	int row;
	int col;
	Coordinates(int r, int c) : row(r), col(c) {}
	bool operator== (const Coordinates& c) {
		return (this->row == c.row && this->col == c.col);
	}

	Coordinates operator+(const Coordinates& point) {
		return Coordinates({ this->row + point.row, this->col + point.col });
	}
};

class Node {
public:
	Coordinates point;
	
	float g = 0.0;
	float h = 0.0;
	float f = 0.0;
	Node* parent;
	Node(Coordinates p, float g = 0.0, float h = 0.0, float f = 0.0, Node* parent = nullptr) :
		point(p), g(g), h(h), f(f), parent(parent) {}
	Node(const Node& node) :
		point(node.point), g(node.g), h(node.h), f(node.f), parent(node.parent) {}
	bool operator< (const Node& node) {
		return this->f > node.f;
	}
};

//bool operator< (const Node& node1, const Node& node2) {
//	return node1.f > node2.f;
//}

class Search {
public:
	Coordinates start, goal;
	Search(Coordinates s, Coordinates g) : start(s), goal(g) {}
	void calculate_g(Node* node);
	void calculate_h(Node* node, char ch);
	void calculate_f(Node* node, char ch);
	bool isValid(const Node& node, const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& visited);
	void clear_Open_Queue(std::priority_queue<Node*>& q);
	void clear_Closed_Queue(std::queue<Node*>& q);
	std::vector<Coordinates> dijkstra_Search(const std::vector<std::vector<int>>& grid);
	std::vector<Coordinates> a_Star_Search(const std::vector<std::vector<int>>& grid);
};

void Search::calculate_g(Node* node) {
	if (node->parent == nullptr) {
		node->g = 0.0;
	}
	else {
		int row1 = node->point.row;
		int col1 = node->point.col;
		int row2 = node->parent->point.row;
		int col2 = node->parent->point.col;
		
		// Manhattan Distance
		node->g = node->parent->g + abs(row2 - row1) + abs(col2 - col1);

		// Euclidean Distance
		//node->g = node->parent->g + sqrt(pow((row2 - row1), 2) + pow((col2 - col1), 2));
	}
}

void Search::calculate_h(Node* node, char ch) {
	if (ch == 'd')
		node->h = 0.0;
	else if (ch == 'a') {
		int row1 = node->point.row;
		int col1 = node->point.col;
		int row2 = goal.row;
		int col2 = goal.col;

		// Manhattan Distance
		node->h = abs(row2 - row1) + abs(col2 - col1);

		// Euclidean Distance
		//node->h = sqrt(pow((row2 - row1), 2) + pow((col2 - col1), 2));
	}
}

void Search::calculate_f(Node* node, char ch) {
	calculate_g(node);
	calculate_h(node, ch);
	node->f = node->g + node->h;
}

bool Search::isValid(const Node& node, const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& visited) {
	int row = node.point.row;
	int col = node.point.col;
	if (row < 0 || row >= grid.size() ||
		col < 0 || col >= grid[0].size() ||
		grid[row][col] == 1 ||
		visited[row][col] == true)
		return false;
	return true;
}

void Search::clear_Open_Queue(std::priority_queue<Node*>& open_queue) {
	while (!open_queue.empty()) {
		Node* node = open_queue.top();
		open_queue.pop();
		delete node;
	}
}

void Search::clear_Closed_Queue(std::queue<Node*>& closed_queue) {
	while (!closed_queue.empty()) {
		Node* node = closed_queue.front();
		closed_queue.pop();
		delete node;
	}
}

std::vector<Coordinates> Search::dijkstra_Search(const std::vector<std::vector<int>>& grid) {
	int steps = 0;
	std::vector<Coordinates> path;
	bool found = false;
	
	std::priority_queue<Node*> open_queue;
	std::queue<Node*> closed_queue;

	int m = grid.size(), n = grid[0].size();
	std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));

	if (grid[goal.row][goal.col] == 1 || grid[start.row][start.col] == 1) {
		std::cout << "\nStart/Goal is invalid.\n" << std::endl;
		return path;
	}

	Node* current = nullptr;
	open_queue.push(new Node(start));
	const std::vector<Coordinates> dir = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };
	while (!open_queue.empty()) {
		current = open_queue.top();
		open_queue.pop();
		closed_queue.push(current);
		visited[current->point.row][current->point.col] = true;
		steps++;
		if (current->point.row == goal.row && current->point.col == goal.col) {
			found = true;
			break;
		}
		for (Coordinates point : dir) {
			Node neighbor({ current->point + point });
			if (isValid(neighbor, grid, visited)) {
				Node* successor = new Node(neighbor);
				successor->parent = current;
				calculate_f(successor, 'd');
				open_queue.push(successor);
			}
		}
	}
	if (found) {
		std::cout << "Steps = " << steps << std::endl;
		while (current != nullptr) {
			path.push_back({ current->point.row, current->point.col });
			current = current->parent;
		}
	}
	else
		std::cout << "Path not found." << std::endl;
	clear_Open_Queue(open_queue);
	clear_Closed_Queue(closed_queue);
	reverse(path.begin(), path.end());
	return path;
}

std::vector<Coordinates> Search::a_Star_Search(const std::vector<std::vector<int>>& grid) {
	std::vector<Coordinates> path;
	bool found = true;
	int steps = 0;

	std::priority_queue<Node*> open_queue;
	std::queue<Node*> closed_queue;

	int m = grid.size(), n = grid[0].size();
	std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));

	if (grid[goal.row][goal.col] == 1 || grid[start.row][start.col] == 1) {
		std::cout << "\nStart/Goal is invalid.\n" << std::endl;
		return path;
	}

	Node* current = nullptr;
	open_queue.push(new Node(start));
	const std::vector<Coordinates> dir = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };

	while (!open_queue.empty()) {
		current = open_queue.top();
		open_queue.pop();
		closed_queue.push(current);
		
		steps++;
		visited[current->point.row][current->point.col] = true;
		if (current->point.row == goal.row && current->point.col == goal.col) {
			found = true;
			break;
		}

		for (Coordinates point : dir) {
			Node neighbor({ current->point + point });
			if (isValid(neighbor, grid, visited)) {
				Node* successor = new Node(neighbor);
				successor->parent = current;
				calculate_f(successor, 'a');
				open_queue.push(successor);
			}
		}
	}
	if (found) {
		std::cout << "\n\nSteps = " << steps << std::endl;
		while (current != nullptr) {
			path.push_back({ current->point.row, current->point.col });
			current = current->parent;
		}
	}
	clear_Open_Queue(open_queue);
	clear_Closed_Queue(closed_queue);
	reverse(path.begin(), path.end());
	return path;
}

void dijkstraSearch() {
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
	Coordinates start(0, 1);
	Coordinates goal(8, 8);
	std::vector<Coordinates> path;
	Search dijkstra(start, goal);
	
	path = dijkstra.dijkstra_Search(grid);
	if (path.empty())
		std::cout << "No path found." << std::endl;
	else {
		std::cout << "Path found by Dijkstra is : " << std::endl;
		for (Coordinates point : path) {
			std::cout << "{" << point.row << "," << point.col << "} ";
		}
	}
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

	Coordinates start(0, 1);
	Coordinates goal(8, 8);
	std::vector<Coordinates> path;
	Search aStar(start, goal);
	path = aStar.a_Star_Search(grid);
	if (path.empty())
		std::cout << "No path found." << std::endl;
	else {
		std::cout << "Path found by A-Star is : " << std::endl;
		for (Coordinates point : path) {
			std::cout << "{" << point.row << "," << point.col << "} ";
		}
	}
}

int main() {
	dijkstraSearch();
	aStarSearch();
	return 0;
}