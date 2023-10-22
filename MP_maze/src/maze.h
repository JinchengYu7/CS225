/* Your code here! */
#pragma once

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "dsets.h"
#include <vector>
#include <utility>
using namespace cs225;
using namespace std;
class SquareMaze{
    public:
    SquareMaze();
	~SquareMaze();
	void makeMaze(int width, int height);  
	bool canTravel(int x, int y, int dir) const;  
	void setWall(int x, int y, int dir, bool exists);  
	vector<int> solveMaze();  
	PNG * drawMaze() const;  
	PNG * drawMazeWithSolution();  
	

    private:
	vector<bool> rightwall_;
    vector<bool> downwall_;
	DisjointSets mazeSets;
	int width_;
	int height_;

};