cmake_minimum_required (VERSION 2.6)
project(ray_tracing)
execute_process(COMMAND all.bat WORKING_DIRECTORY <./header/objects>)
ADD_SUBDIRECTORY(src)