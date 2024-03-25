#pragma once
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <vector>

#include "learn/FLAgent.h"
#include "learn/learningEnvironment.h"
#include "learn/learningParameters.h"
#include "learn/learningAgent.h"
#include "learn/classificationLearningAgent.h"
#include "learn/parallelLearningAgent.h"

#include "tpg/tpgFactory.h"

#include "instructions/instruction.h"
#include "instructions/set.h"


namespace Learn {

    /**
     * \brief Class used to control the learning steps of a TPGGraph within
     * a given LearningEnvironment using two agents.
     */
    template<class BaseLearningAgent>
    class FLAgentManager {
    public:
        ///number of agents
        int nbAgents;
        /// FLAgents
        std::vector<FLAgent<BaseLearningAgent>*> agents;
        /// connections between agents
        std::unordered_map<FLAgent<BaseLearningAgent>*, std::unordered_set<FLAgent<BaseLearningAgent>*>> agentConnections;
    protected:
        /**
         * \brief Connects two FLAgents.
         *
         * This method establishes a connection between two FLAgents. By default, the connection is unidirectional,
         * but it can be made bidirectional by setting the "bothDer" parameter to true.
         *
         * \param[in] agent1 The first FLAgent.
         * \param[in] agent2 The second FLAgent.
         * \param[in] bothDer Flag indicating whether the connection is bidirectional (default is false).
         */
        void connectAgents(FLAgent<BaseLearningAgent>* agent1, FLAgent<BaseLearningAgent>* agent2,bool bothDer = false); 

    public:
        /**
         * \brief Constructor for FLAgentManager.
         *
         * \param[in] le The LearningEnvironment for the TPG.
         * \param[in] iSet Set of Instruction used to compose Programs in the
         *            learning process.
         * \param[in] p The LearningParameters for the LearningAgent.
         * \param[in] factory The TPGFactory used to create the TPGGraph. A
         * default TPGFactory is used if none is provided.
         * \param[in] len The number of agents to be used in the Federated 
         * learning process.
         */
        FLAgentManager(const int len,
                        LearningEnvironment& le,
                        const Instructions::Set& iSet,
                        const LearningParameters& p,
                        const TPG::TPGFactory& factory = TPG::TPGFactory()){
                        // Make sure there is at least 2 agents
                        nbAgents = (len < 2) ? 2 : len;
                        //create nbAgents FLAgents 
                        for (int i = 0; i < nbAgents; i++)
                        {
                            agents.push_back(new FLAgent<BaseLearningAgent>(le,iSet,p,factory));
                        }        
        }

        /**
         * \brief Constructor for FLAgentManager.
         *
         * \param[in] cle The ClassificationLearningAgent for the TPG.
         * \param[in] iSet Set of Instruction used to compose Programs in the
         *            learning process.
         * \param[in] p The LearningParameters for the LearningAgent.
         * \param[in] factory The TPGFactory used to create the TPGGraph. A
         * default TPGFactory is used if none is provided.
         * \param[in] len The number of agents to be used in the Federated 
         * learning process.
         */
        FLAgentManager(const int len,
                        ClassificationLearningEnvironment& cle,
                        const Instructions::Set& iSet,
                        const LearningParameters& p,
                        const TPG::TPGFactory& factory = TPG::TPGFactory()){
                        // Make sure there is at least 2 agents
                        nbAgents = (len < 2) ? 2 : len;
                        //create nbAgents FLAgents 
                        for (int i = 0; i < nbAgents; i++)
                        {
                            agents.push_back(new FLAgent<BaseLearningAgent>(cle,iSet,p,factory));
                        }        
        }
        /**
         * \brief Train both agents and exchange best branches
         *
         * The method trains the TPGGraph for a given number of generation,
         * unless the referenced boolean value becomes false (evaluated at each
         * generation).
         * Optionally, a simple progress bar can be printed within the terminal.
         * The TPGGraph is NOT (re)initialized before starting the training.
         *
         * \param[in] altTraining a reference to a boolean value that can be
         * used to halt the training process before its completion.
         * \param[in] printProgressBar select whether a progress bar will be
         * printed in the console. 
         * \return the number of completed generations.
         */
        uint64_t trainAndExchangeBestBranches(volatile bool& altTraining, bool printProgressBar);
        
        /**
         * \brief Pseudorandomly connects agents within the FLAgentManager.
         *
         * This method establishes connections between FLAgents pseudorandomly based on specified rules:
         * - Each agent has at least one receiving connection.
         * - No agent has more than "maxNbOfConnections" in both directions.
         */
        void connectAgentsPseudoRandomly(); 

