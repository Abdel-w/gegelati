#pragma once
#include <unordered_map>
#include "archive.h"
#include "mutator/mutationParameters.h"
#include "mutator/tpgMutator.h"
#include "tpg/tpgGraph.h"

namespace Mutator {
    namespace BranchMutator {

        // Copy the branch from the original graph to the copied graph
        void copyBranch(TPG::TPGGraph& originalGraph,
                                 TPG::TPGGraph& targetGraph);

         // Helper function to copy a TPG team and its outgoing edges recursively
        void copyTeamAndEdges(TPG::TPGGraph& originalGraph,
                              TPG::TPGGraph& targetGraph,
                              std::unordered_map<const TPG::TPGVertex*,
                                                 TPG::TPGVertex*>& vertexMap);

    };                                // namespace BranchMutator
};                                    // namespace Mutator
