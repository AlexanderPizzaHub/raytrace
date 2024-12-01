#include<iostream>
#include<vector>
#include "src/constants.hpp"
#include <cmath>

#pragma region Line
class Line
{
    /*
    1D object, used as the reference area in 2D space.
    */
    public:
    Line();
    //Line(Line& line);
    Line(const Const::vecDd& start, const Const::vecDd& end);

    ~Line();

    Const::vecDd getcenter();
    scalar getradius();
    Const::vecDd getnormal();

    void getendpoints(Const::vecDd& start, Const::vecDd& end);

    protected:
    Const::vecDd center_;
    scalar radius_;
    Const::vecDd normal_;

};

Line::Line()
{}
/*
Line::Line(Line& line)
{
    center_ = line.getcenter();
    normal_ = line.getnormal();
    radius_ = line.getradius();
}
*/


Line::Line(const Const::vecDd& start, const Const::vecDd& end)
{
    center_[0] = (start[0] + end[0]) / 2;
    center_[1] = (start[1] + end[1]) / 2;
    radius_ = std::sqrt((start[0] - center_[0]) * (start[0] - center_[0]) + (start[1] - center_[1]) * (start[1] - center_[1]));


    normal_[0] = (end[1] - start[1]) / radius_ / 2.0;
    normal_[1] = (start[0] - end[0]) / radius_ / 2.0;
}

Line::~Line()
{
    //delete[] center_;
    //delete[] normal_;
}

Const::vecDd Line::getcenter()
{
    return center_;
}

scalar Line::getradius()
{
    return radius_;
}

Const::vecDd Line::getnormal()
{
    return normal_;
}


void Line::getendpoints(Const::vecDd& start, Const::vecDd& end)
{
    // NEED DOUBLE CHECK
    start[0] = center_[0] - radius_ * normal_[1];
    start[1] = center_[1] + radius_ * normal_[0];
    end[0] = center_[0] + radius_ * normal_[1];
    end[1] = center_[1] - radius_ * normal_[0];
}


#pragma endregion

#pragma region RefArea

class RefArea: public Line
    {
        public:
            //using Line::Line; // use the constructor of the base class
            RefArea(Const::vecDd& start, Const::vecDd& end);
            ~RefArea(); 
            
            //void Hit(); // increase the hit counter
            void WeightedHit(scalar dWeight); // increase the weight
            void WeightAdd(scalar dweight);
            //void WeightedHit(scalar dWeight, Const::vecDd position); // increase the weight. Hitpoint information is used
            void SetWeight(scalar weight); 
            
            void SetRate(scalar rate);
            scalar getrate();
            
            //Const::vecDd getnormal();
            scalar getweightstore();
    
        
        private:

            scalar weightstore_;
            scalar rate_;
            //label hitcounter_; // --> integers storing the number of hits
            // --> store the weight of the ray
    };


RefArea::RefArea(Const::vecDd& start, Const::vecDd& end)
: Line(start, end)
{
    weightstore_ = 0.0;
    rate_ = 0.0;
}

RefArea::~RefArea()
{}

void RefArea::WeightedHit(scalar dWeight)
{
    //std::cout << "hit" << std::endl;
    weightstore_ += dWeight;
    //std::cout <<getweightstore() <<std::endl;
}

/*
void RefArea::WeightedHit(scalar dWeight, Const::vecDd position)
{
    //TODO
}
*/

void RefArea::SetWeight(scalar weight)
{
    weightstore_ = weight;
}

void RefArea::WeightAdd(scalar dweight)
{
    weightstore_ += dweight;
}

scalar RefArea::getweightstore()
{
    return weightstore_;
}




void RefArea::SetRate(scalar rate)
{
    rate_ = rate;
}

scalar RefArea::getrate()
{
    return rate_;
}

int main()
{
    std::vector<RefArea> refareas;
    Const::vecDd start = {0.0, 0.0};
    Const::vecDd end = {1.0, 1.0};

    refareas.emplace_back(RefArea(start, end));
    return 0;
}