@echo off

pushd ..\build

REM -wd4005 stänger av varning om redefention av APIENTRY Från glad biblioteket
cl /MD -nologo -Zi -wd4005 ..\src\cj_GL_GLFW.cpp user32.lib gdi32.lib opengl32.lib glfw3.lib shell32.lib

popd

copy /Y vertexShader.glsl ..\build
copy /Y fragmentShader.glsl ..\build


del *.un~ *.cpp~ *.h~ *.bat~ *.glsl~
