#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <type_traits>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>
#include <richdem/depressions/fill_spill_merge.hpp>
namespace rd = richdem;

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

namespace jlcxx
{
    template <typename T>
    struct IsMirroredType<rd::dephier::Depression<T>> : std::true_type
    {
    };
}

JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
{
    using jlcxx::Parametric;
    using jlcxx::TypeVar;

    mod.add_type<Parametric<TypeVar<1>>>("Depression")
        .apply<rd::dephier::Depression<float>, rd::dephier::Depression<double>>(jlrichdem::WrapDepression());
}

// JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
// {
//     jlcxx::TypeWrapper<rd::dephier::Depression<double>> depression_double =
//         mod.add_type<rd::dephier::Depression<double>>("DepressionDouble");

//     jlcxx::TypeWrapper<rd::dephier::DepressionHierarchy<double>> depression_hierarchy_double =
//         mod.add_type<rd::dephier::DepressionHierarchy<double>>("DepressionHierarchyDouble");

//     jlcxx::TypeWrapper<rd::dephier::Depression<float>> depression_float =
//         mod.add_type<rd::dephier::Depression<float>>("DepressionFloat");

//     jlcxx::TypeWrapper<rd::dephier::DepressionHierarchy<float>> depression_hierarchy_float =
//         mod.add_type<rd::dephier::DepressionHierarchy<float>>("DepressionHierarchyFloat");

//     mod.method("GetDepressionHierarchyDoubleD8", [](const rd::Array2D<double> &topo,
//                                                     rd::Array2D<rd::dephier::dh_label_t> &label,
//                                                     rd::Array2D<int8_t> &flowdirs)
//                { return rd::dephier::GetDepressionHierarchy<double, rd::Topology::D8>(topo, label, flowdirs); });

//     mod.method("FillSpillMergeDouble", [](const rd::Array2D<double> &topo,
//                                           const rd::Array2D<rd::dephier::dh_label_t> &label,
//                                           const rd::Array2D<rd::flowdir_t> &flowdirs,
//                                           rd::dephier::DepressionHierarchy<double> &deps,
//                                           rd::Array2D<double> &wtd)
//                { return rd::dephier::FillSpillMerge(topo, label, flowdirs, deps, wtd); });

//     mod.method("GetDepressionHierarchyFloatD8", [](const rd::Array2D<float> &topo,
//                                                    rd::Array2D<rd::dephier::dh_label_t> &label,
//                                                    rd::Array2D<int8_t> &flowdirs)
//                { return rd::dephier::GetDepressionHierarchy<float, rd::Topology::D8>(topo, label, flowdirs); });

//     mod.method("FillSpillMergeFloat", [](const rd::Array2D<float> &topo,
//                                          const rd::Array2D<rd::dephier::dh_label_t> &label,
//                                          const rd::Array2D<rd::flowdir_t> &flowdirs,
//                                          rd::dephier::DepressionHierarchy<float> &deps,
//                                          rd::Array2D<double> &wtd)
//                { return rd::dephier::FillSpillMerge(topo, label, flowdirs, deps, wtd); });
// }
