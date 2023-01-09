#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>
#include <richdem/depressions/fill_spill_merge.hpp>
namespace rd = richdem;

JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
{
    mod.map_type<rd::dephier::Depression<float>>("Depression");
    mod.map_type<rd::dephier::Depression<double>>("Depression");
}