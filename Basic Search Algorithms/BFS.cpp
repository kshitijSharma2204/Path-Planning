#include <iostream>
#include <vector>
#include <queue>

std::vector<std::pair<int, int>> path;

std::vector<std::pair<int, int>> dir = { {0,1},{1,0},{0,-1},{-1,0} };

bool isValid(int adjRow, int adjCol,
	std::vector<std::vector<int>>& grid,
	std::vector<std::vector<bool>>& visited) {
	if (adjRow < 0 || adjRow >= grid.size() ||
		adjCol < 0 || adjCol >= grid[0].size() ||
		grid[adjRow][adjCol] == 1 ||
		visited[adjRow][adjCol] == true)
		return false;
	return true;
}	

void getPath(std::pair<int, int>& current, 
	std::pair<int,int> start,
	std::vector<std::vector<std::pair<int, int>>>& parentList) {

	//std::cout << "current in beginning --- i:" << current.first << ", j:" << current.second << std::endl;
	//std::cout << "start in beginning --- i:" << start.first << ", j:" << start.second << std::endl;

	while (current != start) {
		//std::cout <<"i:"<< current.first << ", j:" << current.second << std::endl;
		path.push_back(current);
		current = parentList[current.first][current.second];
		//std::cout << "while end - i:" << current.first << ", j:" << current.second << std::endl;
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
}

void bfs(std::vector<std::vector<int>>& grid,
	std::pair<int, int> startNode,
	std::pair<int, int> goalNode,
	std::vector<std::vector<bool>>& visited,
	std::vector<std::vector<std::pair<int, int>>>& parentList) {
	
	bool found = false;
	std::queue<std::pair<int, int>> q;
	q.push(startNode);
	
	while (!q.empty()) {
		std::pair<int, int> currentNode = q.front();
		q.pop();
		
		int row = currentNode.first;
		int col = currentNode.second;
		
		if (row == goalNode.first && col == goalNode.second) {
			getPath(currentNode, startNode, parentList);
			found = true;
			break;
		}
		
		visited[row][col] = true;

		for (int i = 0; i < dir.size(); i++) {
			int adjRow = row + dir[i].first;
			int adjCol = col + dir[i].second;
			if (isValid(adjRow, adjCol, grid, visited)) {
				//std::cout << "adjRow = " << adjRow << " " << "adjCol = " << adjCol << std::endl;
				q.push(std::make_pair(adjRow, adjCol));
				parentList[adjRow][adjCol] = currentNode;
			}
		}
	}
	if (found)
		std::cout << "Path is found." << std::endl;
	else
		std::cout << "No path can be found." << std::endl;
}

int main() {
	std::vector<std::vector<int>> grid = { {0,0,1,0,1,0,0},
										   {0,1,0,0,0,1,0},
										   {0,0,1,0,0,0,1},
										   {0,1,0,0,1,0,0},
										   {0,0,0,0,0,1,0},
										   {0,1,0,1,0,0,0},
										   {0,0,1,0,1,0,0} };

	int m = grid.size();
	int n = grid[0].size();

	for (auto& row : grid) {
		for (auto& point : row) {
			std::cout << point << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));
	std::vector<std::vector<std::pair<int, int>>> parentList(m, std::vector<std::pair<int, int>>(n));
	std::pair<int, int> start = { 0,0 };
	std::pair<int, int> goal = { 6,6 };
	bfs(grid, start, goal, visited, parentList);

	for (const auto& i : path) {
		//std::cout << i.first << "," << i.second << " ";
		grid[i.first][i.second] = 9;
	}
	std::cout << std::endl;
	for (auto& row : grid) {
		for (auto& point : row) {
			std::cout << point << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return 0;
}