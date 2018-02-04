export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/lib/pulseaudio:${PWD}/"
if [ -f player3-update ]; then
    echo "Update found..."
    rm player3
    mv player3-update player3
    chmod +x player3
fi
exec ./player3
