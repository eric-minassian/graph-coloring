import random
import csv
import string

def random_string(length):
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for _ in range(length))

def generate_random_graph(num_vertices : int) -> dict:
    vertices = set()
    graph = {}

    i = 0
    while i < num_vertices:
        vertex = random_string(12)
        if vertex in vertices:
            continue
        else:
            i += 1
            vertices.add(vertex)

    for vertex in vertices:
        num_edges = random.randint(0, num_vertices - 1)
        edges = list(random.sample(vertices, num_edges))
        
        try:
            edges.remove(vertex)
        except ValueError:
            pass

        graph[vertex] = edges
    return graph

def write_inference_graph_to_csv(graph : dict, output_file):
    with open(output_file, 'w') as f:
        for vertex in graph.keys():
            f.write(vertex + "\n")

        for vertex, edges in graph.items():
            for edge in edges:
                f.write(vertex + "," + edge + "\n")

def main(num_vertices:int) -> None:
    graph = generate_random_graph(num_vertices)
    print(len(graph))
    write_inference_graph_to_csv(graph, 'gtest/graphs/auto_generated.csv')

if __name__ == "__main__":
    main(int(input("Enter Num Vertices:\n")))
