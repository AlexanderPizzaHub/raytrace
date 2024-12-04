#include "hrle.hpp" 

#include <vector>
#include <array>


using namespace hrle;

#pragma region RunTypeCode

RunTypeCode::RunTypeCode(gridstate state, int index)
{
    state_ = state;
    index_ = index;
}
RunTypeCode::~RunTypeCode()
{
}

#pragma endregion


#pragma region RLE
RLE::RLE(std::array<int,2> extent)
{
    extent_ = extent;
}
RLE::~RLE()
{
}

#pragma endregion

#pragma region HRLE

HRLE::HRLE(std::array<std::array<int,2>,Const::D> extents)
{
    rles_.reserve(Const::D);
    for(int i = 0; i < Const::D; i++)
    {
        rles_.push_back(RLE(extents[i]));
        rles_[i].startindices_.push_back(0);
        rles_[i].runtypes_.push_back(RunTypeCode(gridstate::UNDEFINED_POS,-1));
        rles_[i].runbreaks_.push_back(extents[i][0]);
        rles_[i].runbreaks_.push_back(extents[i][1]);

    }
}


void insdefgd(RLE& rle, int targetcoord, int targetindex, int startindex, int endindex)
{
    // 仍然假设排好序, 只会加在尾部
    std::vector<int>& runbreaks = rle.runbreaks_;
    if(targetcoord == runbreaks[endindex])
    {
            // 把最后一个runbreak右移一位
            runbreaks[endindex] += 1;
        }
        else
        {
            // 生成一个新的runbreak和runtype
            // assert targetcoord > rle.runbreaks_[endindex]
            //rle.runbreaks_.insert(rle.runbreaks_.begin() + endindex, targetcoord + 1);
            //rle.runtypes_.insert(rle.runtypes_.begin() + endindex, RunTypeCode(gridstate::UNDEFINED,0));
            runbreaks.push_back(targetcoord + 1);
            rle.runtypes_.push_back(RunTypeCode(gridstate::DEFINED, (runbreaks[endindex]-runbreaks[endindex-1]) + rle.runtypes_[endindex-1].index_));
        }
}


void HRLE::InsertDefinedGrid(Const::vecDi &coord)
{
    // 不会写这个
    // 插入时一定是在尾部插入。直接判断最后一个slot就好
    // 反正y方向肯定加在最后一截，x方向也是


    /// TODO!!!!!!!!!!!!!!!!!!!!!
    if(coord[1] = rles_[1].runbreaks_.back())
    {
        rles_[1].runbreaks_.back() += 1; 
    }
    if(coord[1] > rles_[1].runbreaks_.back())
    {
        // 需要新加一块run type
        rles_[1].runbreaks_.push_back(coord[1]);
    }


    int slot_y = -1;
    for (int runbreak : rles_[0].runbreaks_)
    {
        if (coord[0] >= runbreak)
        {
            slot_y++;
        }
    }

}

void HRLE::InsertUndefinedGrid(Const::vecDi &startcoordX, int length, gridstate state)
{
    // 插一个runtype和runbreak就可以了
    rles_[0].runbreaks_.push_back(startcoordX[0]);
    rles_[0].runtypes_.push_back(RunTypeCode(state, -1));

}


// 计划： 初始化HRLE还是手摇。写一个获取领域的函数，以及从老HRLE生成新HRLE的函数
// 之后再写插入和删除的函数
// 从老HRLE生成新的，就按字典序遍历，然后插入defined或者undefined就可以了。



HRLE::~HRLE()
{
}

#pragma endregion