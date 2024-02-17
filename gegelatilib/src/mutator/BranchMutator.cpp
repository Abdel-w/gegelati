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

void Mutator::BranchMutator::copyBranch(const TPG::TPGVertex*  bestBranch, 
                                        TPG::TPGGraph& targetGraph   )
{
    // Map to keep track of the correspondence between original and copied
    // vertices
    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*> vertexMap;

    // Copy the root team and its outgoing edges
    copyTeamAndEdges(bestBranch, targetGraph, vertexMap);

}


void Mutator::BranchMutator::copyTeamAndEdges(
    const TPG::TPGVertex*  bestBranch, TPG::TPGGraph& targetGraph,
    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*>& vertexMap)
{
    //Copy originalGraph's TPGVertex into targetGraph 
    std::unordered_set<const TPG::TPGVertex*> vertices;
    getAllVerticesFromTeam(bestBranch, vertices);
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
                    bool exist;                 
                    vertexMap[originalTeam] = 
                        (TPG::TPGVertex*)(*targetGraph.findAction(
                            ((TPG::TPGAction*)originalTeam)->getActionID(), exist));
                    //if the TPGAction doesn't exist creat new one with the same ActionID
                    if (!exist)
                    {
                        vertexMap[originalTeam] = (TPG::TPGVertex*)&targetGraph.addNewAction(((TPG::TPGAction*)originalTeam)->getActionID());
                    }
                    
            }       
        }else {
            vertexMap[originalTeam] = (TPG::TPGVertex*) originalTeam;
        }
    }

    // Copy originalGraph's TPGEdges into targetGraph 
    std::unordered_set<const TPG::TPGEdge*> edges;
    getAllEdgesFromTeam(bestBranch, edges);
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

void Mutator::BranchMutator::getAllVerticesFromTeam(const TPG::TPGVertex*  bestBranch, std::unordered_set<const TPG::TPGVertex*>&  visited) {

    // Mark the current rootTeam as visited
    visited.insert(bestBranch);

    // Iterate through the outgoing edges of the current team
    for (const TPG::TPGEdge* edge : bestBranch->getOutgoingEdges()) {
        const TPG::TPGVertex* nextVertex = edge->getDestination();

        // If the next vertex is not visited, recursively call the function
        if (visited.find(nextVertex) == visited.end()) {
            getAllVerticesFromTeam(nextVertex,visited);
        }
    }
}


void Mutator::BranchMutator::getAllEdgesFromTeam(const TPG::TPGVertex*  bestBranch, std::unordered_set<const TPG::TPGEdge*>&  visited){

    // Iterate through the outgoing edges of the current team
    for (const TPG::TPGEdge* edge : bestBranch->getOutgoingEdges()) {
        // If the next edge is not visited, add it to the set and recursively call the function
        if (visited.find(edge) == visited.end()) {
            visited.insert(edge);
            getAllEdgesFromTeam(edge->getDestination(), visited);
        }
    }
}
