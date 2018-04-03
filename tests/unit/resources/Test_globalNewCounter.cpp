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

    struct myStruct
    {
        I32 a;
        F32 b;
    };

    myStruct* c = new myStruct;

    BOOST_CHECK(gnc.GetNumNewCalls() == 3);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 2);

    delete c;

    BOOST_CHECK(gnc.GetNumNewCalls() == 3);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 3);
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
