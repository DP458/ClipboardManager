@echo off
msbuild /nologo ..\..\src\ClipboardManager.sln /t:Rebuild /p:"Configuration=Release" /p:"Platform=x86"
