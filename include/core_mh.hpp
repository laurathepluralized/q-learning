#ifndef _RL_MH_COREH_
#define _RL_MH_COREH_

#include <vector>
#include <string>

// This is a simple Monty Hall problem state space for testing purposes

/*
 * DoorState{CCS, CSC, SCC, OCS, COS, OSC, CSO, SCO, SOC, OOC, OCO, COO}
*/
//enum DoorState{BOTH_ON_GROUND, RIGHT_ON_GROUND, LEFT_ON_GROUND, BOTH_NOT_ON_GROUND};
/*
 * Now we have 16 states based on the values of 8 fsrs on each foot,
 * i.e 2 states for each foot i.e front back e.g: value 
 * of front and back  fsrs for each foot, 2 exp 4 = 16 states.
 *    left	 right
 * +--------+ +--------+
 * |0.0  0.0| |0.0  0.0|  front
 * |        | |        |
 * |0.0  0.0| |0.0  0.0|  back
*/
enum DoorState{CCS, CSC, SCC, OCS, COS, OSC, CSO, SCO, SOC, OOC, OCO, COO};

/*
 * Available Patterns
*/
enum PatternType{ PLATEAU , 
    QUIESCENT , 
    AMORTI , 
    OSCILLATORY ,
    SLOWOSCILLATION , 
    FASTOSCILLATION};

/*
 * sigma_s, sigma_f values for the available patterns. 
 * //TODO: Move the '#define' values to a seperate header file ('rl_define.h').
*/
#define Plateau_sigma_s = 0;
#define Plateau_sigma_f = 2.8;
#define Quiescent_sigma_s = 0.5;
#define Quiescent_sigma_f = 0.5;
#define Amorti_sigma_s = 4.5;
#define Amorti_sigma_f = 0.6;
#define Oscillatory_sigma_s = 4.6;
#define Oscillatory_sigma_f = 1.5;
#define Oscillatory_slow_sigma_s = 0;
#define Oscillatory_slow_sigma_f = 0;
#define Oscillatory_fast_sigma_s = 5.6;
#define Oscillatory_fast_sigma_f = 1.15;

/*
 * Struct to represent base sigma_s, sigma_f of rs_neuron
*/
struct rs_neuron_b
{
   PatternType pattern;
};

/*
 * Struct to be used by 'State' etc, as one state would have 24 values i.e one for each joint/CPG
*/

struct rs_neuron
{
   rs_neuron_b rsneuron[24];
};

/*
 * Interface for a state.
*/
class State
{
   public:
   DoorState door_state;

   /*
    * A Simple method to comapare door states
   */
   bool compareStates(const DoorState f1) const
   {
      if(this->door_state == f1)
      {
         return true;
      }
      return false;
   }

   std::string getName()
   {
      std::string state = "";
      switch(door_state)
      {
//enum DoorState{CCS, CSC, SCC, OCS, COS, OSC, CSO, SCO, SOC, OOC, OCO, COO};
         case CCS:
            state += "Closed Closed Selected";
            break;
         case CSC:
            state += "Closed Selected Closed";
            break;
         case SCC:
            state += "Selected Closed Closed";
            break;
         case OCS:
            state += "Open Closed Selected";
            break;
         case COS:
            state += "Closed Open Selected";
            break;
         case OSC:
            state += "Open Selected Closed";
            break;
         case CSO:
            state += "Closed Selected Open";
            break;
         case SCO:
            state += "Selected Closed Open";
            break;
         case SOC:
            state += "Selected Open Closed";
            break;
         case OOC:
            state += "Open Open Closed";
            break;
         case OCO:
            state += "Open Closed Open";
            break;
         case COO:
            state += "Closed Open Open";
            break;
      }
      state += "\n";
      return state;
   }

   /*
    * Overloaded needed because of 'this' thing.
   */
   static std::string getName(DoorState fstate)
   {
      std::string state = "";
      switch(fstate)
      {
//enum DoorState{CCS, CSC, SCC, OCS, COS, OSC, CSO, SCO, SOC, OOC, OCO, COO};
         case CCS:
            state += "Closed Closed Selected";
            break;
         case CSC:
            state += "Closed Selected Closed";
            break;
         case SCC:
            state += "Selected Closed Closed";
            break;
         case OCS:
            state += "Open Closed Selected";
            break;
         case COS:
            state += "Closed Open Selected";
            break;
         case OSC:
            state += "Open Selected Closed";
            break;
         case CSO:
            state += "Closed Selected Open";
            break;
         case SCO:
            state += "Selected Closed Open";
            break;
         case SOC:
            state += "Selected Open Closed";
            break;
         case OOC:
            state += "Open Open Closed";
            break;
         case OCO:
            state += "Open Closed Open";
            break;
         case COO:
            state += "Closed Open Open";
            break;
          default:
            state += "Unknown/Invalid State -- ";
            state += fstate;
            state += " ";
            break;
      }
      state += "\n";
      return state;
   }

};

/*
 * Interface for an Action.
*/
class Action
{
   public:
   rs_neuron rs_neuron_pattern;

   /*
    * A simple method to compare Actions i.e rs_neuron values 
    * //TODO: Also incorporate pfneuron only if applicable.
   */
   bool compareActions(const Action& action) const
   {
      for(int i = 0; i < 24; i++)
      {
         if(this->rs_neuron_pattern.rsneuron[i].pattern == action.rs_neuron_pattern.rsneuron[i].pattern)
            continue;
         else
            return false;
      }
      return true;
   }

   /*
    * To make sure that valid action is generated, invalid actions also got 
    * generated sometimes due to 'unknown' reasons.
   */
   bool isValid()
   {
      bool valid = true;
      for(int i = 0; i < 24; i++)
      {
         switch(rs_neuron_pattern.rsneuron[i].pattern)
         {
            case PLATEAU:
               break;
            case QUIESCENT:
               break;
            case AMORTI:
               break;
            case OSCILLATORY:
               break;
            case SLOWOSCILLATION:
               break;
            case FASTOSCILLATION:
               break;
	    default:
               valid = false;
               break;
         }
      }
      return valid;
   }

   std::string getName()
   {
      std::string action = "";
      for(int i = 0; i < 24; i++)
      {
         switch(rs_neuron_pattern.rsneuron[i].pattern)
         {
            case PLATEAU:
               action += "Plateau ";
               break;
            case QUIESCENT:
               action += "Quiescent ";
               break;
            case AMORTI:
               action += "Amorti ";
               break;
            case OSCILLATORY:
               action += "Oscillatory ";
               break;
            case SLOWOSCILLATION:
               action += "Slow Oscillation ";
               break;
            case FASTOSCILLATION:
               action += "Fast Oscillation ";
               break;
	    default:
               action += "Unknown/Invalid Action -- " ;
               action +=  rs_neuron_pattern.rsneuron[i].pattern;
               action += " ";
               break;
         }
      }
      action += "\n";
      return action;
   }
};

class StateActionPair
{
public:
   State state;
   Action action;
   StateActionPair() {}
   StateActionPair(State state, Action) {this->state = state; this->action = action; } 
   StateActionPair(State& state, Action& action) {this->state = state, this->action = action; }
};

class QTable
{
public:
   StateActionPair state_action_pair;
   double qvalue;
};

/*
 * Interface for an agent.
*/
class Agent
{
public:

   virtual Action getPolicy(const State& state) = 0;

   virtual bool savePolicy(const std::string filename) {return true;};

   virtual ~Agent() {};

};


#endif
