module richdem

__precompile__(false)

using CxxWrap

abstract type AbstractArray2D{T} <: AbstractMatrix{T} end 

@wrapmodule "/workspaces/richdem/build/lib/libjlrichdem"


function __init__()
    @initcxx
end


end # module richdem
