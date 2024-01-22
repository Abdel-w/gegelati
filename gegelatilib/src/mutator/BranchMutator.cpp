#include <memory>
#include "tpg/tpgAction.h"
#include "tpg/tpgEdge.h"
#include "tpg/tpgGraph.h"
#include "tpg/tpgTeam.h"

#include "mutator/BranchMutator.h"
#include "mutator/mutationParameters.h"
#include "mutator/programMutator.h"
#include "mutator/rng.h"
#include "mutator/tpgMutator.h"


void Mutator::BranchMutator::copyBranch(TPG::TPGGraph& originalGraph,
                                        TPG::TPGGraph& targetGraph   )
{
    // Map to keep track of the correspondence between original and copied
    // vertices
    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*> vertexMap;

    // Copy the root team and its outgoing edges
    copyTeamAndEdges(originalGraph, targetGraph, vertexMap);
}


void Mutator::BranchMutator::copyTeamAndEdges(
    TPG::TPGGraph& originalGraph, TPG::TPGGraph& targetGraph,
    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*>& vertexMap)
{
    //Copy originalGraph's TPGVertex into targetGraph 
    for (auto originalTeam : originalGraph.getVertices()) {
        if (targetGraph.hasVertex(*originalTeam) == false) {//
            TPG::TPGVertex* copiedTeam;
            // Create a new Vertex (Team/Action)
            // (at the end of the vertices list)
            if (dynamic_cast<const TPG::TPGTeam*>(originalTeam) != nullptr) {
                    copiedTeam = (TPG::TPGVertex*) &targetGraph.addNewTeam();
            }
            else if (dynamic_cast<const TPG::TPGAction*>(originalTeam) !=
                     nullptr) {
                    copiedTeam = (TPG::TPGVertex*) &targetGraph.addNewAction(
                        ((TPG::TPGAction*)originalTeam)->getActionID());
            }

            vertexMap[originalTeam] = copiedTeam;
        }else {
            vertexMap[originalTeam] = (TPG::TPGVertex*) originalTeam;
        }
    }

    // Copy originalGraph's TPGEdges into targetGraph 
    for (auto &originalEdge : originalGraph.getEdges())         
        copyEdge(originalEdge, targetGraph, vertexMap);

}


void Mutator::BranchMutator::copyEdge(
    const std::unique_ptr<TPG::TPGEdge> &originalEdge, TPG::TPGGraph& targetGraph,
    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*>& vertexMap)
{
    // Copy the destination team/action
    const TPG::TPGVertex* originalDestination = originalEdge->getDestination();
    TPG::TPGVertex* copiedDestination = vertexMap[originalDestination];

    // Copy the program
    const Program::Program& originalProgram = originalEdge->getProgram();
    auto copiedProgram= std::make_shared<Program::Program>(Program::Program(originalProgram));

    // Create the new edge
    targetGraph.addNewEdge(*vertexMap[originalEdge->getSource()],
                           *copiedDestination, copiedProgram);
}

/*
void Mutator::BranchMutator::mutateBranch(
    TPG::TPGGraph& graph, const Archive& archive, const TPG::TPGTeam& root,
    const std::vector<const TPG::TPGTeam*>& preExistingTeams,
    const std::vector<const TPG::TPGAction*>& preExistingActions,
    const std::list<const TPG::TPGEdge*>& preExistingEdges,
    std::list<std::shared_ptr<Program::Program>>& newPrograms,
    const Mutator::MutationParameters& params, Mutator::RNG& rng)
{
    // Perform mutations on the copied branch
    // You can use methods from the TPGMutator class or define new mutations
    // based on your requirements.
    Mutator::TPGMutator::mutateTPGTeam(copiedGraph, Archive(), rootTeam, {}, {},
                                       {}, {}, params, rng);
    // Additional mutations as needed...
    
}
*/