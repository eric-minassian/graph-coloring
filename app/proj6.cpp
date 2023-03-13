#include "proj6.hpp"
#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace proj6;

// assignRegisters
//
// This is where you implement the register allocation algorithm
// as mentioned in the README. Remember, you must allocate at MOST
// d(G) + 1 registers where d(G) is the maximum degree of the graph G.
// If num_registers is not enough registers to accomodate the passed in
// graph you should return an empty map. You MUST use registers in the
// range [1, num_registers] inclusive.
RegisterAssignment proj6::assignRegisters(const std::string &path_to_graph,
                                          int num_registers) noexcept {
  InterferenceGraph<Variable> ig = CSVReader::load(path_to_graph);

  // TODO: You implement this!
  std::unordered_map<std::string, int> ans = {};
  std::vector<std::string> vertices = {};

  unsigned i = 0, j = 0;
  for (auto vertex : ig.vertices()) {
    vertices.push_back(vertex);
    j = i;
    while (j > 0 && ig.degree(vertices[j]) > ig.degree(vertices[j - 1])) {
      std::swap(vertices[j], vertices[j - 1]);
      j--;
    }
    i++;
  }

  for (unsigned i = 1; i <= num_registers; i++) {
    std::string first_uncolored = vertices.front();

    ans.insert({first_uncolored, i});
    std::vector<std::string> temp = {}, same = {first_uncolored};

    for (unsigned j = 1; j < vertices.size(); j++) {
      std::string vertex = vertices[j];

      bool interferes = false;
      for (auto v : same) {
        if (ig.interferes(v, vertex)) {
          interferes = true;
          break;
        }
      }

      if (interferes) {
        temp.push_back(vertex);
      } else {
        ans.insert({vertex, i});
        same.push_back(vertex);
      }
    }

    for (auto v : same) {
      ig.removeVertex(v);
    }
    vertices = temp;

    if (ig.numVertices() == 0) {
      return ans;
    }
  }

  return {};
}
