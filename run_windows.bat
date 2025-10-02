@echo off
rem The Craft - Windows helper (final)

echo The Craft - Windows helper

if exist thecraft.exe (
    echo Found thecraft.exe.
) else (
    echo thecraft.exe not found. Attempting to build...
    where x86_64-w64-mingw32-gcc >nul 2>&1
    if %ERRORLEVEL%==0 (
        set "CC=x86_64-w64-mingw32-gcc"
    ) else (
        where gcc >nul 2>&1
        if %ERRORLEVEL%==0 (
            set "CC=gcc"
        ) else (
            echo No suitable compiler found in PATH. Install MinGW-w64 (or MSYS2) and add it to PATH.
            pause
            exit /b 1
        )
    )

    echo Using %CC% to compile. Ensure SDL2 development files are installed and in your include/lib path.
    "%CC%" -O2 -Wall src\main.c src\world.c -o thecraft.exe -lSDL2main -lSDL2
    if %ERRORLEVEL% neq 0 (
        echo Build failed. If you're using MSYS2, open the mingw64 shell and run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2
        pause
        exit /b 1
    )
)

rem Ensure SDL2.dll is present next to the exe. If you have a deps folder, copy from there.
if not exist SDL2.dll (
    if exist deps\SDL2.dll (
        copy /Y deps\SDL2.dll . >nul
        echo Copied deps\SDL2.dll to current folder.
    ) else (
        echo WARNING: SDL2.dll not found. Place SDL2.dll next to thecraft.exe or install SDL2 runtime.
    )
)

echo Running thecraft.exe
thecraft.exe

echo Exited. Press any key to close.
pause >nul
