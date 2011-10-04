#!/bin/bash
set -e
if [ $(id -u) = "0" -o `uname -o` = "Cygwin" ]; then
    cp -r libqwerk /usr/local/terkos/arm/src
    cp -r examples /usr/local/terkos/arm/src
    cd /usr/local/terkos/arm/src/libqwerk
    make install
else
    echo "script must be run as superuser or root, try: 'sudo ./install.sh'"
fi

echo
echo "Installation was successful!"
