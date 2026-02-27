
path=%CD%\devkitadv\bin

gcc -c -O3 -mthumb main.c 
gcc -mthumb -o main.elf main.o -lm
pause
objcopy -O binary main.elf main.gba
pause

del main.elf del main.o