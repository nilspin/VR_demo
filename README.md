# VR_demo

This program demonstrates a very simple VR pipeline.  

### Ingredients:
    cmake
    glm
    SDL2
    not too old GCC

### To build:
    mkdir build; cd build
    cmake ..
    make
    ./VR_demo

### Usage:
    v - Quad view
    c - Cylindrical view
    b - Barrel distortion
    w/s/a/d/mouse - Move around in 3D space
    r - Reset camera to default position

### Program structure:

- Whole program is encapsulated in `Application` class
- Init: 
    - The window/GL context are set up
    - 2 cameras set up each with 32mm displacement from origin
    - Input images read and stored into buffer using `stb_image`
    - Shaders: Shaders for 3 operations are loaded
        - Quad projection - Renders input images onto a simple quad.
        - Cylindrical projection - Input quad bent into cylinder fitting entire view frustum in vertex shader `cylinder.vert`
        - Barrel distortion - Barrel distortion performed in fragment shader `barrelDist.frag`

    - Input buffers generated, along with texture coordinates. 
        - Index buffers are used to minimize redundant vertices.
        - Shader attributes specified for each buffer

    - Textures generated and image buffers previously read using `stb_image` uploaded to device memory. Both images attached to different texture units to minimize binding during gameloop.

- Loop: Runs indefinitely waiting user input.
    - User can change shaders
    - And move around in 3D space using wsad keys

### License:
MIT