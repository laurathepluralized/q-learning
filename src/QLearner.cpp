
#include <cmath>

#include "../include/QLearner.hpp"

QLearner::QLearner() {}

QLearner::QLearner(float epsilon, float alpha, 
                         float gamma, float cooling_rate): epsilon(epsilon), alpha(alpha),
                    	    gamma(gamma), cooling_rate(cooling_rate), currentQ(NULL)
{
     // Monty hall never chooses a end_cond door on his choice, so initialize end_cond to false and result to true
    end_cond = false; 

    result = true;


}

/*
 * This method is needed as object declaration & initialization cannot 
 * be done seperately (C++), so the 3 params constructor is of no use 
 * if QLearner has to be used a member of a custom class.
*/

void QLearner::init(float epsilon, float alpha, float gamma, float cooling_rate, 
          unsigned int threshold, double myTime)
{
    this->epsilon         = epsilon;
    this->alpha            = alpha;
    this->gamma            = gamma;
    //this->cooling_rate         = cooling_rate;
    //this->threshold = threshold;
    this->myTime          = myTime;

}

/*
    * Returns Q(state,action)     
    * Should return 0.0 if we never seen
    * a state or (state,action) tuple
*/

double QLearner::getQValue(const State& state, const Action& action)
{
    // search '<State, Action> Q' and return the 'Q' value for 
    // that state, if not found return 0

    // Searching for 'Q' value of an action 'a' in state 's'
    std::vector<QTable>::iterator iter;
    for(iter = Q.begin(); iter != Q.end(); ++iter)
    {
        if(isStateSeen(iter->state_action_pair, state, action))
        {
            return iter->qvalue;
        }
    }
    // State-Action does not exist in the Q-Table, so add it
    insertStateActionPair(state, action);
    return 0;
}

/*
 * Called after update(..) to update the new calculated q-value 
*/
bool QLearner::updateQValue(const State& state, const Action& action, double qvalue)
{
    std::vector<QTable>::iterator iter;
    for(iter = Q.begin(); iter != Q.end(); ++iter)
    {
        if(isStateSeen(iter->state_action_pair, state, action))
        {
            iter->qvalue = qvalue;
            return true;
        }
    }
    return false;
}

/*
 * Returns max_action Q(state,action)          
 * where max is over legal actions
 *
 * Max 'Q' value for an action in a state.
*/
double QLearner::getValue(const State& state)
{
    std::vector<Action> actionlist = getTriedActions(state);
    std::vector<Action>::iterator iter;
    std::vector<double> qvalues;
    for(iter = actionlist.begin() ; iter < actionlist.end() ; ++iter)
    {
        qvalues.push_back(getQValue(state, *iter));
    }

    // Got the 'q-values' for all the actions, now find the max q-val
    std::vector<double>::iterator iterd; //iterator double
    double max = -DBL_MAX;
    for(iterd = qvalues.begin() ; iterd < qvalues.end() ; ++iterd)
    {
        if(*iterd > max)
        {
            max = *iterd;
        }
    }
    
    return max;
}

/*
 * What is the best action to take in a state.
*/
Action QLearner::getPolicy(const State& state)
{
    std::vector<Action> actionlist = getTriedActions(state);
    std::vector<Action>::iterator iter;
    std::vector<double> qvalues;

    for(iter = actionlist.begin() ; iter < actionlist.end() ; ++iter)
    {
        qvalues.push_back(getQValue(state, *iter));
    }

    /*
     * Got the 'q-values' for all the actions, now find the max q-val 
     * and then return action for that q-val.
    */

    std::vector<double>::iterator iterd;
    double max = -DBL_MAX;
    int idx = 0;
    int maxidx = 0;
    for(iterd = qvalues.begin() ; iterd < qvalues.end() ; ++iterd)
    {
        if(*iterd > max)
        {
            max = *iterd;
            maxidx = idx;
        }
        idx++;
    }
    
    return actionlist[maxidx];
}

