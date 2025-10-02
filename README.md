# The Craft - Mini Minecraft-like demo (C + SDL2)

This is a tiny voxel demo written in C using SDL2. It's not a full Minecraft remake, but a minimal starting point to explore voxel rendering and world interaction.

Features
- Simple 3D-ish block projection
- Place/remove blocks with keyboard
- Move camera with WASD + Q/E

Requirements
- SDL2 development libraries (headers + pkg-config/sdl2-config)

Build
Run from the repository root:

```bash
make
```

Run

```bash
./thecraft
```

Controls
- W/A/S/D: move camera
- Q/E: move up/down
- Space: place a block above camera
- Backspace: remove a block below camera
- Esc: quit

New controls:
- Move mouse to look around (relative mouse mode is enabled by default)
- Press M to toggle mouse capture


Notes
- This demo is intentionally small and focused on a simple rendering loop. Expand it by adding textures, proper 3D projection, lighting, chunking, and saving/loading.

Windows
------
There is a helper batch script `run_windows.bat` to build and run on Windows. It will:

- Try to run `thecraft.exe` if present.
- Attempt to compile using a MinGW or GCC in PATH (you still need SDL2 development files installed).
- Copy `SDL2.dll` from `deps\SDL2.dll` if available, or warn you to place the DLL next to the EXE.

If you use MSYS2, the recommended commands are:

```bash
# in MSYS2 mingw64 shell
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2
gcc -O2 -Wall src/main.c src/world.c -o thecraft.exe -lmingw32 -lSDL2main -lSDL2
```

# The-Craft
MC remake lol - made with C
# PLEASE DONT BE RUDE TO ME!
There might be nothing in here but the readme.md
# I AM MAKING IT STILL  
so yeah.
