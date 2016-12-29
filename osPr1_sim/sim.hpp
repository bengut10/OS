//
//  sim.hpp
//  sim
//
//  Created by Benjamin Gutierrez on 10/4/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.
//

#ifndef sim_hpp
#define sim_hpp

#include <stdio.h>
#include "readyQueue.hpp"
#include "eventQueue.hpp"
#include <fstream>
#include <iomanip> 

extern float simClock;

class sim
{
private:
    std::ofstream myfile;
    eventQueue *eveQueue;
    readyQueue *proQueue;
    float lambda, waiting, inverse, numOfProcesses,
    turnAround, quantum, scheduler, arrivalRate, idle, lastOn,nextEvent;
    float waitingAccum, turnAccum;
    int arrival,departure,id;
    bool cpuBusy;
    
public:
    sim();
    ~sim();
    sim(int scheduler, float lambda, float quantum, float inverse);
    void init();
    void run_sim();
    int process_ArrivalEvent(eventQueue::event* eve);
    int process_DepEvent(eventQueue::event* eve);
    int process_TimeSliceEvent(float quantum);
    int process_preemption();
    float exp(float lambda);
    float urand();
    void genResults();
    void writeHeader();
};


#endif /* sim_hpp */
