#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>
#include <richdem/depressions/fill_spill_merge.hpp>
#include <typeinfo>
#include <vector>
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
    struct WrapDepressionHierarchy
    {
        template <typename TypeWrapperT>
        void operator()(TypeWrapperT &&wrapped)
        {
            using WrappedT = typename TypeWrapperT::type;      // WrappedT = std::vector<Depression<float>>
            using DepressionT = typename WrappedT::value_type; // DepressionT = Depression<float>
            using ScalarT = typename DepressionT::value_type;  // float
            wrapped.method("length", [](const WrappedT &vec)
                           { return vec.size(); });

            // Overloading functions in the julia_base_module
            wrapped.module()
                .set_override_module(jl_base_module);
            wrapped.module().method("getindex", [](const WrappedT &vec, int_t i)
                                    { return vec[i - 1]; });
            wrapped.module().method("setindex!", [](WrappedT &vec, DepressionT value, int_t i)
                                    { vec[i - 1] = value; });
            wrapped.module().unset_override_module();
        }
    };
}

JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
{
    using jlcxx::Parametric;
    using jlcxx::TypeVar;
    // mod.map_type<rd::dephier::Depression<float>>("Depression");
    // mod.map_type<rd::dephier::Depression<double>>("Depression");
    mod.map_type<rd::dephier::Depression<float>>("DepressionFloat");
    mod.add_type<Parametric<TypeVar<1>>>("DepressionHierarchy", jlcxx::julia_type("AbstractVector"))
        .apply<rd::dephier::DepressionHierarchy<float>>(jlrichdem::WrapDepressionHierarchy());
}