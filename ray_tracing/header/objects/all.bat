echo off
echo #pragma once>all_objects.h
for %%f in (*.h) do (if not %%f==all_objects.h echo #include"objects/%%f" >>all_objects.h)
move all_objects.h ../