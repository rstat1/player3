export STEAMLINK_SDK_PATH=$HOME/Apps/third_party/steamlink-sdk/
clear && ./build/build.py && sshpass -e scp Debug-armv7l/player3 root@192.168.1.14:/home/apps/streamlink/
#Debug-armv7l/app/streamlinkclient.dll
