#include "include/QLearningSimulate.hpp" 

int main(int argc, char** argv)
{

   std::string qtablePath = "/home/laura/repos/q-learning/persistent_storage/qtable.uy";
   std::string policyPath = "/home/laura/repos/q-learning/persistent_storage/policy.uy";
   QLearningSimulate simulate(qtablePath, policyPath);
   
   simulate.run();

   return 0;
}

