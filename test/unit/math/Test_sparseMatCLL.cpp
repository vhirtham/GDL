#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/math/serial/sparseMatCLLSerial.h"
#include "test/tools/ExceptionChecks.h"



using namespace GDL;


// Ctor ---------------------------------------------------------------------------------------------------------------

template <typename _type>
void Test_Construction()
{
    SparseMatCLLSerial<_type> mat(5, 5);

    BOOST_CHECK(mat.Rows() == 5);
    BOOST_CHECK(mat.Cols() == 5);
}



BOOST_AUTO_TEST_CASE(Construction_Serial)
{
    Test_Construction<F32>();
    Test_Construction<F64>();
}



// Getter / Setter ----------------------------------------------------------------------------------------------------

template <typename _type>
void SetAndCheck(SparseMatCLLSerial<_type>& mat, U32 row, U32 col, _type val, U32 numVals)
{
    mat.Set(row, col, val);
    if (val == ApproxZero<_type>())
        BOOST_CHECK(mat(row, col) == ApproxZero<_type>());
    else
        BOOST_CHECK(mat(row, col) == Approx(val));
    BOOST_CHECK(mat.CountStoredValues() == numVals);
}



template <typename _type>
void RemoveAndCheck(SparseMatCLLSerial<_type>& mat, U32 row, U32 col, U32 numVals)
{
    mat.Remove(row, col);
    BOOST_CHECK(mat(row, col) == ApproxZero<_type>());
    BOOST_CHECK(mat.CountStoredValues() == numVals);
}



template <typename _type>
void Test_GetterSetter()
{
    SparseMatCLLSerial<_type> mat(5, 5);

    // first col
    SetAndCheck<_type>(mat, 1, 0, 2., 1);
    SetAndCheck<_type>(mat, 4, 0, 5., 2);
    SetAndCheck<_type>(mat, 3, 0, 3., 3);
    SetAndCheck<_type>(mat, 3, 0, 4., 3);
    SetAndCheck<_type>(mat, 3, 0, 0., 2);
    SetAndCheck<_type>(mat, 0, 0, 1., 3);
    RemoveAndCheck<_type>(mat, 4, 0, 2);

    // second col
    RemoveAndCheck<_type>(mat, 2, 1, 2);
    SetAndCheck<_type>(mat, 2, 1, 3., 3);
    SetAndCheck<_type>(mat, 0, 1, 1., 4);
    RemoveAndCheck<_type>(mat, 4, 1, 4);
    SetAndCheck<_type>(mat, 1, 1, 2., 5);

    // third col
    SetAndCheck<_type>(mat, 0, 2, 3., 6);
    RemoveAndCheck<_type>(mat, 0, 2, 5);
    SetAndCheck<_type>(mat, 0, 2, 3., 6);
    SetAndCheck<_type>(mat, 1, 2, 2., 7);
    RemoveAndCheck<_type>(mat, 0, 2, 6);
    SetAndCheck<_type>(mat, 2, 2, 3., 7);
    SetAndCheck<_type>(mat, 3, 2, 4., 8);

    // fourth col
    SetAndCheck<_type>(mat, 2, 3, 3., 9);
    SetAndCheck<_type>(mat, 3, 3, 4., 10);
    SetAndCheck<_type>(mat, 4, 3, 5., 11);

    // fifth col
    SetAndCheck<_type>(mat, 3, 4, 4., 12);
    SetAndCheck<_type>(mat, 4, 4, 5., 13);

    GDL_CHECK_THROW_DEV_DISABLE(mat.Set(2, 5, 6.), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(mat.Set(5, 2, 6.), Exception);

    //    std::cout << mat << std::endl;
    //    std::cout << mat.CountStoredValues() << std::endl;
}



BOOST_AUTO_TEST_CASE(GetterSetter_Serial)
{
    Test_GetterSetter<F32>();
    Test_GetterSetter<F64>();
}
