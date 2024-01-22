#pragma once
#include "learn/learningAgent.h"


namespace Learn {

	class FLAgent: public LearningAgent
    {
      public:
        /**
         * \brief Constructor for FLAgent.
         *
         * \param[in] le The LearningEnvironment for the TPG.
         * \param[in] iSet Set of Instruction used to compose Programs in the
         *            learning process.
         * \param[in] p The LearningParameters for the LearningAgent.
         * \param[in] factory The TPGFactory used to create the TPGGraph. A
         * default TPGFactory is used if none is provided.
         */
        FLAgent(LearningEnvironment& le, const Instructions::Set& iSet,
                      const LearningParameters& p,
                      const TPG::TPGFactory& factory = TPG::TPGFactory())
            : LearningAgent(le, iSet, p, factory)
        {
        }
        
    };

}
