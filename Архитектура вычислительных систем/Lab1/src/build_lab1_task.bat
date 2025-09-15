del lab1_task.exe
set masm32_path=c:/masm32
%masm32_path%\bin\ml /c /coff /I "%masm32_path%/include" lab1_task.asm
%masm32_path%\bin\link /SUBSYSTEM:CONSOLE /LIBPATH:%masm32_path%/lib lab1_task.obj
pause
lab1_task.exe
pause
