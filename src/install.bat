echo off
xcopy /E /I libqwerk \cygwin\usr\local\terkos\arm\src\libqwerk
IF %errorlevel%==1 goto :error
xcopy /E /I examples \cygwin\usr\local\terkos\arm\src\examples 
IF ERRORLEVEL 1 goto :error
cd \cygwin\usr\local\terkos\arm\src\libqwerk
set WINDOWS_MACHINE=yes
set PATH=%PATH%;C:\cygwin\bin
make install
IF ERRORLEVEL 1 goto :error
echo.
echo.
echo Installation successful!
pause
goto :end
:error
echo.
echo.
echo There was an error during installation!
pause
:end

