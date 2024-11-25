#include "marchingcube.hpp"
#include <iostream>

/*
    从左上起，逆时针动，0表示正，1表示负，二进制数映射
    共16种情况分别写函数算
    每个函数接收refareaptrlist_，产生的新参考面加入到refareaptrlist_中
    */

scalar interpolant_zero(scalar pos0, scalar pos1, scalar phi0, scalar phi1)
{
    scalar zero = pos0 - phi0 * (pos1 - pos0) / (phi1 - phi0) ;
    //std::cout <<"check: " << pos1-pos0 << " "<<phi1 - phi0<< std::endl;
    //std::cout << pos0 << " " << pos1 << " " << phi0 << " " << phi1 << " " << zero << std::endl;
    return zero;
}


void case0000_(Square& square)
{
    return;
}

void case0001_(Square& square)
{
    /*
    ----------
    |     \  |
    |      \ |
    |        |
    ----------
    */
    Const::vecDd start;
    Const::vecDd end;
    std::cout <<"case0001" << std::endl;
    start[1] = square.linked_grids_[3]->y;
    // assert grid size =1 
    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, square.linked_grids_[3]->phi, square.linked_grids_[0]->phi);

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, square.linked_grids_[2]->phi, square.linked_grids_[3]->phi);

    Line* line = new Line(start, end); // change normal by swapping x and y

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case0010_(Square& square)
{
    /*
    ----------
    |        |
    |      / |
    |     /  |
    ----------
    */
    Const::vecDd start;
    Const::vecDd end;

    std::cout<<"case0010" << std::endl;
    // assert grid size =1 
    start[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, square.linked_grids_[1]->phi, square.linked_grids_[2]->phi);
    start[1] = square.linked_grids_[2]->y;

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[3]->y, square.linked_grids_[2]->y, square.linked_grids_[3]->phi, square.linked_grids_[2]->phi);

    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case0011_(Square& square)
{
    /*
    -----------
    |    |    |
    |    |    |
    |    |    |
    -----------
    */
    Const::vecDd start;
    Const::vecDd end;

    std::cout << "case0011" << std::endl;
    // assert grid size =1 
    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, square.linked_grids_[3]->phi, square.linked_grids_[0]->phi);
    start[1] = square.linked_grids_[3]->y;


    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, square.linked_grids_[1]->phi, square.linked_grids_[2]->phi);
    end[1] = square.linked_grids_[2]->y;
    
    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case0100_(Square& square)
{
    /*
    ----------
    |        |
    |\       |
    | \      |
    ----------
    */
    Const::vecDd start;
    Const::vecDd end;

    std::cout << "case0100" << std::endl;
    // assert grid size =1 
    start[0] = square.linked_grids_[0]->x;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, square.linked_grids_[0]->phi, square.linked_grids_[1]->phi);
    
    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, square.linked_grids_[1]->phi, square.linked_grids_[2]->phi);
    end[1] = square.linked_grids_[1]->y;
    

    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case0101_(Square& square)
{
    /*
    ----------
    |     \  |
    | \    \ |
    |  \     |
    ----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    //std::cout << "case0101" << std::endl;
}

void case0110_(Square& square)
{
    /*
    ----------
    |        |
    |--------|
    |        |
    ----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    std::cout << "case0110" << std::endl;

    Const::vecDd start;
    Const::vecDd end;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, square.linked_grids_[0]->phi, square.linked_grids_[1]->phi);
    start[0] =square.linked_grids_[0]->x;
    

    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, square.linked_grids_[2]->phi, square.linked_grids_[3]->phi);
    end[0] = square.linked_grids_[2]->x;
    
    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case0111_(Square& square)
{
    /*
    -----------
    |  /      |
    | /       |
    |/        |
    -----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    std::cout << "case0111" << std::endl;


    Const::vecDd start;
    Const::vecDd end;
    start[0] = interpolant_zero(square.linked_grids_[0]->x, square.linked_grids_[3]->x, square.linked_grids_[0]->phi, square.linked_grids_[3]->phi);
    start[1] =square.linked_grids_[0]->y;
    
    end[0] = square.linked_grids_[1]->x;
    end[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, square.linked_grids_[0]->phi, square.linked_grids_[1]->phi);
    

    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);

}

void case1000_(Square& square)
{
    /*
    ----------
    | /      |
    |/       |
    |        |
    ----------
    */
    //TODO
    std::cout << "case1000" << std::endl;

    Const::vecDd start;
    Const::vecDd end;
    start[0] = interpolant_zero(square.linked_grids_[0]->x, square.linked_grids_[3]->x, square.linked_grids_[0]->phi, square.linked_grids_[3]->phi);
    start[1] =square.linked_grids_[0]->y;
    
    end[0] = square.linked_grids_[1]->x;
    end[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, square.linked_grids_[0]->phi, square.linked_grids_[1]->phi);
    

    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}


