#include "mutator/BranchMutator.h"
#include "tpg/tpgGraph.h"
#include "environment.h"
#include "instructions/addPrimitiveType.h"
#include "instructions/multByConstant.h"
#include "instructions/instruction.h"
#include "file/tpgGraphDotExporter.h"
#include <gtest/gtest.h>
#include <unordered_set>

class BranchMutatorTest : public ::testing::Test
{
  protected:
    const size_t size1{24};
    const size_t size2{32};
    std::vector<std::reference_wrapper<const Data::DataHandler>> vect;
    Instructions::Set set;
    Environment* e = NULL;
    std::shared_ptr<Program::Program> progP0_t;
    std::shared_ptr<Program::Program> progP1_t;
    std::shared_ptr<Program::Program> progP2_t;
    std::shared_ptr<Program::Program> progP3_t;

    std::shared_ptr<Program::Program> progP0_o;
    std::shared_ptr<Program::Program> progP1_o;
    std::shared_ptr<Program::Program> progP2_o;

    const TPG::TPGTeam* root_t;  // root team for the targetGraph
    const TPG::TPGTeam* team0_t; // for the targetGraph
    const TPG::TPGTeam* team1_t;
    const TPG::TPGAction* a0_t;
    const TPG::TPGAction* a1_t;

    const TPG::TPGTeam* root_o; // root team for the originalGraph
    const TPG::TPGTeam* team_o;
    const TPG::TPGAction* a0_o; // for the originalGraph
    const TPG::TPGAction* a1_o;