/*
 * Once the agent has learned all the 'good actions' for all 
 * 'possible states' then no need to go through, 
 * just use the 'policy' which it had already learned :) 
 * call Action::isValid() to make sure that you get the correct action
*/
Action QLearner::justPolicy(State& state)
{
    std::vector<QTable>::iterator iter;
    for(iter = Policy.begin(); iter != Policy.end(); ++iter)
    {
         if(iter->state_action_pair.state.compareStates(state.curr_state))
         {
             return iter->state_action_pair.action;
         }
    }
    /* If policy for unseen state is requested then just send the 
     * random action, the caller should have known not to use 
     * QLearner::justPolicy(..) when correct policy is not 
     * discovered for many possible states.
    */
    Action action = getAction(state);
    LOG("No policy found for state: %s, generating random action: %s\n", 
              state.getName().c_str(), action.getName().c_str());
    return action;
}

/*
 * What action to take in the current state. With
 * probability self.epsilon, we should take a random
 * action and take the best policy action otherwise.
     
 * After you choose an action make sure to
 * inform your parent self.doAction(state,action) 
 * This is done for you, just don't clobber it
         
 * HINT: you might want to use util.flipCoin
 * Where..... (see util.py)
 
 * Greater the value of 'epsilon', greater will be the chances 
 * to select a random action.

*/
Action QLearner::getAction(State& state)
{
    /*
     * If state is not 'seen' then do a random action with probability '1'.
    */
    float epsilon = this->epsilon;
    if(!isStatePresent(Q, state))
    {
         epsilon = 1.0f;
    }
    Action action;
    if(flipCoin(epsilon))
    {
        std::vector<Action> listofactions;
        /*
         * With 'cooling_rate' probability try 'type 2' solution as this is our base 
         * solution, else try random stuff.
        */
        if(flipCoin(cooling_rate))
        {
             listofactions = getLegalActions(state, 0); // type '0'
             //TODO: Try other types as well
        }
        else
        {
             listofactions = getLegalActions(state, randomLimit(0, 2));
        }

        action = listofactions[randomLimit(0, listofactions.size())];
    }
    else
    {
         action = getPolicy(state);
    }

    return action;
}

/*
 * Do the action for real, might not be needed as the action 
 * has to be directly applied to the motors from the 'Controller'.
*/
void QLearner::doAction(Action& action)
{
    
}

/*
 * The parent class calls this to observe a 
 * state = action => nextState and reward transition.
 * You should do your Q-Value update here
        
 * NOTE: You should never call this function,
 * it will be called on your behalf
*/
void QLearner::update(State& state, Action& action, State& nextstate, int reward)
{
    std::vector<Action> actionlist = getTriedActions(nextstate);
    std::vector<Action>::iterator iter;
    std::vector<double> qvalues;
    double sample;
    for(iter = actionlist.begin() ; iter < actionlist.end() ; ++iter)
    {
        qvalues.push_back(getQValue(nextstate, *iter));
    }
    if(qvalues.size() == 0)
    {
         sample = reward;
    }
    else
    {
        // For speed avoiding call to getValue() as half of compuatation 
        // is already performed in the above loop...
        double max = -DBL_MAX;
        std::vector<double>::iterator iterd;
        for(iterd = qvalues.begin() ; iterd < qvalues.end() ; ++iterd)
        {
            if(*iterd > max)
            {
                max = *iterd;
            }
        }
        sample = reward + (gamma * max);
    }
    
    //double *ptr = NULL;
    double valueupdate = ( (1.0 - alpha) * getQValue(state, action) ) + (alpha * sample);
    // update the 'q-value'
    if(updateQValue(state, action, valueupdate))
    {
        LOG("Updated qvalue for state: %s , action: %s with qvalue: %f.\n", 
             state.getName().c_str(), action.getName().c_str(), valueupdate);
    }
    else
    {
         LOG("'QLearner::update()': Failed to Update qvalue for state: %s , action: %s with qvalue: %f. State-Action pair not found in 'Q-Table'.\n", state.getName().c_str(), action.getName().c_str(), valueupdate);
    }
}

int QLearner::getReward()
{
    if(!result) // no living reward
    {
        return 0;
    }
    else 
    {
         // TODO: wait for few seconds 'in case of perturbation' to get reward, 
         // as it will take some time to fall, sleep()
        if(!end_cond) 
        {
             // robot survives the collision 
             // TODO: how to detect that robot actually survives or is falling end_cond
            return 10;
        }
        else
        {
            return -5;
        }
    }
}

