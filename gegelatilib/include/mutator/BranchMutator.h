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

        // Helper function to copy an outgoing edge
        void copyEdge(const std::unique_ptr<TPG::TPGEdge> &originalEdge,
                      TPG::TPGGraph& targetGraph,
                      std::unordered_map<const TPG::TPGVertex*,
                                         TPG::TPGVertex*>& vertexMap);

        /**
         * \brief Mutates a Branch by stochastically adding, deleting, and
         * mutating the Program and destination of outgoing TPGEdge.
         *
         * This function successively:
         * - removes outgoing TPGEdge from all the TPGTeam of the branch,
         * - adds outgoing TPGEdge to all the TPGTeam of the branch
         * - selects the Program whose behavior should be mutated, an mutates
         * destination of outgoing TPGEdge. Mutation of the Program behavior is
         * not performed in this function. Program to mutate are instead stored
         * in the newPrograms list for later mutation (potentially in parallel).
         * Probabilities in given MutationParameters are used to control
         * the application of previous mutations.
         *
         * \param[in,out] graph the TPGGraph within which the team and edge are
         *                stored.
         * \param[in] archive Archive used to assess the uniqueness of the
         *            mutated Program behavior.
         * \param[in] root The copied branch from the neighboring TPG.
         * \param[in] preExistingTeams the TPGTeam candidates for destination.
         * \param[in] preExistingActions the TPGAction candidates for
         *            destination.
         * \param[in,out] newPrograms List of new Program created during
         *                mutations of the TPGTeam. The behavior of these
         *                Program must be mutated to complete the mutation
         *                process.
         * \param[in] params Probability parameters for the mutation.
         * \param[in] rng Random Number Generator used in the mutation process.
         *
        void mutateBranch(
            TPG::TPGGraph& graph, const Archive& archive,
            const TPG::TPGTeam& root,
            const std::vector<const TPG::TPGTeam*>& preExistingTeams,
            const std::vector<const TPG::TPGAction*>& preExistingActions,
            const std::list<const TPG::TPGEdge*>& preExistingEdges,
            std::list<std::shared_ptr<Program::Program>>& newPrograms,
            const Mutator::MutationParameters& params, Mutator::RNG& rng);*/

    };                                // namespace BranchMutator
};                                    // namespace Mutator
