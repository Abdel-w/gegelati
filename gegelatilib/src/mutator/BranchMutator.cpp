#include <memory>
#include "tpg/tpgAction.h"
#include "tpg/tpgEdge.h"
#include "tpg/tpgGraph.h"
#include "tpg/tpgTeam.h"
#include "tpg/tpgVertex.h"

#include "mutator/BranchMutator.h"
#include "mutator/mutationParameters.h"
#include "mutator/programMutator.h"
#include "mutator/rng.h"
#include "mutator/tpgMutator.h"

void Mutator::BranchMutator::copyBranch(const TPG::TPGVertex* bestRoot,
                                        TPG::TPGGraph& targetGraph   )
{
    // Map to keep track of the correspondence between original and copied
    // vertices
    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*> vertexMap;

    // Copy the root team and its outgoing edges
    copyTeamAndEdges(bestRoot, targetGraph, vertexMap);

}


void Mutator::BranchMutator::copyTeamAndEdges(
    const TPG::TPGVertex* bestRoot, TPG::TPGGraph& targetGraph,
    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*>& vertexMap)
{
    //Copy originalGraph's TPGVertex into targetGraph 
    std::unordered_set<const TPG::TPGVertex*> vertices;
    getAllVerticesFromTeam(bestRoot, vertices);
    for (auto originalTeam : vertices) {
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
    std::unordered_set<const TPG::TPGEdge*> edges;
    getAllEdgesFromTeam(bestRoot, edges);
    for (auto& originalEdge : edges) {
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

void Mutator::BranchMutator::getAllVerticesFromTeam(const TPG::TPGVertex* rootTeam, std::unordered_set<const TPG::TPGVertex*>&  visited) {

    // Mark the current rootTeam as visited
    visited.insert(rootTeam);

    // Iterate through the outgoing edges of the current team
    for (const TPG::TPGEdge* edge : rootTeam->getOutgoingEdges()) {
        const TPG::TPGVertex* nextVertex = edge->getDestination();

        // If the next vertex is not visited, recursively call the function
        if (visited.find(nextVertex) == visited.end()) {
            getAllVerticesFromTeam(nextVertex,visited);
        }
    }
}


void Mutator::BranchMutator::getAllEdgesFromTeam(const TPG::TPGVertex* rootTeam, std::unordered_set<const TPG::TPGEdge*>&  visited) {

    // Iterate through the outgoing edges of the current team
    for (const TPG::TPGEdge* edge : rootTeam->getOutgoingEdges()) {
        const TPG::TPGVertex* nextVertex = edge->getDestination();

        // If the next edge is not visited, add it to the set and recursively call the function
        if (visited.find(edge) == visited.end()) {
            visited.insert(edge);
            getAllEdgesFromTeam(edge->getDestination(), visited);
        }
    }
}
