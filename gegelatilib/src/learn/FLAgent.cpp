#include <inttypes.h>
#include <queue>

#include "data/hash.h"
#include "learn/evaluationResult.h"
#include "mutator/rng.h"
#include "mutator/tpgMutator.h"
#include "mutator/BranchMutator.h"
#include "tpg/tpgExecutionEngine.h"

#include "learn/learningAgent.h"
#include "learn/FLAgent.h"

uint64_t Learn::FLAgent::train(volatile bool& altTraining,
                                     bool printProgressBar, const TPG::TPGVertex* bestBranch)
{
    const int barLength = 50;
    uint64_t generationNumber = 0;

    uint64_t aggregationNumber = 0;

    while (!altTraining && generationNumber < this->params.nbGenerations) {
        // Train one generation
        if (generationNumber == this->params.nbGenerationPerAggregation * (aggregationNumber+1))
        {
            //Mutator::BranchMutator::copyBranch(*bestBranch, *this->getTPGGraph());
   
        }
        
        trainOneGeneration(generationNumber);
        generationNumber++;

        // Print progressBar (homemade, probably not ideal)
        if (printProgressBar) {
            printf("\rTraining ["); // back
            // filling ratio
            double ratio =
                (double)generationNumber / (double)this->params.nbGenerations;
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
