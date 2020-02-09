echo off
echo #pragma once>all_material.h
for %%f in (*.h) do (if not %%f==all_material.h echo #include"material/%%f" >>all_material.h)
move all_material.h ../