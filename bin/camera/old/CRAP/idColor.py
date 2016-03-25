#! /usr/bin/python

# Module Imports
import cv2
import numpy as np

# Function Imports
from cv2 import imread        # Open a file
from cv2 import cvtColor      # Used for color conversion
from camera import getImage   # Used to capture an image from a camera

imageFile = "test_blur.png"
#imageFile = "test.png"

getImage(imageFile)

img = imread(imageFile)

cvtImg = cvtColor(img,cv2.COLOR_BGR2HSV)

hue = cvtImg[:,:,0]
saturation = cvtImg[:,:,1]
value = cvtImg[:,:,2]

np.savetxt('hue.txt', hue, fmt='%4i', delimiter=', ')
np.savetxt('saturation.txt', saturation, fmt='%4i', delimiter=', ')
np.savetxt('value.txt', value, fmt='%4i', delimiter=', ')


#lYellow = np.array([16,200,45])
lYellow = np.array([15,158,45])
uYellow = np.array([30,255,255])
lgYellow = np.array([17,25,204])
ugYellow = np.array([57,80,255])

lSUNgreen = np.array([0,25,0])
uSUNgreen = np.array([50,153,89])
lgreen = np.array([30,12,82])
ugreen = np.array([71,64,143])
#lgreen = np.array([18,51,0])
#ugreen = np.array([99,96,55])

#lblue = np.array([100,45,30])
lblue = np.array([90,45,30])
#ublue = np.array([110,255,255])
ublue = np.array([127,255,255])

lred = np.array([165,45,45])
ured = np.array([180,255,255])

#ynmask = cv2.inRange(cvtImg, lYellow, uYellow)
#ygmask = cv2.inRange(cvtImg, lgYellow, ugYellow)
ymask = cv2.inRange(cvtImg, lYellow, uYellow)
gmask = cv2.inRange(cvtImg, lgreen, ugreen)
bmask = cv2.inRange(cvtImg, lblue, ublue)
rmask = cv2.inRange(cvtImg, lred, ured)

#ymask = cv2.bitwise_or(ynmask, ygmask)


#yHSV = cv2.bitwise_and(cvtImg, cvtImg mask= ymask)

yRGB = cv2.bitwise_and(img, img, mask= ymask)

#gHSV = cv2.bitwise_and(cvtImg, cvtImg, mask= gmask)
gRGB = cv2.bitwise_and(img, img, mask= gmask)

#bHSV = cv2.bitwise_and(cvtImg, cvtImg, mask= bmask)
bRGB = cv2.bitwise_and(img, img, mask= bmask)

#rHSV = cv2.bitwise_and(cvtImg, cvtImg, mask= rmask)
rRGB = cv2.bitwise_and(img, img, mask= rmask)

#cv2.imshow('img',img)
#cv2.imshow('mask',mask)

#cv2.imshow('yHSV',yHSV)
cv2.imshow('yRGB',yRGB)

#cv2.imshow('gHSV',gHSV)
cv2.imshow('gRGB',gRGB)

#cv2.imshow('bHSV',bHSV)
cv2.imshow('bRGB',bRGB)

#cv2.imshow('rHSV',rHSV)
cv2.imshow('rRGB',rRGB)

cv2.waitKey(0)

#px1 = cvtImg[259,399]

#px2 = cvtImg[240,518]
#px3 = cvtImg[227,221]
#
#px4 = cvtImg[481,331]
#px5 = cvtImg[453,531]
#px6 = cvtImg[409,153]
#
#print "--- GREEN ---"
#
#print px1
#print px2
#print px3
#
#print "--- YELLOW ---"
#
#print px4
#print px5
#print px6
#
