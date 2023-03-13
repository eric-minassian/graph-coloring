#ifndef __INTERFERENCE_GRAPH__HPP
#define __INTERFERENCE_GRAPH__HPP

#include <exception>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class UnknownVertexException : public std::runtime_error {
public:
  UnknownVertexException(const std::string &v)
      : std::runtime_error("Unknown vertex " + v) {}
};

class UnknownEdgeException : public std::runtime_error {
public:
  UnknownEdgeException(const std::string &v, const std::string &w)
      : std::runtime_error("Unknown edge " + v + " - " + w) {}
};

// InterferenceGraph
//
// This is a class representing an interference graph
// as described in "Part 1: Interference Graph" of the README.md
// file. Though this class is templated, because of the usage of exceptions
// UnknownVertexException and UnknownEdgeException it will
// ONLY be tested with strings.
template <typename T> class InterferenceGraph {
public:
  // Custom type used to represent edges. This is mainly
  // used in the utility function for reading and writing
  // the graph structure to/from files. You don't need to use it.
  using EdgeTy = std::pair<T, T>;

  InterferenceGraph();

  ~InterferenceGraph();

  void addEdge(const T &v, const T &w);

  void addVertex(const T &vertex) noexcept;

  void removeEdge(const T &v, const T &w);

  void removeVertex(const T &vertex);

  std::unordered_set<T> vertices() const noexcept;

  std::unordered_set<T> neighbors(const T &vertex) const;

  unsigned numVertices() const noexcept;

  unsigned numEdges() const noexcept;

  bool interferes(const T &v, const T &w) const;

  unsigned degree(const T &v) const;

private:
  // Private member variables here.
  std::unordered_map<T, std::unordered_set<T>> graph;
  std::unordered_set<T> vertices_set;
  unsigned num_edges;
};

template <typename T>
InterferenceGraph<T>::InterferenceGraph()
    : graph({}), vertices_set({}), num_edges(0) {}

template <typename T> InterferenceGraph<T>::~InterferenceGraph() {}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
  auto vertex_node = graph.find(vertex);

  if (vertex_node == graph.end()) {
    throw UnknownVertexException(vertex);
  } else {
    return vertex_node->second;
  }
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
  return vertices_set;
}

template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
  return (unsigned)vertices_set.size();
}

template <typename T> unsigned InterferenceGraph<T>::numEdges() const noexcept {
  return num_edges;
}

template <typename T>
void InterferenceGraph<T>::addEdge(const T &v, const T &w) {
  auto vertex_1 = graph.find(v);
  auto vertex_2 = graph.find(w);

  if (vertex_1 == graph.end()) {
    throw UnknownVertexException(v);
  }
  if (vertex_2 == graph.end()) {
    throw UnknownVertexException(w);
  }

  if (vertex_1->second.find(w) == vertex_1->second.end() && v != w) {
    vertex_1->second.insert(w);
    vertex_2->second.insert(v);
    num_edges++;
  }
}

template <typename T>
void InterferenceGraph<T>::removeEdge(const T &v, const T &w) {
  auto vertex_1 = graph.find(v);
  auto vertex_2 = graph.find(w);

  if (vertex_1 == graph.end()) {
    throw UnknownVertexException(v);
  }
  if (vertex_2 == graph.end()) {
    throw UnknownVertexException(w);
  }

  if (vertex_1->second.find(w) == vertex_1->second.end() ||
      vertex_2->second.find(v) == vertex_2->second.end()) {
    throw UnknownEdgeException(v, w);
  } else {
    vertex_1->second.erase(w);
    vertex_2->second.erase(v);
    num_edges--;
  }
}

template <typename T>
void InterferenceGraph<T>::addVertex(const T &vertex) noexcept {
  if (graph.find(vertex) == graph.end()) {
    graph[vertex] = {};
    vertices_set.insert(vertex);
  }
}

template <typename T> void InterferenceGraph<T>::removeVertex(const T &vertex) {
  if (graph.find(vertex) == graph.end()) {
    throw UnknownVertexException(vertex);
  } else {
    graph.erase(vertex);
    vertices_set.erase(vertex);
  }
}

template <typename T>
bool InterferenceGraph<T>::interferes(const T &v, const T &w) const {
  auto vertex_1 = graph.find(v);
  auto vertex_2 = graph.find(w);

  if (vertex_1 == graph.end()) {
    throw UnknownVertexException(v);
  }
  if (vertex_2 == graph.end()) {
    throw UnknownVertexException(w);
  }

  if (vertex_1->second.find(w) == vertex_1->second.end()) {
    return false;
  } else {
    return true;
  }
}

template <typename T> unsigned InterferenceGraph<T>::degree(const T &v) const {
  auto vertex_1 = graph.find(v);

  if (vertex_1 == graph.end()) {
    throw UnknownVertexException(v);
  }

  return (unsigned)vertex_1->second.size();
}

#endif
