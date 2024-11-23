#ifndef GEOMETRY_OBJECTS_HPP
#define GEOMETRY_OBJECTS_HPP

#include "../geometry/mesh.hpp"
#include "../raytracer/ray.hpp"
#include "../constants.hpp"
#include <vector>

/*
Implement basic geometrical objects. 
Objects that are possibly used for ray tracing provides related functions.
*/

class RefArea;

struct Grid
{
    /*
    0D object, for unstructured grids.
    */
    scalar coordX;
    scalar coordY;
    label index;

    scalar phi;
};

struct GridCartesian
{
    /*
    0D object. Assert Cartesian mesh, the grid is indexed by i,j.
    */
   label x;
   label y;
   scalar phi;
};

class Square
{
    /*
    2D object, used as the cell in 2D space.
    */

    public:

    Square(Square& square);
    //Square(Const::vecDd lowerleft, Const::vecDd upperright);
    Square(Const::vecDi lowerleft, Const::vecDi upperright);
    ~Square();

    scalar Intersect(Ray& ray); //implement ray-square intersection. Return hit time

    // set the linked objects: as reference area, square may be linked with certain grids or cells
    label linked_upperleft_grid_index_; 
    std::vector<RefArea*> linked_refareas_; 

    label* getlowerleft();
    label* getupperright();

    private:
    Const::vecDi lowerleft_;
    Const::vecDi upperright_;

};


class Line
{
    /*
    1D object, used as the reference area in 2D space.
    */
    public:

    Line(Line& line);
    Line(Const::vecDd start, Const::vecDd end);
    Line(Const::vecDd center, scalar radius, Const::vecDd normal);
    ~Line();

    scalar Intersect(Ray& ray); //implement ray-line intersection. Return hit time

    // set the linked objects: as reference area, line may be linked with certain grids or cells
    std::vector<Square*> linked_objects_;  // detailed in future

    scalar* getcenter();
    scalar getradius();
    scalar* getnormal();

    private:
    Const::vecDd center_;
    scalar radius_;
    Const::vecDd normal_;

};

class Rectangle
{
    /*
    2D object, used as the bounding box in 2D space. Rays traverse through the box and hit the objects inside.
    */
   
    public:

    Rectangle(Rectangle& rectangle);
    Rectangle(Const::vecDd lowerleft, Const::vecDd upperright);
    ~Rectangle();

    void Intersect(Ray& ray, scalar& time_in, scalar& time_out); //implement ray-rectangle intersection.

    // set the linked objects: as reference area, rectangle is linked with sub-rectangles or reference areas inside. Used for kd-tree.
    std::vector<scalar> linked_objects_; // detailed in future

    private:
    Const::vecDd lowerleft_;
    Const::vecDd upperright_;

};



#endif 