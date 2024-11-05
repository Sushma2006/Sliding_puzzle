#include <iostream> 
#include <vector> 
#include <queue> 
#include <unordered_set> 
#include <algorithm> 
#include <cmath> 
#include <string> 
 
using namespace std; 
 
const int N = 3; 
const vector<vector<int>> GOAL_STATE = { 
    {1, 2, 3}, 
    {4, 5, 6}, 
    {7, 8, 0} 
}; 
 
void printPuzzle(const vector<vector<int>>& puzzle) { 
    for (const auto& row : puzzle) { 
        for (int cell : row) { 
            cout << cell << " "; 
        } 
        cout << "\n"; 
    } 
    cout << "\n"; 
} 
 
int manhattanDistance(const vector<vector<int>>& puzzle) { 
    int distance = 0; 
    for (int i = 0; i < N; ++i) { 
        for (int j = 0; j < N; ++j) { 
            int value = puzzle[i][j]; 
            if (value != 0) { 
                int targetX = (value - 1) / N; 
  
                int targetY = (value - 1) % N; 
                distance += abs(i - targetX) + abs(j - targetY); 
            } 
        } 
    } 
    return distance; 
} 
 
struct PuzzleState { 
    vector<vector<int>> board; 
    int x, y; 
    int g, h; 
    vector<string> moves; 
 
    int f() const { return g + h; } 
 
    bool operator<(const PuzzleState& other) const { 
        return f() > other.f(); 
    } 
}; 
 
string boardToString(const vector<vector<int>>& board) { 
    string result; 
    for (const auto& row : board) 
        for (int cell : row) 
            result += to_string(cell); 
    return result; 
} 
 
bool isGoal(const vector<vector<int>>& board) { 
    return board == GOAL_STATE; 
} 
 
vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; 
vector<string> moveNames = {"Down", "Up", "Right", "Left"}; 
 
  
vector<PuzzleState> getNeighbors(const PuzzleState& state) { 
    vector<PuzzleState> neighbors; 
    for (int i = 0; i < directions.size(); ++i) { 
        int newX = state.x + directions[i].first; 
        int newY = state.y + directions[i].second; 
 
        if (newX >= 0 && newX < N && newY >= 0 && newY < N) { 
            auto newBoard = state.board; 
            swap(newBoard[state.x][state.y], newBoard[newX][newY]); 
             
            PuzzleState neighbor = {newBoard, newX, newY, state.g + 1, manhattanDistance(newBoard), 
state.moves}; 
            neighbor.moves.push_back(moveNames[i]); 
            neighbors.push_back(neighbor); 
        } 
    } 
    return neighbors; 
} 
 
bool solvePuzzle(const vector<vector<int>>& initial) { 
    int startX, startY; 
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j) 
            if (initial[i][j] == 0) { 
                startX = i; 
                startY = j; 
            } 
 
    PuzzleState startState = {initial, startX, startY, 0, manhattanDistance(initial), {}}; 
 
    priority_queue<PuzzleState> openSet; 
    openSet.push(startState); 
 
    unordered_set<string> closedSet; 
    closedSet.insert(boardToString(initial)); 
 
  
    while (!openSet.empty()) { 
        PuzzleState current = openSet.top(); 
        openSet.pop(); 
 
        if (isGoal(current.board)) { 
            cout << "Solution found with moves:\n"; 
            for (const auto& move : current.moves) { 
                cout << move << "\n"; 
            } 
 
            // Display the puzzle after each move 
            cout << "\nPuzzle progression:\n"; 
            vector<vector<int>> temp = initial; 
            int x = startX, y = startY; 
 
            for (const auto& move : current.moves) { 
                int dir = find(moveNames.begin(), moveNames.end(), move) - moveNames.begin(); 
                int newX = x + directions[dir].first; 
                int newY = y + directions[dir].second; 
 
                swap(temp[x][y], temp[newX][newY]); 
                x = newX; 
                y = newY; 
 
                cout << "Move: " << move << "\n"; 
                printPuzzle(temp); 
            } 
            return true; 
        } 
 
        for (const auto& neighbor : getNeighbors(current)) { 
            string neighborStr = boardToString(neighbor.board); 
            if (closedSet.find(neighborStr) == closedSet.end()) { 
                openSet.push(neighbor); 
                closedSet.insert(neighborStr); 
            } 
  
        } 
    } 
 
    cout << "No solution found.\n"; 
    return false; 
} 
 
int main() { 
    vector<vector<int>> puzzle(N, vector<int>(N)); 
 
    cout << "Enter the 3x3 sliding puzzle configuration (use 0 for the blank space):\n"; 
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j) 
            cin >> puzzle[i][j]; 
 
    cout << "\nInitial puzzle configuration:\n"; 
    printPuzzle(puzzle); 
 
    solvePuzzle(puzzle); 
 
    return 0; 
} 
