BUILDTYPE=$1

if [$1 == '' ]; then
	BUILDTYPE='player3'
fi
export STEAMLINK_SDK_PATH=$HOME/code/third_party/steamlink-sdk/
echo "Build player3"
clear && ./build/build.py
echo "Copy player3 to steam link"
sshpass -e scp Debug-armv7l/player3 root@192.168.1.64:/home/apps/$BUILDTYPE/
echo "Copy UI files to steam link"
sshpass -e scp src/ui/Home.uism root@192.168.1.64:/home/apps/$BUILDTYPE/
sshpass -e scp src/ui/Activation.uism root@192.168.1.64:/home/apps/$BUILDTYPE/
sshpass -e scp src/ui/ChatUI.uism root@192.168.1.64:/home/apps/$BUILDTYPE/
sshpass -e scp src/ui/StreamIsLive.uism root@192.168.1.64:/home/apps/$BUILDTYPE/
sshpass -e scp src/ui/Connecting.uism root@192.168.1.64:/home/apps/$BUILDTYPE/
sshpass -e scp Debug-armv7l/google-controller.png root@192.168.1.64:/home/apps/$BUILDTYPE/
sshpass -e scp build/resources/player3.sh root@192.168.1.64:/home/apps/$BUILDTYPE/