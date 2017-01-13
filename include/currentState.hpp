#ifndef _STATESPACE_H_
#define _STATESPACE_H_

// This is a simple Monty Hall problem state space for testing purposes
enum CurrentState{CCS, 
                  CSC, 
                  SCC, 
                  OCS, 
                  COS, 
                  OSC, 
                  CSO, 
                  SCO, 
                  SOC, 
                  OOC, 
                  OCO, 
                  COO,
                  NUM_STATES}; // This last entry isn't a real state; it just gives the correct number of states

// That trick was suggested here:
//http://stackoverflow.com/questions/10910615/c-randomly-assigning-an-enumerated-type-to-a-variable/10910634#10910634

#define LEN_STATE_VEC 2


#endif
