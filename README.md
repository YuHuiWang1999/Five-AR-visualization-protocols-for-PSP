Project Overview
This project is an integrated solution combining ​​Augmented Reality (AR) visualization​​ with ​​high-performance medical image processing​​, designed to bridge real-time 3D interaction and computational analysis. Built on Unity for immersive user experience and C++ for backend computation, this system enables:

🎮 ​​Mixed Reality Interaction​​: Leveraging Vuforia and MRTK 2.8.3 for holographic object manipulation
🧠 ​​Real-time Image Analysis​​: Powered by ITK/VTK for DICOM/NIfTI data processing
🌐 ​​Distributed Computing​​: ZeroMQ-based network architecture for multi-device communication
🖥️ ​​Cross-platform GUI​​: Qt 6.8.2 interface for desktop control panel
​​Key Innovation​​: The unique integration of AR tracking (Vuforia) with medical imaging pipelines (ITK/VTK) allows surgeons to visualize 3D reconstructed anatomy superimposed on physical environments, with sub-millimetre precision.

Core Components
​​AR Client (Unity)​​
Vuforia-based marker detection
MRTK-controlled holographic UI
Real-time pose streaming via NDI
​​Compute Server (C++)​​
Eigen-optimized matrix operations
ITK segmentation & VTK surface rendering
Multi-threaded task scheduler
​​Network Layer​​
ZeroMQ PUB/SUB pattern for 10ms latency data transfer
Protocol Buffers for message serialization

Reproducibility Statement
To ensure transparency and reproducibility:

​​Full source code​​ of the proposed algorithms is permanently available at:
https://github.com/YuHuiWang1999/Five-AR-visualization-protocols-for-PSP

If you use this work in your research, please cite:
@Article{Wang 2025VisualComputer,
  title={Enhanced In Situ Visualization for Robot-Assisted Pedicle Screw Placement: A Comparative Analysis of AR Guidance Concepts}, 
  author={Yuhui Wang, Chuanba Liu, Lu Xue, Zhiyuan He and Tao Sun},
  journal={The Visual Computer},
  year={2025},
  volume={00},
  pages={00},
  doi={10.0000/0000},
  url={https://github.com/YuHuiWang1999/Five-AR-visualization-protocols-for-PSP}
}

# ------------------
# Project Dependencies
# ------------------
# Unity Dependencies
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

# C++ Dependencies
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
