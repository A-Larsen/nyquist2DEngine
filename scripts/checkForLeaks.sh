if [ -z "$1" ]; then
    echo "no file to check"
else
    valgrind --leak-check=full  ./nyquist2DEngine "$1"
fi

