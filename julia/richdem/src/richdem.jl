module richdem

__precompile__(false)

using CxxWrap

@wrapmodule "/workspaces/richdem/build/lib/libjlrichdem"


function __init__()
    @initcxx
end

Base.size(m::Array2D) = (width(m), height(m))
Base.IndexStyle(::Type{<:Array2D}) = IndexCartesian()


end # module richdem
