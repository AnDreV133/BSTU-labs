set masm32_path=c:\masm32
set file_name=lab2_l
del %file_name%.exe
%masm32_path%\bin\ml /c /coff /I "%masm32_path%\include" %file_name%.asm
%masm32_path%\bin\link /SUBSYSTEM:CONSOLE /LIBPATH:%masm32_path%\lib %file_name%.obj
%file_name%.exe
