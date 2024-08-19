@echo off
setlocal
set INSTALL_DIR=%cd%\py_installer
set PYTHON_INSTALLER=%INSTALL_DIR%\python-3.7.6-amd64.exe
mkdir %INSTALL_DIR%
echo Descargando Python...
powershell -Command "Invoke-WebRequest -Uri https://www.python.org/ftp/python/3.7.6/python-3.7.6-amd64.exe -OutFile %PYTHON_INSTALLER%"
echo Instalando Python...
%PYTHON_INSTALLER% /quiet InstallAllUsers=1 PrependPath=1
timeout /t 30
set PYTHON_DIR=%cd%\py_installer\Python37
set PATH=%PYTHON_DIR%;%PYTHON_DIR%\Scripts;%PATH%
echo Verificando Python...
python --version
echo Instalando Flask y Pygame...
python -m pip install --upgrade pip --user
python -m pip install flask pygame --user
echo Ejecutando el servidor...
cd server
python wifibocina.py
pause
