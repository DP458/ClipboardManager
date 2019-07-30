@echo off
msbuild /nologo ..\..\src\ClipboardManager.sln /t:Rebuild /p:"Configuration=Debug" /p:"Platform=ARM64"
