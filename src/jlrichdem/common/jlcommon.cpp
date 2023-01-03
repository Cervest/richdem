#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
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
            using xyT = typename WrappedT::xy_t;
            wrapped.template constructor<const std::string &>();
            wrapped.template constructor<xyT, xyT, const ScalarT &>();
            wrapped.method("width", &WrappedT::width);
            wrapped.method("height", &WrappedT::height);
            wrapped.method("numDataCells", &WrappedT::numDataCells);
            wrapped.method("saveGDAL", &WrappedT::saveGDAL);
            wrapped.method("get_projection", [](const WrappedT &mat)
                           { return mat.projection; });
            wrapped.method("isNoData", [](WrappedT &mat, xyT x, xyT y)
                           { return mat.isNoData(x, y); });
            wrapped.method("setNoData", [](WrappedT &mat, const ScalarT &ndval)
                           { return mat.setNoData(ndval);});

            // Overloading functions from julia base module.
            wrapped.module()
                .set_override_module(jl_base_module);
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
    mod.add_bits<int32_t>("xy_t");
    mod.add_bits<rd::Topology>("Topology", jlcxx::julia_type("CppEnum"));
    mod.set_const("D4", rd::Topology::D4);
    mod.set_const("D8", rd::Topology::D8);
    mod.add_type<Parametric<TypeVar<1>>>("Array2D", jlcxx::julia_type("AbstractMatrix"))
        .apply<rd::Array2D<double>, rd::Array2D<float>, rd::Array2D<rd::flowdir_t>, rd::Array2D<rd::dephier::dh_label_t>>(jlrichdem::WrapArray2D());
}