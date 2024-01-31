#pragma once
#include "learn/learningAgent.h"


namespace Learn {

	class FLAgent: public LearningAgent
    {
      private:
      //the best branch received from another tpgGraph
      TPG::TPGVertex*  bestBranch;
      public:
      TPG::TPGVertex* getBestBranch();

      void setBestBranch(TPG::TPGVertex* rootTeam);
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
        /**
         * \brief Train the TPGGraph for a given number of generation.
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
         * printed in the console. \return the number of completed generations.
         */
        uint64_t train(volatile bool& altTraining, bool printProgressBarr,
                       const TPG::TPGVertex* Branch);       
        
    };

}
