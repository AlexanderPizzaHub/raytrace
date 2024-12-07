#include "HRLE/hrle.hpp"


#include <vector>
#include <array>
#include <iostream>
#include <cassert>

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
    maxdataindex_.fill(0);
}


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
        maxdataindex_[dim] += rles_[dim].runbreaks_[nbreaks - 1] - rles_[dim].runbreaks_[nbreaks - 2];
        //std::cout <<  "here!! "<< rles_[dim].runbreaks_[nbreaks - 1] - rles_[dim].runbreaks_[nbreaks - 2] << std::endl;
    }
    //std::cout << "!!" << std::endl;
    
    rles_[dim].runbreaks_.push_back(startcoord);
    rles_[dim].runtypes_.push_back(RunTypeCode(DEFINED, maxdataindex_[dim]));
    std::cout << maxdataindex_[0] << " "<< maxdataindex_[1] << std::endl;
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
    //std::cout << "search coord: " << coord << " layerindex: " << layerindex << " dim: " << cartdim << std::endl;
    int hrledim = Const::D - 1 - cartdim;

    if (cartdim < 0)
    {
        // 说明已经到了数据表上
        //std::cout << "return here 1: "<< layerindex << std::endl;
        assert(layerindex >= 0);
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
            //std::cout << "!!@@## "<< idtf_runtype << std::endl; 

            int runtype_lend = idtf_runtype;// - layerindex;
            int runtype_rend = (layerindex < rle.startindices_.size() - 1) ? rle.startindices_[layerindex + 1] : rle.runtypes_.size()-1;
            //std::cout << rle.runtypes_.size() << " !@# " << rle.startindices_.size() - 1<<std::endl;
            //std::cout << "dim, layerindex, left and right ends: " << hrledim << " " << layerindex << " "<< runtype_lend << " " << runtype_rend << std::endl;
            //std::cout << rle.runbreaks_.size() << " " << rle.runtypes_.size() << std::endl;
            //std::cout << "run breaks: " << rle.runbreaks_[runtype_lend] << " " << rle.runbreaks_[runtype_rend] << std::endl;
            assert(runtype_lend >= 0);
            assert(runtype_rend >= 0);
            //assert(runtype_rend <= rle.runbreaks_.size()-1);

            int breakpos = runtype_lend - layerindex;
            for (breakpos; breakpos < runtype_rend - layerindex; breakpos++)
            {
                //std::cout << "$$$ "<< coord <<" "<< rle.runbreaks_[breakpos] << std::endl; 
                if (coord >= rle.runbreaks_[breakpos])
                    idtf_runtype++;
                else
                    break;
            }
            // 如果未定义，返回
            //std::cout <<"check here "<< rle.runtypes_[idtf_runtype].state_  << " runtype ind " << idtf_runtype << std::endl;
            assert(idtf_runtype >= 0 && idtf_runtype < rle.runtypes_.size());
            if (rle.runtypes_[idtf_runtype].state_ != DEFINED)
                return rle.runtypes_[idtf_runtype].state_;
            else
            {
                // 找到它在哪一层次级网格
                int sublayerindex = coord - rle.runbreaks_[breakpos-1] + rle.runtypes_[idtf_runtype].index_;
                
                //std::cout << "sublayerindex: " << sublayerindex << " coords: " << coord << " runbreaks: " << rle.runbreaks_[breakpos] << " index: " << rles_[hrledim].runtypes_[idtf_runtype].index_ << "dim "<< cartdim << std::endl;
                assert(sublayerindex >= 0);
                
                /*
                进入这一层网格的编码，找它的位置
                */
                return CartesianToIndex(coords, sublayerindex, cartdim - 1);
            }
        }
    }
}

// 计划： 初始化HRLE还是手摇。写一个从老HRLE生成新HRLE的函数.
//然后，写更新函数。


// 程序可能要重新翻一遍，整理一下基础的数据结构
void HRLE::CreateSquare(Const::vecDi &coords)
{
    //std::cout << "get square" << std::endl;
    int index_ur = CartesianToIndex(Const::vecDi{coords[0],coords[1]+1}, 0, 1);
    int index_ll = CartesianToIndex(Const::vecDi{coords[0],coords[1]}, 0, 1);
    int index_lr = CartesianToIndex(Const::vecDi{coords[0]+1,coords[1]}, 0, 1);
    int index_ul = CartesianToIndex(Const::vecDi{coords[0]+1,coords[1]+1}, 0, 1);

    /*
    // 把合法性检验留给IndentifyEffectiveCube吧
    assert(index_ll >=0 || index_ur >=0);
    if(index_ur < 0) index_ur = index_ul;
    if(index_ll < 0) index_ll = index_lr;
    if(index_lr < 0) index_lr = index_ll;
    if(index_ul < 0) index_ul = index_ur;
    */

    activeSquares_.push_back(Square(index_ul, index_ll, index_lr, index_ur));
    //Square square();
    //return square;
}

std::array<int, Const::D> HRLE::GetActiveGrid(int index)
{
    return activegrids_[index];
}

HRLE::~HRLE()
{
}
#pragma endregion