bool QLearner::loadPolicy(const std::string filename)
{
    LOG("QLearner::loadPolicy()\n");
    std::ifstream file(filename.c_str());
    if(!file)
    {
        return false;
    }
    std::string line;
    /*
     * CurrentState Q-value action1,action2... \n
     */
    while(std::getline(file, line))
    {
        std::vector<double> tvalues;
        std::stringstream linestream(line);
        double j;
        while(linestream >> j)
            tvalues.push_back(j);
        QTable qtab;
        int state;
        state = tvalues[0];
        qtab.qvalue = tvalues[1];
        qtab.state_action_pair.state.curr_state = getCurrentState(state);
        for(unsigned int i = 2; i < tvalues.size(); ++i)
        {
              qtab.state_action_pair.action.ac_set.action_vec[i-2].ac_space_member = getAction(tvalues[i]);
        }
        Policy.push_back(qtab);
    }
        LOG("Size Policy: %zu\n", Policy.size());
        file.close();
        return true;
}

bool QLearner::savePolicy(const std::string filename)
{
    std::vector<QTable> policyQ = getCurrentPolicy();
    std::vector<QTable>::iterator iter;
    FILE* file= NULL;
    file = fopen(filename.c_str(),"w");
    if(!file)
    {
        return false;
    }
    for(iter = policyQ.begin(); iter != policyQ.end(); ++iter)
    {
        /*
         * CurrentState Q-value action1,action2... \n
         */
        fprintf(file,"%i %f ",iter->state_action_pair.state.curr_state, iter->qvalue);
        for(unsigned int i = 0; i < 24; i++)
        {
            fprintf(file, "%i ",iter->state_action_pair.action.ac_set.action_vec[i].ac_space_member);
        }
        fprintf(file,"\n");
    }
    fclose(file);
    return true;
}

/*
 * Print 'current policy' from the 'Q' table.
*/

void QLearner::printCurrentPolicy()
{
    LOG("QLearner::printCurrentPolicy()\n");
    std::vector<QTable> policyQ = getCurrentPolicy();
    std::vector<QTable>::iterator iter;
    LOG("# of elements in policy : %zu\n", policyQ.size());
    LOG("\nState\n  * Action\n     -> Q-value\n\n");
    int count = 1;
    for(iter = policyQ.begin(); iter != policyQ.end(); ++iter)
    {
        LOG("%i.\n%s\n *  %s\n  ->  %lf\n\n", count, iter->state_action_pair.state.getName().c_str(), 
                  iter->state_action_pair.action.getName().c_str(), iter->qvalue);
        count++;
    }
}

/*
 * Print the 'stored' policy.
*/

void QLearner::printPersistentPolicy()
{
    LOG("QLearner::printPersistentPolicy()\n");
    std::vector<QTable>::iterator iter;
    LOG("# of elements in policy : %zu\n", Policy.size());
    LOG("\nState\n  * Action\n     -> Q-value\n\n");
    int count = 1;
    for(iter = Policy.begin(); iter != Policy.end(); ++iter)
    {
        LOG("%i.\n%s\n *  %s\n  ->  %lf\n\n", count, 
                  iter->state_action_pair.state.getName().c_str(), 
                  iter->state_action_pair.action.getName().c_str(), 
                  iter->qvalue);
        count++;
    }
}

bool QLearner::loadQTable(const std::string filename)
{
    LOG("QLearner::loadQTable()\n");
    std::ifstream file(filename.c_str());
    if(!file)
        return false;
    std::string line;
    /*
     * CurrentState Q-value action1,action2... \n
     */
    while(std::getline(file, line))
    {
        std::vector<double> tvalues;
        std::stringstream linestream(line);
        double j;
        while(linestream >> j)
        {
            tvalues.push_back(j);
        }
        QTable qtab;
        int state;
        state = tvalues[0];
        qtab.qvalue = tvalues[1];
        qtab.state_action_pair.state.curr_state = getCurrentState(state);
        for(unsigned int i = 2; i < tvalues.size(); ++i)
        {
              qtab.state_action_pair.action.ac_set.action_vec[i-2].ac_space_member = getAction(tvalues[i]);
        }
        Q.push_back(qtab);
    }
        LOG("Size QTable: %zu\n", Q.size());
        file.close();
        return true;
}

