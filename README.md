# Build

First, install the dependencies:

```sudo apt-get install cmake build-essentials libglu1-mesa-dev freeglut3-dev mesa-common-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libopenal-dev libalut-dev libyaml-cpp-dev```

Then, create a `build` folder and run cmake:

```
mkdir build
cd build
cmake ..
```

Finally, build the engine:

```cmake --build .```

# Running

## Game mode

In the `build` folder, run:

```./ShadowEngine -d {DATA_FOLDER}```

Usually, the data folder is `../data`.

## Editor mode

Just add the `-e`argument to the command line:

```./ShadowEngine -d ../data -e```
