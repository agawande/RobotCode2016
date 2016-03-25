#! /usr/bin/python

import cv2
from cv2 import imread
from cv2 import cvtColor

img = imread("blocks.png")

cvtImg = cvtColor(img,cv2.COLOR_BRG2HSV)

px1 = cvtImg[60,420]
px2 = cvtImg[263,436]
px3 = cvtImg[275,477]
px4 = cvtImg[234,442]
px5 = cvtImg[390,482]

print "--- YELLOW ---"

print px1
print px2
print px3
print px4
print px5

print "--- GREEN ---"

px11 = cvtImg[39,269]
px22 = cvtImg[81,269]
px33 = cvtImg[166,271]
px44 = cvtImg[217,183]
px55 = cvtImg[236,229]
px66 = cvtImg[91,35]
px77 = cvtImg[222,383]
px88 = cvtImg[441,245]

print px11
print px22
print px33
print px44
print px55
print px66
print px77
print px88
