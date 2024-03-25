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
#include "instructions/lambdaInstruction.h"
#include "mutator/rng.h"
#include "mutator/tpgMutator.h"

#include "learn/learningAgent.h"
#include "learn/FLAgent.h"
#include "learn/learningEnvironment.h"
#include "learn/learningParameters.h"
#include "learn/parallelLearningAgent.h"
#include "learn/FakeFederatedLearningEnvironement.h"

#include "file/tpgGraphDotExporter.h"


class trainFLAgentTest : public ::testing::Test
{
  protected:

    Learn::FakeFederatedLearningEnvironement* le;
    std::vector<double> fakeData;
    Instructions::Set set;
    Environment* e = NULL;
    std::vector<std::reference_wrapper<const Data::DataHandler>> vect;
    Learn::LearningParameters params;

    virtual void SetUp()
    {
        fakeData.push_back(1.0);
        fakeData.push_back(2.0);
        fakeData.push_back(3.0);
        fakeData.push_back(4.0);

        std::function<double(double, double)> minus = [](double a, double b) {
            return a - b;
        };
        std::function<double(double, double)> plus = [](double a, double b) {
            return a + b;
        };
        set.add(*(new Instructions::LambdaInstruction<double, double>(minus)));
        set.add(*(new Instructions::LambdaInstruction<double, double>(plus)));
                 
        le = new Learn::FakeFederatedLearningEnvironement(2,fakeData);
        vect = le->getDataSources();        
        e = new Environment(set, vect, 8, 5);

        params.mutation.tpg.maxInitOutgoingEdges = 1;
        params.mutation.prog.maxProgramSize = 1;
        params.mutation.tpg.nbRoots = 1;
        params.mutation.tpg.pEdgeDeletion = 0.7;
        params.mutation.tpg.pEdgeAddition = 0.7;
        params.mutation.tpg.pProgramMutation = 0.2;
        params.mutation.tpg.pEdgeDestinationChange = 0.1;
        params.mutation.tpg.pEdgeDestinationIsAction = 0.5;
        params.mutation.tpg.maxOutgoingEdges = 3;
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
TEST_F(trainFLAgentTest, behavioralTest_FL)
{
    params.archiveSize = 50;
    params.archivingProbability = 0.5;
    params.maxNbActionsPerEval = 11;
    params.nbIterationsPerPolicyEvaluation = 5;
    params.ratioDeletedRoots = 0.2;
    params.nbGenerations =2;
    params.nbGenerationPerAggregation =1;

  

    auto p0 = std::shared_ptr<Program::Program>(new Program::Program(*e));
    auto p1 = std::shared_ptr<Program::Program>(new Program::Program(*e));
    Program::Line& l0 = p0->addNewLine();
    Program::Line& l1 = p1->addNewLine();

    // L1: Register 0 = DataSource[1] + DataSource[0] =2+1
    l1.setDestinationIndex(0);
    l1.setOperand(0, 0, 1);
    l1.setOperand(1, 0, 0);
    l1.setInstructionIndex(1); 

    // L0: Register 0 = Datasource[3] - DataSource[2] = 4 - 3   
    l0.setDestinationIndex(0);
    l0.setOperand(0, 1, 1);
    l0.setOperand(1, 1, 2);
    l0.setInstructionIndex(0);

    TPG::TPGGraph branch(*e); 

    auto root = &branch.addNewTeam();       //      0     0
                                            //      |     |
    auto a0   = &branch.addNewAction(0);    //      `\   /'
    auto a1   = &branch.addNewAction(1);    //        \ /
    branch.addNewEdge(*root, *a0, p0);      //         0
    branch.addNewEdge(*root, *a1, p1);      //         

    ASSERT_EQ(branch.getNbVertices(), 3);

    Learn::FLAgent la(*le, set, params);  
    la.init();

    bool alt = false;
    
    ASSERT_NO_THROW(la.train(alt, true))
        << "Training a TPG for several generation should not fail.";

    alt = true;
    ASSERT_NO_THROW(la.train(alt, true))
        << "Using the boolean reference to stop the training should not fail.";
}