echo off
echo #pragma once>all_pdf.h
for %%f in (*.h) do (if not %%f==all_pdf.h echo #include"pdf/%%f" >>all_pdf.h)
move all_pdf.h ../