@echo off
echo Starting build script...

REM Change to the project root directory from the bat directory
cd /d "%~dp0..\..\"

REM Compile the C program
gcc src\main\main.c -o out\assignment2.exe

IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
) ELSE (
    echo Compilation successful.
)
