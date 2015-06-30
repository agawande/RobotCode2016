import cv2
import numpy as np
import time
import qrtools

img = cv2.imread("qr.png")
img = cv2.imread("qrcode_simple.png")
img = cv2.imread("qrcode.jpg")
#img = cv2.imread("qrcode2.jpg")
#img = cv2.imread("q3.jpg")

# convert to greyscale
img_orig = img

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

# Draw qr contours on original image
image = cv2.drawContours( img_orig, qrSquares, -1, (0, 255, 0), 5 )

#image = cv2.resize(img_orig, (0,0), fx=0.20, fy=0.20)

cv2.imshow('squares', image)

cv2.waitKey(0)

cv2.destroyAllWindows()


count=0
qrCentroid = {}
for cnt in qrSquares:
    M = cv2.moments(cnt)
    cx = int(M['m10']/M['m00'])
    cy = int(M['m01']/M['m00'])
    img = cv2.circle(img_orig,(cx,cy), 6, (0,0,255), -1)

    qrCentroid[count] = str(cx)+":"+str(cy)
    count+=1
    
# dist b/w two centroids = root((x-x1)^2 + (y-y1)^2)
dist01 = ((int(qrCentroid[0].split(":")[0]) - int(qrCentroid[1].split(":")[0]))**2 + (int(qrCentroid[0].split(":")[1]) - int(qrCentroid[1].split(":")[1]))**2)**0.5
dist02 = ((int(qrCentroid[0].split(":")[0]) - int(qrCentroid[2].split(":")[0]))**2 + (int(qrCentroid[0].split(":")[1]) - int(qrCentroid[2].split(":")[1]))**2)**0.5
dist12 = ((int(qrCentroid[1].split(":")[0]) - int(qrCentroid[2].split(":")[0]))**2 + (int(qrCentroid[1].split(":")[1]) - int(qrCentroid[2].split(":")[1]))**2)**0.5

print dist01
print dist02
print dist12

# determine which contour is topleft
if dist01 > dist02 and dist01 > dist12:
    topleft = 2
    a,b = 0,1
elif dist02 > dist01 and dist02 > dist01:
    topleft = 1
    a,b = 0,2
else:
    topeleft = 0
    a,b = 1,2

# center of longest line using
centerx=(int(qrCentroid[a].split(":")[0]) + int(qrCentroid[b].split(":")[0]))/2
centery=(int(qrCentroid[a].split(":")[1]) + int(qrCentroid[b].split(":")[1]))/2

print "center x: ", centerx
print "center y: ", centery

# just seeing where it actually marks it on the image
img = cv2.circle(img,(centerx,centery), 5, (0,0,255), -1)

# get slope of the longest line
slope = (int(qrCentroid[a].split(":")[1]) - int(qrCentroid[b].split(":")[1]))/(int(qrCentroid[a].split(":")[0]) - int(qrCentroid[b].split(":")[0]))

# get the slope of the perpendicular to the longest line using the center of the longest line and the centroid of the top left
m_Per = (centery - int(qrCentroid[topleft].split(":")[1]))/(centerx - int(qrCentroid[topleft].split(":")[0]))


if (slope > 0 and m_Per >=0) or (slope < 0 and m_Per < 0):
    right = a
    bottom = b
elif (slope < 0 and m_Per >=0) or (slope >= 0 and m_Per < 0):
    right = b
    bottom = a
else:  # else randomly assign, works for acute angles - i think
    right = a
    bottom = b

#img = cv2.circle(img,(int(qrCentroid[bottom].split(":")[0]), int(qrCentroid[bottom].split(":")[1])), 10, (0,0,255), -1)

# now we need to get the four corners of topleft, bottom, and right
count=0
for cnt in qrSquares:
    if count == topleft:
        cnt_top = cnt
    elif count == right:
        cnt_right = cnt
    elif count == bottom:
        cnt_bottom = cnt
    count+=1
#    print list(cnt)
#    for i in range (0, len(list(cnt))):
#        print list(cnt)[i][0][0], " ", list(cnt)[i][0][1]
#        print ""
    #print farthest_point()



#img = cv2.circle(img,(0, 0), 5, (0,0,255), -1)

cv2.imshow('centroid', img)

cv2.waitKey(0)

cv2.destroyAllWindows()

def farthest_point(cnt, c_x, c_y):
    #img = cv2.imread("qrcode.jpg")
    distlist = []
    max = 0
    for i in range (0, len(list(cnt))):
        cntx = int(list(cnt)[i][0][0])
        cnty = int(list(cnt)[i][0][1])
        dist = ((cntx-c_x)**2+(cnty-c_y)**2)**0.5
        if dist > max:
            max = dist
            cntx_max = cntx
            cnty_max = cnty
        distlist.append(dist)
    #img = cv2.circle(img,(cntx_max,cnty_max), 5, (0,0,255), -1)
    #cv2.imshow('hello'+str(cntx_max), img)
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()
    return int(cntx_max), int(cnty_max)

crx, cry = farthest_point(cnt_right, centerx, centery)
cbx, cby = farthest_point(cnt_bottom, centerx, centery)
ctx, cty = farthest_point(cnt_top, centerx, centery)

img = cv2.imread("qrcode.jpg")
pts1 = np.float32([[ctx,cty],[cbx,cby],[crx,cry]])
pts2 = np.float32([[0,0],[300,0],[0,300]])

M = cv2.getAffineTransform(pts1,pts2)

dst = cv2.warpAffine(img,M,(300,300))

cv2.imshow('hello', img)
cv2.imshow('dst', dst)
cv2.waitKey(0)
cv2.destroyAllWindows()


cv2.imwrite("hello.png",dst)

qr = qrtools.QR()
qr.decode("hello.png")
print("\nData:\n")
print(qr.data)
