@echo off
echo Compiling project...

g++ main.cpp -std=c++17 -o punctuation_project.exe

if %errorlevel% neq 0 (
    echo Compile failed.
    pause
    exit /b
)

echo Running project...
punctuation_project.exe

echo Done.
pause