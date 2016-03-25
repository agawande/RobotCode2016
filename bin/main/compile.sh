#!/bin/bash

# Add /home/pi/local/lib to /etc/ld.so.conf, then sudo ldconfig
g++ $1 -I/home/pi/local/include/ -L/home/pi/local/lib -lwiringPi -lraspicam_still_robot_cv -lmmal -lmmal_core -lmmal_util -I/home/pi/Downloads/raspicam-0.1.3/ -pthread -I/home/pi/Downloads/raspicam-0.1.3/src -I/home/pi/Downloads/raspicam-0.1.3/src/private_still -lraspicam -L/opt/vc/lib -lmmal -lmmal_core -lmmal_util -lopencv_imgproc -lopencv_imgproc -lopencv_highgui -lopencv_core
