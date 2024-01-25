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
        if (targetGraph.hasVertex(*originalTeam) == false) {
            // Create a new Vertex (Team/Action)
            // (at the end of the vertices list)
            if (dynamic_cast<const TPG::TPGTeam*>(originalTeam) != nullptr) {
                vertexMap[originalTeam] =
                    (TPG::TPGVertex*)&targetGraph.addNewTeam();
            }
            else if (dynamic_cast<const TPG::TPGAction*>(originalTeam) !=
                     nullptr) {                  
                    vertexMap[originalTeam] = 
                        (TPG::TPGVertex*)(*targetGraph.findAction(
                            ((TPG::TPGAction*)originalTeam)->getActionID()));
            }       
        }else {
            vertexMap[originalTeam] = (TPG::TPGVertex*) originalTeam;
        }
    }

    // Copy originalGraph's TPGEdges into targetGraph 
    for (auto& originalEdge : originalGraph.getEdges()) {
        // Copy the destination team/action
        const TPG::TPGVertex* originalDestination =
            originalEdge->getDestination();
        TPG::TPGVertex* copiedDestination = vertexMap[originalDestination];

        // Copy the program
        const Program::Program& originalProgram = originalEdge->getProgram();
        auto copiedProgram = std::make_shared<Program::Program>(
            Program::Program(originalProgram));

        // Create the new edge
        targetGraph.addNewEdge(*vertexMap[originalEdge->getSource()],
                               *copiedDestination, copiedProgram);
    }      

}
