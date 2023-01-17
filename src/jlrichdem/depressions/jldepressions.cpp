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
            using ScalarT = typename WrappedT::value_type;

            wrapped.method("pit_cell", [](WrappedT &depression)
                           { return depression.pit_cell; });
            wrapped.method("out_cell", [](WrappedT &depression)
                           { return depression.out_cell; });
            wrapped.method("parent", [](WrappedT &depression)
                           { return depression.parent; });
            wrapped.method("odep", [](WrappedT &depression)
                           { return depression.odep; });
            wrapped.method("geolink", [](WrappedT &depression)
                           { return depression.geolink; });
            wrapped.method("pit_elev", [](WrappedT &depression)
                           { return depression.pit_elev; });
            wrapped.method("out_elev", [](WrappedT &depression)
                           { return depression.out_elev; });
            wrapped.method("lchild", [](WrappedT &depression)
                           { return depression.lchild; });
            wrapped.method("rchild", [](WrappedT &depression)
                           { return depression.rchild; });
            wrapped.method("ocean_parent", [](WrappedT &depression)
                           { return depression.ocean_parent; });
            wrapped.method("ocean_linked", [](WrappedT &depression)
                           { return jlcxx::ArrayRef<rd::dephier::dh_label_t, 1>(&(depression.ocean_linked[0]), depression.ocean_linked.size()); });
            wrapped.method("dep_label", [](WrappedT &depression)
                           { return depression.dep_label; });
            wrapped.method("cell_count", [](WrappedT &depression)
                           { return depression.cell_count; });
            wrapped.method("dep_vol", [](WrappedT &depression)
                           { return depression.dep_vol; });
            wrapped.method("water_vol", [](WrappedT &depression)
                           { return depression.water_vol; });
            wrapped.method("total_elevation", [](WrappedT &depression)
                           { return depression.total_elevation; });
        }
    };

    struct WrapDepressionHierarchy
    {
        template <typename TypeWrapperT>
        void operator()(TypeWrapperT &&wrapped)
        {
            using WrappedT = typename TypeWrapperT::type;
            using StructT = typename WrappedT::value_type;

            wrapped.method("size", [](WrappedT &vec)
                           { return vec.size(); });

            // Overloading functions from julia base module.
            wrapped.module()
                .set_override_module(jl_base_module);
            wrapped.module().method("getindex", [](const WrappedT &vec, int_t i)
                                    { return vec[i - 1]; });
            wrapped.module().method("setindex!", [](WrappedT &vec, StructT value, int_t i)
                                    { vec[i - 1] = value; });
            wrapped.module().unset_override_module();
        }
    };
}

namespace jlcxx
{
    template <typename T>
    struct IsMirroredType<rd::dephier::Depression<T>> : std::false_type
    {
    };

    template <typename T>
    struct IsMirroredType<rd::dephier::DepressionHierarchy<T>> : std::false_type
    {
    };
}

JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
{
    using jlcxx::Parametric;
    using jlcxx::TypeVar;

    mod.add_type<Parametric<TypeVar<1>>>("Depression")
        .apply<rd::dephier::Depression<float>, rd::dephier::Depression<double>>(jlrichdem::WrapDepression());

    mod.add_type<Parametric<TypeVar<1>>>("DepressionHierarchy")
        .apply<rd::dephier::DepressionHierarchy<float>,
               rd::dephier::DepressionHierarchy<double>>(jlrichdem::WrapDepressionHierarchy());

    mod.method("GetDepressionHierarchyFloatD8", [](const rd::Array2D<float> &topo,
                                                   rd::Array2D<rd::dephier::dh_label_t> &label,
                                                   rd::Array2D<int8_t> &flowdirs)
               { return rd::dephier::GetDepressionHierarchy<float, rd::Topology::D8>(topo, label, flowdirs); });
    mod.method("GetDepressionHierarchyDoubleD8", [](const rd::Array2D<double> &topo,
                                                    rd::Array2D<rd::dephier::dh_label_t> &label,
                                                    rd::Array2D<int8_t> &flowdirs)
               { return rd::dephier::GetDepressionHierarchy<double, rd::Topology::D8>(topo, label, flowdirs); });

    mod.method("FillSpillMergeFloat", [](const rd::Array2D<float> &topo,
                                         const rd::Array2D<rd::dephier::dh_label_t> &label,
                                         const rd::Array2D<rd::flowdir_t> &flowdirs,
                                         rd::dephier::DepressionHierarchy<float> &deps,
                                         rd::Array2D<double> &wtd)
               { return rd::dephier::FillSpillMerge(topo, label, flowdirs, deps, wtd); });

    mod.method("FillSpillMergeDouble", [](const rd::Array2D<double> &topo,
                                          const rd::Array2D<rd::dephier::dh_label_t> &label,
                                          const rd::Array2D<rd::flowdir_t> &flowdirs,
                                          rd::dephier::DepressionHierarchy<double> &deps,
                                          rd::Array2D<double> &wtd)
               { return rd::dephier::FillSpillMerge(topo, label, flowdirs, deps, wtd); });
}
