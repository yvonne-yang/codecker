@echo off
pushd %~dp0
echo %cd%
echo Starting code testing script...
g++ -Wall -std=c++11 parser.cpp Shape.cpp -o parser.exe 
IF %errorlevel% == 0 (
echo Compilation successful!
echo ##########################################################
parser.exe < in.txt > out.txt
echo Please copy the below text and paste it on CodeDecker, 
echo or upload the text file named "out.txt"
echo ##########################################################
type out.txt
)
pause
exit