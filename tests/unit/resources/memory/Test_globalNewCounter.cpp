#include <boost/test/unit_test.hpp>


#include "gdl/GDLTypedefs.h"
#include "gdl/resources/memory/utility/globalNewCounter.h"



#include <map>
#include <vector>


using namespace GDL;



BOOST_AUTO_TEST_CASE(New_Delete)
{
    GlobalNewCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    I32* a = new I32(1);

    BOOST_CHECK(gnc.GetNumNewCalls() == 1);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    delete a;

    BOOST_CHECK(gnc.GetNumNewCalls() == 1);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 1);

    F32* b = new F32[12];

    BOOST_CHECK(gnc.GetNumNewCalls() == 2);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 1);

    delete[] b;

    BOOST_CHECK(gnc.GetNumNewCalls() == 2);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 2);


    I32* c = new (std::nothrow) I32(1);

    BOOST_CHECK(gnc.GetNumNewCalls() == 3);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 2);

    delete c;

    BOOST_CHECK(gnc.GetNumNewCalls() == 3);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 3);

    F32* d = new F32[12];

    BOOST_CHECK(gnc.GetNumNewCalls() == 4);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 3);

    delete[] d;

    BOOST_CHECK(gnc.GetNumNewCalls() == 4);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 4);

    struct myStruct
    {
        I32 a;
        F32 b;
    };

    myStruct* e = new myStruct;

    BOOST_CHECK(gnc.GetNumNewCalls() == 5);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 4);

    delete e;

    BOOST_CHECK(gnc.GetNumNewCalls() == 5);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 5);

    struct ThrowOnNew
    {
        ThrowOnNew()
        {
            throw std::exception();
        }
    };

    try
    {
        ThrowOnNew* f = new (std::nothrow) ThrowOnNew();
        BOOST_CHECK(false);
        delete f;
    }
    catch (std::exception& e)
    {
        BOOST_CHECK(gnc.GetNumNewCalls() == 6);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 6);
    }

    try
    {
        ThrowOnNew* g = new (std::nothrow) ThrowOnNew[12];
        BOOST_CHECK(false);
        delete[] g;
    }
    catch (std::exception& e)
    {
        BOOST_CHECK(gnc.GetNumNewCalls() == 7);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 7);
    }
}


BOOST_AUTO_TEST_CASE(STL_Container)
{
    GlobalNewCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);


    {
        std::map<I32, F32> m;
        std::vector<F32> v;

        BOOST_CHECK(gnc.GetNumNewCalls() == 0);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

        v.push_back(1.);

        BOOST_CHECK(gnc.GetNumNewCalls() == 1);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

        v.reserve(200);

        BOOST_CHECK(gnc.GetNumNewCalls() == 2);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 1);

        m.emplace(1, 1.);

        BOOST_CHECK(gnc.GetNumNewCalls() == 3);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 1);

        m.emplace(2, 2.);

        BOOST_CHECK(gnc.GetNumNewCalls() == 4);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 1);

        m.clear();

        BOOST_CHECK(gnc.GetNumNewCalls() == 4);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == 3);
    }


    BOOST_CHECK(gnc.GetNumNewCalls() == 4);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 4);
}
