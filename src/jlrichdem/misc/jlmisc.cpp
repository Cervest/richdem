#include <jlcxx/jlcxx.hpp>
#include <jlcxx/stl.hpp>
#include <typeinfo>

#include <richdem/common/Array2D.hpp>
#include <richdem/common/constants.hpp>
#include <richdem/depressions/depression_hierarchy.hpp>
#include <richdem/misc/misc_methods.hpp>

namespace rd = richdem;

JLCXX_MODULE define_misc_module(jlcxx::Module &mod)
{
    mod.method("BucketFillFromEdgesD8Double", [](const rd::Array2D<double> &check_raster,
                                                 rd::Array2D<rd::dephier::dh_label_t> &set_raster,
                                                 double check_value, rd::dephier::dh_label_t set_value)
               { return rd::BucketFillFromEdges<rd::Topology::D8>(check_raster, set_raster, check_value, set_value); });

    mod.method("BucketFillFromEdgesD8Float", [](const rd::Array2D<float> &check_raster,
                                                rd::Array2D<rd::dephier::dh_label_t> &set_raster,
                                                float check_value, rd::dephier::dh_label_t set_value)
               { return rd::BucketFillFromEdges<rd::Topology::D8>(check_raster, set_raster, check_value, set_value); });
}