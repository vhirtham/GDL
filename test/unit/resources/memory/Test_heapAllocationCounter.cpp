#include <boost/test/unit_test.hpp>


#include "gdl/base/fundamentalTypes.h"
#include "gdl/resources/memory/utility/heapAllocationCounter.h"



#include <map>
#include <vector>

using namespace GDL;


BOOST_AUTO_TEST_CASE(New_Delete_Unaligned)
{
    I32 expAlloc = 0;
    I32 expDealloc = 0;

    HeapAllocationCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    I32* a = new I32(1);
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete(a);
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    F32* b = new F32[12];
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete[](b);
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);


    I32* c = new (std::nothrow) I32(1);
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete(c, std::nothrow);
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    F32* d = new (std::nothrow) F32[12];
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete[](d, std::nothrow);
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

#ifdef __cpp_sized_deallocation
    // sized deletes
    I32* e = new I32(12);
    expAlloc++;

    ::operator delete(e, sizeof(I32));
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    I32* f = new I32[12];
    expAlloc++;

    ::operator delete[](f, sizeof(I32[12]));
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);
#endif // __cpp_sized_deallocation

    struct myStruct
    {
        I32 a;
        F32 b;
    };

    myStruct* g = new myStruct;
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    delete g;
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    struct ThrowOnNew
    {
        ThrowOnNew()
        {
            throw std::exception();
        }
    };

    try
    {
        ThrowOnNew* h = new (std::nothrow) ThrowOnNew();
        BOOST_CHECK(false);
        delete h;
    }
    catch (std::exception&)
    {
        expAlloc++;
        expDealloc++;
        BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);
    }

    try
    {
        ThrowOnNew* i = new (std::nothrow) ThrowOnNew[12];
        BOOST_CHECK(false);
        delete[] i;
    }
    catch (std::exception&)
    {
        expAlloc++;
        expDealloc++;
        BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);
    }
}

BOOST_AUTO_TEST_CASE(New_Delete_Aligned)
{
    struct alignas(32) AlignedStruct
    {
        I32 mMember = 0;
    };

    I32 expAlloc = 0;
    I32 expDealloc = 0;

    HeapAllocationCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    AlignedStruct* a = new AlignedStruct;
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete(a, static_cast<std::align_val_t>(alignof(AlignedStruct)));
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    AlignedStruct* b = new AlignedStruct[12];
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete[](b, static_cast<std::align_val_t>(alignof(AlignedStruct)));
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);


    AlignedStruct* c = new (std::nothrow) AlignedStruct;
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete(c, static_cast<std::align_val_t>(alignof(AlignedStruct)), std::nothrow);
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    AlignedStruct* d = new AlignedStruct[12];
    expAlloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    ::operator delete[](d, static_cast<std::align_val_t>(alignof(AlignedStruct)), std::nothrow);
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

#ifdef __cpp_sized_deallocation
    // sized deletes
    AlignedStruct* e = new AlignedStruct;
    expAlloc++;

    ::operator delete(e, sizeof(AlignedStruct), static_cast<std::align_val_t>(alignof(AlignedStruct)));
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

    AlignedStruct* f = new AlignedStruct[12];
    expAlloc++;

    ::operator delete[](f, sizeof(AlignedStruct[12]), static_cast<std::align_val_t>(alignof(AlignedStruct)));
    expDealloc++;

    BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);

#endif // __cpp_sized_deallocation

    struct alignas(32) ThrowOnNew
    {
        ThrowOnNew()
        {
            throw std::exception();
        }
    };

    try
    {
        ThrowOnNew* h = new (std::nothrow) ThrowOnNew();
        BOOST_CHECK(false);
        delete h;
    }
    catch (std::exception&)
    {
        expAlloc++;
        expDealloc++;
        BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);
    }

    try
    {
        ThrowOnNew* i = new (std::nothrow) ThrowOnNew[12];
        BOOST_CHECK(false);
        delete[] i;
    }
    catch (std::exception&)
    {
        expAlloc++;
        expDealloc++;
        BOOST_CHECK(gnc.GetNumNewCalls() == expAlloc);
        BOOST_CHECK(gnc.GetNumDeleteCalls() == expDealloc);
    }
}

BOOST_AUTO_TEST_CASE(STL_Container)
{
    HeapAllocationCounter gnc;
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
