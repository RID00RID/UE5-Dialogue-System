del *.sln
rmdir /s /q .vs
rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q DerivedDataCache


set MyFullPath="%cd%\UE5_DialogueSystem"
set MyUVS="C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"

IF EXIST %MyUVS% ( 

%MyUVS% /projectfiles %MyFullPath%.uproject

) 
