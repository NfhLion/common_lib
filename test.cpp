#include <vector>
#include <string>



// 例1
vector<vector<int>> theList = { {}, {} };

vector<vector<int>> getThem()
{
	vector<vector<int>> list1;
	for (auto x : theList) {
		if (x[0] == 4)
			list1.push_back(x);
	}
	return list1;
}

/*
* 假设我们在开发一个扫雷游戏，theList表示为盘面的单元格列表，盘面上的每一个单元格用一个数组表示，
* 0下标的条目是一种状态值，该状态值为4的时候表示已标记。获取已标记的单元格。
*/

// 例2
vector<vector<int>> gameBoard = { {}, {} };
constexpr int STATUS_VALUE = 0;
constexpr int FLAGGED = 4;

vector<vector<int>> getFlaggedCells()
{
	vector<vector<int>> flaggedCells;
	for (const auto& cell : gameBoard) {
		if (cell[STATUS_VALUE] == FLAGGED)
			flaggedCells.push_back(cell);
	}
	return flaggedCells;
}

// 例3
constexpr int STATUS_VALUE = 0;
constexpr int FLAGGED = 4;
class Cell 
{
public:
    bool isFlagged() const { return _cell[STATUS_VALUE] == FLAGGED; }
private:
    vector<int> _cell;
};
vector<Cell> gameBoard = { {}, {} };

vector<Cell> getFlaggedCells()
{
	vector<Cell> flaggedCells;
	for (const Cell& cell : gameBoard) {
		if (cell.isFlagged())
			flaggedCells.push_back(cell);
	}
	return flaggedCells;
}