bool QLearner::saveQTable(const std::string filename)
{
    std::vector<QTable>::iterator iter;
    FILE* file= NULL;
    file = fopen(filename.c_str(),"w");
    if(!file)
    {
        return false;
    }
    for(iter = Q.begin(); iter != Q.end(); ++iter)
    {
        /*
         * CurrentState Q-value action1,action2... \n
         */
        fprintf(file,"%i %f ",iter->state_action_pair.state.curr_state, iter->qvalue);
        for(unsigned int i = 0; i < 24; i++)
        {
            fprintf(file, "%i ",iter->state_action_pair.action.ac_set.action_vec[i].ac_space_member);
        }
        fprintf(file,"\n");
    }
    fclose(file);
    return true;
}

/*
 * Print the whole 'Q' table.
*/

void QLearner::printQTable()
{
    LOG("QLearner::printQTable()\n");
    std::vector<QTable>::iterator iter;
    LOG("# of elements in QTable : %zu\n", Q.size());
    LOG("\nState\n  * Action\n     -> Q-value\n\n");
    int count = 1;
    for(iter = Q.begin(); iter != Q.end(); ++iter)
    {
        LOG("%i.\n%s\n *  %s\n  ->  %lf\n\n", count, 
                  iter->state_action_pair.state.getName().c_str(), 
                  iter->state_action_pair.action.getName().c_str(), 
                  iter->qvalue);
        count++;
    }
}

bool QLearner::createPersistence(const std::string qtablePath, const std::string policyPath)
{
    FILE* file= NULL;
    file = fopen(qtablePath.c_str(),"w");
    if(!file)
    {
        ERROR("Error in creating file %s; please check that you have the required permissions...\n", 
                  qtablePath.c_str());
        return false;
    }
    fclose(file);
    file = fopen(policyPath.c_str(),"w");
    if(!file)
    {
        ERROR("Error in creating file %s; please check that you have the required permissions...\n", 
                  policyPath.c_str());
        return false;
    }
    fclose(file);
    return true;
}

/*
 * Returns true if state already seen by 'Q' datastructure :)
 */
bool QLearner::isStateSeen(const StateActionPair& s_a_pair, const State& state, 
 			const Action& action) const
{
    // Step 1. Compare the foot states
    if(s_a_pair.state.compareStates(state.curr_state))
    {
        // Step 2. Compare the point of impact.
        // TODO: Right now no way of getting 'point of impact'.
        // Step 3. Now compare the actions
        if(s_a_pair.action.compareActions(action))
        {
            return true;
        }
    }

    return false;
}

/*
 * Get all the already 'tried' actions i.e the ones stored in Q-table.
*/
std::vector<Action> QLearner::getTriedActions(const State& state) const
{
    std::vector<Action> actionlist;
    std::vector<QTable>::const_iterator iter;
    for(iter = Q.begin(); iter != Q.end(); ++iter)
    {
        if(iter->state_action_pair.state.compareStates(state.curr_state))
        {
            actionlist.push_back(iter->state_action_pair.action);
        }
    }
    //TODO: @warn: remove this code
    CurrentState fstate = state.curr_state;
    LOG("Size [TriedActions]: %zu for State: %s\n", actionlist.size(), State::getName(fstate).c_str());
    return actionlist;
}

/*
 * Get all the legal actions for a state [in our case, all states have same legal 
 * actions], first return the actions which were already tried before and for 
 * which we already have a Q-value.
*/
std::vector<Action> QLearner::getLegalActions(const State& state, unsigned int type) const
{
    LOG("getLegalActions().. -- type : %i\n", type);
    std::vector<Action> actionlist;
    Action action;
        for(unsigned int val = 0 ; val < LEN_AC_VEC; ++val)
        {
            for(int i = 0; i < NUM_ACTIONS; ++i)
            {
                action.ac_set.action_vec[i].ac_space_member = static_cast<ActionSpace>(rand() % NUM_ACTIONS);
            }
            actionlist.push_back(action);
        }

    LOG("# of legal actions: %zu\n", actionlist.size());

    return actionlist;          
}

/*
 * Use for selecting an action with probability 'p'
 * TODO: Use better version of random generation, sth like 'std::uniform_real_distribution'
 * @imp: Candidate for improvement.
*/