        /**
         * \brief Exchange the best branch among all FLAgents in the FLAgentManager.
         *
         * This method iterates through all FLAgents in the FLAgentManager and exchanges the best root
         * among connected agents. Each FLAgent sends its best root to the connected agents,
         * and each agent receives the best root from its connected agents.
         */
        void exchangeBestBranchs();
    };
}  // namespace Learn


template <class BaseLearningAgent>
uint64_t Learn::FLAgentManager<BaseLearningAgent>::trainAndExchangeBestBranches(
    volatile bool& altTraining, bool printProgressBar)
{
    const int barLength = 50;
    uint64_t generationNumber = 0;

    uint64_t aggregationNumber = 0;

    while (!altTraining && generationNumber < this->agents[0]->params.nbGenerations) {
        // Train one generation
        if (generationNumber == this->agents[0]->params.nbGenerationPerAggregation * (aggregationNumber+1))
        {
            exchangeBestBranchs();
            //for each agent copy all received branchs in the TPGGraph
            std::for_each(this->agents.begin(),this->agents.end(),
                            [](Learn::FLAgent<BaseLearningAgent>* agent){

                                // copy all branchs 
                                for (auto branch : agent->getBestBranch()){
                                    Mutator::BranchMutator::copyBranch(branch, *(agent->getTPGGraph()));
                                } 
                                //Empty Epmty the bestBranchs vector to get ready to receive new ones
                                agent->emptyBranchs(); 
                            });
            aggregationNumber++;
        }
        for (auto agent : this->agents)
        {
            agent->trainOneGeneration(generationNumber);
        }
        generationNumber++;
        
        // Print progressBar (homemade, probably not ideal)
        if (printProgressBar) {
            printf("\rTraining ["); // back
            // filling ratio
            double ratio =
                (double)generationNumber / (double)this->agents[0]->params.nbGenerations;
            int filledPart = (int)((double)ratio * (double)barLength);
            // filled part
            for (int i = 0; i < filledPart; i++) {
                printf("%c", (char)219);
            }

            // empty part
            for (int i = filledPart; i < barLength; i++) {
                printf(" ");
            }

            printf("] %4.2f%%", ratio * 100.00);
        }
    }

    if (printProgressBar) {
        if (!altTraining) {
            printf("\nTraining completed\n");
        }
        else {
            printf("\nTraining alted at generation %" PRIu64 ".\n",
                   generationNumber);
        }
    }
    return generationNumber;
}

template <class BaseLearningAgent>
void Learn::FLAgentManager<BaseLearningAgent>::connectAgentsPseudoRandomly()
{
    Mutator::RNG rng;

    for (FLAgent<BaseLearningAgent>* agent : this->agents) {
        // Ensure each agent has at least one receiving connection
        FLAgent<BaseLearningAgent>* sendingAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];
        // Put agent in the sendingAgent's list of connections (sendingAgent can send data to agent)
        this->connectAgents(sendingAgent, agent,false);

        // Determine the number of additional connections
        uint64_t nbAdditionalConnections = rng.getUnsignedInt64(0,agent->params.maxNbOfConnections);

        // Connect pseudorandomly up to maxNbOfConnections in both directions
        for (int i = 0; i < nbAdditionalConnections && this->agentConnections[agent].size() < agent->params.maxNbOfConnections ; ++i) {
            FLAgent<BaseLearningAgent>* targetAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];

            // Avoid self-connection 
            while (targetAgent == agent ) {
                targetAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];
            }
            //avoid exceeding maxNbOfConnections for targetAgent
            if (this->agentConnections[targetAgent].size() >= agent->params.maxNbOfConnections){
                this->connectAgents(agent, targetAgent,false);
            }else{
                this->connectAgents(agent, targetAgent,true);
            }          
        }
    }
}


template <class BaseLearningAgent>
void Learn::FLAgentManager<BaseLearningAgent>::exchangeBestBranchs()
{
    for (FLAgent<BaseLearningAgent>* sender : this->agents) {
        for (FLAgent<BaseLearningAgent>* receiver : this->agentConnections[sender]) {
            // Skip if the sender and receiver are the same
            if (sender == receiver) {
                continue;
            }

            // Sender sends its best root to the receiver
            receiver->setBestBranch((TPG::TPGVertex*) sender->getBestRoot().first);
        }
    }
}

template <class BaseLearningAgent>
void Learn::FLAgentManager<BaseLearningAgent>::connectAgents(FLAgent<BaseLearningAgent>* agent1, FLAgent<BaseLearningAgent>* agent2,
                                          bool bothDer)
{
    agentConnections[agent1].insert(agent2);
    // If bothDer is true, then we need to make agents connected in both directions
    if (bothDer)
    {
        agentConnections[agent2].insert(agent1);
    }
}
