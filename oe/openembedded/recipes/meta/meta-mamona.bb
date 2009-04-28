DESCRIPTION = "Meta package for Mamona"
LICENSE = "MIT"
PR = "r2"

inherit meta
EXCLUDE_FROM_WORLD = "1"

RDEPENDS = " \
task-mamona-base \
task-mamona \
task-mamona-devel \
task-mamona-sdk \
task-mamona-sdk-noemu \
task-mamona-wm \
task-mamona-python \
task-mamona-systemtap \
coreutils \
cpio \
dbus \
diffstat \
dnsmasq \
dosfstools \
e2fsprogs \
expat \
findutils \
firefox \
fontconfig \
gconf \
glib-2.0 \
gnome-common \
gnome-vfs \
groff \
gstreamer \
gtk+ \
gtk+-doc \
gtk-engines-dev \
ifupdown \
initscripts \
intltool \
libart-lgpl \
libdbi \
libelf \
libfontenc \
libmatchbox \
liboil \
libpng \
libtelepathy \
libtool \
libusb \
libxau \
libxcursor \
libxdmcp \
libxext \
libxfixes \
libxfont \
libxi \
libxkbfile \
libxml2 \
libxpm \
libxrandr \
libxrender \
libxslt \
libxt \
libxtst \
libxv \
lsof \
makedev \
matchbox-wm \
midori \
mime-support \
module-init-tools \
mplayer-maemo \
ncurses \
net-tools \
netbase \
openobex \
portmap \
ppp \
procps \
qt4-x11-free \
quilt \
renderproto-dev \
samba \
sgml-common \
shared-mime-info \
slang \
sqlite \
sqlite3 \
sudo \
sysvinit \
telepathy-gabble \
telepathy-glib \
telepathy-mission-control \
tslib \
ttf-bitstream-vera \
udev \
util-linux \
vim \
wget \
xkbd \
xtrans-dev \
"