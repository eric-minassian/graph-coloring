#include "CSVReader.hpp"
#include "IGWriter.hpp"
#include "InterferenceGraph.hpp"
#include "proj6.hpp"
#include "verifier.hpp"
#include "gtest/gtest.h"
#include <string>
#include <unordered_set>

// Warning: These are *NOT* exhaustive tests.
// You should consider creating your own unit tests
// to test the functionality of your code entirely.

namespace {

using namespace proj6;

TEST(RequiredPart1, SimpleAddEdgesAndVertices) {
  const auto &GRAPH = "gtest/graphs/simple.csv";

  const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);

  EXPECT_EQ(ig.numEdges(), 3);
  EXPECT_EQ(ig.numVertices(), 3);
}

TEST(RequiredPart1, Neighbors) {
  const auto &GRAPH = "gtest/graphs/simple.csv";

  const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);
  const std::unordered_set<Variable> &expected_neighbors = {"y", "z"};

  EXPECT_EQ(ig.neighbors("x"), expected_neighbors);
}

// TEST(RequiredPart2, SimpleSuccess) {
//   // Load the graph pointed to by simple.csv
//   const auto &GRAPH = "gtest/graphs/simple.csv";
//   const auto NUM_REGS = 3;

//   // Provide 3 registers to the register allocation algorithm.
//   // Note, the highest degree in this graph is two, so all three
//   // will be needed.
//   const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
//   IGWriter::write(CSVReader::load(GRAPH), "gtest/graphs/simple_success.dot",
//                   allocation);

//   EXPECT_TRUE(verifyAllocation(GRAPH, NUM_REGS, allocation));
// }

// TEST(RequiredPart2, SimpleFail) {
//   // Load the graph pointed to by simple.csv
//   const auto &GRAPH = "gtest/graphs/simple.csv";
//   const auto NUM_REGS = 2;

//   // Provide 2 registers to the register allocation algorithm.
//   // Note, the highest degree in this graph is two, so the allocation
//   // cannot be completed.
//   const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
//   EXPECT_TRUE(allocation.empty());
// }

TEST(AdditionalPart1, AddVertex1300) {
  InterferenceGraph<std::string> graph;

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      for (char e = 'a'; e <= 'b'; e++) {
        std::string temp = "";
        temp += c;
        temp += d;
        temp += e;
        graph.addVertex(temp);
      }
    }
  }
}

TEST(AdditionalPart1, TimeRestriction) {
  const auto &GRAPH = "gtest/graphs/auto_generated.csv";

  const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);

  EXPECT_EQ(ig.numVertices(), 1000);

  EXPECT_EQ(ig.vertices().size(), 1000);
}

TEST(AdditionalPart1, AddEdgeException) {
  InterferenceGraph<std::string> graph;
  EXPECT_THROW(graph.addEdge("hello", "world"), UnknownVertexException);

  graph.addVertex("hello");

  EXPECT_THROW(graph.addEdge("hello", "world"), UnknownVertexException);

  graph.removeVertex("hello");
  graph.addVertex("world");

  EXPECT_THROW(graph.addEdge("hello", "world"), UnknownVertexException);

  graph.addVertex("hello");

  graph.addEdge("hello", "world");
}

TEST(AdditionalPart1, AddEdge1) {
  InterferenceGraph<std::string> graph;
  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");

  std::unordered_set<std::string> edges_1 = graph.neighbors("hello");
  std::unordered_set<std::string> edges_2 = graph.neighbors("world");
  std::unordered_set<std::string> edges_3 = graph.neighbors("class");

  EXPECT_EQ(edges_1.size(), 0);
  EXPECT_EQ(edges_2.size(), 0);
  EXPECT_EQ(edges_3.size(), 0);

  graph.addEdge("hello", "world");
  graph.addEdge("hello", "class");
  graph.addEdge("world", "class");
  graph.addEdge("world", "class");

  std::unordered_set<std::string> edges_4 = graph.neighbors("hello");
  std::unordered_set<std::string> edges_5 = graph.neighbors("world");
  std::unordered_set<std::string> edges_6 = graph.neighbors("class");

  EXPECT_EQ(edges_4.size(), 2);
  EXPECT_EQ(edges_5.size(), 2);
  EXPECT_EQ(edges_6.size(), 2);

  EXPECT_NE(edges_4.find("world"), edges_4.end());
  EXPECT_NE(edges_4.find("class"), edges_4.end());
  EXPECT_NE(edges_5.find("class"), edges_5.end());
  EXPECT_NE(edges_5.find("hello"), edges_5.end());
  EXPECT_NE(edges_6.find("hello"), edges_6.end());
  EXPECT_NE(edges_6.find("world"), edges_6.end());
}

