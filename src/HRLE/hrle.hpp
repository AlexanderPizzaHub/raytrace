#pragma once

#include "../constants.hpp"
#include <vector>

namespace hrle
{

    enum gridstate
    {
        UNDEFINED = -2,
        DEFINED = -1
    };

    class RunTypeCode
    {
        public:
            gridstate state_;
            label index_;

            RunTypeCode(gridstate state, label index);
            ~RunTypeCode();
    };

    class RLE
    {
    public:
        std::vector<label> startindices_;
        std::vector<RunTypeCode> runtypes_;
        std::vector<label> runbreaks_;

        std::array<label, 2> extent_; // [min,max] box

        RLE(std::array<label,2> extent); // constructor defined by a bounding box
        ~RLE();
    };


    class HRLE
    {
        public:
            HRLE(std::array<std::array<label,2>,Const::D> extents);
            ~HRLE();

            std::vector<RLE> rles_;

            void GetCellNeighbors(label index, std::vector<label> &neighbors);
            
            void Iterator(label startindex); // stop once for undefined run, stop once for each position on defined run

            label CartesianToIndex(Const::vecDi &coord);
            void IndexToCartesian(label index, Const::vecDi &coord);

            void InsertGrid(Const::vecDi &coord); // new value appended to the end of the LSF, and RLE is changed accordingly by coordinate
            void RemoveGrid(Const::vecDi &coord); 



    };


}  // namespace HRLE