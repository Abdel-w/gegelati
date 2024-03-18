/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2019 - 2023) :
 *
 * Karol Desnos <kdesnos@insa-rennes.fr> (2019 - 2022)
 * Nicolas Sourbier <nsourbie@insa-rennes.fr> (2019 - 2020)
 * Pierre-Yves Le Rolland-Raumer <plerolla@insa-rennes.fr> (2020)
 * Quentin Vacher <qvacher@insa-rennes.fr> (2023)
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
#include <queue>

#include "data/hash.h"
#include "learn/evaluationResult.h"
#include "mutator/rng.h"
#include "mutator/tpgMutator.h"
#include "mutator/BranchMutator.h"
#include "tpg/tpgExecutionEngine.h"

#include "learn/learningAgent.h"
#include "learn/FLAgent.h"

// template <class BaseLearningAgent>
// std::vector<TPG::TPGVertex*> Learn::FLAgent<BaseLearningAgent>::getBestBranch() {
//     return this->bestBranchs;
// } 

//template <class BaseLearningAgent>
//void Learn::FLAgent<BaseLearningAgent>::setBestBranch(TPG::TPGVertex* rootTeam) {
//    this->bestBranchs.push_back(rootTeam);
//}

// template <class BaseLearningAgent>
// void Learn::FLAgent<BaseLearningAgent>::emptyBranchs()
// {
//     size_t count =this->bestBranchs.size();
//     for (size_t i = 0; i < count ; i++)
//     {
//         this->bestBranchs.pop_back();
//     }    
// }

// template <class BaseLearningAgent>
// uint64_t Learn::FLAgent<BaseLearningAgent>::train(volatile bool& altTraining,
//                                bool printProgressBar)
// {
//     const int barLength = 50;
//     uint64_t generationNumber = 0;

//     uint64_t aggregationNumber = 0;

//     while (!altTraining && generationNumber < this->params.nbGenerations) {
//         // Train one generation
//         if (generationNumber == this->params.nbGenerationPerAggregation * (aggregationNumber+1))
//         {
//             //copy all received branchs in the TPGGraph
//             std::for_each(this->bestBranchs.begin(),this->bestBranchs.end(),
//                             [this](TPG::TPGVertex* b){
//                                 Mutator::BranchMutator::copyBranch(b, *this->getTPGGraph());
//                             });

//             //Empty Epmty the bestBranchs vector to get ready to receive new ones
//             this->emptyBranchs();

//             aggregationNumber++;
//         }
        
//         this->trainOneGeneration(generationNumber);
//         generationNumber++;
        
//         // Print progressBar (homemade, probably not ideal)
//         if (printProgressBar) {
//             printf("\rTraining ["); // back
//             // filling ratio
//             double ratio =
//                 (double)generationNumber / (double)this->params.nbGenerations;
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
