#include <cmath>
#include <iterator>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  
}
ImageTraversal::Iterator::Iterator(PNG image, ImageTraversal * trvsltype, Point start, double tolerance){
  travsl=trvsltype;
   start_=start;
  toler_=tolerance;
  image_=image;
  curpt=travsl->peek();
  unsigned totalpix=image_.width()*image_.height();
  isvisited.assign(totalpix,false);

}
bool ImageTraversal::Iterator::isVisited(const Point & point){
  return isvisited[point.x + point.y * image_.width()];
}
bool ImageTraversal::Iterator::isDeltaok(const Point & point){
  HSLAPixel start=image_.getPixel(start_.x,start_.y);
  HSLAPixel currentpoint= image_.getPixel(point.x,point.y);
  return (calculateDelta(start,currentpoint)<toler_);
}
bool ImageTraversal::Iterator::CanVisit(const Point & point){
  if (point.x >= image_.width() || point.y >= image_.height()) return false;
  if(isVisited(point)) return false;
  return isDeltaok(point);

}
/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  if(travsl==NULL) return *this;
  Point top=travsl->pop();
  isvisited[top.x + top.y * image_.width()]=true;
  Point right_ = Point(top.x + 1, top.y);
    Point below_ = Point(top.x, top.y + 1);
    Point left_ = Point(top.x - 1, top.y);
    Point above_ = Point(top.x, top.y - 1);
    
    if(CanVisit(right_)) travsl->add(right_);
    if(CanVisit(below_)) travsl->add(below_);
    if(CanVisit(left_)) travsl->add(left_);
    if(CanVisit(above_)) travsl->add(above_);
    while(!travsl->empty() && isVisited(Point(travsl->peek().x,travsl->peek().y))) travsl->pop();
    if(travsl->empty()) {done=true;
    travsl=NULL;
   }
    else{
      curpt=travsl->peek();
    }
  //   Point next = travsl->pop();
  // while (isvisited[next.x + next.y * image_.width()]) {
  //   if (travsl->empty()) {

  //     return *this;
  //   }
  //   next = travsl->pop();
  // }

  // curpt = next;
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return curpt;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return (done!=other.done);
}

