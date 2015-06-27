import cv2
import numpy as np
import time

img = cv2.imread("qr.png")
img = cv2.imread("qrcode_simple.png")
img = cv2.imread("qrcode.jpg")
img = cv2.imread("qrcode2.jpg")

# convert to greyscale
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# coverting to binary - threshold
ret,thresh1 = cv2.threshold(img,127,255,cv2.THRESH_BINARY)

# Canny is an edge detection algorithm, http://opencv-python-tutroals.readthedocs.org/en/latest/py_tutorials/py_imgproc/py_canny/py_canny.html
canny=cv2.Canny(thresh1, 100, 200)

# find contours, http://opencv-python-tutroals.readthedocs.org/en/latest/py_tutorials/py_imgproc/py_contours/py_contours_begin/py_contours_begin.html
image, contours, hierarchy = cv2.findContours(canny,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
#RETR_TREE: read heirarchies

print hierarchy

qrSquares = []
count=0
for cnt in contours:
    if count+5 == len(contours):
        break

    # need to find those contours which are five level deep to identify the three squares on the qrcode

    # 1st = 0 from numpy, count=contour number, 2=child of the contour
    child = hierarchy[0][count][2]
    gchild = hierarchy[0][count+1][2]
    g2child = hierarchy[0][count+2][2]
    g3child = hierarchy[0][count+3][2]
    g4child = hierarchy[0][count+4][2]
    g5child = hierarchy[0][count+5][2]

    # make sure that fifth contour is childless
    if g5child == -1:
        # if child hierarchy is legit
        if child+1 == gchild and gchild+1 == g2child and g2child+1 == g3child and g3child+1 == g4child:
            print child
            print gchild
            print g2child
            print g3child
            print g4child
            print g5child
            qrSquares.append(cnt)
    count+=1

cv2.drawContours( img, qrSquares, -1, (0, 255, 0), 30 )

#print(count)

image = cv2.resize(img, (0,0), fx=0.20, fy=0.20)

cv2.imshow('squares', image)

cv2.waitKey(0)

cv2.destroyAllWindows()
