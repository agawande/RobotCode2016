#include "robot.hpp"
#include <unistd.h>

/*void initRobot()
{

} //initRobot*/

void Robot::selectCourseFromSwitch()
{
  //motion.setCourse(gpio.readSelect());
} //end of selectCourseFromSwitch

void Robot::waitForStartSwitch()
{
  while (gpio.readStart() == OFF)
  {
    cout << "Waiting for start switch..." << endl;
    if(gpio.readStart() == ON) {
      cout << "The game is afoot!" << endl;
      break;
    }
    sleep(1);
  }
} //waitForGoSwitch()

//Grabs a block and exits when both limit switches are closed
void Robot::grabBlock()
{
   //close clamper, need to modify to wait
   grabber.clamper.close();

   //start grabber conveyor
   grabber.conveyor.moveForward();

   //start main conveyor
   mainConv.moveForward();

   //might need a timer here
   while(gpio.readLS1() == OFF && gpio.readLS2() == OFF) {
      if(blocksInOneCol = 1 && gpio.readLS1() == OFF) {
         //one small block remains, hence two limit switches would never close
         break;
      }

      //flap till both limit switches are ON
      if(grabber.clamper.getState() == CLAMPER_OPEN) {
         grabber.clamper.close();
      } else {
         grabber.clamper.open();
      }
   } //end of while

   if(grabber.clamper.getState() == CLAMPER_CLOSE) {
      grabber.clamper.open();
   }

   //stop grabber conveyor
   grabber.conveyor.stop();

   //stop main conveyor
   mainConv.stop();
}

void Robot::storeBlock(int barge)
{
   //identify colors only on barge B and C
   if (barge != NUM_BARGE_A)
   {
     int color;
     switch( colorProcessor.idBlock() )
     {
        case COLOR_RED       :  color = SORTER_RED; break;
        case COLOR_BLUE      :  color = SORTER_BLUE; break;
        case COLOR_YELLOW    :  color = SORTER_YELLOW; break;
        case COLOR_GREEN     :  color = SORTER_GREEN; break;
        case COLOR_UNDEFINED :  cout << "Color unidentified" <<  endl; break;
     }
     //that is more blocks are there
     if(blocksInOneCol == 4) {
        sorter.sort(SORTER_BIG, 1, color);
     } else if (blocksInOneCol == 2) {
        sorter.sort(SORTER_BIG, 0, color);
     }
     largeCageStatus[color]++;
  }
  else {
     //all blocks are blue, assuming the blue column is full
     int tryColor[3] = {SORTER_RED, SORTER_YELLOW, SORTER_GREEN};
     for (int k = 0; k < 3; k++) {
         if(largeCageStatus[tryColor[k]] < BIG_CAGE_SIZE) {
            if(blocksInOneCol == 4) {
               sorter.sort(SORTER_BIG, 1, tryColor[k]);
            } else if (blocksInOneCol == 2) {
               sorter.sort(SORTER_BIG, 0, tryColor[k]);
            }
            break;
         }
     }
  }
   //decrement by 2 i.e. large block was picked up
   //used in the loop in processBarge
   //WHEN THIS FUNCTION IS MODIFIED FROM SMALL BLOCKS
   //THEN FOR SMALL BLOCK DECREMENT BY 1
   blocksInOneCol =- 2;
}

void Robot::processBarge(int numCols)
{
  for( int i = numCols; i > 0; i-- ) {

     //!!!!!!!!!!!LASER PROCESS HERE!!!!!!!!!!!!!!!

     for ( int j = blocksInOneCol; j>0; j-- ) {
        grabBlock();
        storeBlock(numCols);
     }

    //motion.nextBlock();
  } //end of for for number of columns on the barge
  blocksInOneCol = 4;
} //end of processBarge

void Robot::depositTruck()
{
  //Turn on all conveyors, except blue
  cage.redConveyor.moveForward();
  cage.yellowConveyor.moveForward();
  cage.greenConveyor.moveForward();

  //Sleep for 5 seconds
  usleep(CONV_SLEEP);

  //Turn off all conveyors except blue
  cage.redConveyor.stop();
  cage.yellowConveyor.stop();
  cage.greenConveyor.stop();

  largeCageStatus[SORTER_RED]    = 0;
  largeCageStatus[SORTER_YELLOW] = 0;
  largeCageStatus[SORTER_GREEN]  = 0;
}

void Robot::depositBoat()
{
  //Turn on all conveyors
  cage.redConveyor.moveForward();
  cage.yellowConveyor.moveForward();
  cage.greenConveyor.moveForward();
  cage.blueConveyor.moveForward();

  //Sleep for 5 seconds
  usleep(CONV_SLEEP);

  //Turn off all conveyors
  cage.redConveyor.stop();
  cage.yellowConveyor.stop();
  cage.greenConveyor.stop();
  cage.blueConveyor.stop();

  largeCageStatus[SORTER_RED]    = 0;
  largeCageStatus[SORTER_YELLOW] = 0;
  largeCageStatus[SORTER_GREEN]  = 0;
  largeCageStatus[SORTER_BLUE]   = 0;
}

void Robot::depositHopper( int color )
{
  switch(color)
  {
    case SORTER_RED:
                cage.redConveyor.moveForward();
                usleep(CONV_SLEEP);
                cage.redConveyor.stop();
                largeCageStatus[SORTER_RED] = 0;
                break;
    case SORTER_YELLOW:
                cage.yellowConveyor.moveForward();
                usleep(CONV_SLEEP);
                cage.yellowConveyor.stop();
                largeCageStatus[SORTER_YELLOW] = 0;
                break;
    case SORTER_BLUE:
                cage.blueConveyor.moveForward();
                usleep(CONV_SLEEP);
                cage.blueConveyor.stop();
                largeCageStatus[SORTER_BLUE] = 0;
                break;
    case SORTER_GREEN:
                cage.greenConveyor.moveForward();
                usleep(CONV_SLEEP);
                cage.greenConveyor.stop();
                largeCageStatus[SORTER_GREEN] = 0;
                break;
  }
}
