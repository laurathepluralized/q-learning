#ifndef _QLEARNER_
#define _QLEARNER_

#include <float.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <random>

#include "core_mh.hpp"
#include "log.hpp"
/*
 * Agent that uses Q-learning with ...
*/
class QLearner: public Agent
{

     // NOTE: I think TSP stands for support phase time (T_sp)
     // If any TSW or T_sw terms pop up, probably stands for swing phase time
     // CPG = Central Pattern Generator
private:

    std::vector<QTable> Q;

    std::vector<QTable> Policy;
    
    float epsilon; /* exploration prob */
    float alpha; /* learning rate */
    float gamma; /* discount rate */

    float cooling_rate; /* probability to try a particular TSP solution */

    double myTime; //TODO: temporary variable, time at which collision occurs
    int resCount=0;
    unsigned int threshold;

    bool result; // true when specific result occurs
    bool end_cond; // true when end condition occurs

    double *currentQ; // need to update 'Q-values' :)

    bool isStateSeen(const StateActionPair& s_a_pair, const State& state, 
			const Action& action) const;

    std::vector<Action> getTriedActions(const State& state) const;

    std::vector<Action> getLegalActions(const State& state, unsigned int type) const ;

    bool flipCoin (double p);

    int randomLimit(unsigned int min, unsigned int max) const;

    bool insertStateActionPair(const State& state, const Action& action);

    CurrentState determineState();

    bool isStatePresent(std::vector<QTable>& q, const State& state) const;

    int getStateIndex(std::vector<QTable>& q, const State& state) const;

    std::vector<QTable> getCurrentPolicy() const;

    //Action getBaseActionTSP() const;

    //Action getAllActionTSP() const;  

    CurrentState getCurrentState(const int idx) const;

    ActionSpace getAction(const int idx) const;

    ActionSpace getRandomPattern(unsigned int min_val, unsigned int max_val) const;

public:

    QLearner();

    QLearner(float epsilon, float alpha, 
                float gamma, float cooling_rate
                );

    void init(float epsilon, float alpha, float gamma, float cooling_rate, 
              unsigned int threshold, double myTime);

    int getReward();

    double getQValue(const State& state, const Action& action);

    bool updateQValue(const State& state, const Action& action, double qvalue);

    double getValue(const State& state);

    Action getAction(State& state);

    void doAction(Action& action);

    virtual Action getPolicy(const State& state);

    Action justPolicy(State& state);

    void update(State& state, Action& action, State& nextstate, int reward);

    virtual ~QLearner();

    virtual bool savePolicy(const std::string filename);

    bool loadPolicy(const std::string filename);
    
    void printCurrentPolicy();

    void printPersistentPolicy();

    bool loadQTable(const std::string filename);

    bool saveQTable(const std::string filename);

    void printQTable();

    bool createPersistence(const std::string qtablePath, const std::string policyPath);

    //bool detectPerturbation(double myTime);

    bool detectEnd(CurrentState fstate);
 
    bool getRes();

    bool getEnd();
    std::default_random_engine gen_;
};

#endif
