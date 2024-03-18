/**
 * Copyright or � or Copr. IETR/INSA - Rennes (2019 - 2021) :
 *
 * Karol Desnos <kdesnos@insa-rennes.fr> (2019 - 2020)
 * Nicolas Sourbier <nsourbie@insa-rennes.fr> (2020)
 * Thomas Bourgoin <tbourgoi@insa-rennes.fr> (2021)
 *
 * GEGELATI is an open-source reinforcement learning framework for training
 * artificial intelligence based on Tangled Program Graphs (TPGs).
 *
 * This software is governed by the CeCILL-C license under French law and
 * abiding by the rules of distribution of free software. You can use,
 * modify and/ or redistribute the software under the terms of the CeCILL-C
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty and the software's author, the holder of the
 * economic rights, and the successive licensors have only limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading, using, modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean that it is complicated to manipulate, and that also
 * therefore means that it is reserved for developers and experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and, more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C license and that you accept its terms.
 */
#include <inttypes.h>
#include <cstdint>
#include "mutator/BranchMutator.h"

#include "learn/learningAgent.h"
#include "learn/FLAgentManager.h"        

// template <class BaseLearningAgent>
// uint64_t Learn::FLAgentManager<BaseLearningAgent>::trainAndExchangeBestBranches(
//     volatile bool& altTraining, bool printProgressBar)
// {
//     const int barLength = 50;
//     uint64_t generationNumber = 0;

//     uint64_t aggregationNumber = 0;

//     while (!altTraining && generationNumber < this->agents[0]->params.nbGenerations) {
//         // Train one generation
//         if (generationNumber == this->agents[0]->params.nbGenerationPerAggregation * (aggregationNumber+1))
//         {
//             exchangeBestBranchs();
//             //for each agent copy all received branchs in the TPGGraph
//             std::for_each(this->agents.begin(),this->agents.end(),
//                             [](Learn::FLAgent<BaseLearningAgent>* agent){

//                                 // copy all branchs 
//                                 for (auto branch : agent->getBestBranch()){
//                                     Mutator::BranchMutator::copyBranch(branch, *(agent->getTPGGraph()));
//                                 } 
//                                 //Empty Epmty the bestBranchs vector to get ready to receive new ones
//                                 agent->emptyBranchs(); 
//                             });
//             aggregationNumber++;
//         }
//         for (auto agent : this->agents)
//         {
//             agent->trainOneGeneration(generationNumber);
//         }
//         generationNumber++;
        
//         // Print progressBar (homemade, probably not ideal)
//         if (printProgressBar) {
//             printf("\rTraining ["); // back
//             // filling ratio
//             double ratio =
//                 (double)generationNumber / (double)this->agents[0]->params.nbGenerations;
//             int filledPart = (int)((double)ratio * (double)barLength);
//             // filled part
//             for (int i = 0; i < filledPart; i++) {
//                 printf("%c", (char)219);
//             }

//             // empty part
//             for (int i = filledPart; i < barLength; i++) {
//                 printf(" ");
//             }

//             printf("] %4.2f%%", ratio * 100.00);
//         }
//     }

//     if (printProgressBar) {
//         if (!altTraining) {
//             printf("\nTraining completed\n");
//         }
//         else {
//             printf("\nTraining alted at generation %" PRIu64 ".\n",
//                    generationNumber);
//         }
//     }
//     return generationNumber;
// }

// template <class BaseLearningAgent>
// void Learn::FLAgentManager<BaseLearningAgent>::connectAgentsPseudoRandomly()
// {
//     Mutator::RNG rng;

//     for (FLAgent<BaseLearningAgent>* agent : this->agents) {
//         // Ensure each agent has at least one receiving connection
//         FLAgent<BaseLearningAgent>* sendingAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];
//         // Put agent in the sendingAgent's list of connections (sendingAgent can send data to agent)
//         this->connectAgents(sendingAgent, agent,false);

//         // Determine the number of additional connections
//         uint64_t nbAdditionalConnections = rng.getUnsignedInt64(0,agent->params.maxNbOfConnections);

//         // Connect pseudorandomly up to maxNbOfConnections in both directions
//         for (int i = 0; i < nbAdditionalConnections && this->agentConnections[agent].size() < agent->params.maxNbOfConnections ; ++i) {
//             FLAgent<BaseLearningAgent>* targetAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];

//             // Avoid self-connection 
//             while (targetAgent == agent ) {
//                 targetAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];
//             }
//             //avoid exceeding maxNbOfConnections for targetAgent
//             if (this->agentConnections[targetAgent].size() >= agent->params.maxNbOfConnections){
//                 this->connectAgents(agent, targetAgent,false);
//             }else{
//                 this->connectAgents(agent, targetAgent,true);
//             }          
//         }
//     }
// }


// template <class BaseLearningAgent>
// void Learn::FLAgentManager<BaseLearningAgent>::exchangeBestBranchs()
// {
//     for (FLAgent<BaseLearningAgent>* sender : this->agents) {
//         for (FLAgent<BaseLearningAgent>* receiver : this->agentConnections[sender]) {
//             // Skip if the sender and receiver are the same
//             if (sender == receiver) {
//                 continue;
//             }

//             // Sender sends its best root to the receiver
//             receiver->setBestBranch((TPG::TPGVertex*) sender->getBestRoot().first);
//         }
//     }
// }

// template <class BaseLearningAgent>
// void Learn::FLAgentManager<BaseLearningAgent>::connectAgents(FLAgent<BaseLearningAgent>* agent1, FLAgent<BaseLearningAgent>* agent2,
//                                           bool bothDer)
// {
//     agentConnections[agent1].insert(agent2);
//     // If bothDer is true, then we need to make agents connected in both directions
//     if (bothDer)
//     {
//         agentConnections[agent2].insert(agent1);
//     }
// }
