## Folder Structure & File Types

Here is the purpose of each folder included in this project and when you should use it.

### 1. STL Folder (For 3D Printing)
* **What it is:** The universal standard for 3D printing. `.stl` files are "mesh" files made of millions of tiny triangles.
* **Purpose:** Use this if you want to **3D print** the object.
* **Pros/Cons:** It is lightweight and universally accepted by 3D printer software (slicers). However, it is very difficult to edit because it lacks "smart" geometry (it's just a hollow shell).

### 2. STEP Folder (For CAD & Precision Editing)
* **What it is:** The professional standard for sharing solid models between different CAD programs (like SolidWorks, Fusion 360, Rhino, or FreeCAD).
* **Purpose:** Use this if you need to **modify the dimensions** or **CNC machine** the part.
* **Pros/Cons:** Unlike STL, this format understands "true" geometry (perfect circles, smooth curves, solid volume). It is the best format for engineering work but does not contain the design history (timeline).

### 3. F3Z Folder (For Autodesk Fusion 360 Users)
* **What it is:** A native archive file specifically for **Autodesk Fusion 360**. It contains the main design file plus any linked components or sub-assemblies.
* **Purpose:** Use this only if you use Fusion 360 and want to **see the full design history**.
* **Pros/Cons:** It preserves the "timeline," meaning you can roll back time to see how the object was built and change the original sketches. It is useless if you do not use Fusion 360.

### 4. Polygon Folder (For Animation, VR & Rendering)
* **What it is:** This contains formats like `.OBJ`, `.FBX`, or `.PLY`. These are mesh files similar to STLs but usually support textures, colors, and UV maps.
* **Purpose:** Use this if you are using artistic software like **Blender, Maya, 3ds Max, or Unity/Unreal Engine**.
* **Pros/Cons:** Ideal for making renders, animations, or video game assets. It is generally not suitable for precision manufacturing or CAD engineering.
