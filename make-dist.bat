@echo off
echo Cleaning dist

rmdir /S /Q dist
mkdir dist\bin
mkdir dist\src
mkdir dist\src\src
mkdir dist\src\src\icons

echo Copying files

rem bin
copy readme.txt dist\bin
copy ReducePing.exe dist\bin

rem src
copy readme.txt dist\src
copy make-gcc.bat dist\src
copy make-dist.bat dist\src
copy src\manifest.xml dist\src\src
copy src\reduceping.c dist\src\src
copy src\reduceping.rc dist\src\src
copy src\resource.h dist\src\src
copy src\icons\16x16.ico dist\src\src\icons
copy src\icons\32x32.ico dist\src\src\icons
copy src\icons\48x48.ico dist\src\src\icons