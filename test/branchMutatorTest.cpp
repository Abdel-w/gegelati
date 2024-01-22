#include "mutator/BranchMutator.h"
#include "tpg/tpgGraph.h"
#include "environment.h"
#include "instructions/addPrimitiveType.h"
#include "instructions/multByConstant.h"
#include "instructions/instruction.h"
#include <gtest/gtest.h>

class BranchMutatorTest : public ::testing::Test
{
  protected:
    const size_t size1{24};
    const size_t size2{32};
    std::vector<std::reference_wrapper<const Data::DataHandler>> vect;
    Instructions::Set set;
    Environment* e = NULL;
    std::shared_ptr<Program::Program> progP0_o;
    std::shared_ptr<Program::Program> progP1_o;
    std::shared_ptr<Program::Program> progP2_o;
    std::shared_ptr<Program::Program> progP3_o;

    std::shared_ptr<Program::Program> progP0_t;
    std::shared_ptr<Program::Program> progP1_t;
    std::shared_ptr<Program::Program> progP2_t;

    virtual void SetUp()
    {
        vect.push_back(
            *(new Data::PrimitiveTypeArray<double>((unsigned int)size1)));
        vect.push_back(
            *(new Data::PrimitiveTypeArray<float>((unsigned int)size2)));

     
        set.add(*(new Instructions::MultByConstant<double>()));
        set.add(*(new Instructions::AddPrimitiveType<double>()));

        e = new Environment(set, vect, 8, 5);
        progP0_o = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP1_o = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP2_o = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP3_o = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP0_t = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP1_t = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP2_t = std::shared_ptr<Program::Program>(new Program::Program(*e));
    }

    virtual void TearDown()
    {
        delete e;
        delete (&(vect.at(0).get()));
        delete (&(vect.at(1).get()));
        delete (&set.getInstruction(0));
        delete (&set.getInstruction(1));
    }

    
};

TEST_F(BranchMutatorTest, CopyBranch)
{
    TPG::TPGGraph originalGraph(*e);
    const TPG::TPGTeam* root_o; // root team for the originalGraph
    const TPG::TPGTeam* team0_o; // for the originalGraph
    const TPG::TPGTeam* team1_o; 
    const TPG::TPGAction* a0_o;  
    const TPG::TPGAction* a1_o;

    TPG::TPGGraph targetGraph(*e);
    const TPG::TPGTeam* root_t; // root team for the targetGraph
    const TPG::TPGTeam* team_t; 
    const TPG::TPGAction* a0_t;//for the targetGraph
    const TPG::TPGAction* a1_t;

    root_o = &originalGraph.addNewTeam();                 //      0      0
    team0_o = &originalGraph.addNewTeam();                //      |      |
    team1_o = &originalGraph.addNewTeam();                //      0      0
    a0_o = &originalGraph.addNewAction(0);                //      '\    /'
    a1_o = &originalGraph.addNewAction(1);                //        \  /
    originalGraph.addNewEdge(*root_o, *team0_o, progP0_o);//         \/
    originalGraph.addNewEdge(*root_o, *team1_o, progP1_o);//          0
    originalGraph.addNewEdge(*team0_o, *a0_o, progP2_o);  //
    originalGraph.addNewEdge(*team1_o, *a1_o, progP3_o);  //

    root_t = &targetGraph.addNewTeam();                //          0     0
    team_t = &targetGraph.addNewTeam();                //          |     |
    a0_t = &targetGraph.addNewAction(0);               //          `\   /'
    a1_t = &targetGraph.addNewAction(1);               //            \ / 
    targetGraph.addNewEdge(*root_t, *team_t, progP0_t);//             0
    targetGraph.addNewEdge(*team_t, *a0_t, progP1_t);  //             |
    targetGraph.addNewEdge(*team_t, *a0_t, progP2_t);  //             0              


    ASSERT_EQ(originalGraph.getNbVertices(), 5);
    ASSERT_EQ(targetGraph.getNbVertices(), 4);

    Mutator::BranchMutator::copyBranch(originalGraph, targetGraph);

   
    ASSERT_EQ(originalGraph.getNbVertices(), 7);
    ASSERT_EQ(originalGraph.getNbRootVertices(), 2);
    ASSERT_EQ(originalGraph.getEdges().size(), 7);
}
