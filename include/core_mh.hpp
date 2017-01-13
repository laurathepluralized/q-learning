#ifndef _RL_MH_COREH_
#define _RL_MH_COREH_

#include <vector>
#include <string>

#include "currentState.hpp"
#include "actionSpace.hpp"

/*
 * Struct to represent actions_set
*/
struct actions
{
   ActionSpace ac_space_member;
};

/*
 * Struct to be used by 'State'
*/

struct actions_set
{
   actions action_vec[24];
};

/*
 * Interface for a state.
*/
class State
{
   public:
   CurrentState curr_state;

   /*
    * A Simple method to comapare door states
   */
   bool compareStates(const CurrentState f1) const
   {
      if(this->curr_state == f1)
      {
         return true;
      }
      return false;
   }

   std::string getName()
   {
      std::string state = "";
      switch(curr_state)
      {
//enum CurrentState{CCS, CSC, SCC, OCS, COS, OSC, CSO, SCO, SOC, OOC, OCO, COO};
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
            state += "INVALID";
            break;
      }
      state += "\n";
      return state;
   }

   /*
    * Overloaded needed because of 'this' thing.
   */
   static std::string getName(CurrentState fstate)
   {
      std::string state = "";
      switch(fstate)
      {
//enum CurrentState{CCS, CSC, SCC, OCS, COS, OSC, CSO, SCO, SOC, OOC, OCO, COO};
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
   actions_set ac_set;

   /*
    * A simple method to compare Actions i.e actions_set values 
    * //TODO: Also incorporate pfneuron only if applicable.
   */
   bool compareActions(const Action& action) const
   {
      for(int i = 0; i < 24; i++)
      {
         if(this->ac_set.action_vec[i].ac_space_member == action.ac_set.action_vec[i].ac_space_member)
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
      for(int i = 0; i < LEN_AC_VEC; i++)
      {
         if(!(ac_set.action_vec[i].ac_space_member >= 0) || !(ac_set.action_vec[i].ac_space_member < LEN_AC_VEC))
         {
               valid = false;
         }
      }
      return valid;
   }

   std::string getName()
   {
      std::string action = "";
      for(int i = 0; i < 24; i++)
      {
         switch(ac_set.action_vec[i].ac_space_member)
         {
            case SWITCH:
               action += "Switch ";
               break;
            case REMAIN:
               action += "Remain ";
               break;
	    default:
               action += "Unknown/Invalid Action -- " ;
               action +=  ac_set.action_vec[i].ac_space_member;
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
