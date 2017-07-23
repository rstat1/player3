export STEAMLINK_SDK_PATH=$HOME/Apps/third_party/steamlink-sdk/
clear && ./build/build.py
sshpass -e scp Debug-armv7l/player3 root@192.168.1.14:/home/apps/streamlink/
sshpass -e scp Debug-armv7l/ui.asar root@192.168.1.14:/home/apps/streamlink/
sshpass -e scp Debug-armv7l/lib/libexternal.so root@192.168.1.14:/home/apps/streamlink/lib
#Debug-armv7l/app/streamlinkclient.dll
