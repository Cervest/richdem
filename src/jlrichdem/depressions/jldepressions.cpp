#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>
#include <richdem/depressions/fill_spill_merge.hpp>
namespace rd = richdem;

namespace jlrichdem
{
    // Helper to wrap Depression instances.
    struct WrapDepression
    {
        template <typename TypeWrapperT>
        void operator()(TypeWrapperT &&wrapped)
        {
            using WrappedT = typename TypeWrapperT::type;
            wrapped.method("pit_cell", [](const WrappedT &dep)
                           { return dep.pit_cell; });
        }
    };
}

JLCXX_MODULE define_depressions_module(jlcxx::Module &mod)
{
    using jlcxx::Parametric;
    using jlcxx::TypeVar;

    mod.add_type<Parametric<TypeVar<1>>>("Depression")
        .apply<rd::dephier::Depression<double>>(jlrichdem::WrapDepression());
}