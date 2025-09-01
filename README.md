## 包结构

```
├── control_joy #手柄
│   ├── CMakeLists.txt
│   ├── include
│   ├── launch
│   ├── package.xml
│   ├── scripts
│   └── src
├── mult_nav #多点导航
│   ├── CMakeLists.txt
│   ├── dis
│   ├── include
│   ├── package.xml
│   └── src
├── nav_demo #建图+导航+算法调试
│   ├── CMakeLists.txt
│   ├── config
│   ├── launch
│   ├── map
│   ├── package.xml
│   ├── param
│   └── src
└── URDF02_gazebo #gazebo仿真
    ├── CMakeLists.txt
    ├── launch
    ├── package.xml
    ├── urdf
    └── worlds

```

将这几个包放到自己的工作空间编舞无误即可
