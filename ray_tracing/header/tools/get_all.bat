echo off
echo #pragma once>all_tools.h
for %%f in (*.h) do (if not %%f==all_tools.h echo #include"tools/%%f" >>all_tools.h)
move all_tools.h ../