# Welcome to MotionByte Framework

## Introduction

Hello! Welcome to my development project. As I am currently in the development phase, there is no detailed documentation available yet. However, if you are interested in testing my project, you can follow the instructions below to set up the environment and run a simple test.
[![Demo](https://img.youtube.com/vi/vN4mTlsvrZo/0.jpg)](https://www.youtube.com/watch?v=vN4mTlsvrZo)
## Setup Environment

### For Linux

Run the following script to set up the environment:

```bash
./setup_environment.sh
```

### For both Windows and Linux

1. Create a build directory and navigate to it:

```bash
mkdir build
cd build
```

2. Run CMake to configure the build:

```bash
cmake ..
```

3. Build the project:

```bash
cmake --build .
```

## Run Test

After setting up the environment, you can run the test executable. For Windows, locate the `windows_test.exe` in the build folder and run it from your terminal. This will launch a window with a rectangle inside.

You can interact with the rectangle by click and drag on retangle. Observe the animation as the rectangle modifies accordingly.

Feel free to experiment and provide me with feedback as I continue to develop and improve my project. Thank you for your interest!