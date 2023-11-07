@echo off

REM Change to the bat directory where build.bat is located
cd /d "%~dp0"

REM Compile the C program
call build.bat

REM Check if the compilation was successful
IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)

REM Change to the project root directory
cd /d "%~dp0..\..\"

REM Run the program with the specified input file
out\assignment2.o "resources\scheduler-simulation\scenario9.csv" 2 "resources\schemas\memory_schema_1.csv" 0 0 > out\scheduler-simulation\scenario9.csv

REM Display the contents of the output file
type out\scheduler-simulation\scenario9.csv
