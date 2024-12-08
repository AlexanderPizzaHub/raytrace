#pragma once

#include "../constants.hpp"
#include "geometry/objects.hpp"
#include <vector>

namespace hrle
{

    enum gridstate
    {
        UNDEFINED_POS = -1,
        UNDEFINED_NEG = -2,
        DEFINED = -3
    };

    class RunTypeCode
    {
        public:
            gridstate state_;
            int index_; // index=-1 if undefined

            RunTypeCode(gridstate state, int index);
            ~RunTypeCode();
    };

    class RLE
    {
    public:
        std::vector<int> startindices_;
        std::vector<RunTypeCode> runtypes_;
        std::vector<int> runbreaks_; //ascending order

        std::array<int, 2> extent_; // [min,max] box

        RLE(std::array<int,2> extent); // constructor defined by a bounding box
        ~RLE();
    };


    class HRLE
    {
        public:
            HRLE(std::array<std::array<int,2>,Const::D> extents);
            ~HRLE();

            void AddNewLayer(int dim, int layerindex, gridstate state);
            void AddDefinedSection(int dim, int startcoord); // new value appended to the end of the LSF, and RLE is changed accordingly by coordinate
            void AddUndefinedSection(int dim, int startcoord, gridstate state);

            int GetNumActiveGrids();

            //void ConstructAllSquares();

            //void CreateSquare(Const::vecDi& coords); //以后会优化到vector中
        
            //void Iterator(int startindex); // stop once for undefined run, stop once for each position on defined run
            bool DenseIterator(Const::vecDi& coord); // 明天写这个
            bool MixedIterator();

            int CartesianToIndex(Const::vecDi coords, int layerindex, int cartdim);
            //void IndexToCartesian(int index, Const::vecDi &coord);

            void GetSquareNeighbours(Const::vecDi coords, std::vector<int> indexlist);

            std::array<int,Const::D> GetActiveGrid(int index);

            
            //void RemoveGrid(Const::vecDi &coord); 

        private:
            std::vector<RLE> rles_;
            std::array<int,Const::D> maxdataindex_;
            std::vector<std::array<int, Const::D> > activegrids_;
            //std::vector<Square> activeSquares_;

    };


}  // namespace HRLE