require ${PN}.inc

PR = "r1"

SRC_URI = "${HANDHELDS_CVS};tag=${TAG};module=opie/noncore/applets/pyquicklaunch \
           ${HANDHELDS_CVS};tag=${TAG};module=opie/pics \
           ${HANDHELDS_CVS};tag=${TAG};module=opie/apps"
