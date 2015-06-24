import cv2

img = cv2.imread("qrcode_simple.png")

# convert to greyscale
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# coverting to binary - threshold
ret,thresh1 = cv2.threshold(img,127,255,cv2.THRESH_BINARY)

# find contours, http://opencv-python-tutroals.readthedocs.org/en/latest/py_tutorials/py_imgproc/py_contours/py_contours_begin/py_contours_begin.html
image, contours, hierarchy = cv2.findContours(thresh1,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)
#RETR_TREE: read heirarchies

count=0
for cnt in contours:
    perimeter = cv2.arcLength(cnt,True)
    print cnt
    print(perimeter)
    count+=1

print(count)

cv2.imshow('squares', image)

cv2.waitKey(0)

cv2.destroyAllWindows()
