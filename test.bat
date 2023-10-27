@echo off
echo %1
CALL CMD /c ""C:\avrdude-v7.2-windows-x64\avrdude.exe"" -c usbasp -p atmega328p -D -U flash:w:C:/Users/Augusto/MPLABXProjects/lmd.X/dist/default/production/lmd.X.production.hex:i
