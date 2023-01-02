module richdem
using libcxxwrap_julia_jll

__precompile__(false)

module array
    using CxxWrap 
    @wrapmodule "/workspaces/richdem/build/lib/libjlrichdem.so"

    function __init__()
        @initcxx
    end

    Base.size(m::Array2D) = (width(m), height(m))
    Base.IndexStyle(::Type{<:Array2D}) = IndexCartesian()
end #array

end # module richdem
