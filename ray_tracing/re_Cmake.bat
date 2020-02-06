echo off
rmdir /Q/S cmakefiles
rmdir /Q/S lib
del /Q/S cmake_install.cmake
del /Q/S Makefile
del /Q/S CMakeCache.txt
for /d /r %%f in (*) do (
	rmdir /Q/S %%f\cmakefiles
	rmdir /Q/S %%f\lib
	del /Q/S %%f\cmake_install.cmake
	del /Q/S %%f\Makefile
	del /Q/S %%f\CMakeCache.txt
	del /Q/S %%ray_tracing.exe
	)
for /d /r %%t in (*.bat) do (
	cd %%t if not %%t==re_Cmake.bat start %%t\get_all.bat)