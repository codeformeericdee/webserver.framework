@echo off

:START:

echo Enter "b" for build, or "r" for run.
echo -
set /p command=Type the command then press enter... : 

echo Your command is: %command%

if "%command%"=="b" (
    docker pull ubuntu
    powershell start-process "%CD%\Docker\Dockerstartscript.bat" -verb runas
    echo Press enter when Docker has finished starting.
    pause
	echo The image is being built. This can take a few minutes.
	docker build --no-cache=true ./Docker -t serverdev
	pause
	exit

) else if "%command%"=="r" (
	echo The container is being started. It will be accessible on 127.0.0.1:256. This can take a minute.
	echo.
	powershell -command docker run -p 127.0.0.1:256:256 --user=root -it -v "${pwd}:/root/env" --rm serverdev
	pause
	exit

) else (
	echo Invalid command. Try again.
	pause
	exit
)