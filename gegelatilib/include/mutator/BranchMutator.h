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
#pragma once
#include <unordered_map>
#include "archive.h"
#include "mutator/mutationParameters.h"
#include "mutator/tpgMutator.h"
#include "tpg/tpgGraph.h"
#include <unordered_set>
namespace Mutator {
    namespace BranchMutator {

        /**
         * \brief Copy the branch from the original graph to the copied graph
         */
        void copyBranch(const TPG::TPGVertex* bestBranch,
                                 TPG::TPGGraph& targetGraph);

         /**
          * \brief  Helper function to copy a TPG team and its outgoing edges recursively
          */
        void copyTeamAndEdges(const TPG::TPGVertex*  bestBranch,
                              TPG::TPGGraph& targetGraph,
                              std::unordered_map<const TPG::TPGVertex*,
                                                 TPG::TPGVertex*>& vertexMap);

        /**
         * \brief Helper function to get All vertices from rootTeam recursively
         */
        void getAllVerticesFromTeam(const TPG::TPGVertex*  bestBranch, 
                            std::unordered_set<const TPG::TPGVertex*>&  visited);
                            
        /** 
         * \brief Helper function to get All Edges from rootTeam recursively
         */
        void getAllEdgesFromTeam(const TPG::TPGVertex*  bestBranch, 
                                std::unordered_set<const TPG::TPGEdge*>&  visited);
    };                                // namespace BranchMutator
};                                    // namespace Mutator
