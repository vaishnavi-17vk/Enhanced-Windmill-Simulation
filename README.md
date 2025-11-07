# 🌬️ Windmill Simulation – C++ & OpenGL (OOP-Based)

**An interactive Object-Oriented C++ project demonstrating a complete windmill simulation with animated graphics using OpenGL (FreeGLUT).**

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-FreeGLUT-orange.svg)
![Status](https://img.shields.io/badge/Build-Stable-brightgreen.svg)

---

## 🎯 **Overview**

This project simulates a dynamic **windmill environment** with animated windmills, clouds, and day-night transitions.  
It’s designed to **demonstrate Object-Oriented Programming principles** (all 7 concepts) integrated with **computer graphics** using OpenGL.

---

## 💡 **Features**

| Feature | Description |
|----------|--------------|
| 🎡 **Multiple Windmills** | 3 windmills rotating independently |
| ☁️ **Animated Clouds** | Clouds move across the sky dynamically |
| 🌞 **Day/Night Mode** | Realistic lighting and sky transition |
| ⚡ **Speed Control** | Adjust windmill rotation speed interactively |
| ➕ **Dynamic Object Creation** | Add windmills and clouds at runtime |
| 🎮 **User Controls** | Keyboard-based real-time interaction |
| 🎓 **OOP Concepts** | All 7 OOP pillars demonstrated |

---

## 🧠 **OOP Concepts Used**

| Concept | Implementation |
|----------|----------------|
| **Encapsulation** | Private class members with getters/setters |
| **Abstraction** | Abstract base class `Drawable` |
| **Inheritance** | Classes like `Windmill`, `Cloud`, and `CelestialBody` derive from `Drawable` |
| **Polymorphism** | Virtual `draw()` and `update()` functions |
| **Composition** | `Scene` class manages all drawable objects |
| **Constructor/Destructor** | Proper initialization and cleanup |
| **Static Members** | Track number of windmills & selected object |

---

## 🕹️ **Controls**

| Key | Action |
|-----|---------|
| `1`, `2`, `3` | Select windmill |
| `+` / `-` | Increase / decrease speed |
| `D` / `N` | Toggle day/night |
| `C` | Add new cloud |
| `W` | Add new windmill |
| `P` | Pause / resume animation |
| `R` | Reset scene |
| `Q` / `ESC` | Exit program |

---

## 🏗️ **Project Structure**

```
Windmill-Simulation/
├── windmill_simulation.cpp     # Main source file
├── README.md                   # Project documentation (this file)
├── freeglut/                     # OpenGL dependency
└──   ├── include/                
      ├── lib/               
      └── bin/
                   
```

---

## ⚙️ **How to Run**

### 🔹 **Option 1: Run Directly**
Double-click **`run.bat`**  
→ The windmill simulation window will open automatically.

### 🔹 **Option 2: Compile Manually**
Run this command in terminal:
```bash
g++ windmill_simulation.cpp -o windmill_simulation.exe -I./freeglut/include -L./freeglut/lib -lfreeglut -lopengl32 -lglu32 -lgdi32
```

---

## 🌈 **Visual Scenes**

### ☀️ Day Mode
- Bright blue sky  
- Yellow sun with rays  
- Green landscape

### 🌙 Night Mode
- Deep blue sky  
- Glowing moon  
- White clouds for contrast

---

## 🎓 **Academic Use**

This project is ideal for:
- Object-Oriented Programming submissions  
- Computer Graphics coursework  
- Portfolio or demonstration projects  

**Learning Outcomes:**
- Practical understanding of OOP  
- OpenGL 2D graphics rendering  
- Event-driven and modular C++ design  

---


---

## 🧩 **Future Enhancements**

- 🌟 Add stars in night mode  
- 🌅 Implement sunrise/sunset transition  
- 🌧️ Add weather effects (rain, snow)  
- 🖱️ Mouse-based windmill selection  
- 💾 Save/Load scene configuration  

---

## 📄 **License**

This project is open for educational use.  
Feel free to modify, share, and learn from it.

---

## ❤️ **Author & Acknowledgments**

**Developed by:** Vaishnavi Kandekar  
**Institution:** K. K. Wagh Institute of Engineering Education & Research, Nashik  
**Guided by:** Prof. [Priti Vaidya]
**Tools Used:** C++, OpenGL, FreeGLUT, MinGW  

---

## 🚀 **Ready to Upload to GitHub**

### Steps to Push:
```bash
git init
git add .
git commit -m "Initial commit - Windmill Simulation OOP Project"
git branch -M main
git remote add origin https://github.com/<your-username>/windmill-simulation.git
git push -u origin main
```

---


