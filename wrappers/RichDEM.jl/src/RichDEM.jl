module RichDEM
module common

using CxxWrap
using RichDEM_jll

#@wrapmodule("/workspaces/richdem/build/lib/libjlrichdem.so", :define_julia_module, Libdl.RTLD_GLOBAL)
@wrapmodule(RichDEM_jll.libjlrichdem_path, :define_julia_module)

function __init__()
    @initcxx
end

Base.size(m::Array2D) = (width(m), height(m))
Base.IndexStyle(::Type{<:Array2D}) = IndexCartesian()
end #common

module misc
using Libdl
using CxxWrap
using RichDEM_jll

#@wrapmodule("/workspaces/richdem/build/lib/libjlrichdem.so", :define_misc_module, Libdl.RTLD_GLOBAL)
@wrapmodule(RichDEM_jll.libjlrichdem_path, :define_misc_module, Libdl.RTLD_GLOBAL)

function __init__()
    @initcxx
end
end #misc

module depressions
using Libdl
using CxxWrap
using RichDEM_jll

export Depression
#@wrapmodule("/workspaces/richdem/build/lib/libjlrichdem.so", :define_depressions_module, Libdl.RTLD_GLOBAL)
@wrapmodule(RichDEM_jll.libjlrichdem_path, :define_depressions_module, Libdl.RTLD_GLOBAL)

function __init__()
    @initcxx
end
struct DepressionSubset{T}
    pit_cell::UInt32
    out_cell::UInt32
    parent::UInt32
    odep::UInt32
    geolink::UInt32
    pit_elev::T
    out_elev::T
    lchild::UInt32
    rchild::UInt32
    ocean_parent::Bool
    dep_label::UInt32
    cell_count::UInt32
    dep_vol::Float64
    water_vol::Float64
    total_elevation::Float64
end

mutable struct Depression{T}
    pit_cell::UInt32
    out_cell::UInt32
    parent::UInt32
    odep::UInt32
    geolink::UInt32
    pit_elev::T
    out_elev::T
    lchild::UInt32
    rchild::UInt32
    ocean_parent::Bool
    dep_label::UInt32
    cell_count::UInt32
    dep_vol::Float64
    water_vol::Float64
    total_elevation::Float64
    ocean_linked::Vector{UInt32}
end

expand_struct(sub) = map(n -> getfield(sub, n), fieldnames(typeof(sub)))

function Depression(dep::CxxDepression)
    dep_obj = dep.cpp_object
    type = typeof(dep).parameters[1]
    dep_sub_ptr = Ptr{DepressionSubset{type}}(dep_obj)
    dep_sub = unsafe_load(Ptr{DepressionSubset{type}}(dep_sub_ptr))
    ol = ocean_linked(dep)
    Depression{type}(expand_struct(dep_sub)..., ol)
end

Base.IndexStyle(::Type{<:DepressionHierarchy}) = IndexLinear()
Base.size(v::DepressionHierarchy) = (Int(size(v)),)
Base.getindex(v::DepressionHierarchy, i::Int) = CxxWrap.StdLib.cxxgetindex(v, i)[]
Base.setindex!(v::DepressionHierarchy{T}, val, i::Int) where {T} =
    CxxWrap.StdLib.cxxsetindex!(v, convert(T, val), i)
end #depressions

end # module richdem
