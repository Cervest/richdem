module RichDEM

__precompile__(false)

module common
    using CxxWrap 
    @wrapmodule "/workspaces/richdem/build/lib/libjlrichdem"

    function __init__()
        @initcxx
    end

    Base.size(m::Array2D) = (width(m), height(m))
    Base.IndexStyle(::Type{<:Array2D}) = IndexCartesian()
end #common

module misc
    using CxxWrap
    @wrapmodule("/workspaces/richdem/build/lib/libjlrichdem.so", :define_misc_module)

    function __init__()
        @initcxx
    end
end #misc

module depressions
    using CxxWrap
    
    @wrapmodule("/workspaces/richdem/build/lib/libjlrichdem.so", :define_depressions_module)

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

    expand_struct(sub) = map(n->getfield(sub,n), fieldnames(typeof(sub)))

    function Depression(dep::CxxDepression)
        dep_obj = dep.cpp_object
        type = typeof(dep).parameters[1]
        dep_sub_ptr = Ptr{DepressionSubset{type}}(dep_obj)
        dep_sub = unsafe_load(Ptr{DepressionSubset{type}}(dep_sub_ptr))
        ol = ocean_linked(dep)
        Depression{type}(expand_struct(dep_sub)..., ol)
    end

    Base.size(dep_hier::DepressionHierarchy) = (size(dep_hier),)
    Base.IndexStyle(::Type{<:DepressionHierarchy}) = IndexCartesian()
    Base.length(dep_hier::DepressionHierarchy) = size(dep_hier)
end #depressions

end # module richdem
