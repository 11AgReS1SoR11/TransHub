# Сборка для Windows 10.

Сборка состоит из двух этапов:
1. Установка переменных среды (пути до средств сборки и важных директорий). Важно перепроверить корректность путей и в случае необходимости заменить на свои.
2. Сборка.

## Сборка с MSVC2015 x64

QMAKE - путь до qmake.exe под наш компилятор
MAKE - путь до jom.exe
CONFIGURE_VC - путь до vcvarsall.bat (Microsoft Visual Studio)
QTDIR - путь до "корневой" папки с файлами QT под наш компилятор
DEPLOY - путь до файла windeployqt.exe под наш компилятор


Выполнить:

```
set QMAKE=C:\Qt\Qt5.9.6\5.9.6\msvc2015_64\bin\qmake.exe
set MAKE=C:\Qt\Qt5.9.6\Tools\QtCreator\bin\jom.exe
set CONFIGURE_VC=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat
set QTDIR=C:\Qt\Qt5.9.6\5.9.6\msvc2015_64
set DEPLOY=%QTDIR%\bin\windeployqt.exe

cd ComponentsCore5alpha
mkdir ComponentsCore5alpha-build
cd ComponentsCore5alpha-build
"%CONFIGURE_VC%" amd64
"%QMAKE%" ..\ComponentsCore5alpha\ComponentsCore5alpha.pro -spec win32-msvc
"%MAKE%"
```

При необходимости, можно установить библиотеку в систему, дополнительно выполнив:

```
"%MAKE%" install
"%DEPLOY%" "C:\Program Files\ComponentsCore5alpha\lib\ComponentsCore5alpha1.dll"
```

## Сборка с mingw x86

Выполнить:

PATH - добавляем пути до директорий с .a и .lib файлами QT под наш комилятор, а так же директорию с исполняемым файлом gcc/g++
QMAKE - путь до qmake.exe под наш компилятор
MAKE - путь до make.exe
QTDIR - путь до "корневой" папки с файлами QT под наш компилятор
DEPLOY - путь до файла windeployqt.exe под наш компилятор

```
set PATH=%PATH%;C:\Qt\Qt5.9.6\Tools\mingw530_32\bin;C:\Qt\Qt5.9.6\5.9.6\mingw53_32\bin;
set QMAKE=C:\Qt\Qt5.9.6\5.9.6\mingw53_32\bin\qmake.exe
set MAKE=C:\Program Files (x86)\GnuWin32\bin\make.exe
set QTDIR=C:\Qt\Qt5.9.6\5.9.6\mingw53_32
set DEPLOY=%QTDIR%\bin\windeployqt.exe

cd ComponentsCore5
mkdir ComponentsCore5alpha-build
cd ComponentsCore5alpha-build
"%QMAKE%" ..\ComponentsCore5alpha\ComponentsCore5alpha.pro
"%MAKE%"
```

При необходимости, можно установить библиотеку в систему, дополнительно выполнив:

```
"%MAKE%" install
"%DEPLOY%" "C:\Program Files\ComponentsCore5alpha\lib\ComponentsCore5alpha1.dll"
```
