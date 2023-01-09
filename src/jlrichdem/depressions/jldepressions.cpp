#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>
#include <richdem/depressions/fill_spill_merge.hpp>
namespace rd = richdem;

// JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
// {
//     jlcxx::TypeWrapper<rd::dephier::Depression<double>> depression_double =
//         mod.add_type<rd::dephier::Depression<double>>("DepressionDouble");

//     jlcxx::TypeWrapper<rd::dephier::DepressionHierarchy<double>> depression_hierarchy_double =
//         mod.add_type<rd::dephier::DepressionHierarchy<double>>("DepressionHierarchyDouble");

//     mod.method("GetDepressionHierarchyDoubleD8", [](const rd::Array2D<double> &topo,
//                                                     rd::Array2D<rd::dephier::dh_label_t> &label,
//                                                     rd::Array2D<int8_t> &flowdirs)
//                { return rd::dephier::GetDepressionHierarchy<double, rd::Topology::D8>(topo, label, flowdirs); });

//     mod.method("FillSpillMerge", [](const rd::Array2D<double> &topo,
//                                     const rd::Array2D<rd::dephier::dh_label_t> &label,
//                                     const rd::Array2D<rd::flowdir_t> &flowdirs,
//                                     rd::dephier::DepressionHierarchy<double> &deps,
//                                     rd::Array2D<double> &wtd)
//                { return rd::dephier::FillSpillMerge(topo, label, flowdirs, deps, wtd); });
// }

namespace jlrichdem
{
    struct WrapDepression
    {
        template <typename TypeWrapperT>
        void operator()(TypeWrapperT &&wrapped)
        {
            using WrappedT = typename TypeWrapperT::type;
        }
    };
}

JLCXX_MODULE define_depressions_module(jlcxx::Module &types)
{
    using jlcxx::Parametric;
    using jlcxx::TypeVar;
    types.add_type<Parametric<TypeVar<1>>>("Depression", jlcxx::julia_type("AbstractDepression"))
        .apply<rd::dephier::Depression<double>>(jlrichdem::WrapDepression());
}