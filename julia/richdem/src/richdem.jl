module richdem

__precompile__(false)

using CxxWrap

# @wrapmodule constructs a table of existing functions and maps each function a hex number.
# @initcxx the function index is actually mapped to the function pointer, whenever the package is loaded.
@wrapmodule "/workspaces/richdem/build/lib/libjlrichdem"


# 
function __init__()
    @initcxx
end


end # module richdem
