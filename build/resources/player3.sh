export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/lib/pulseaudio:${PWD}/"
exec ./player3
if [ -f player3-update ]; then
    rm player3
    mv player3-update player3
fi