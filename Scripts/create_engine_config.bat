@echo off
echo Creating config file in %AppData%/Robert Bennett/OneShotEngine
cd "%AppData%/Robert Bennett/OneShotEngine/"
echo ^<Configuration^> > Engine.config
echo    ^<VirtualPaths^> >> Engine.config
echo         ^<Root cvar="false" type="string"^>%~dp0..\^</Root^> >> Engine.config
echo         ^<Logs cvar="false" type="string"^>/Root/Logs^</Logs^> >> Engine.config
echo         ^<Geometry cvar="false" type="string"^>/Root/Geometry^</Geometry^> >> Engine.config
echo         ^<Shaders cvar="false" type="string"^>/Root/Shaders^</Shaders^> >> Engine.config
echo     ^</VirtualPaths^> >> Engine.config
echo     ^<Window^> >> Engine.config
echo         ^<Width cvar="true" type="int"^>1920^</Width^> >> Engine.config
echo         ^<Height cvar="true" type="int"^>1080^</Height^> >> Engine.config
echo     ^</Window^> >> Engine.config
echo ^</Configuration^> >> Engine.config

PAUSE