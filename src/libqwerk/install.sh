if [ $(id -u) = "0" ]; then
	/usr/local/arm/make/make install	
else
	echo "script must be run as superuser or root, try: 'sudo ./install.sh'"
fi

