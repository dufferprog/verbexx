@:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@::
@::  m_all -- make both verbex_base and verbex_imports
@::
@:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

nmake /nologo /fE:\Data01\program\verbexx\mk\verbexx_base.mk 

@IF %ERRORLEVEL% NEQ 0 ( GOTO end )

nmake /nologo /fE:\Data01\program\verbexx\mk\verbexx_imports.mk 

:end