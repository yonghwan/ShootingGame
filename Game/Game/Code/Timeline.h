#ifndef TIMELINE_H
#define TIMELINE_H

class Timeline {
      public:
      Timeline(int stageID);
      ~Timeline();
      bool tick(int time);
             
      protected:
      bool done;
      int pointer;
      int numEvents;
      int* eventTimes;
      int* eventTypes;
      float* eventX;
      float* eventY;
      float* eventArg1;
      float* eventArg2;
};


#endif
