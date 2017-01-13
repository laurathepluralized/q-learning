#include "QLearner.hpp"
#include <errno.h>
class QLearningSimulate
{
   QLearner agent;
   std::string qtablePath;
   std::string policyPath;
   double myTime;
   double timeStep;
public:
   QLearningSimulate();
   QLearningSimulate(std::string qtablePath, std::string policyPath);
   bool initialize();
   double* simulateStateData(int type);
   int randomLimit(unsigned int min, unsigned int max);
   bool flipCoin (double p);
   CurrentState determineState();
   int run();
};