    virtual void SetUp()
    {
        vect.push_back(
            *(new Data::PrimitiveTypeArray<double>((unsigned int)size1)));
        vect.push_back(
            *(new Data::PrimitiveTypeArray<float>((unsigned int)size2)));

     
        set.add(*(new Instructions::MultByConstant<double>()));
        set.add(*(new Instructions::AddPrimitiveType<double>()));

        e = new Environment(set, vect, 8, 5);
        progP0_t = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP1_t = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP2_t = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP3_t = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP0_o = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP1_o = std::shared_ptr<Program::Program>(new Program::Program(*e));
        progP2_o = std::shared_ptr<Program::Program>(new Program::Program(*e));

        
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
    TPG::TPGGraph targetGraph(*e);

    root_t = &targetGraph.addNewTeam();                 //      0      0
    team0_t = &targetGraph.addNewTeam();                //      |      |
    team1_t = &targetGraph.addNewTeam();                //      0      0
    a0_t = &targetGraph.addNewAction(0);                //      '\    /'
    a1_t = &targetGraph.addNewAction(1);                //        \  /
    targetGraph.addNewEdge(*root_t, *team0_t, progP0_t);//         \/
    targetGraph.addNewEdge(*root_t, *team1_t, progP1_t);//          0
    targetGraph.addNewEdge(*team0_t, *a0_t, progP2_t);  //
    targetGraph.addNewEdge(*team1_t, *a1_t, progP3_t);  //

    root_o = &originalGraph.addNewTeam();                //          0     0
    team_o = &originalGraph.addNewTeam();                //          |     |
    a0_o = &originalGraph.addNewAction(0);               //          `\   /'
    a1_o = &originalGraph.addNewAction(1);               //            \ / 
    originalGraph.addNewEdge(*root_o, *team_o, progP0_o);//             0
    originalGraph.addNewEdge(*team_o, *a0_o, progP1_o);  //             |
    originalGraph.addNewEdge(*team_o, *a1_o, progP2_o);  //             0              


    ASSERT_EQ(originalGraph.getNbVertices(), 4);
    ASSERT_EQ(targetGraph.getNbVertices(), 5);

    Mutator::BranchMutator::copyBranch((const TPG::TPGVertex*) root_o, targetGraph);
    /*File::TPGGraphDotExporter tpg("/home/abdrissi/Documents/res.dot",targetGraph);

    tpg.print();
    */
    ASSERT_EQ(targetGraph.getNbVertices(), 7);
    ASSERT_EQ(targetGraph.getNbRootVertices(), 2);
    ASSERT_EQ(targetGraph.getEdges().size(), 7);

    ASSERT_EQ(a0_t->getIncomingEdges().size(), 2);
    ASSERT_EQ(a1_t->getIncomingEdges().size(), 2);

    ASSERT_EQ(a0_t->getOutgoingEdges().size(), 0);
    ASSERT_EQ(a1_t->getOutgoingEdges().size(), 0);

}

TEST_F(BranchMutatorTest, copyTeamAndEdges)
{
    TPG::TPGGraph originalGraph(*e);
    TPG::TPGGraph targetGraph(*e);

    root_t = &targetGraph.addNewTeam();                  //      0      0
    team0_t = &targetGraph.addNewTeam();                 //      |      |
    team1_t = &targetGraph.addNewTeam();                 //      0      0
    a0_t = &targetGraph.addNewAction(0);                 //      '\    /'
    a1_t = &targetGraph.addNewAction(1);                 //        \  /
    targetGraph.addNewEdge(*root_t, *team0_t, progP0_t); //         \/
    targetGraph.addNewEdge(*root_t, *team1_t, progP1_t); //          0
    targetGraph.addNewEdge(*team0_t, *a0_t, progP2_t);   //
    targetGraph.addNewEdge(*team1_t, *a1_t, progP3_t);   //

    root_o = &originalGraph.addNewTeam();                 //          0     0
    team_o = &originalGraph.addNewTeam();                 //          |     |
    a0_o = &originalGraph.addNewAction(0);                //          `\   /'
    a1_o = &originalGraph.addNewAction(1);                //            \ /
    originalGraph.addNewEdge(*root_o, *team_o, progP0_o); //             0
    originalGraph.addNewEdge(*team_o, *a0_o, progP1_o);   //             |
    originalGraph.addNewEdge(*team_o, *a1_o, progP2_o);   //             0


    std::unordered_map<const TPG::TPGVertex*, TPG::TPGVertex*> vertexMap;

    Mutator::BranchMutator::copyTeamAndEdges((const TPG::TPGVertex*) root_o, targetGraph, vertexMap);

    ASSERT_TRUE(
        targetGraph.hasVertex((const TPG::TPGVertex&)(*root_t)));

    ASSERT_TRUE(
        targetGraph.hasVertex((const TPG::TPGVertex&)(*vertexMap[root_o])));
    ASSERT_TRUE(
        targetGraph.hasVertex((const TPG::TPGVertex&)(*vertexMap[team_o])));
    ASSERT_TRUE(targetGraph.hasVertex((const TPG::TPGVertex&)(*vertexMap[a0_o])));
    ASSERT_TRUE(targetGraph.hasVertex((const TPG::TPGVertex&)(*vertexMap[a1_o])));

}

TEST_F(BranchMutatorTest, getAllVerticesFromTeam)
{
    TPG::TPGGraph Graph(*e);

    root_t = &Graph.addNewTeam();                  //      0      0
    team0_t = &Graph.addNewTeam();                 //      |      |
    team1_t = &Graph.addNewTeam();                 //      0      0
    a0_t = &Graph.addNewAction(0);                 //      '\    /'
    a1_t = &Graph.addNewAction(1);                 //        \  /
    Graph.addNewEdge(*root_t, *team0_t, progP0_t); //         \/
    Graph.addNewEdge(*root_t, *team1_t, progP1_t); //          0
    Graph.addNewEdge(*team0_t, *a0_t, progP2_t);   //
    Graph.addNewEdge(*team1_t, *a1_t, progP3_t);   //

    std::unordered_set<const TPG::TPGVertex *> vertices;
    Mutator::BranchMutator::getAllVerticesFromTeam((const TPG::TPGVertex*) root_t, vertices);

    ASSERT_EQ(vertices.size(),5);
    ASSERT_TRUE(vertices.find(root_t)!=vertices.end());
    ASSERT_TRUE(vertices.find(team0_t)!=vertices.end());
    ASSERT_TRUE(vertices.find(team1_t)!=vertices.end());
    ASSERT_TRUE(vertices.find(a0_t)!=vertices.end());
    ASSERT_TRUE(vertices.find(a1_t)!=vertices.end());

}

TEST_F(BranchMutatorTest, getAllEdgesFromTeam)
{
    TPG::TPGGraph Graph(*e);

    root_t = &Graph.addNewTeam();                  //      0      0
    team0_t = &Graph.addNewTeam();                 //      |      |
    team1_t = &Graph.addNewTeam();                 //      0      0
    a0_t = &Graph.addNewAction(0);                 //      '\    /'
    a1_t = &Graph.addNewAction(1);                 //        \  /
    Graph.addNewEdge(*root_t,  *team0_t, progP0_t);//         \/
    Graph.addNewEdge(*root_t,  *team1_t, progP1_t);//          0
    Graph.addNewEdge(*team0_t, *a0_t,    progP2_t);//
    Graph.addNewEdge(*team1_t, *a1_t,    progP3_t);

    std::unordered_set<const TPG::TPGEdge *> edges;
    Mutator::BranchMutator::getAllEdgesFromTeam((const TPG::TPGVertex*) root_t, edges);

    ASSERT_EQ(edges.size(),4);
}