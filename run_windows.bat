@echo off
rem The Craft - Windows helper (clean)

echo The Craft - Windows helper

;if exist thecraft.exe goto runexe
;echo thecraft.exe not found. Attempting to build...
where x86_64-w64-mingw32-gcc >nul 2>&1
if errorlevel 1 (
    where gcc >nul 2>&1
    if errorlevel 1 (
        echo No suitable compiler found in PATH. Install MinGW-w64 (or MSYS2) and add it to PATH.
        pause
        exit /b 1
    ) else (
        set CC=gcc
    )
) else (
    set CC=x86_64-w64-mingw32-gcc
)

echo Using %CC% to compile. Ensure SDL2 development files are installed and in your include/lib path.
"%CC%" -O2 -Wall src\main.c src\world.c -o thecraft.exe -lSDL2main -lSDL2
if errorlevel 1 (
    echo Build failed. If you're using MSYS2, open the mingw64 shell and install SDL2 + gcc.
    pause
    exit /b 1
)

:runexe
if exist deps\SDL2.dll copy /Y deps\SDL2.dll . >nul
if not exist SDL2.dll echo WARNING: SDL2.dll not found. Place SDL2.dll next to thecraft.exe or install SDL2 runtime.

echo Running thecraft.exe
thecraft.exe

echo Exited. Press any key to close.
pause >nul
