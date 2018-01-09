BUILDTYPE=$1

if [$1 == '' ]; then
	BUILDTYPE='player3'
fi
export STEAMLINK_SDK_PATH=$HOME/Apps/third_party/steamlink-sdk/
clear && ./build/build.py
sshpass -e scp Debug-armv7l/player3 root@192.168.1.6:/home/apps/$BUILDTYPE/
sshpass -e scp Debug-armv7l/Activation.uism root@192.168.1.6:/home/apps/$BUILDTYPE/
sshpass -e scp Debug-armv7l/google-controller.png root@192.168.1.6:/home/apps/$BUILDTYPE/
#sshpass -e scp Debug-armv7l/lib/libexternal.so root@192.168.1.14:/home/apps/streamlink/lib
#Debug-armv7l/app/streamlinkclient.dll
