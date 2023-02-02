# RichDEM

This is a minimal package to call the C++ classes and methods of the `richdem` C++ library by Richard Barnes.
The wrapper library can be found here : [RichDEM_jll.jl](https://github.com/JuliaBinaryWrappers/RichDEM_jll.jl).

Here is a list of calls of the methods that have been wrapped :

- `Array2D{Float32}` and `Array2D{Float64}` constructors which take a string denoting path to file
  ```julia
tiff_array = RichDEM.common.Array2D{Float32}("path/to/geotiff")
```
- Constructor to instantiate an empty array, e.g, `RichDEM.common.Array2D{Float64}()`
- `Array2D{T}` constructor for assigning same value to all entries of a matrix works, e.g,
```julia
RichDEM.common.Array2D{UInt32}(width, height, 0)
```
The parameter `T` can be of types : `Float32`, `Float64`, `UInt32` and `Int8`.
- An example of generating an empty array, resizing it
```julia
weights = RichDEM.common.Array2D{Float64}()
RichDEM.common.resize(weights, width, height, 10)
```
- The following methods have also been wrapped:
```julia
RichDEM.misc.BucketFillFromEdgesD8Double(check_raster, set_raster, check_value, set_value)
```
where `check_raster` is of type `Array2D{Float32}` or `Array2D{Float64}`, `set_raster` is of type
`Array2D{UInt32}` and `check_value` is a scalar of type `Float32` or `Float64` depending on the type of `check_raster` array and `set_value` is a type `UInt32`.
