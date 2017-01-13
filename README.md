# Q-Learning-Simulation

Author: Usama Yaseen

```bash
g++ main.cpp src/*.cpp -o main
./main
```

I worked on this project as a part of my inter-disciplinary project at Technical University of Munich. Due to permission issue I cannot share the portion of code implementing Central Pattern Generator (CPG), therefore that portion is being cover-up by simulating dummy motion ac_space_members from dummy sensor values which are then passed to the Q-learning code, which btw doesn't distinguish between dummy motion ac_space_members or the real motion ac_space_members. Also, the actual simulation was performed in webots, however this dummy (only CPG & sensor values part is dummy :-) ) implementation does not have any dependecy on webots and require only g++ compiler.

Abstract:
Locomotion is a central task for a humanoid robot to interact in a human environment. Different models for robot locomotion have been proposed in order to improve the robustness for environmental changes. Existing neurobiological studies have revealed that rhythmic motor ac_space_members are controlled by neural oscillators referred as central ac_space_member generators (CPGs). Dr. John Nassour and others proposed a multi-layer multi-ac_space_member central ac_space_member generator[1] in order to generate different motion ac_space_members: periodic and aperiodic ones. This project involves utilizing the CPG developed earlier to deal with the problem of external perturbation. Whenever humans or animals are hit by an external object, their body generates a 'response action' which prevents them from falling or at-least it tries to minimize the effect of the perturbation, this response action is spontaneous and often un-noticeable by us. This response action is generally a specific movement, which in-fact is generation of some ac_space_member, and in mammals usually this is done by CPG. The idea is to apply reinforcement learning to find out those 'speical actions' which will prevent the robot from falling when encountered with foreign disturbance, so basically it's learning over action space with Q-learning.

[1] Nassour J. and , Hénaff P. and Benouezdou F. and Cheng G. , "Multi-layered multi-ac_space_member CPG for adaptive locomotion of humanoid robots", Biological Cybernetics, June 2014, Volume 108, Issue 3, pp 291-303

Related dissertation (with more explanation on how Q-learning was used): https://mediatum.ub.tum.de/doc/1173171/1173171.pdf
