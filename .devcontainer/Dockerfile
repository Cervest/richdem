FROM julia:latest

# Install dependencies, definitely install gdal and openmp required by richdem
RUN apt-get update && apt-get install -y build-essential gdb gdbserver \
    libatomic1 python gfortran perl wget m4 cmake pkg-config git libgdal-dev g++ \
    libomp-dev

# Clone and Install libcxxwrap
RUN set -eux; \
    git clone https://github.com/JuliaInterop/libcxxwrap-julia.git; \
    mkdir build; \
    cd build; \
    cmake -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/usr/local -DAPPEND_OVERRIDES_TOML=ON ../libcxxwrap-julia/; \
    cmake --build . --config RelWithDebInfo --target install; \
    sed -i 's!libcxxwrap_julia = .*!libcxxwrap_julia = "/usr/local"!g' ~/.julia/artifacts/Overrides.toml; \
    julia -e 'using Pkg; Pkg.add("CxxWrap")'; \
    julia -e 'using Pkg; Pkg.test("CxxWrap")'; \
    julia -e 'using CxxWrap; println(CxxWrap.prefix_path())'; \
    rm -rf libcxxwrap-julia build

# Update C env vars so compiler can find gdal
ENV CPLUS_INCLUDE_PATH=/usr/include/gdal
ENV C_INCLUDE_PATH=/usr/include/gdal