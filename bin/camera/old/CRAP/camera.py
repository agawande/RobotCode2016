#! /usr/bin/python

from subprocess import call

def getImage( outFile ):
   "This captures an image using one of the two connected cameras"

   #cmd = "raspistill -h 600 -w 600 -hf -vf -o " + outFile + " -e png"
   #cmd = "raspistill -awb flash -ifx blur -br 55 -mm spot -t 0 -k -h 600 -w 600 -hf -vf -o " + outFile + " -e png"
   cmd = "raspistill -ifx blur -br 55 -mm spot -t 0 -k -h 600 -w 600 -hf -vf -o " + outFile + " -e png"

   call(cmd,shell=True)
