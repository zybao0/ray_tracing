echo off
echo #pragma once>all_texture.h
for %%f in (*.h) do (if not %%f==all_texture.h echo #include"texture/%%f" >>all_texture.h)
move all_texture.h ../