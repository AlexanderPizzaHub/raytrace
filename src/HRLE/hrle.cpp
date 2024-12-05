#include "hrle.hpp"

#include <vector>
#include <array>
#include <iostream>

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
RLE::RLE(std::array<int, 2> extent)
{
    extent_ = extent;
}
RLE::~RLE()
{
}

#pragma endregion

#pragma region HRLE

HRLE::HRLE(std::array<std::array<int, 2>, Const::D> extents)
{
    rles_.reserve(Const::D);
    for (int i = 0; i < Const::D; i++)
    {
        rles_.push_back(RLE(extents[i]));
        //rles_[i].startindices_.push_back(0);
        //rles_[i].runtypes_.push_back(RunTypeCode(gridstate::UNDEFINED_POS, -1));
        //rles_[i].runbreaks_.push_back(extents[i][0]);
    }
    //rles_[0].startindices_.push_back(0);
    maxdataindex.fill(0);
}

/*
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
*/

void HRLE::AddNewLayer(int dim, int layerindex, gridstate state)
{
    /*
在指定维度添加一层新的编码.
dim=0 -> y axis
dim=1 -> x axis
*/
    if(rles_[dim].startindices_.size() < layerindex + 1)
        rles_[dim].startindices_.resize(layerindex + 1);
    rles_[dim].startindices_[layerindex] = rles_[dim].runtypes_.size();
    rles_[dim].runtypes_.push_back(RunTypeCode(state, -1)); // 默认生成一节未定义
    //std::cout << "## " << dim << " " << rles_[dim].runbreaks_.size()<<std::endl;
}

void HRLE::AddDefinedSection(int dim, int startcoord)
{
    int nbreaks = rles_[dim].runbreaks_.size();
    if(nbreaks > 1)
    {
        maxdataindex[dim] += rles_[dim].runbreaks_[nbreaks - 1] - rles_[dim].runbreaks_[nbreaks - 2];
        //std::cout <<  "here!! "<< rles_[dim].runbreaks_[nbreaks - 1] - rles_[dim].runbreaks_[nbreaks - 2] << std::endl;
    }
    //std::cout << "!!" << std::endl;
    
    rles_[dim].runbreaks_.push_back(startcoord);
    rles_[dim].runtypes_.push_back(RunTypeCode(DEFINED, maxdataindex[dim]));
    std::cout << maxdataindex[0] << " "<< maxdataindex[1] << std::endl;
}

void HRLE::AddUndefinedSection(int dim, int startcoord, gridstate state)
{
    rles_[dim].runbreaks_.push_back(startcoord);
    rles_[dim].runtypes_.push_back(RunTypeCode(state, -1));
}

int HRLE::CartesianToIndex(Const::vecDi coords, int layerindex, int cartdim)
{
    // 这里的dim跟笛卡尔坐标一致,x->0, y->1, z->2
    // hrle的dim, z->0, y->1, x->2
    // cartdim从2开始
    // Q:如果把hrle的dim和笛卡尔坐标的dim改成一致，会不会影响性能？
    int coord = coords[cartdim];
    int hrledim = Const::D - 1 - cartdim;

    if (cartdim < 0)
    {
        // 说明已经到了数据表上
        std::cout << "return here 1: "<< layerindex << std::endl;
        return layerindex;
    }
    else
    {
        RLE &rle = rles_[hrledim];
        // 在维度dim上找位置
        if (rle.runbreaks_.size() == 0) 
        {
            // 用来检测空HRLE
            //std::cout << "return here" << std::endl;
            return -1; // 说明没有已定义的
        }
        else
        {
            // 找到它对应哪一个runtype
            int idtf_runtype = rle.startindices_[layerindex];
            std::cout << "!!@@## "<< idtf_runtype << std::endl; 

            int runtype_lend = idtf_runtype;
            int runtype_rend = (layerindex >= rle.startindices_.size() - 1) ? rle.runtypes_.size() : rle.startindices_[layerindex + 1];

            for (int breakpos = runtype_lend; breakpos < runtype_rend; breakpos++)
            {
                //std::cout << "$$$ "<< coord <<" "<< rle.runbreaks_[breakpos] << std::endl; 
                if (coord > rle.runbreaks_[breakpos])
                    idtf_runtype++;
                else
                    break;
            }
            // 如果未定义，返回
            std::cout <<"check here "<< rle.runtypes_[idtf_runtype].state_  << "runtype ind " << idtf_runtype << std::endl;
            if (rle.runtypes_[idtf_runtype].state_ != DEFINED)
                return rle.runtypes_[idtf_runtype].state_;
            else
            {
                // 找到它在哪一层次级网格
                int sublayerindex = coords[cartdim] - rles_[hrledim].runbreaks_[idtf_runtype] + rles_[hrledim].runtypes_[idtf_runtype].index_;
                /*
                进入这一层网格的编码，找它的位置
                */
                return CartesianToIndex(coords, sublayerindex, cartdim - 1);
            }
        }
    }
}

// 计划： 初始化HRLE还是手摇。写一个获取领域的函数，以及从老HRLE生成新HRLE的函数
// 之后再写插入和删除的函数
// 从老HRLE生成新的，就按字典序遍历，然后插入defined或者undefined就可以了。

HRLE::~HRLE()
{
}

#pragma endregion