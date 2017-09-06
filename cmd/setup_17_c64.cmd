@ECHO *************************************************************************************************************************************************************************************************************************************************************
@ECHO * 
@ECHO *   setup_17_c64.cmd -- Set up x64 C++ compiler (VS2017)
@ECHO *
@ECHO *     -- Boost  1.64        (installed in C:\local directory)
@ECHO *     -- Python 3.6.2       (installed in c:\local\Python36-32)   
@ECHO *     -- fftw   3.3.4       (installed in C:\local directory)
@ECHO *
@ECHO ************************************************************************************************************************************************************************************************************************************************************* 
::::::::::
@set    boost_path=C:\local\boost_1_64_0\lib64-msvc-14.1
@set boost_include=C:\local\boost_1_64_0
@set     boost_lib=C:\local\boost_1_64_0\lib64-msvc-14.1
::::
@set       py_path=C:\local\Python36-32
::::
@set     fftw_path=C:\local\fftw_3_3_4\dll
@set      fftw_lib=C:\local\fftw_3_3_4\lib
@set  fftw_include=C:\local\fftw_3_3_4\h
::::::::::
@set  program_path=E:\Data01\program\exe;E:\Data01\program\cmd;E:\Data01\program\dll
::::::::::
::::::::::
::::::::::
cd \
C:
chcp 1252
::set VSCMD_DEBUG=3
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
::::::::::
@set PATH=%PATH%;%boost_path%;%py_path%;%fftw_path%;%program_path%
@set INCLUDE=%INCLUDE%;%boost_include%;%fftw_include%
@set LIB=%LIB%;%boost_lib%;%fftw_lib%
::::::::::
@set VERBEXX_IMBED_PATH=E:\Data01\program\verbexx\txt\
::::::::::
@echo =============================================================================================================================================================================================================================================================
@echo PATH = 
@echo %PATH%
@echo -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@echo PATHEXT = 
@echo %PATHEXT%
@echo -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@echo LIBPATH = 
@echo %LIBPATH%
@echo -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@echo LIB =
@echo %LIB%
@echo -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@echo INCLUDE = 
@echo %INCLUDE%
@echo -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@echo VERBEXX_IMBED_PATH =
@echo %VERBEXX_IMBED_PATH%
@echo =============================================================================================================================================================================================================================================================
::::::::::