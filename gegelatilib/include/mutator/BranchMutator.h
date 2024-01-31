#pragma once
#include <unordered_map>
#include "archive.h"
#include "mutator/mutationParameters.h"
#include "mutator/tpgMutator.h"
#include "tpg/tpgGraph.h"
#include <unordered_set>
namespace Mutator {
    namespace BranchMutator {

        // Copy the branch from the original graph to the copied graph
        void copyBranch(const TPG::TPGVertex* bestRoot,
                                 TPG::TPGGraph& targetGraph);

         // Helper function to copy a TPG team and its outgoing edges recursively
        void copyTeamAndEdges(const TPG::TPGVertex* bestRoot,
                              TPG::TPGGraph& targetGraph,
                              std::unordered_map<const TPG::TPGVertex*,
                                                 TPG::TPGVertex*>& vertexMap);

        // Helper function to get All vertices from rootTeam recursively
        void getAllVerticesFromTeam(const TPG::TPGVertex* rootTeam, 
                            std::unordered_set<const TPG::TPGVertex*>&  visited);
                            
        // Helper function to get All Edges from rootTeam recursively
        void getAllEdgesFromTeam(const TPG::TPGVertex* rootTeam, 
                            std::unordered_set<const TPG::TPGEdge*>&  visited);
    };                                // namespace BranchMutator
};                                    // namespace Mutator
