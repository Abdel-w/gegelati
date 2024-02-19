#pragma once
#include <cstdint>

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
    public:
        /// FLAgent1
        FLAgent agent1;
        /// FLAgent1
        FLAgent agent2;

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
         */
        FLAgentManager(LearningEnvironment& le,
                       const Instructions::Set& iSet,
                       const LearningParameters& p,
                       const TPG::TPGFactory& factory = TPG::TPGFactory())
            : agent1(le, iSet, p, factory),
              agent2(le, iSet, p, factory) {
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
    };
}  // namespace Learn