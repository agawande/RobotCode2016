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

void Motion::moveToCoordinate()
{
  Complex *temp = courseCoor.next();
  int newX = temp->getX();
  int newY = temp->getY();
  int newOrient = temp->getOrient();

  int currentX = currentPosition.getX();
  int currentY = currentPosition.getY();
  int currentOrient = currentPosition.getOrient();

  if (currentOrient  != newOrient) {
      //SpinCommand
      //Counter clockwise for negative
      //Clockwise for positive
  } else {
      if(course == 0) {
	  switch(newOrient)
	  {
             //Oriented North
	     case 0:
                   if (newX > currentX) {
                        newX = newX - currentX;
			//moveWest(newX)
                   } else {
			newX = currentX - newX;
			//moveEast(newX)
		   }

     		   if (newY > currentY) {
			newY = newY - currentY;
			//moveNorth(newY)
		   } else {
			newY = currentY - newY;
			//moveSouth(newY)
		   }
	  	   break; //end of case -

 	     //Oriented East
	     case 1:
       		   if (newX > currentX) {
			newX = newX - currentX;
			//moveSouth(newX)
		   } else {
			newX = currentX - newX;
			//moveNorth(newX)
		   }

  		   if (newY > currentY)	{
			newY = newY - currentY;
			//moveWest(newY)
		   } else {
			newY = currentY - newY;
			//moveEast(newY)
		   }
                 break; //end of case 1

	    case 2:
	           if (newX > currentX) {
	                 newX = newX - currentX;
	                 //moveEast(newX)
	           } else {
	                 newX = currentX - newX;
	                 //moveWest(newX)
	           }

               	   if (newY > currentY) {
	                 newY = newY - currentY;
	                 //moveSouth(newY)
	           } else {
	                 newY = currentY - newY;
	                 //moveNorth(newY)
	           }
                   break;  //end of case 2
   	    case 3:
                   if (newX > currentX) {
	                 newX = newX - currentX;
	                 //moveNorth(newX)
	           } else {
	                 newX = currentX - newX;
	                 //moveSouth(newX)
	          }

	          if (newY > currentY) {
	                 newY = newY - currentY;
	                 //moveEast(newY)
	          } else {
	                 newY = currentY - newY;
                         //moveWest(newY)
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
			//moveEast(newX)
		  } else {
			newX = currentX - newX;
		 	//moveWast(newX)
		  }

                  if (newY > currentY) {
			newY = newY - currentY;
	        	//moveNorth(newY)
		  } else {
			newY = currentY - newY;
			//moveSouth(newY)
		  }
	          break; //end of case 0

				//Oriented East
	     case 1:
 		  if (newX > currentX) 	{
			newX = newX - currentX;
			//moveNorth(newX)
		  } else {
			newX = currentX - newX;
			//moveSouth(newX)
		  }

		  if (newY > currentY) {
			newY = newY - currentY;
			//moveWest(newY)
		  } else {
			newY = currentY - newY;
			//moveEast(newY)
		  }
 		  break; //end of case 1

	     case 2:
	          if (newX > currentX) {
	                newX = newX - currentX;
	                //moveWest(newX)
	          } else {
	                newX = currentX - newX;
	                //moveEast(newX)
	          }

               	  if (newY > currentY) {
	                newY = newY - currentY;
	                //moveSouth(newY)
	      	  } else {
	                newY = currentY - newY;
	                //moveNorth(newY)
	          }
	          break;  //end of case 2

	      case 3:
	          if (newX > currentX) {
	                newX = newX - currentX;
	                //moveSouth(newX)
	          } else {
                   	newX = currentX - newX;
                       	//moveNorth(newX)
                  }

                  if (newY > currentY) {
	                newY = newY - currentY;
	                //moveEast(newY)
	          } else {
	                newY = currentY - newY;
	                //moveWest(newY)
	          }
                  break;  //end of case 3

	      default: break;
	}  //end of switch
      } //end ocourse 1
   } //end of else
} //end of moveToCoordinate
