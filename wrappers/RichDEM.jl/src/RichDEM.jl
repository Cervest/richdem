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
    mutable struct Depression{T <: Real}
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
    @wrapmodule("/workspaces/richdem/build/lib/libjlrichdem.so", :define_depressions_module)

    function __init__()
        @initcxx
    end

    # Base.size(dep_hier::DepressionHierarchyDouble) = size(dep_hier)
    # Base.IndexStyle(::Type{<:DepressionHierarchyDouble}) = IndexCartesian()
end #depressions

end # module richdem
