#pragma once
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <vector>

#include "learn/FLAgent.h"
#include "learn/learningEnvironment.h"
#include "learn/learningParameters.h"
#include "learn/learningAgent.h"

#include "tpg/tpgFactory.h"

#include "instructions/instruction.h"
#include "instructions/set.h"


namespace Learn {

    /**
     * \brief Class used to control the learning steps of a TPGGraph within
     * a given LearningEnvironment using two agents.
     */
    class FLAgentManager {
    private:
        ///number of agents
        int nbAgents;
        /// FLAgents
        std::vector<FLAgent*> agents;
        /// connections between agents
        std::unordered_map<FLAgent*, std::unordered_set<FLAgent*>> agentConnections;
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
        void connectAgents(FLAgent* agent1, FLAgent* agent2,bool bothDer = false) {
            agentConnections[agent1].insert(agent2);
            // If bothDer is true, then we need to make agents connected in both directions
            if (bothDer)
            {
                agentConnections[agent2].insert(agent1);
            }
        }

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
        FLAgentManager(LearningEnvironment& le,
                       const Instructions::Set& iSet,
                       const LearningParameters& p,
                       const TPG::TPGFactory& factory = TPG::TPGFactory(),
                       const int len = 2){
                        // Make sure there is at least 2 agents
                        nbAgents = (len < 2) ? 2 : len;
                        //create nbAgents FLAgents 
                        for (int i = 0; i < nbAgents; i++)
                        {
                            agents.push_back(new FLAgent(le,iSet,p,factory));
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
        void connectAgentsPseudoRandomly() {
            Mutator::RNG rng;

            for (FLAgent* agent : this->agents) {
                // Ensure each agent has at least one receiving connection
                FLAgent* sendingAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];
                // Put agent in the sendingAgent's list of connections (sendingAgent can send data to agent)
                this->connectAgents(sendingAgent, agent,false);

                // Determine the number of additional connections
                uint64_t nbAdditionalConnections = rng.getUnsignedInt64(0,agent->params.maxNbOfConnections);

                // Connect pseudorandomly up to maxNbOfConnections in both directions
                for (int i = 0; i < nbAdditionalConnections && this->agentConnections[agent].size() < agent->params.maxNbOfConnections ; ++i) {
                    FLAgent* targetAgent = this->agents[rng.getUnsignedInt64(0, this->nbAgents - 1)];

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
    };
}  // namespace Learn