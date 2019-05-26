#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/forwardList.h"
#include "gdl/base/container/vector.h"


namespace GDL
{

//! @brief Sparse matrix type that is implemented as a column linked list
//! @tparam _type: Data type
//! @remark Information about the data type can be found in the book "Direct Methods for Sparse Matrices" (second
//! edition) from Duff, Erisman and Reid.
template <typename _type>
class SparseMatCLLSerial
{


    //! @brief Internal data type of the linked list
    struct Node
    {
        U32 mRow;
        _type mValue;


        inline Node() = delete;
        inline Node(const Node& other) = default;
        inline Node(Node&& other) = default;
        inline Node& operator=(const Node& other) = default;
        inline Node& operator=(Node&& other) = default;
        inline ~Node() = default;

        //! @brief ctor
        //! @param row: The row index of the node
        //! @param value: The value of the node
        inline Node(U32 row, _type value);
    };


    U32 mRows;
    Vector<ForwardList<Node>> mCols;

public:
    inline SparseMatCLLSerial() = delete;
    inline SparseMatCLLSerial(const SparseMatCLLSerial& other) = delete;
    inline SparseMatCLLSerial(SparseMatCLLSerial&& other) = delete;
    inline SparseMatCLLSerial& operator=(const SparseMatCLLSerial& other) = delete;
    inline SparseMatCLLSerial& operator=(SparseMatCLLSerial&& other) = delete;
    inline ~SparseMatCLLSerial() = default;


    //! @brief ctor
    //! @param rows: Number of rows of the matrix
    //! @param cols: Number of columns of the matrix
    inline SparseMatCLLSerial(U32 rows, U32 cols);

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline _type operator()(U32 row, U32 col) const;

    //! @brief Gets the number of columns of the matrix
    //! @return Number of columns of the matrix
    inline U32 Cols() const;

    //! @brief Counts the number of stored values
    //! @return Number of stored values
    inline U32 CountStoredValues() const;

    //! @brief Removes a value from the matrix
    //! @param row: Row of the value
    //! @param col: Column of the value
    inline void Remove(U32 row, U32 col);

    //! @brief Gets the number of rows of the matrix
    //! @return Number of rows of the matrix
    inline U32 Rows() const;

    //! @brief Sets the value at the specified location.
    //! @param row: Row index of the value
    //! @param col: Column index of the value
    //! @param value: Value
    inline void Set(U32 row, U32 col, _type value);
};



//! @brief Offstream operator
//! @tparam _type: Data type of the matrix
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
template <typename _type>
std::ostream& operator<<(std::ostream& os, const SparseMatCLLSerial<_type>& mat);


} // namespace GDL


#include "sparseMatCLLSerial.inl"
