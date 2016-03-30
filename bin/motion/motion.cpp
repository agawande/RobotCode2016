#include "motion.hpp"

void Motion::setCourse(int myCourse)
{
  course = myCourse;
  processCourse();
}  //end of setCourse

void Motion::processCourse()
{
  courseCoor.setFileName(COURSE);
  if(course == 0) {
    tunnelCoor.setFileName(TUNNEL_0);
  } else {
    tunnelCoor.setFileName(TUNNEL_1);
  }
}  //end  of processCourse

void Motion::sendCmd(int command, int dist)
{
    i2cDispatcher.sendData(msgFmt.buildMessage(command, dist));
    msgFmt.updateKey();
    //wait for command to finish
    i2cDispatcher.checkForCompletion();
} //end of senCmd

void Motion::moveThroughTunnel()
{
  if(course == 0){
     sendCmd(MOTION_TUNNEL_0, 0);
  } else {
     sendCmd(MOTION_TUNNEL_1, 0);
  }

  //square up to barge
  double angle = laserProcessor.getWallAngle();
  if(angle > 0) {
     sendCmd(MOTION_SPIN_CW, angle);
  } else {
     sendCmd(MOTION_SPIN_ACW, angle);
  }

  //Initialize currentPosition
  //Turn and measure distance to side
  if(course == 0) {
    sendCmd(MOTION_SPIN_CW, 90);
  } else {
    sendCmd(MOTION_SPIN_ACW, 90);)
  }

  int x = laserProcessor.getDistance();

  //Face the barge again
  if(course == 0) {
    sendCmd(MOTION_SPIN_ACW, 90);
  } else {
    sendCmd(MOTION_SPIN_CW, 90);)
  }

  //Measure distance to the front
  int y = BARGE_LENGTH - (laserProcessor.getDistance() + BARGE_WIDTH) ;

  currentPosition.setX(x);
  currentPosition.setY(y);
  //index and orientation are intialized to zero

} //end of moveThroughTunnel

