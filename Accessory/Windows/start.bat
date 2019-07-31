@echo off
SET ROOT_PATH=%~dp0
SET PATH=%ROOT_PATH%Libraries\CPP\Nulstar\0.1.0;%ROOT_PATH%Libraries\CPP\Qt\5.12.3;%PATH%
call %ROOT_PATH%Modules\Nulstar\Nulstar\0.1.0\Nulstar.exe