TEST(AdditionalPart1, AddVertex) {
  InterferenceGraph<std::string> graph;

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      std::string temp = "";
      temp += c;
      temp += d;
      graph.addVertex(temp);
    }
  }

  std::unordered_set<std::string> vertices = graph.vertices();

  EXPECT_EQ(vertices.size(), 676);

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      std::string temp = "";
      temp += c;
      temp += d;
      EXPECT_NE(vertices.find(temp), vertices.end());
    }
  }
}

TEST(AdditionalPart1, AddVertexDuplicate) {
  InterferenceGraph<std::string> graph;

  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addEdge("hello", "world");

  graph.addVertex("hello");
  graph.addVertex("world");

  std::unordered_set<std::string> edges_1 = graph.neighbors("hello");
  std::unordered_set<std::string> edges_2 = graph.neighbors("world");

  EXPECT_EQ(edges_1.size(), 1);
  EXPECT_NE(edges_1.find("world"), edges_1.end());

  EXPECT_EQ(edges_2.size(), 1);
  EXPECT_NE(edges_2.find("hello"), edges_2.end());

  EXPECT_EQ(graph.numVertices(), 2);

  std::string word1 = "testing";
  std::string word2 = "testingAgain";

  graph.addVertex(word1);
  graph.addVertex(word2);

  EXPECT_EQ(graph.numVertices(), 4);

  graph.addVertex(word1);
  graph.addVertex(word2);

  EXPECT_EQ(graph.numVertices(), 4);
}

TEST(AdditionalPart1, RemoveVertex) {
  InterferenceGraph<std::string> graph;
  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      std::string temp = "";
      temp += c;
      temp += d;
      graph.addVertex(temp);
    }
  }

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      std::string temp = "";
      temp += c;
      temp += d;
      graph.removeVertex(temp);
    }
  }

  std::unordered_set<std::string> vertices = graph.vertices();

  EXPECT_EQ(vertices.size(), 0);
}

TEST(AdditionalPart1, RemoveEdgeVertexException) {
  InterferenceGraph<std::string> graph;
  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");

  graph.addEdge("hello", "world");

  EXPECT_THROW(graph.removeEdge("test", "work"), UnknownVertexException);
  EXPECT_THROW(graph.removeEdge("hello", "test"), UnknownVertexException);
  EXPECT_THROW(graph.removeEdge("test", "world"), UnknownVertexException);
}

TEST(AdditionalPart1, RemoveEdgeEdgeException) {
  InterferenceGraph<std::string> graph;
  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");

  graph.addEdge("hello", "world");

  EXPECT_THROW(graph.removeEdge("world", "class"), UnknownEdgeException);
}

TEST(AdditionalPart1, RemoveEdge) {
  InterferenceGraph<std::string> graph;
  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");

  graph.addEdge("hello", "world");

  std::unordered_set<std::string> edges_1 = graph.neighbors("hello");
  std::unordered_set<std::string> edges_2 = graph.neighbors("world");

  EXPECT_EQ(edges_1.size(), 1);
  EXPECT_NE(edges_1.find("world"), edges_1.end());

  EXPECT_EQ(edges_2.size(), 1);
  EXPECT_NE(edges_2.find("hello"), edges_2.end());

  graph.removeEdge("hello", "world");

  std::unordered_set<std::string> edges_3 = graph.neighbors("hello");
  std::unordered_set<std::string> edges_4 = graph.neighbors("world");

  EXPECT_EQ(edges_3.size(), 0);
  EXPECT_EQ(edges_3.find("world"), edges_3.end());

  EXPECT_EQ(edges_4.size(), 0);
  EXPECT_EQ(edges_4.find("hello"), edges_4.end());
}

