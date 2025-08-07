# Spherical Terrain Generator in OpenGL
<img width="509" height="498" alt="image" src="https://github.com/user-attachments/assets/b0bad817-4442-47f1-afb9-77fe6a08a928" />

## 1. Tetrahedron Subdivision Algorithm

This project implements a tetrahedron subdivision algorithm to approximate a sphere, based on the method proposed in the paper [Generation and Rendering of Fractal Terrains on Approximated
 Spherical Surfaces](https://worldcomp-proceedings.com/proc/p2013/CGV4061.pdf), by J. M. Willemse and K. A. Hawick.
 <img width="300" height="300" alt="image" src="https://github.com/user-attachments/assets/bbfc9958-cf1a-4be7-9bc8-14a33c36646d" />
 <img width="300" height="300" alt="image" src="https://github.com/user-attachments/assets/64bdd71c-11e7-42f9-9f38-a16829b93286" />
 <img width="300" height="300" alt="image" src="https://github.com/user-attachments/assets/950ab67f-56e1-45b5-afd8-b10647b430a4" />
 
*A visualization of subdivided tretrahedron at the depth of 2, 4, 6 respectively where each faces are marked corresponding to its type: White (Top), Red (Left), Green (Right), and Blue (Center)*


 
The process uses a tree traversal data structure to recursively subdivide a **normalized regular tetrahedron**. Each triangular face is subdivided into four smaller triangles by introducing three new midpoint vertices along its edges. These midpoints may include slight random variations in elevation, while remaining within the bounds of the unit sphere. Similar to binary tree structure, each tetrahedron node has four child nodes storing four child triangles: Top, Center, Left, Right. Each triangle node also stores its three vertices defining the triangle, three midpoint vertices assigned during fractal displacement, and the address from the root node i.e. TLR (Top -> Left -> Right). As we subdivide each node, we use the node's address to determine the neighbor node (detail on this are carefully explained in the paper) and check whether the neighbor node sharing the same edge has been subdivided. If so, we must use the same vertice on the shared edge to join the subdivided triangles. This recursive subdivision creates increasingly detailed sphere approximations with a natural, terrain-like appearance.

## 2. Biome Classification

The project includes a simplified version of the [**Whittaker biome model**](http://pcg.wikidot.com/pcg-algorithm:whittaker-diagram), using **temperature** and **precipitation** as the two main classification factors.

### Temperature

Temperature is influenced by both **latitude** and **elevation**:

- **Latitude-based baseline temperature:**
  - `0°–30°` → High temperature
  - `30°–60°` → Moderate temperature
  - `60°–90°` → Low temperature

- **Elevation adjustment:**
  - Higher elevations from sea level reduce the baseline temperature further to simulate varying elevation at the same latitude regions.

### Precipitation

Precipitation is estimated solely from elevation, assuming that higher elevation areas are farther from water sources and thus receive less moisture.

---

## Biome Matrix

Each node is assigned a **temperature level** and a **moisture level**, which are used to index into a matrix that determines the biome type and assigns an appropriate color.

<img width="924" height="293" alt="image" src="https://github.com/user-attachments/assets/ea64ec0e-a2a9-4e98-bfa6-a9c2be5fcd36" />

---

## TODO / Future Work

Overall, the future goal is for the sphere to look like the second Earth as possible.
- Improve biome blending at boundaries between biome and between ocean and land
- Improve moisture estimation (perhaps add wind condition or random humidity)
  

---


## Acknowledgements

This project utilizes materials provided in CS450: Introduction to Computer Graphics, taught by Mike Bailey and CS 554: Geometric Modelling taught by Eugene Zhang at Oregon State University.
Some code snippets, project structure were adapted from the course resources shared publicly for classroom use in the [classroom website](https://web.engr.oregonstate.edu/~mjb/cs450/).

## Resources:
- [Polygonal Map Generation for Games](http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/)
- [Generation and Rendering of Fractal Terrains on Approximated
 Spherical Surfaces](https://worldcomp-proceedings.com/proc/p2013/CGV4061.pdf), by J. M. Willemse and K. A. Hawick.
- [Cliamte Zones](https://www.noaa.gov/jetstream/global/climate-zones)
- [Whittaker biome model](http://pcg.wikidot.com/pcg-algorithm:whittaker-diagram)

