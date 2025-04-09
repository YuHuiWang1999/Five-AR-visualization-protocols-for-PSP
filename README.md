# ================
# Project Dependencies
# ================

# ------------------
# Unity Dependencies
# ------------------

1. Vuforia Engine
   - Purpose: Core SDK for AR development
   - Installation:
     - Via Unity Package Manager (UPM): Add package URL:
       "https://download.vuforia.com/engine/package.json"
     - Or download .unitypackage from:
       https://developer.vuforia.com/

2. MRTK 2.8.3 (Mixed Reality Toolkit)
   - Purpose: Mixed reality interaction framework
   - Installation:
     - Unity Package Manager → Add from Git URL:
       "https://github.com/microsoft/MixedRealityToolkit-Unity.git?path=Assets/MRTK#v2.8.3"
   - Note: Requires Unity 2019.4+


# -----------------
# C++ Dependencies
# -----------------

3. ConnectNDI
   - Purpose: Network Device Interface communication
   - Installation:
     - Download SDK from: https://ndi.tv/sdk/
     - Set environment variable: NDI_ROOT=<installation_path>

4. Eigen 3.4.0 (Header-only)
   - Purpose: Linear algebra library
   - Installation:
     - Linux/macOS:
       wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
       tar -xzf eigen-3.4.0.tar.gz
       sudo cp -r eigen-3.4.0 /usr/local/include/eigen3

5. ITK 5.0.0
   - Purpose: Medical image processing
   - Installation:
     git clone https://github.com/InsightSoftwareConsortium/ITK.git --branch v5.0.0
     mkdir ITK-build && cd ITK-build
     cmake ../ITK -DBUILD_SHARED_LIBS=ON
     make -j$(nproc) && sudo make install

6. VTK 9.4.0
   - Purpose: 3D visualization
   - Installation:
     git clone https://gitlab.kitware.com/vtk/vtk.git --branch v9.4.0
     mkdir VTK-build && cd VTK-build
     cmake ../vtk -DBUILD_SHARED_LIBS=ON
     make -j$(nproc) && sudo make install

7. libzmq 4.3.5
   - Installation:
     wget https://github.com/zeromq/libzmq/releases/download/v4.3.5/zeromq-4.3.5.tar.gz
     tar -xzf zeromq-4.3.5.tar.gz
     cd zeromq-4.3.5
     ./configure && make && sudo make install

8. cppzmq 4.10.0
   - Installation:
     wget https://github.com/zeromq/cppzmq/archive/refs/tags/v4.10.0.tar.gz
     tar -xzf v4.10.0.tar.gz
     sudo cp cppzmq-4.10.0/zmq.hpp /usr/local/include

9. Qt 6.8.2
   - Purpose: GUI framework
   - Installation:
     - Download installer: https://www.qt.io/download-qt-installer
     - Select components:
       - Qt 6.8.2 (MSVC/GCC based on platform)
       - Qt Creator
       - Qt Network Authorization
     - Set environment variable: QT_DIR=<installation_path>


# ======================
# Package Manager Option
# ======================
For C++ dependencies, consider using vcpkg:
vcpkg install eigen3 itk vtk cppzmq

# ====================
# CMake Configuration
# ====================
Add to CMakeLists.txt:
find_package(ITK REQUIRED)
find_package(VTK REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

Notes:​​

Adjust installation commands for your OS (Windows/macOS/Linux)
Verify compatibility between library versions
For Windows users:
Use CMake GUI for ITK/VTK compilation
Prefer Qt Maintenance Tool for simplified installation
Recommended to use C++17 or newer standard