void case1001_(Square& square)
{
    /*
    ----------
    |        |
    |--------|
    |        |
    ----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    std::cout << "case1001" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    start[0] =square.linked_grids_[0]->x;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, square.linked_grids_[0]->phi, square.linked_grids_[1]->phi);
    

    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, square.linked_grids_[2]->phi, square.linked_grids_[3]->phi);
    end[0] = square.linked_grids_[2]->x;
    
    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case1010_(Square& square)
{
    /*
    ----------
    |        |
    |      / |
    |     /  |
    ----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    //std::cout << "case1010" << std::endl;
}

void case1011_(Square& square)
{
    /*
    -----------
    |         |
    |\        |
    | \       |
    -----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    std::cout << "case1011" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    // assert grid size =1 
    start[0] = square.linked_grids_[0]->x;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, square.linked_grids_[0]->phi, square.linked_grids_[1]->phi);
    
    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, square.linked_grids_[1]->phi, square.linked_grids_[2]->phi);
    end[1] = square.linked_grids_[1]->y;
    

    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case1100_(Square& square)
{
    /*
    ----------
    |    |   |
    |    |   |
    |    |   |
    ----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    std::cout << "case1100" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, square.linked_grids_[3]->phi, square.linked_grids_[0]->phi);
    start[1] = square.linked_grids_[3]->y;


    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, square.linked_grids_[1]->phi, square.linked_grids_[2]->phi);
    end[1] = square.linked_grids_[2]->y;
    
    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case1101_(Square& square)
{
    /*
    ----------
    |       /|
    |      / |
    |     /  |
    ----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    std::cout << "case1101" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    // assert grid size =1 
    start[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, square.linked_grids_[1]->phi, square.linked_grids_[2]->phi);
    start[1] = square.linked_grids_[2]->y;

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[3]->y, square.linked_grids_[2]->y, square.linked_grids_[3]->phi, square.linked_grids_[2]->phi);

    Line* line = new Line(start, end); 

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case1110_(Square& square)
{
    /*
    ----------
    |      \ |
    |       \|
    |        |
    ----------
    */
    //TODO NEED TO DEAL AMBIGUOUITY
    std::cout << "case1110" << std::endl;
    Const::vecDd start;
    Const::vecDd end;
    start[1] = square.linked_grids_[3]->y;
    // assert grid size =1 
    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, square.linked_grids_[3]->phi, square.linked_grids_[0]->phi);

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, square.linked_grids_[2]->phi, square.linked_grids_[3]->phi);

    Line* line = new Line(start, end); // change normal by swapping x and y

    RefArea* refarea = new RefArea(line);
    square.linked_refareas_.emplace_back(refarea);
}

void case1111_(Square& square)
{
    return;
}

MarchingCube2D::MarchingCube2D()
{   
    case0000 = case0000_;
    case0001 = case0001_;
    case0010 = case0010_;
    case0011 = case0011_;
    case0100 = case0100_;
    case0101 = case0101_;
    case0110 = case0110_;
    case0111 = case0111_;
    case1000 = case1000_;
    case1001 = case1001_;
    case1010 = case1010_;
    case1011 = case1011_;
    case1100 = case1100_;
    case1101 = case1101_;
    case1110 = case1110_;
    case1111 = case1111_;



    methodptrs_[0] = case0000;
    methodptrs_[1] = case0001;
    methodptrs_[2] = case0010;
    methodptrs_[3] = case0011;
    methodptrs_[4] = case0100;
    methodptrs_[5] = case0101;
    methodptrs_[6] = case0110;
    methodptrs_[7] = case0111;
    methodptrs_[8] = case1000;
    methodptrs_[9] = case1001;
    methodptrs_[10] = case1010;
    methodptrs_[11] = case1011;
    methodptrs_[12] = case1100;
    methodptrs_[13] = case1101;
    methodptrs_[14] = case1110;
    methodptrs_[15] = case1111;



}

MarchingCube2D::~MarchingCube2D()
{
    // do nothing
}

void MarchingCube2D::SetRefArea(Square& square)
{
    label ul, ll, lr, ur;
    ul = (label)(square.linked_grids_[0]->phi < 0);
    ll = (label)(square.linked_grids_[1]->phi < 0);
    lr = (label)(square.linked_grids_[2]->phi < 0);
    ur = (label)(square.linked_grids_[3]->phi < 0);

std::cout << ul << " " << ll << " " << lr << " " << ur << std::endl;
    label index = ul * 8 + ll * 4 + lr * 2 + ur;

    methodptrs_[index](square);
}




 


