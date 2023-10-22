/* Your code here! */
#include "maze.h"
#include "cs225/PNG.h"
#include "dsets.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <queue>
#include <utility>
#include <algorithm>
using namespace cs225;
using namespace std;
SquareMaze::SquareMaze() {
	
}

SquareMaze::~SquareMaze() {
	
}

void SquareMaze::makeMaze(int width, int height) {
    width_=width;
    height_=height;
    int mazesize=width*height;
    mazeSets.addelements(mazesize);
    rightwall_.assign(mazesize,true);
    downwall_.assign(mazesize,true);
    for(int i=0;i<mazesize-1;i++){
        int random=rand()%2;
        //if random =0, open right wall
        if(random==0&& ((i + 1) % width_ != 0) &&(mazeSets.find(i) != mazeSets.find(i+1))){
            mazeSets.setunion(i,i+1);
            rightwall_[i] = false;
        }else if(random==1 && !(i >= (width_ * (height_ - 1))) && (mazeSets.find(i) != mazeSets.find(i+width_))){
            mazeSets.setunion(i,i+width);
            downwall_[i]=false;
        }
        else if(((i + 1) % width_ != 0) &&(mazeSets.find(i) != mazeSets.find(i+1))){
            mazeSets.setunion(i,i+1);
            rightwall_[i] = false;
        }else if(!(i >= (width_ * (height_ - 1))) && (mazeSets.find(i) != mazeSets.find(i+width_))){
            mazeSets.setunion(i,i+width);
            downwall_[i]=false;
        }
}
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    switch(dir){
    case 0:
      return !rightwall_[y * width_ + x];
    case 1: 
      return !downwall_[y * width_ + x];
    case 2:
      return (x != 0) && !(rightwall_[y * width_ + x - 1]);
    case 3:
      return (y != 0) && !(downwall_[y * width_ + x - width_]);
  }
  return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if(dir == 0){
    rightwall_[y * width_ + x] = exists;
  }
  else if(dir == 1){
    downwall_[y * width_ + x] = exists;
  }
}
vector<int> SquareMaze::solveMaze(){
    vector<int> length;
    vector<int> path;
    vector<int> prev;
    queue<int> q;
    int mazesize=width_*height_;
    path.assign(mazesize, 0);
    length.assign(mazesize, -1);
    prev.assign(mazesize, 0);
    q.push(0);
    while(!q.empty()){
      int top=q.front();
      q.pop();
      int x= top%width_;
      int y =top/width_;
      if (canTravel(x, y, 0)) {
        if(length[top + 1] ==-1){
      path[top+1] = 0;
      prev[top + 1] = top;
      length[top + 1] = length[top] + 1;
      q.push(top + 1);
        }
    }
    if (canTravel(x, y, 1)) {
      if(length[top + width_] ==-1){
      path[top+width_] = 1;
      prev[top + width_] = top;
      length[top + width_] = length[top] + 1;
      q.push(top + width_);
      }
    }
    if (canTravel(x, y, 2)) {
      if(length[top - 1] ==-1){
      path[top -1] = 2;
      prev[top - 1] = top;
      length[top - 1] = length[top] + 1;
      q.push(top - 1);
      }
    }
    if (canTravel(x, y, 3)) {
        if(length[top - width_] ==-1){
      path[top-width_] = 3;
      prev[top - width_] = top;
      length[top - width_] = length[top] + 1;
      q.push(top - width_);
        }
    }
    }
    int destindex = 0;
    int longest = 0;
  for (int i =width_*(height_-1); i < width_ * height_; i++) {
    if (length[i] > longest) {
      destindex= i;
      longest = length[i];
    }
  }
  vector<int> solution;
  while (destindex> 0) { 
    solution.push_back(path[destindex]);
    destindex= prev[destindex];
  }
  reverse(solution.begin(), solution.end());
  return solution;
}

PNG * SquareMaze::drawMaze() const {
    PNG* pic = new PNG(width_ * 10 + 1, height_ * 10 + 1);
    int picwidth=width_ * 10 + 1;
    int picheight=height_ * 10 + 1;
    for(int x = 10; x < picwidth; x++){
    HSLAPixel& p = pic->getPixel(x, 0);
    p.l = 0; 
    }
    for(int y = 0; y < picheight; y++){
    HSLAPixel& p = pic->getPixel(0, y);
    p.l = 0; 
    }
    for(int x = 0; x < width_; x++){
    for(int y = 0; y < height_; y++){
      if(rightwall_[y * width_ + x]){
        for(int k = 0; k <= 10; k++){
          HSLAPixel& p = pic->getPixel((x + 1) * 10, y * 10 + k);
          p.l = 0; 
        }
      }
      if(downwall_[y * width_ + x]){
        for(int k = 0; k <= 10; k++){
          HSLAPixel& p = pic->getPixel(x * 10 + k, (y + 1) * 10);
          p.l = 0;  
        }
      }
    }
  }
    return pic;
}
PNG * SquareMaze::drawMazeWithSolution(){
  PNG* pic = drawMaze();
  vector<int> solution = solveMaze();
  int x = 5;
	int y = 5;
  for(auto it : solution){
		switch(it){
      case 0:
      for(int k = 0; k <= 10; k++){
          HSLAPixel& p = pic->getPixel(x + k, y);
          p.h = 0;
          p.s = 1;
          p.l = 0.5;
          p.a = 1;
        }
        x+=10;
        break;
      case 1:
        for(int k = 0; k <= 10; k++){
        HSLAPixel& p = pic->getPixel(x, y + k);
        p.h = 0;
        p.s = 1;
        p.l = 0.5;
        p.a = 1;
      }
        y += 10;
        break;
      case 2:
      for(int k = 0; k <= 10; k++){
        HSLAPixel& p = pic->getPixel(x - k, y);
        p.h = 0;
        p.s = 1;
        p.l = 0.5;
        p.a = 1;
      }
        x -= 10;
        break;
      case 3:
      for(int k = 0; k <= 10; k++){
        HSLAPixel& p = pic->getPixel(x, y - k);
        p.h = 0;
        p.s = 1;
        p.l = 0.5;
        p.a = 1;
      }
        y -= 10;
        break;
    }

	}
  int curx=x/10;
  int cury=y/10;

  for(int k = 1; k <= 9; k++){
    HSLAPixel& p = pic->getPixel(curx*10 +k, (cury+1)*10);
    p.l = 1;
  }
    return pic;
}