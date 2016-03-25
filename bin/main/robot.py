#!/usr/bin/python

import sysv_ipc as ipc
from time import sleep
from config import *

shm = ipc.SharedMemory(SHARED_MEMORY_KEY, flags = SHARED_MEMORY_FLAGS, size = SHARED_MEMORY_SIZE)

shm.attach()

i = 1

while True:
   print('hi')
   shm.write(str(i))
   sleep(2)
   i += 1




