@echo off
If a%1% == ajp (
	:jp
	taskkill /im keppen_jp.exe /F /T 1>nul 2>nul
	gcc -Wall -Wextra -O3 -flto -DJP main.c screen.c -o keppen_jp.exe
	set built=jp
) else if a%1% == aus (
	:us
	taskkill /im keppen_us.exe /F /T 1>nul 2>nul
	gcc -Wall -Wextra -O3 -flto main.c screen.c -o keppen_us.exe
	set built=us
) else if a%1% == a (
	taskkill /im keppen_us.exe /F /T 1>nul 2>nul
	taskkill /im keppen_jp.exe /F /T 1>nul 2>nul
	gcc -Wall -Wextra -O3 -flto -DJP main.c screen.c -o keppen_jp.exe
	gcc -Wall -Wextra -O3 -flto main.c screen.c -o keppen_us.exe
	set built=jpus
) else (
	echo "Invalid argument : %1%"
	exit
)

If ERRORLEVEL 1 (
	pause
) else if %built% == jp (
	start keppen_jp.exe
) else if %built% == us (
	start keppen_us.exe
) else if %built% == jpus (
    start keppen_jp.exe
	start keppen_us.exe
)