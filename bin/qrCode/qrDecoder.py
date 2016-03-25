#! /usr/bin/python

import qrtools
qr = qrtools.QR()
qr.decode("FlippedVert.png")
print qr.data

