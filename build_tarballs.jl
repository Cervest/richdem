# Note that this script can accept some limited command-line arguments, run
# `julia build_tarballs.jl --help` to see a usage message.
using BinaryBuilder, Pkg

name = "librichdem"
version = v"2.3.1-cervest"

# Collection of sources required to complete build
sources = [
    GitSource("https://github.com/Cervest/richdem.git", "e9aaf851b8ccab32b74b8c3e6694117bbe69f5e4"),
]

# Bash recipe for building across all platforms
script = raw"""
# Override compiler ID to silence the horrible "No features found" cmake error
if [[ $target == *"apple-darwin"* ]]; then
  macos_extra_flags="-DCMAKE_CXX_COMPILER_ID=AppleClang -DCMAKE_CXX_COMPILER_VERSION=10.0.0 -DCMAKE_CXX_STANDARD_COMPUTED_DEFAULT=17"
fi
Julia_PREFIX=$prefix
mkdir build
cd build
cmake -DUSE_GDAL=ON -DJulia_PREFIX=$Julia_PREFIX -DCMAKE_FIND_ROOT_PATH=$prefix -DJlCxx_DIR=$prefix/lib/cmake/JlCxx -DCMAKE_INSTALL_PREFIX=$prefix -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TARGET_TOOLCHAIN} $macos_extra_flags -DCMAKE_BUILD_TYPE=Release ../richdem/
VERBOSE=ON cmake --build . --config Release --target install -- -j${nproc}
"""

# These are the platforms we will build for by default, unless further
# platforms are passed in on the command line
platforms = [
    Linux(:armv7l; libc=:glibc, compiler_abi=CompilerABI(cxxstring_abi=:cxx17)),
    Linux(:x86_64; libc=:glibc, compiler_abi=CompilerABI(cxxstring_abi=:cxx17)),
    MacOS(:x86_64; compiler_abi=CompilerABI(cxxstring_abi=:cxx17)),
    Windows(:x86_64; compiler_abi=CompilerABI(cxxstring_abi=:cxx17)),
]

# The products that we will ensure are always built
products = [
    LibraryProduct("librichem", :librichdem),
]

# Dependencies that must be installed before this package can be built
dependencies = [
    Dependency("libcxxwrap_julia_jll"),
    Dependency("GDAL_jll"),
    Dependency("IntelOpenMP_jll"),
    BuildDependency(PackageSpec(name="Julia_jll", version=v"1.5.0"))
]

# Build the tarballs, and possibly a `build.jl` as well.
build_tarballs(ARGS, name, version, sources, script, platforms, products, dependencies; preferred_gcc_version = v"10")