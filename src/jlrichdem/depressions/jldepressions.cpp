#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>
#include <richdem/depressions/fill_spill_merge.hpp>
namespace rd = richdem;

struct TestDepression
{
    rd::dephier::flat_c_idx pit_cell = rd::dephier::NO_VALUE;
    float pit_elev = std::numeric_limits<float>::infinity();
};

JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
{
    mod.add_type<TestDepression>("TestDepression", jlcxx::julia_type("TestDepression"));
}