/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>
using namespace std;
/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
    ifstream file;
    V2D result;
    file.open(filename);
    if(file.is_open()){
        size_t index=0;
        string line;
        while(getline(file,line)){
            result.push_back(vector<string> ());
            while(!line.empty()){
                string insert;
                auto finda=line.find(',');
                if(finda!=string::npos){
                    insert=Trim(line.substr(0,finda));
                }
                else{
                    insert=Trim(line);
                    line="";
                }
                result.at(index).push_back(insert);
                if(line.find(',') != string::npos){
                    line=line.substr(line.find(',') + 1, line.size());
                }
            }
            ++index;
        }
    }
    return result;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    // YOUR CODE HERE
    V2D cleaned;
    for (const vector<string> &course : cv){
        string coursename=course.at(0);
        cleaned.push_back(vector<string>{course.at(0)});
        for (size_t i = 1; i < course.size(); i++){
            const vector<string> *student_list = NULL;
            const string &coursestudent = course.at(i);
            for (const vector<string> &students_courses : student)
            {
                if (students_courses.at(0) == coursestudent)
                {
                    student_list = &students_courses;
                    break;
                }
            }
            for (size_t j = 1; student_list != NULL && j < student_list->size(); j++)
            {
                
                if (student_list->at(j) == coursename)
                {
                    cleaned.back().push_back(coursestudent);
                }
                
            }
            student_list = NULL;
        }
        if (cleaned.back().size() == 1){
            cleaned.pop_back();
        }
            
    }
    return cleaned;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Your code here!
    V2D result;
    Graph g;
    unordered_map<string,unordered_set<string>> map1;
    for(auto x : courses){
        for(unsigned i=1;i<x.size();i++){
            map1[x[i]].insert(x[0]);
        }
    }
    for(auto x : courses){
        g.add(x[0]);
        for(unsigned i=1;i<x.size();i++){
            for(auto j : map1[x[i]]){
                if(j!=x[0]){
                    g.add(x[0],j);
                }
            }
        }
    }
    auto v = g.vertex();
    do{
        if(g.maxclr(v)<=timeslots.size()){
            result=vector<vector<string>>(timeslots.size(),vector<string>(0));
            for(unsigned i=0;i<timeslots.size();i++){
                result[i].push_back(timeslots[i]);
            }
            for(auto i : g.colormap){
                result[i.second-1].push_back(i.first);
            }
            return result;
        }
    }while(next_permutation(v.begin(), v.end()));
    result=vector<vector<string>>(1,vector<string>(1,"-1"));
    return result;
}