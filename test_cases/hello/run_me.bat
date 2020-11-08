@echo off
pushd %~dp0
echo %cd%
echo Starting code testing script...
g++ -Wall -std=c++11 main.cpp -o program.exe 
IF %errorlevel% == 0 (
echo Compilation successful!
echo ##########################################################
program.exe < in.txt > out.txt
echo Please copy the below text and paste it on CodeDecker, 
echo or upload the text file named "out.txt"
echo ##########################################################
type out.txt
echo ##########################################################
)
pause
exit