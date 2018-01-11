BUILDTYPE=$1

if [$1 == '' ]; then
	BUILDTYPE='player3'
fi
export STEAMLINK_SDK_PATH=$HOME/Apps/third_party/steamlink-sdk/
echo "Build player3"
clear && ./build/build.py
echo "Copy player3 to steam link"
sshpass -e scp Debug-armv7l/player3 root@192.168.1.6:/home/apps/$BUILDTYPE/
echo "Copy UI files to steam link"
sshpass -e scp src/Home.uism root@192.168.1.6:/home/apps/$BUILDTYPE/
sshpass -e scp src/Activation.uism root@192.168.1.6:/home/apps/$BUILDTYPE/
sshpass -e scp src/ChatUI.uism root@192.168.1.6:/home/apps/$BUILDTYPE/
sshpass -e scp src/StreamIsLive.uism root@192.168.1.6:/home/apps/$BUILDTYPE/
sshpass -e scp Debug-armv7l/google-controller.png root@192.168.1.6:/home/apps/$BUILDTYPE/
#sshpass -e scp Debug-armv7l/lib/libexternal.so root@192.168.1.14:/home/apps/streamlink/lib
#Debug-armv7l/app/streamlinkclient.dll
