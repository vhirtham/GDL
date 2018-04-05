#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/internalStackAllocator.h"
#include "gdl/resources/memory/utility/globalNewCounter.h"

#include <vector>


using namespace GDL;

constexpr U32 memorySize = 2000;

BOOST_AUTO_TEST_CASE(Memory_Manger_Setup)
{
    GlobalNewCounter gnc;
    MemoryManager::SetInternalThreadStackSize(memorySize);
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
    MemoryManager::Initialize();
    BOOST_CHECK(gnc.GetNumNewCalls() == 1);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    BOOST_CHECK_THROW(MemoryManager::SetInternalThreadStackSize(10000), Exception);
}

BOOST_AUTO_TEST_CASE(STL_Vector)
{
    GlobalNewCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    std::vector<I32, InternalStackAllocator<I32>> v;
    for (I32 i = 0; i < 20; ++i)
        v.push_back(i);

    for (I32 i = 0; i < 20; ++i)
        BOOST_CHECK(v[i] == i);

    BOOST_CHECK(gnc.GetNumNewCalls() == 2);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    BOOST_CHECK_THROW(v.reserve(memorySize), Exception);
}


BOOST_AUTO_TEST_CASE(STL_Map)
{
    GlobalNewCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
    std::map<I32, F32, std::less<I32>, InternalStackAllocator<std::pair<const I32, F32>>> m;

    for (I32 i = 0; i < 20; ++i)
        m.emplace(i, static_cast<F32>(i));

    for (I32 i = 0; i < 20; ++i)
        BOOST_CHECK(m.find(i) != m.end());

    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
}


#include <functional>
#include <utility>
#include <thread>
#include <future>


// https://stackoverflow.com/questions/10766112/c11-i-can-go-from-multiple-args-to-tuple-but-can-i-go-from-tuple-to-multiple
namespace detail
{
template <typename F, typename Tuple, typename resultType, bool Done, int Total, int... N>
struct call_impl
{
    static resultType call(F f, Tuple&& t)
    {
        return call_impl<F, Tuple, resultType, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(
                f, std::forward<Tuple>(t));
    }
};

// partielle spezialisierung der obigen version mit Done == true!
template <typename F, typename Tuple, typename resultType, int Total, int... N>
struct call_impl<F, Tuple, resultType, true, Total, N...>
{
    static resultType call(F f, Tuple&& t)
    {
        return f(std::get<N>(std::forward<Tuple>(t))...);
    }
};
}

// user invokes this
template <typename resultType, typename F, typename Tuple>
resultType call(F f, Tuple&& t)
{

    typedef typename std::decay<Tuple>::type ttype;
    return detail::call_impl<F, Tuple, resultType, 0 == std::tuple_size<ttype>::value,
                             std::tuple_size<ttype>::value>::call(f, std::forward<Tuple>(t));
}


template <class F, class... Args>
class myPackagedTask
{

    F* mFunction;
    std::tuple<Args...> mArgTuple;
    using resultType = std::result_of_t<F && (Args && ...)>;

public:
    myPackagedTask(F&& func, Args... args)
        : mFunction(std::move(func))
        , mArgTuple(std::forward<Args>(args)...)
    {
    }

    resultType execute()
    {
        return call<resultType>(mFunction, mArgTuple);
        //        return resultType{};
    }
};



typedef std::basic_string<char, std::char_traits<char>, InternalStackAllocator<char>> String;

float testF(int a, float b)
{
    std::cout << "... ";
    return b + a;
}

String testG(int val, std::map<I32, F32, std::less<I32>, InternalStackAllocator<std::pair<const I32, F32>>>& m)
{
    auto it = m.find(val);
    if (it == m.end())
    {
        m.emplace(val, static_cast<F32>(val));
        return "Nothing found, inserted something!";
    }
    // std::cout << std::to_string(it->second) << std::endl;

    return "Found something!";
}


BOOST_AUTO_TEST_CASE(Packaged_Task)
{
    GlobalNewCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
    std::map<I32, F32, std::less<I32>, InternalStackAllocator<std::pair<const I32, F32>>> m;

    std::tuple<int, std::map<I32, F32, std::less<I32>, InternalStackAllocator<std::pair<const I32, F32>>>> testTuple(1,
                                                                                                                     m);

    myPackagedTask f(testF, 50, 3000.);
    myPackagedTask g(testG, 1, std::ref(m));
    myPackagedTask h(testG, 1, std::ref(m));

    // f.execute();
    // g.execute();
    //    std::cout << "f returned: " << f.execute() << std::endl;
    std::cout << "g returned: " << g.execute() << std::endl;
    std::cout << "h returned: " << h.execute() << std::endl;
    //    m.clear();
    // auto test2 = std::bind(testF, 50, 300.);
    //    {
    //        auto test = std::bind(testF, 1, 30., m);
    //        test2 = std::move(test);
    //    }

    // test2();
    // test();

    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
}
