#include "hrle.cpp"
#include "../constants.hpp"

int main()
{
    std::array<std::array<int, 2>, Const::D> extents;
    extents[0][0] = 0;
    extents[0][1] = 11;
    extents[1][0] = 0;
    extents[1][1] = 10;

    HRLE hrle(extents);

    using namespace hrle;

    // 生成y轴游程编码
    hrle.AddNewLayer(0, 0, UNDEFINED_POS);
    //hrle.AddUndefinedSection(0, -3, UNDEFINED_POS);
    hrle.AddDefinedSection(0, -1);
    hrle.AddUndefinedSection(0,6, UNDEFINED_POS);
    

    // int dim = 0;
    //  生成x轴游程编码
    // int nbreaks = hrle.rles_[dim].runbreaks_.size();
    // hrle.rles_[1].startindices_.reserve(maxdataindex[0] + rles_[0].runbreaks_[nbreaks - 1] - rles_[0].runbreaks_[nbreaks - 2])

    hrle.AddNewLayer(1, 0, UNDEFINED_POS);

    hrle.AddDefinedSection(1, -4);
    hrle.AddUndefinedSection(1, 3, UNDEFINED_POS);

    hrle.AddNewLayer(1, 1, UNDEFINED_POS);

    hrle.AddDefinedSection(1, -5);
    hrle.AddUndefinedSection(1, 4, UNDEFINED_POS);

    hrle.AddNewLayer(1, 2, UNDEFINED_POS);

    hrle.AddDefinedSection(1, -5);
    hrle.AddUndefinedSection(1, -3, UNDEFINED_NEG);
    hrle.AddDefinedSection(1, 0);
    hrle.AddUndefinedSection(1, 3, UNDEFINED_POS);

    /*
    RLE& rle0 = hrle.rles_[0];
    std::cout << "indecies: " << rle0.startindices_[0] << std::endl;
    std::cout << "runbreaks: "  << std::endl;
    for(auto i : rle0.runbreaks_)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "runtypes: " << std::endl;

    for(auto i : rle0.runtypes_)
    {
        std::cout << i.state_ << " " << i.index_ << std::endl;
    }

    RLE& rle1 = hrle.rles_[1];


    std::cout << "indecies: " << std::endl;
    for(auto i : rle1.startindices_)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;

    std::cout << "runbreaks: "  << std::endl;

    for(auto i : rle1.runbreaks_)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;

    std::cout << "runtypes: " << std::endl;

    for(auto i : rle1.runtypes_)
    {
        std::cout << i.state_ << " " << i.index_ << std::endl;
    }

    std::cout << "indecies: " << rle1.startindices_[1] << std::endl;
    */


    
    int index;
    Const::vecDi coords;
    for (int i = -3; i < 1; i++)
    {
        for (int j = -6; j < 5; j++)
        {
            coords[0] = j;
            coords[1] = i;
            index = hrle.CartesianToIndex(coords, 0, 1);

            std::cout << j << " " << " " << i<< " " <<index << std::endl;
        }
    }
    

    return 0;
}