#############################################################################################################
#
#
#   =================    ---------------------------------------
#   verbexx_implib.mk -- included makefile with import libraries (for use with Windows nmake)
#   =================    ---------------------------------------
#
#    note: also defines other global equates 
#
#
#
#############################################################################################################
 

STACK_SIZE = 2000000000
CFLAGS_ALWAYS = /EHa /MD /O2 /Zm256 /std:c++latest  

################### Windows import libraries ####################

IMP_LIBS = \
advapi32.lib \
comdlg32.lib \
D2d1.lib \
delayimp.lib \
Dwrite.lib \
DXGI.lib \
gdi32.lib \
gdiplus.lib \
kernel32.lib \
odbc32.lib \
odbccp32.lib \
ole32.lib \
oleaut32.lib \
shell32.lib \
user32.lib \
uuid.lib \
WindowsCodecs.lib \
winspool.lib \
Winmm.lib \
libfftw3-3.lib  


################################################################################################################################  