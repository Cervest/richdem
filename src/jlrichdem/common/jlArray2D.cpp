#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>

namespace rd = richdem;

namespace jlrichdem
{
    struct WrapArray2D
    {
        template <typename TypeWrapperT>
        void operator()(TypeWrapperT &&wrapped)
        {
            using WrappedT = typename TypeWrapperT::type;
            using ScalarT = typename WrappedT::value_type;
            wrapped.template constructor<const std::string &>();
            wrapped.method("width", &WrappedT::width);
            wrapped.method("height", &WrappedT::height);
            wrapped.method("numDataCells", &WrappedT::numDataCells);
            wrapped.method("saveGDAL", &WrappedT::saveGDAL);

            wrapped.module().set_override_module(jl_base_module);
            wrapped.module().method("getindex", [](const WrappedT &m, int_t i, int_t j)
                                    { return m(i - 1, j - 1); });
            wrapped.module().method("setindex!", [](WrappedT &m, ScalarT value, int_t i, int_t j)
                                    { m(i - 1, j - 1) = value; });
            wrapped.module().unset_override_module();
        }
    };

}

namespace jlcxx
{
    template <typename T>
    struct BuildParameterList<rd::Array2D<T>>
    {
        using type = jlcxx::ParameterList<T>;
    };
} // namespace jlcxx

JLCXX_MODULE define_julia_module(jlcxx::Module &mod)
{
    using jlcxx::Parametric;
    using jlcxx::TypeVar;
    mod.add_type<Parametric<TypeVar<1>>>("Array2D", jlcxx::julia_type("AbstractMatrix"))
        .apply<rd::Array2D<float>, rd::Array2D<rd::flowdir_t>, rd::Array2D<rd::dephier::dh_label_t>>(jlrichdem::WrapArray2D());
}
