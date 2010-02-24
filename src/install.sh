if [ $(id -u) = "0" ]; then
    cp -r libqwerk /usr/local/terkos/arm/src
    cp -r examples /usr/local/terkos/arm/src
    cd /usr/local/terkos/arm/src/libqwerk
    /usr/local/arm/make/make install
else
    echo "script must be run as superuser or root, try: 'sudo ./install.sh'"
fi
