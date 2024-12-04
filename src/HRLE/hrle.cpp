#include "hrle.hpp" 

#include <vector>
#include <array>


using namespace hrle;

#pragma region RunTypeCode

RunTypeCode::RunTypeCode(gridstate state, label index)
{
    state_ = state;
    index_ = index;
}
RunTypeCode::~RunTypeCode()
{
}

#pragma endregion


#pragma region RLE
RLE::RLE(std::array<label,2> extent)
{
    extent_ = extent;
}
RLE::~RLE()
{
}

#pragma endregion

#pragma region HRLE

HRLE::HRLE(std::array<std::array<label,2>,Const::D> extents)
{
    rles.reserve(Const::D);
    for(int i = 0; i < Const::D; i++)
    {
        rles_.push_back(RLE(extents[i]));
        rles_
    }
}

HRLE::~HRLE()
{
}

#pragma endregion