bool QLearner::flipCoin(double p)
{
    std::uniform_real_distribution<double> coin_flipper_(0.0,1.0);
    double r = coin_flipper_(gen_);
    
    return r < p;
}

/*
 * Generate random number in between the limit (boundry values included).
 * TODO: Use better version of random generation, sth like 'std::uniform_real_distribution'
 * @imp: Candidate for improvement.
*/

int QLearner::randomLimit(unsigned int min, unsigned int max) const
{
    return (min + (rand() % (int)(max - min + 1)));
}

bool QLearner::insertStateActionPair(const State& state, const Action& action)
{
    QTable q;
    q.state_action_pair.state = state;
    q.state_action_pair.action = action;
    q.qvalue = 0.0; // for the new experienced state, 'q-value' is 0
    Q.push_back(q);
    return false;
}

/*
 * This method takes the 8 FSRS of both feet to determine the state of the robot, 
 * which would be one state out of possible 
 * 'CurrentState', see core.hpp for more details.
*/

CurrentState QLearner::determineState()
{
    CurrentState fstate;
    // Insert logic to determine which state we are currently in here
    //
    // Right now, using a stupid/naive random choice
    fstate = static_cast<CurrentState>(rand() % NUM_STATES);
    return fstate;
}

/*
 * Used by print/load/save policy to the identify the unique states.
*/

bool QLearner::isStatePresent(std::vector<QTable>& q, const State& state) const
{
    std::vector<QTable>::iterator iter;
    for(iter = q.begin(); iter != q.end(); ++iter)
    {
        if(iter->state_action_pair.state.compareStates(state.curr_state))
        {
            return true;
        }
    }
    return false;
}

/*
 * Used by print/load/save policy to the get the index of unique states.
*/

int QLearner::getStateIndex(std::vector<QTable>& q, const State& state) const
{
    std::vector<QTable>::iterator iter;
    int idx = 0;
    for(iter = q.begin(); iter != q.end(); ++iter)
    {
        if(iter->state_action_pair.state.compareStates(state.curr_state))
        {
            return idx;
        }
        idx++;
    }

    return -1;

}

/*
 * This method is needed by printPolicy() & savePolicy() to get the unique 'policy' for each state.
*/

std::vector<QTable> QLearner::getCurrentPolicy() const
{
    // Get max action for all the states.
    std::vector<QTable>::const_iterator iter;
    std::vector<QTable> policyQ;
    /*
     * Get unique states.
    */
    for(iter = Q.begin(); iter != Q.end(); ++iter)
    {
        if(!(isStatePresent(policyQ, iter->state_action_pair.state)))
        {
            policyQ.push_back(*iter);
        }
        else
        {
            /*
             * If the 'q-value' of that state for another action is '>' than our q-value, 
             * then erase the previous action and add this one.
            */
            int idx = getStateIndex(policyQ, iter->state_action_pair.state);
            if(policyQ.at(idx).qvalue < iter->qvalue)
            {
                policyQ.erase(policyQ.begin() + idx);
                policyQ.push_back(*iter);
            }
                
        } 
    }
    return policyQ;
}

