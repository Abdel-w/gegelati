/**
 * Copyright or � or Copr. IETR/INSA - Rennes (2019 - 2023) :
 *
 * QuentinVacher <98522623+QuentinVacher-rl@users.noreply.github.com> (2023)
 * Karol Desnos <kdesnos@insa-rennes.fr> (2019 - 2022)
 * Nicolas Sourbier <nsourbie@insa-rennes.fr> (2020)
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

#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>

#include "log/laBasicLogger.h"

#include "tpg/instrumented/tpgActionInstrumented.h"
#include "tpg/instrumented/tpgEdgeInstrumented.h"
#include "tpg/instrumented/tpgInstrumentedFactory.h"
#include "tpg/instrumented/tpgTeamInstrumented.h"
#include "tpg/instrumented/tpgVertexInstrumentation.h"
#include "tpg/policyStats.h"
#include "tpg/tpgGraph.h"

#include "instructions/addPrimitiveType.h"
#include "mutator/rng.h"
#include "mutator/tpgMutator.h"

#include "learn/learningAgent.h"
#include "learn/FLAgent.h"
#include "learn/learningEnvironment.h"
#include "learn/learningParameters.h"
#include "learn/parallelLearningAgent.h"
#include "learn/stickGameWithOpponent.h"


class FLAgentTest : public ::testing::Test
{
  protected:
    Instructions::Set set;
    StickGameWithOpponent le;
    Learn::LearningParameters params;

    virtual void SetUp()
    {
        set.add(*(new Instructions::AddPrimitiveType<int>()));
        set.add(*(new Instructions::AddPrimitiveType<double>()));

        // Proba as in Kelly's paper
        params.mutation.tpg.maxInitOutgoingEdges = 3;
        params.mutation.prog.maxProgramSize = 96;
        params.mutation.tpg.nbRoots = 15;
        params.mutation.tpg.pEdgeDeletion = 0.7;
        params.mutation.tpg.pEdgeAddition = 0.7;
        params.mutation.tpg.pProgramMutation = 0.2;
        params.mutation.tpg.pEdgeDestinationChange = 0.1;
        params.mutation.tpg.pEdgeDestinationIsAction = 0.5;
        params.mutation.tpg.maxOutgoingEdges = 4;
        params.mutation.prog.pAdd = 0.5;
        params.mutation.prog.pDelete = 0.5;
        params.mutation.prog.pMutate = 1.0;
        params.mutation.prog.pSwap = 1.0;
        params.mutation.prog.pConstantMutation = 0.5;
        params.mutation.prog.minConstValue = 0;
        params.mutation.prog.maxConstValue = 1;
    }

    virtual void TearDown()
    {
        delete (&set.getInstruction(0));
        delete (&set.getInstruction(1));
    }
};

TEST_F(FLAgentTest, Constructor)
{
    Learn::FLAgent* la;

    ASSERT_NO_THROW(la = new Learn::FLAgent(le, set, params))
        << "Construction of the learningAgent failed.";

    ASSERT_NO_THROW(delete la) << "Destruction of the LearningAgent failed.";
}

TEST_F(FLAgentTest, Train)
{
    params.archiveSize = 50;
    params.archivingProbability = 0.5;
    params.maxNbActionsPerEval = 11;
    params.nbIterationsPerPolicyEvaluation = 5;
    params.ratioDeletedRoots = 0.2;
    params.nbGenerations =3;
    params.nbGenerationPerAggregation =2;

    Learn::LearningAgent la1(le, set, params);
    Learn::FLAgent la2(le, set, params);

    la1.init();
    la2.init();
    bool alt = false;

    ASSERT_NO_THROW(la1.train(alt, true))
        << "Training a TPG for several generation should not fail.";

    ASSERT_NO_THROW(la2.train(alt, true,la1.getBestRoot().first))
        << "Training a TPG for several generation should not fail.";

    // File::TPGGraphDotExporter tpg1(
    //     "C:\\Users\\abdrissi\\Documents\\GitHub\\tpgbranch.dot",
    //     *(la1.getTPGGraph()));
    // File::TPGGraphDotExporter tpg2(
    //     "C:\\Users\\abdrissi\\Documents\\GitHub\\tpgRes.dot",
    //     *(la2.getTPGGraph()));

    // tpg1.print();
    // tpg2.print();
    alt = true;
    ASSERT_NO_THROW(la1.train(alt, true))
        << "Using the boolean reference to stop the training should not fail.";
    ASSERT_NO_THROW(la2.train(alt, true, la1.getBestRoot().first))
        << "Using the boolean reference to stop the training should not fail.";
}
