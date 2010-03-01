echo off
xcopy /E /I libqwerk \OpenEmbedded\src\libqwerk
IF %errorlevel% ==1 goto :error
xcopy /E /I examples \OpenEmbedded\src\examples
IF %errorlevel% ==1 goto :error
cd \OpenEmbedded\src\libqwerk
set WINDOWS_MACHINE=yes
\OpenEmbedded\make\bin\make install
IF %errorlevel% ==1 goto :error
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

