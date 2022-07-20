#include <iostream>
#include <vector>
#include <queue>

std::vector<std::pair<int, int>> path;

std::pair<int, int> start = { 0,0 };
std::pair<int, int> goal = { 6,6 };

void getPath(std::pair<int,int>& current,
			 std::vector<std::vector<std::pair<int,int>>>& parentList) {
		
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

void dfs(const std::vector<std::vector<int>>& grid, 
		 std::pair<int, int> currentNode, 
		 std::pair<int,int> parent,
		 std::vector<std::vector<bool>>& visited,
		 std::vector<std::vector<std::pair<int,int>>>& parentList) {
	//std::pair<int, int> currentNode = start;
	int i = currentNode.first;
	int j = currentNode.second;
	//std::cout << "child:" << i << "," << j << "parent::" << parent.first << "," << parent.second << std::endl;
	if (i < 0 || j < 0 || i >= grid[0].size() || j >= grid.size() || grid[i][j] == 1 || visited[i][j] == true)
		return;
	parentList[i][j] = parent;
	//std::cout << "child:"<<i<<","<<j<<"parent::" << parent.first << "," << parent.second << std::endl;

	visited[i][j] = true;
	if (i == goal.first && j == goal.second) {
		getPath(currentNode, parentList);
		return;
	}
	
	dfs(grid, std::make_pair(i - 1, j - 1), currentNode, visited, parentList);
	dfs(grid, std::make_pair(i + 1, j + 1), currentNode, visited, parentList);
	dfs(grid, std::make_pair(i + 1, j - 1), currentNode, visited, parentList);
	dfs(grid, std::make_pair(i - 1, j + 1), currentNode, visited, parentList);
	dfs(grid, std::make_pair(i + 1, j), currentNode, visited, parentList);
	dfs(grid, std::make_pair(i, j + 1), currentNode, visited, parentList);
	dfs(grid, std::make_pair(i - 1, j), currentNode, visited, parentList);
	dfs(grid, std::make_pair(i, j - 1), currentNode, visited, parentList);
	
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

	for (auto &row:grid) {
		for (auto &point: row) {
			std::cout << point << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;



	std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));
	std::vector<std::vector<std::pair<int, int>>> parentList(m, std::vector<std::pair<int, int>>(n));
	dfs(grid, start, std::make_pair(-1,-1), visited, parentList);
	//std::cout << path.size() << std::endl;
	for (const auto& i : path) {
		//std::cout << i.first << "," << i.second << " ";
		grid[i.first][i.second] = 9;
	}
	std::cout<< std::endl;
	for (auto& row : grid) {
		for (auto& point : row) {
			std::cout << point << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return 0;
}