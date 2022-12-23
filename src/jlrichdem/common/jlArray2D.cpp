#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>

#include <richdem/common/Array2D.hpp>

namespace rd = richdem;

namespace jlrichdem
{
    // A functor object : Inside the struct there is one templated function
    struct WrapArray2D
    {
        template <typename TypeWrapperT>
        void operator()(TypeWrapperT &&wrapped)
        {
            typedef typename TypeWrapperT::type WrappedT;
            wrapped.method("get_first", &WrappedT::get_first);
            wrapped.method("get_second", &WrappedT::get_second);
        }
    };

    JLCXX_MODULE define_julia_module_richdem_common(jlcxx::Module &mod)
    {
        using jlcxx::Parametric;
        using jlcxx::TypeVar;
        mod.add_type<Parametric<TypeVar<1>>>("Array2D", jlcxx::julia_type("AbstractArray2D"))
            .apply<rd::Array2D<float>>(WrapArray2D);
    }
}