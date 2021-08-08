# Raytracer (++)

Mustafa Quraish (2021)

---

A ray-tracer written in C++. Heavily WIP.

---

## Implemented

Geometry:
- Arbitrary object transformations
- Wavefront OBJ parser
- Two level Bounded Volume Hierarchies
- Implicit surfaces through Ray Marching
- Parametric Surfaces (Paraboloids, etc)*
- Surfaces of revolution*
- Beveled curves and knots*
- Displacement mapped planes / spheres*
- Basic L-Systems

> \* Converted to meshes

Shading:
- Ambient Occlusion
- Path tracing with next event estimation
- Direct Lighting

Materials (Path tracing):
- Specular reflection
- Specular transmission
- Lambertian
- Oren Nayar

Misc:
- Texture mapping
- Procedurally generated textures
- Normal mapping
- Depth of Field

---

Some meshes from the following resource have been tested successfully: (McGuire Computer Graphics Archive)[https://casual-effects.com/data/].

Currently the engine only supports PNG/PPM/BMP3 files, so if the textures included with the `.obj` file is in a different format, then use the provided script `./scripts/convert_images