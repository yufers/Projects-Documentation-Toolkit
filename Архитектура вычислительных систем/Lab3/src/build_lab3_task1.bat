del lab3_task1.exe
set masm32_path=c:/masm32
%masm32_path%\bin\ml /c /coff /I "%masm32_path%/include" lab3_task1.asm
%masm32_path%\bin\link /SUBSYSTEM:CONSOLE /LIBPATH:%masm32_path%/lib lab3_task1.obj
pause
lab3_task1.exe
pause
