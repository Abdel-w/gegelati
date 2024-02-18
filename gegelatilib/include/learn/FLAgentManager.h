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

    class FLAgentManager {
    private:
        FLAgent agent1;
        FLAgent agent2;

    public:
        FLAgentManager(LearningEnvironment& le,
                       const Instructions::Set& iSet,
                       const LearningParameters& p,
                       const TPG::TPGFactory& factory = TPG::TPGFactory())
            : agent1(le, iSet, p, factory),
              agent2(le, iSet, p, factory) {
        }

        // Train both agents and exchange best branches
        uint64_t trainAndExchangeBestBranches(volatile bool& altTraining, bool printProgressBar);
    };
}  // namespace Learn