# 🏛️ FH Hall of KUET — 3D Architectural Visualization

A 3D architectural visualization of Fazlul Haque Hall, KUET, developed using Modern OpenGL (Core 3.3) in C++. The project models structural elements and applies realistic lighting techniques to simulate a detailed virtual environment.

---

## ✨ Features

- Full 3D architectural model of FH Hall  
- Custom 3D objects (cubes, spheres, Bézier curves)  
- Multiple lighting models:
  - Directional light  
  - Point light  
  - Spotlight  
- Shading techniques:
  - Gouraud Shading  
  - Phong Shading  
  - Phong Shading with Texture  
- Camera movement and scene navigation  
- Texture mapping support  

---

## ⚙️ Technologies

- C++  
- OpenGL 3.3 (Core Profile)  
- GLSL Shaders  
- stb_image for texture loading  
- Visual Studio  

---

## 📁 Project Structure

- `main.cpp` — application entry point  
- `shader.h` — shader management  
- `camera.h`, `basic_camera.h` — camera control  
- `cube.h`, `Sphere2.h` — 3D object definitions  
- `pointLight.h` — lighting implementation  
- `vertexShader*.vs` — vertex shaders  
- `fragmentShader*.fs` — fragment shaders  
- Texture assets for material rendering  

---

## 🎯 Purpose

Developed as part of a Computer Graphics course to demonstrate 3D modeling, lighting, and shading techniques using modern OpenGL.