TEST(AdditionalPart1, Vertices) {
  InterferenceGraph<std::string> graph;
  std::unordered_set<std::string> expected_graph_vertices = {};

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      for (char e = 'a'; e <= 'b'; e++) {
        std::string temp = "";
        temp += c;
        temp += d;
        temp += e;
        graph.addVertex(temp);
        expected_graph_vertices.insert(temp);
      }
    }
  }

  std::unordered_set<std::string> graph_vertices = graph.vertices();

  EXPECT_EQ(graph_vertices.size(), expected_graph_vertices.size());

  for (auto v : expected_graph_vertices) {
    EXPECT_NE(graph_vertices.find(v), graph_vertices.end());
  }

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      std::string temp = "";
      temp += c;
      temp += d;
      temp += 'a';
      expected_graph_vertices.erase(temp);
      graph.removeVertex(temp);
    }
  }

  std::unordered_set<std::string> graph_vertices_2 = graph.vertices();

  EXPECT_EQ(graph_vertices_2.size(), expected_graph_vertices.size());

  for (auto v : expected_graph_vertices) {
    EXPECT_NE(graph_vertices_2.find(v), graph_vertices_2.end());
  }
}

TEST(AdditionalPart1, Neighbors) {
  InterferenceGraph<std::string> graph;

  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");
  graph.addEdge("hello", "world");
  graph.addEdge("hello", "class");
  graph.addEdge("world", "class");

  std::unordered_set<std::string> e_edges_1 = {"world", "class"},
                                  e_edges_2 = {"hello", "class"},
                                  e_edges_3 = {"hello", "world"};

  auto edges_1 = graph.neighbors("hello"), edges_2 = graph.neighbors("world"),
       edges_3 = graph.neighbors("class");

  EXPECT_EQ(e_edges_1.size(), edges_1.size());
  EXPECT_EQ(e_edges_2.size(), edges_2.size());
  EXPECT_EQ(e_edges_3.size(), edges_3.size());

  for (auto v : e_edges_1) {
    EXPECT_NE(edges_1.find(v), edges_1.end());
  }
  for (auto v : e_edges_2) {
    EXPECT_NE(edges_2.find(v), edges_2.end());
  }
  for (auto v : e_edges_3) {
    EXPECT_NE(edges_3.find(v), edges_3.end());
  }
}

TEST(AdditionalPart1, NumVertices) {
  InterferenceGraph<std::string> graph;

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      for (char e = 'a'; e <= 'z'; e++) {
        std::string temp = "";
        temp += c;
        temp += d;
        temp += e;
        graph.addVertex(temp);
      }
    }
  }

  EXPECT_EQ(graph.numVertices(), 17576);

  for (char c = 'a'; c <= 'z'; c++) {
    for (char d = 'a'; d <= 'z'; d++) {
      for (char e = 'a'; e <= 'y'; e++) {
        std::string temp = "";
        temp += c;
        temp += d;
        temp += e;
        graph.removeVertex(temp);
      }
    }
  }

  EXPECT_EQ(graph.numVertices(), 676);
}

// @ TODO : Add tests
TEST(AdditionalPart1, NumEdges) {
  InterferenceGraph<std::string> graph;

  EXPECT_EQ(graph.numEdges(), 0);

  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");
  graph.addVertex("test");
  graph.addEdge("hello", "world");
  graph.addEdge("hello", "class");
  graph.addEdge("world", "class");

  EXPECT_EQ(graph.numEdges(), 3);

  graph.addEdge("world", "class");

  EXPECT_EQ(graph.numEdges(), 3);

  graph.addEdge("test", "world");

  EXPECT_EQ(graph.numEdges(), 4);

  graph.removeEdge("test", "world");

  EXPECT_EQ(graph.numEdges(), 3);
}

TEST(AdditionalPart1, InterferencesException) {
  InterferenceGraph<std::string> graph;

  EXPECT_THROW(graph.interferes("hello", "world"), UnknownVertexException);

  graph.addVertex("hello");

  EXPECT_THROW(graph.interferes("hello", "world"), UnknownVertexException);
  EXPECT_THROW(graph.interferes("world", "hello"), UnknownVertexException);
}

