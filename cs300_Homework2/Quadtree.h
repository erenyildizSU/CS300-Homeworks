// HUSEYIN EREN YILDIZ - 31047 - HOMEWORK2

#ifndef CS300_HOMEWORK2_QUADTREE_H
#define CS300_HOMEWORK2_QUADTREE_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

struct coordinate_points {

    string nameOfCities;
    int x,y;

    coordinate_points(const string & cityName, int x_point, int y_point)
        : nameOfCities(cityName) , x(x_point), y(y_point)
    {}
};

class pointNode {

    coordinate_points points;
    pointNode * SE;
    pointNode * SW;
    pointNode * NE;
    pointNode * NW;

    pointNode(coordinate_points point, pointNode * SE_way, pointNode * SW_way, pointNode * NE_way, pointNode * NW_way)
            :points(point),SE(SE_way),SW(SW_way),NE(NE_way),NW(NW_way)
            {}

    pointNode(): points ("",-1,-1){
        SE = nullptr;
        NE = nullptr;
        SW = nullptr;
        NW = nullptr;
    }

    ~pointNode(){
        delete SE;
        delete SW;
        delete NE;
        delete NW;
    }

    friend class Quadtree;
};


class Quadtree {

private:
    pointNode * root;
    void pretty_print(const pointNode*);
    bool isCircleInQuadrant(int, int, int, int, int, const string& ) const;

public:
    Quadtree(pointNode*);
    ~Quadtree();
    vector <string> path;
    vector <string> city_name;
    void search_cities(int,int,int);
    void insert(const coordinate_points &) ;
    void pretty_print();
};


#endif //CS300_HOMEWORK2_QUADTREE_H
