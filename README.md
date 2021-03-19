<!-- omit in toc -->
# Real-Time Volumetric Editor

<!-- omit in toc -->
## Table of Contents

- [Description](#description)
- [Run](#run)
  - [Dependencies](#dependencies)
  - [Launch](#launch)
- [Contributors](#contributors)

## Description

This project is a real-time volumetric simulation editor. It is inspired by [EmberGen](https://jangafx.com/software/embergen/). It is realized for one of the courses of the Imaging Science Master's degree at the Gustave Eiffel University.

This is a C++ project made thanks to Qt Quick 2 and OpenGL.

## Run

### Dependencies

To build and run Smoke, you must have:

- Cmake
- Qt
- OpenGL 4.3+ compatible hardware

### Launch

- Prepare the build with Cmake in **release** mode `-DCMAKE_BUILD_TYPE=Release` with the flag (or equivalent) `-DCMAKE_PREFIX_PATH="C:/Qt/5.15.0/msvc2019_64"`
- Build
- Run (you might add to your path: `C:/Qt/5.15.0/msvc2019_64/bin;C:/Qt/5.15.0/msvc2019_64/plugins/platforms`)

## Contributors

- Clément CHOMICKI
- Jules FOUCHY
- Julien HAUDEGOND
- Antoine LIBERT