TEST(AdditionalPart1, Interference) {
  InterferenceGraph<std::string> graph;

  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");
  graph.addEdge("hello", "world");
  graph.addEdge("world", "class");

  EXPECT_TRUE(graph.interferes("hello", "world"));
  EXPECT_TRUE(graph.interferes("world", "hello"));
  EXPECT_TRUE(graph.interferes("class", "world"));
  EXPECT_TRUE(graph.interferes("world", "class"));

  EXPECT_FALSE(graph.interferes("class", "hello"));
  EXPECT_FALSE(graph.interferes("hello", "class"));
}

TEST(AdditionalPart1, DegreeException) {
  InterferenceGraph<std::string> graph;

  EXPECT_THROW(graph.degree("hello"), UnknownVertexException);

  graph.addVertex("hello");

  EXPECT_THROW(graph.degree("test"), UnknownVertexException);
}

TEST(AdditionalPart1, Degree) {
  InterferenceGraph<std::string> graph;

  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");
  graph.addEdge("hello", "world");
  graph.addEdge("world", "class");

  EXPECT_EQ(graph.degree("hello"), 1);
  EXPECT_EQ(graph.degree("world"), 2);
  EXPECT_EQ(graph.degree("class"), 1);

  graph.addEdge("hello", "class");

  EXPECT_EQ(graph.degree("hello"), 2);
  EXPECT_EQ(graph.degree("world"), 2);
  EXPECT_EQ(graph.degree("class"), 2);
}

TEST(AdditionalPart1, RemoveVertexNeighbors) {
  InterferenceGraph<std::string> graph;

  graph.addVertex("hello");
  graph.addVertex("world");
  graph.addVertex("class");
  graph.addEdge("hello", "world");
  graph.addEdge("hello", "class");
  graph.addEdge("world", "class");

  graph.removeVertex("hello");

  EXPECT_EQ(graph.degree("world"), 1);
  EXPECT_EQ(graph.degree("class"), 1);
}

TEST(AdditionalPart2, TimeRestriction) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/auto_generated.csv";
  const auto NUM_REGS = 1000;

  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);

  EXPECT_TRUE(verifyAllocation(GRAPH, NUM_REGS, allocation));
}

TEST(AdditionalPart2, PubTests) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/pub_tests.csv";
  const auto NUM_REGS = 5;

  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  IGWriter::write(CSVReader::load(GRAPH), "gtest/graphs/pub_tests.dot",
                  allocation);

  EXPECT_TRUE(verifyAllocation(GRAPH, NUM_REGS, allocation));
}

TEST(AdditionalPart2, ThreeReg) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/three_reg.csv";
  const auto NUM_REGS = 2;

  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  IGWriter::write(CSVReader::load(GRAPH), "gtest/graphs/three_reg.dot",
                  allocation);

  EXPECT_TRUE(verifyAllocation(GRAPH, NUM_REGS, allocation));
}

TEST(AdditionalPart2, FullStressTestPass) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/full_stress_test.csv";
  const auto NUM_REGS = 500;

  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  IGWriter::write(CSVReader::load(GRAPH), "gtest/graphs/full_stress_test.dot",
                  allocation);

  EXPECT_TRUE(verifyAllocation(GRAPH, NUM_REGS, allocation));
}

TEST(AdditionalPart2, FullStressTestFail) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/full_stress_test.csv";
  const auto NUM_REGS = 499;

  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);

  EXPECT_EQ(allocation.size(), 0);
  EXPECT_FALSE(verifyAllocation(GRAPH, NUM_REGS, allocation));
}

TEST(AdditionalPart2, Cycle6) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/cycle_6.csv";
  const auto NUM_REGS = 2;

  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  IGWriter::write(CSVReader::load(GRAPH), "gtest/graphs/cycle_6.dot",
                  allocation);

  EXPECT_TRUE(verifyAllocation(GRAPH, NUM_REGS, allocation));
}

} // end namespace
