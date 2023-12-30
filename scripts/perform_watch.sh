while true; do
    clear
    if pidof $1 > /dev/null; then
        ps -p $(pidof $1) -o %cpu,%mem
    fi
    sleep 1.2
done
