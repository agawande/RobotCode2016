import sysv_ipc
import time
# Create shared memory object
memory = sysv_ipc.SharedMemory(60)

finished = 'completed'
invalid = 'invalidop'

while(1):
	#while(1):
	nb = raw_input('Choose a number: ')
	print nb
	check = memory.read()
	#if (nb not in check):		
	# Read value from shared memory
	memory.write(nb)
	
	while(1):
		time.sleep(1)
		check = memory.read()

		if (finished in check):
			print "Completed!"
			print " "
			break
		
		if (invalid in check):
			print "Invalid Operation!!"
			print " "
			break
			
			


