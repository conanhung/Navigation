//
//  PathFinding.cpp
//  Navigation
//
//  Created by Danny  on 4/14/17.
//  Copyright © 2017 Danny . All rights reserved.
//

#include "PathFinding.hpp"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

///////////////////////
/* PRIVATE FUNCTIONS */
///////////////////////

void PathFinding::reorderHelper(vector<int> &vec) {
    //int highest;
    int index;
    int dis;
    int endIndex = map->getEndIndex();
    vector<int> distancesFromEnd;
    
    for (int i = 0; i < vec.size(); i++) { // initialize end distance
        dis = abs(vec[i] - endIndex);
        distancesFromEnd.push_back(dis);
    }
    
    for (int i = 0; i < distancesFromEnd.size(); i++) {
        //highest = distancesFromEnd[i];
        index = i;
        
        for (int x = (i + 1); x < distancesFromEnd.size(); x++) {
            if (distancesFromEnd[index] < distancesFromEnd[x]) {
                //highest = distancesFromEnd[x];
                index = x;
            }
        }
        
        if (index != i) { // swap both vectors
            int temp = distancesFromEnd[i];
            distancesFromEnd[i] = distancesFromEnd[index];
            distancesFromEnd[index] = temp;
            
            temp = vec[i];
            vec[i] = vec[index];
            vec[index] = temp;
        }
    }
    
}

/* PUBLIC FUNCTIONS */

// 
PathFinding::PathFinding(int occupancyGrid[], int oneL, int twoL, float resolution){
    map = new Map(oneL, twoL);
    updateMap(occupancyGrid);
    this->resolution = resolution;
    solutionFound = false;
    
    for (int i = 0; i < oneL; i++) {
        visited.push_back(false);
    }

}

// reorder map based of end index
// endIndex must be set before calling reorder
void PathFinding::reorder() {
    vector<int> temp;
    int len = map->getOneDLength();
    
    for (int i = 0; i < len; i++) {
        temp = map->getAdjPaths(i);
        reorderHelper(temp);
        map->eraseAdjPaths(i, temp);
    }
}

void PathFinding::setEnd(int index) {
    map->setEndIndex(index);
    reorder();
}

void PathFinding::setCurrentIndex(int index){
    map->setCurrentIndex(index);
}

/*
    Make sure current and end index are both set
 */
bool PathFinding::findPath() {
    bool pathFound = false; // if path is found or not
    bool next;
    bool outOfOptions = false; // when path is empty and we pop
    int currentIndex = map->getCurrentIndex();
    int count, vecSize, tempLoc = 0;
    mapObject tempObj;
    vector<int> adjList;
    
    path.push(currentIndex);
    
    while (!pathFound && !outOfOptions) {
    
        if (currentIndex == map->getEndIndex()) {
            pathFound = true;
            return pathFound;
        }
            
        count = 0; // starts at 0, 0 is your next cloestest index
        adjList = map->getAdjPaths(currentIndex);
        vecSize = (int)adjList.size();
        next = false;
    
        while (count <= vecSize && !next) { // finds next index if there is one
            tempLoc = adjList[count];
            tempObj = map->getMapObject(tempLoc);
            
//            cout << "TempLoc: " << tempLoc << endl;
//            cout << "Object " << tempObj << endl;
            
            if (tempObj == wall)
                visited[tempLoc] = true; // sets wall to true to avoid it
            else if (tempObj == space && !visited[tempLoc]) {
                next = true;
                visited[tempLoc] = true;
                path.push(tempLoc);
            }
            else if (tempObj == unknown && !visited[tempLoc]){
                next = true;
                visited[tempLoc] = true;
                path.push(tempLoc);
            }
            else if (tempObj == endPosition) {
                next = true;
                pathFound = true;
                path.push(tempLoc);
                visited[tempLoc] = true;
            }
            
            count++;
        }
        
        if (next) {
            currentIndex = tempLoc;
        } else {
            if (!path.isEmpty()) { // if path is not empty
                path.pop();
                currentIndex = path.peek();
            } else {
                outOfOptions = true;
            }
        }
    
    }
    return pathFound;
}

bool PathFinding::isSolution(){
    return solutionFound;
}

void PathFinding::clearVisited() {
    int oneL = getOneDLength();
    
    if (visited.size() != oneL)
        visited.resize(oneL);
    
    for (int i = 0; i < oneL; i++){
        visited[i] = false;
    }
}

void PathFinding::updateMap(int occupancyGird[]){
    map->updateMap(occupancyGird);
}

void PathFinding::setOneDLength(int oneD) {
    map->setOneDLength(oneD);
}

void PathFinding::setTwoDLength(int twoD) {
    map->setTwoDLength(twoD);
}

int PathFinding::getOneDLength() {
    return map->getOneDLength();
}

int PathFinding::getTwoDLength() {
    return map->getTwoDLength();
}

/////////////////
//Test Funciton//
/////////////////

void PathFinding::printLinkedList() {
    map->printLinkedList();
}

void PathFinding::printMap() {
    map->printMap();
}

vector<int> PathFinding::getPath() {
    return path.getPath();
}