void Motion::moveToCoordinate()
{
  Coordinate *temp = courseCoor.next();
  int newX = temp->getX();
  int newY = temp->getY();
  int newOrient = temp->getOrient();

  int currentX = currentPosition.getX();
  int currentY = currentPosition.getY();
  int currentOrient = currentPosition.getOrient();

  //update current position
  currentPosition = *temp;

  if (currentOrient != newOrient) {
      if ( newOrient < 0 ) {
         sendCmd(MOTION_SPIN_CW, 90);
      } else if ( newOrient) {
         sendCmd(MOTION_SPIN_ACW, 90);
      }
  } else {
      if(course == 0) {
	  switch(newOrient)
	  {
             //Oriented North
	     case 0:
                   if (newX > currentX) {
                        newX = newX - currentX;
                        sendCmd(MOTION_WEST, newX);
                   } else {
			newX = currentX - newX;
                        sendCmd(MOTION_EAST, newX);
		   }

     		   if (newY > currentY) {
			newY = newY - currentY;
                        sendCmd(MOTION_NORTH, newY);
		   } else {
			newY = currentY - newY;
			sendCmd(MOTION_SOUTH, newY);
		   }
	  	   break; //end of case 0

 	     //Oriented East
	     case 1:
       		   if (newX > currentX) {
			newX = newX - currentX;
                        sendCmd(MOTION_SOUTH, newX);
		   } else {
			newX = currentX - newX;
                        sendCmd(MOTION_NORTH, newX);
		   }

  		   if (newY > currentY)	{
			newY = newY - currentY;
                        sendCmd(MOTION_WEST, newY);
		   } else {
			newY = currentY - newY;
                        sendCmd(MOTION_EAST, newY);
		   }
                 break; //end of case 1

	    case 2:
	           if (newX > currentX) {
	                 newX = newX - currentX;
                         sendCmd(MOTION_EAST, newX);
	           } else {
	                 newX = currentX - newX;
                         sendCmd(MOTION_WEST, newX);
	           }

               	   if (newY > currentY) {
	                 newY = newY - currentY;
                         sendCmd(MOTION_SOUTH, newY);
	           } else {
	                 newY = currentY - newY;
                         sendCmd(MOTION_NORTH, newY);
	           }
                   break;  //end of case 2
   	    case 3:
                   if (newX > currentX) {
	                 newX = newX - currentX;
                         sendCmd(MOTION_NORTH, newX);
	           } else {
	                 newX = currentX - newX;
                         sendCmd(MOTION_SOUTH, newX);
	          }

	          if (newY > currentY) {
	                 newY = newY - currentY;
                         sendCmd(MOTION_EAST, newY);
	          } else {
	                 newY = currentY - newY;
                         sendCmd(MOTION_WEST, newY);
	          }
		  break; //end of case 3

            default: break;
         }
   }  //end of if course == 0

   else if(course == 1) {
         switch(newOrient) {
	     //Oriented North
  	     case 0:
	          if (newX > currentX) {
			newX = newX - currentX;
                        sendCmd(MOTION_EAST, newX);
		  } else {
			newX = currentX - newX;
                        sendCmd(MOTION_WEST, newX);
		  }

                  if (newY > currentY) {
			newY = newY - currentY;
                        sendCmd(MOTION_NORTH, newY);
		  } else {
			newY = currentY - newY;
                        sendCmd(MOTION_SOUTH, newY);
		  }
	          break; //end of case 0

             //Oriented East
	     case 1:
 		  if (newX > currentX) 	{
			newX = newX - currentX;
                        sendCmd(MOTION_NORTH, newX);
		  } else {
			newX = currentX - newX;
			sendCmd(MOTION_SOUTH, newX);
		  }

		  if (newY > currentY) {
			newY = newY - currentY;
                        sendCmd(MOTION_WEST, newY);
		  } else {
			newY = currentY - newY;
                        sendCmd(MOTION_EAST, newY);
		  }
 		  break; //end of case 1

	     case 2:
	          if (newX > currentX) {
	                newX = newX - currentX;
                        sendCmd(MOTION_WEST, newX);
	          } else {
	                newX = currentX - newX;
                        sendCmd(MOTION_EAST, newX);
	          }

               	  if (newY > currentY) {
	                newY = newY - currentY;
                        sendCmd(MOTION_SOUTH, newY);
	      	  } else {
	                newY = currentY - newY;
                        sendCmd(MOTION_NORTH, newY);
	          }
	          break;  //end of case 2

	      case 3:
	          if (newX > currentX) {
	                newX = newX - currentX;
                        sendCmd(MOTION_SOUTH, newX);
	          } else {
                   	newX = currentX - newX;
                        sendCmd(MOTION_NORTH, newX);
                  }

                  if (newY > currentY) {
	                newY = newY - currentY;
                        sendCmd(MOTION_EAST, newY);
	          } else {
	                newY = currentY - newY;
                        sendCmd(MOTION_WEST, newY);
	          }
                  break;  //end of case 3

	      default: break;
	}  //end of switch
      } //end of course 1
   } //end of else
} //end of moveToCoordinate

void Motion::next()
{
  moveToCoordinate();
}

void Motion::nextBlock(int shouldAlignToBlock)
{
  //Move back and to the side
  if(course == 0) {
      inFrontOfNextBlock(MOTION_WEST);
  } else {
      inFrontOfNextBlock(MOTION_EAST);
  }

  if(shouldAlignToBlock == 1) {
      //Need to align

      //First become parallel
      double angleError = laserProcessor.getBlockAngle();
      if( angleError > 0 ) {
          sendCmd(MOTION_SPIN_CW, angleError);
      } else {
          sendCmd(MOTION_SPIN_ACW, angleError);
      }

      //Then correct alignment error
      int error = laserProcessor.getCorrectionDistance();
      //Center the block
      if(course == 0) {
          if(error > 0) {
             sendCmd(MOTION_WEST, error);
          } else {
             sendCmd(MOTION_EAST, -1*error);
          }
      } else {
          if(error > 0) {
             sendCmd(MOTION_EAST, error);
          } else {
             sendCmd(MOTION_WEST, -1*error);
          }
      }
  }

  //Move into the block
  sendCmd(MOTION_NORT, ALIGN_DIST);
}

void Motion::inFrontOfNextBlock(int direction)
{
  //Move back
  sendCmd(MOTION_SOUTH, ALIGN_DIST);

  //Move side
  sendCmd(direction, INCH);
}
