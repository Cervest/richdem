#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>

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
            wrapped.template constructor<const std::string &>();
            wrapped.method("width", &rd::Array2D::width);
            wrapped.method("height", &rd::Array2D::height);
            wrapped.method("numDataCells", &rd::Array2D::numDataCells);
        }
    };

}

JLCXX_MODULE define_julia_module(jlcxx::Module &mod)
{
    using jlcxx::Parametric;
    using jlcxx::TypeVar;
    mod.add_type<Parametric<TypeVar<1>>>("Array2D", jlcxx::julia_type("AbstractArray2D"))
        .apply<rd::Array2D<float>, rd::Array2D<rd::flowdir_t>, rd::Array2D<rd::dephier::dh_label_t>>(jlrichdem::WrapArray2D());
}
