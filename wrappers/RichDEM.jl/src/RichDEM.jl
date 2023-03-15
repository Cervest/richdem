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
    ocean_linked::Vector{UInt32}
    dep_label::UInt32
    cell_count::UInt32
    dep_vol::Float64
    water_vol::Float64
    total_elevation::Float64
end

expand_struct(sub) = map(n -> getfield(sub, n), fieldnames(typeof(sub)))

function Depression(dep::CxxDepression{T}) where {T}
    Depression{T}(pit_cell(dep), out_cell(dep), parent(dep), odep(dep), geolink(dep), pit_elev(dep), 
        out_elev(dep), lchild(dep), rchild(dep), ocean_parent(dep), ocean_linked(dep), dep_label(dep), 
        cell_count(dep), dep_vol(dep), water_vol(dep), total_elevation(dep))

end


Base.IndexStyle(::Type{<:DepressionHierarchy}) = IndexLinear()
Base.size(v::DepressionHierarchy) = (Int(size(v)),)
Base.getindex(v::DepressionHierarchy, i::Int) = CxxWrap.StdLib.cxxgetindex(v, i)[]
Base.setindex!(v::DepressionHierarchy{T}, val, i::Int) where {T} =
    CxxWrap.StdLib.cxxsetindex!(v, convert(T, val), i)

@cxxdereference function Base.push!(v::DepressionHierarchy, val) 
    CxxWrap.StdLib.push_back(v, val)
    return v
end

@cxxdereference function Base.resize!(v::DepressionHierarchy, n::Integer)
    CxxWrap.StdLib.resize(v, n)
    return v    
end
end #depressions

end # module richdem
