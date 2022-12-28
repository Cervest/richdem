module richdem

__precompile__(false)

using CxxWrap

@wrapmodule "/workspaces/richdem/build/lib/libjlrichdem"


function __init__()
    @initcxx
end


end # module richdem
