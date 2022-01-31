call "%UE4PATH%/Engine/Build/BatchFiles/RunUAT.bat" BuildCookRun -project=%cd%/navgen.uproject platform=Win64 -clientconfig=Development -build

call "%UE4PATH%/Engine/Binaries/Win64/UE4Editor" %cd%/navgen.uproject -ExecCmds="Automation RunTests NavMeshGen" -TestExit="Automation Test Queue Empty" -nosplash -nullrhi -log

timeout 10
