import cv2
import numpy as np
import time
import os

def main():
    cap = cv2.VideoCapture(0)
    print "carptue: ", cap.isOpened()
    detect=False

    while(True):
        ret, img = cap.read()

        cv2.imwrite('qrcode.jpg', img, [int(cv2.IMWRITE_JPEG_QUALITY), 90])

        os.system("python readQR.py")

        time.sleep(2)

if __name__ == "__main__":
    main()
