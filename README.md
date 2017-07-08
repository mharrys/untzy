# Untzy
A very basic music player.

## Packages
Install development packages for Qt5 (>= 5.6.0) and GStreamer 1.0.

## Support
Install gst-plugins-ugly for Mp3 support.

## Build
From root directory:

    $ mkdir bin
    $ cd bin
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make -j8

## Run all tests
From build directory:

    $ ./test/run_all_tests

## Run Untzy
From build directory:

    $ ./untzy