/*
 * One known solution used by TSP
Action QLearner::getBaseActionTSP() const
{
    Action action;
    action.ac_set.action_vec[0].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[1].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[2].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[3].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[4].ac_space_member = PLATEAU;
    action.ac_set.action_vec[5].ac_space_member = PLATEAU;
    action.ac_set.action_vec[6].ac_space_member = PLATEAU;
    action.ac_set.action_vec[7].ac_space_member = PLATEAU;
    action.ac_set.action_vec[8].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[9].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[10].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[11].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[12].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[13].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[14].ac_space_member = PLATEAU;
    action.ac_set.action_vec[15].ac_space_member = PLATEAU;
    action.ac_set.action_vec[16].ac_space_member = PLATEAU;
    action.ac_set.action_vec[17].ac_space_member = PLATEAU;
    action.ac_set.action_vec[18].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[19].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[20].ac_space_member = PLATEAU;
    action.ac_set.action_vec[21].ac_space_member = PLATEAU;
    action.ac_set.action_vec[22].ac_space_member = PLATEAU;
    action.ac_set.action_vec[23].ac_space_member = PLATEAU;
    return action;
}

*/
/*
 * Solution used by TSP which contains all the actions.
 * TODO: Make it more sensible :/
Action QLearner::getAllActionTSP() const
{
    Action action;
    action.ac_set.action_vec[0].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[1].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[2].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[3].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[4].ac_space_member = PLATEAU;
    action.ac_set.action_vec[5].ac_space_member = PLATEAU;
    action.ac_set.action_vec[6].ac_space_member = PLATEAU;
    action.ac_set.action_vec[7].ac_space_member = PLATEAU;
    action.ac_set.action_vec[8].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[9].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[10].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[11].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[12].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[13].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[14].ac_space_member = PLATEAU;
    action.ac_set.action_vec[15].ac_space_member = PLATEAU;
    action.ac_set.action_vec[16].ac_space_member = PLATEAU;
    action.ac_set.action_vec[17].ac_space_member = PLATEAU;
    action.ac_set.action_vec[18].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[19].ac_space_member = QUIESCENT;
    action.ac_set.action_vec[20].ac_space_member = PLATEAU;
    action.ac_set.action_vec[21].ac_space_member = SLOWOSCILLATION; //PLATEAU
    action.ac_set.action_vec[22].ac_space_member = PLATEAU;
    action.ac_set.action_vec[23].ac_space_member = OSCILLATORY; //PLATEAU
    return action;
}

*/
CurrentState QLearner::getCurrentState(const int idx) const
{
    //CurrentState fstate = (CurrentState)idx;
    return (CurrentState)idx;
    /*
    switch(idx)
    {
        case 0:
            fstate = ZERO_FSRS;
            break;
      case 1:
            fstate = R_BACK;
            break;
      case 2:
            fstate = L_BACK;
            break;
      case 3:
            fstate = L_R_BACK;
            break;
      case 4:
            fstate = R_FRONT;
            break;
      case 5:
            fstate = R_FRONT_BACK;
            break;
      case 6:
            fstate = L_BACK_R_FRONT;
            break;
      case 7:
            fstate = L_R_BACK_R_FRONT;
            break;
      case 8:
            fstate = L_FRONT;
            break;
      case 9:
            fstate = L_FRONT_R_BACK;
            break;
      case 10:
            fstate = L_FRONT_BACK;
            break;
      case 11:
            fstate = L_R_BACK_L_FRONT;
            break;
      case 12:
            fstate = L_R_FRONT;
            break;
      case 13:
            fstate = L_R_FRONT_R_BACK;
            break;
      case 14:
            fstate = L_R_FRONT_L_BACK;
            break;
      case 15:
            fstate = ALL_FSRS;
            break;
      default:
            fstate = ZERO_FSRS;
            ERROR("Unknown/Invalid State \n");
            break;
    }
    */
    //return fstate;
}

ActionSpace QLearner::getAction(const int idx) const
{
    return (ActionSpace)idx;
}

/*
 * A naive implementation to get random ac_space_member for the action, 
 * hoping with the probability "p" ('?') to find the appropriate ac_space_member for 
 * the perturbation.
*/

ActionSpace QLearner::getRandomPattern(unsigned int min_val = 0, unsigned int max_val = 5) const
{
    return getAction(randomLimit(min_val, max_val));
}

/*
 * Right now the only way to detect perturbation is via 'time'. 
 * TODO: Try to get perturbation detection with accelerometer data.
 * @imp: Candidate for improvement.
*/
/*
bool QLearner::detectPerturbation(double myTime)
{
    if(myTime < this->myTime)
    {
        return false;
    }
    else
    {
        result = true;
    }
    return true;
}
*/

/*
 * Take the CurrentState and start the counter of 'fallindex'.
 * TODO: Find a better way to detect 'robot fall'. 
 * @imp: Candidate for improvement.
*/
bool QLearner::detectEnd(CurrentState fstate)
{
    switch(fstate)
    {
        case 0:
            resCount++;
            break;
        default:
            resCount = -1;
            break;
    }

    if(resCount == -1)
    {
        return false;
    }
    else if(resCount < threshold)
    {
        return false;
    } 
    else
    {
        end_cond = true;
    }
    return true; 

}

bool QLearner::getRes()
{
    return result;
}

bool QLearner::getEnd()
{
    return end_cond;
}

QLearner::~QLearner() {}
