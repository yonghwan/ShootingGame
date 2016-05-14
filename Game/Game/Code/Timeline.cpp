#include "Timeline.h"
#include "Enemy.h"
#include "Midboss.h"
#include "Boss.h"
#include "Neutral.h"
#include <string>
#include <iostream>
#include <fstream>

Timeline::Timeline(int stageID) {
    //If I'm right, this is on the stack.
    std::ifstream ifs("Timelines/timeline1.dat",std::ifstream::in);
    
    //Initialize vars
    pointer = 0;
    done = false;
    ifs >> numEvents;
    
    //Initialize arrays
    eventTimes = new int[numEvents];
    eventTypes = new int[numEvents];
    eventX = new float[numEvents];
    eventY = new float[numEvents];
    eventArg1=new float[numEvents];
    eventArg2=new float[numEvents];
    
    //Populate arrays
    for (int i=0;i<numEvents;i++) {
          ifs >> eventTimes[i];
          ifs >> eventTypes[i];
          ifs >> eventX[i];
          ifs >> eventY[i];
          ifs >> eventArg1[i];
          ifs >> eventArg2[i];
          //Break off if I messed up the number of events
          if (!ifs.good()) {
             numEvents = i;
             break;
          }
    }
}

Timeline::~Timeline() {
    delete[] eventTimes;
    delete[] eventTypes;
    delete[] eventX;
    delete[] eventY;
    delete[] eventArg1;
    delete[] eventArg2;
}

bool Timeline::tick(int time) {
     if (done) return done;
    while (time >= eventTimes[pointer]) {
          //Bleah. This is the only way I know
          // that allows a bunch of constructors dependent on input
          // since C++ doesn't allow arrays of constructor pointers
          // because apparently that is impossible.
          switch (eventTypes[pointer]) {
                 case 0:
                 new Enemy2(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer]);
                 break;
                 
                 case 1:
                 new Enemy1(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer]);
                 break;
                 
                 case 2:
                 new Enemy3(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer],
                            eventArg2[pointer]);
                 break;
                 
                 case 3:
                 new Enemy4(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer],
                            eventArg2[pointer]);
                 break;
                 
                 case 4:
                 new Enemy5(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer],
                            eventArg2[pointer]);
                 break;
                 
                 case 5:
                 new Enemy6(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer],
                            eventArg2[pointer]);
                 break;
                 
                 case 6:
                 new Enemy7(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer],
                            eventArg2[pointer]);
                 break;
                 
                 case 7:
                 new Enemy8(eventX[pointer],
                            eventY[pointer],
                            eventArg1[pointer],
                            eventArg2[pointer]);
                 break;
                 
                 case 8:
                 new MidbossSpider();
                 break;
                 
                 case 9:
                 new Boss();
                 break;
                 
                 case 10:
                 new StageClear();
                 break;
          }
          
          
          
          pointer++;
          if (pointer > numEvents) {
             done = true;
          }
    }
    return done;
}
