echo off
copy install.sh c:\cygwin\tmp
xcopy /E /I libqwerk c:\cygwin\tmp\libqwerk
xcopy /E /I examples c:\cygwin\tmp\examples
c:
cd \cygwin\bin
bash --login -i -c "cd /tmp; ./install.sh"
pause
del /F /Q c:\cygwin\tmp\install.sh
del /F /S /Q c:\cygwin\tmp\libqwerk
del /F /S /Q c:\cygwin\tmp\examples

