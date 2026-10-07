# Knowledge Graph

**A command-line knowledge graph engine in C, built around custom data structures and graph algorithms.**

Knowledge Graph models people, companies, locations, and events as entities connected by directed, weighted relationships. It loads data from CSV files and processes queries to find entities, inspect connections, and compute paths using **Breadth-First Search** and **Dijkstra's algorithm**.

[Getting Started](#getting-started) · [Queries](#supported-queries) · [Data Format](#input-format) · [Implementation](#data-structures-and-algorithms) · [Tests](#testing)

## Features

- **Entity lookup** using a binary search tree indexed by name.
- **Relationship queries** to check direct connections and list outgoing neighbors.
- **Shortest paths by number of edges** using BFS.
- **Minimum-cost paths** using Dijkstra's algorithm with a binary min-heap.
- **Batch processing** of queries in their original order through a FIFO queue.
- **Debug output** for inspecting adjacency lists and queued queries.

## Getting Started

### Requirements

- A C compiler with C11 support, such as GCC or Clang.
- Make and a POSIX environment, such as Linux, macOS, or WSL on Windows.

### Build

Clone the repository, then build the command-line application:

```bash
git clone https://github.com/AlexandruCujba19/Knowledge-Graph.git
cd Knowledge-Graph
make build/student_kg
```

Use the explicit target above: the default `make` target refers to checker files that are not included in this repository.

### Run

The application accepts three input files: entities, relationships, and queries.

```bash
./build/student_kg <entities.csv> <relations.csv> <queries.txt>
```

Try the included sample dataset:

```bash
./build/student_kg \
  tests/public/pas4/entitati.csv \
  tests/public/pas4/relatii.csv \
  tests/public/pas4/interogari.txt
```

Expected output:

```text
EXISTS Ana: DA
EXISTS Bob: NU
EDGE Ana Firma: DA
NEIGHBORS Ana: Firma Mara
PATH Ana Iasi: Ana -> Firma -> Iasi
DIJKSTRA Ana Iasi: COST = 1.80; DRUM = Ana -> Firma -> Iasi
```

The current output uses Romanian labels: `DA` means yes, `NU` means no, and `DRUM` means path.

Append `--debug` to a run command to print adjacency lists and queued queries to standard error. Query results remain on standard output.

## Supported Queries

| Query | Description | Example |
| --- | --- | --- |
| `EXISTS <name>` | Check whether an entity exists. | `EXISTS Ana` |
| `EDGE <source> <destination>` | Check for a directed edge from source to destination. | `EDGE Ana Firma` |
| `NEIGHBORS <name>` | List outgoing neighbors in edge insertion order. | `NEIGHBORS Ana` |
| `PATH <source> <destination>` | Find a path with the fewest edges using BFS. | `PATH Ana Iasi` |
| `DIJKSTRA <source> <destination>` | Find a minimum-cost path and print its total cost. | `DIJKSTRA Ana Iasi` |

`PATH` ignores edge weights; `DIJKSTRA` uses them. Missing or unreachable endpoints produce `NU` for path queries. `NEIGHBORS` returns `NULL` when the entity is missing or has no outgoing edges.

## Input Format

CSV files use **semicolons** as separators and must include a header row. Queries are stored one per line in a separate text file.

### Entities

Each entity has a name and one of four types: `PERSON`, `COMPANY`, `LOCATION`, or `EVENT`.

```csv
NUME;TIP
Firma;COMPANY
Ana;PERSON
Iasi;LOCATION
Mara;PERSON
Forum;EVENT
```

### Relationships

Each relationship contains a source, a relation type, a destination, and a numeric cost. Supported relation types are `works_at`, `friend_of`, `located_in`, and `participates_in`.

```csv
SURSA;RELATIE;DESTINATIE;COST
Ana;works_at;Firma;1.20
Ana;friend_of;Mara;0.70
Mara;participates_in;Forum;1.50
Forum;located_in;Iasi;0.80
Firma;located_in;Iasi;0.60
```

All relationships are directed, including `friend_of`. Add a reverse relationship explicitly if both directions are needed.

### Queries

```text
EXISTS Ana
EXISTS Bob
EDGE Ana Firma
NEIGHBORS Ana
PATH Ana Iasi
DIJKSTRA Ana Iasi
```

Use unique entity names without spaces, match names and type labels exactly, and use non-negative edge costs for Dijkstra's algorithm. The CSV reader expects simple, unquoted fields.

## Data Structures and Algorithms

The implementation separates storage, indexing, query scheduling, and pathfinding into dedicated modules.

| Component | Implementation | Purpose |
| --- | --- | --- |
| Graph | Node array with linked adjacency lists | Store entities and outgoing weighted relationships. |
| Name index | Binary search tree | Resolve entity names to graph nodes. |
| Query queue | Linked FIFO queue | Preserve the order of input queries. |
| BFS | Array-based FIFO queue, visited array, and parent array | Explore nodes level by level and reconstruct a path. |
| Dijkstra | Binary min-heap, distance array, and parent array | Prioritize lower-cost candidates and reconstruct a minimum-cost path. |

### Complexity

Let `V` be the number of entities, `E` the number of relationships, `h` the height of the name-index BST, and `deg⁺(u)` the number of outgoing edges from a node. Treating name comparisons as constant-time:

| Operation | Time complexity |
| --- | --- |
| `EXISTS` | `O(h)` |
| `EDGE` / `NEIGHBORS` | `O(h + deg⁺(u))` |
| `PATH` — BFS | `O(V + E)` |
| `DIJKSTRA` — min-heap | `O(V + E log(E + 1))` |

The BST is not self-balancing: lookup is typically `O(log V)` for a well-shaped tree, but can degrade to `O(V)`. Dijkstra inserts a new heap entry whenever a distance improves and skips previously finalized nodes when popping; its heap can therefore contain multiple entries for the same node. Graph storage uses `O(V + E)` space, excluding the text of entity names.

## Project Structure

| Path | Contents |
| --- | --- |
| [`include/`](include/) | Public headers for entities, graph storage, BST, queue, heap, and queries. |
| [`src/main.c`](src/main.c) | Input loading, initialization, and command-line entry point. |
| [`src/entity.c`](src/entity.c) | Entity lifecycle and type conversions. |
| [`src/graph.c`](src/graph.c) | Directed graph storage and relationship operations. |
| [`src/bst.c`](src/bst.c) | Name-based indexing and lookup. |
| [`src/queue.c`](src/queue.c) | FIFO query queue. |
| [`src/heap.c`](src/heap.c) | Binary min-heap operations. |
| [`src/query.c`](src/query.c) | Query parsing, BFS, and Dijkstra. |
| [`tests/public/`](tests/public/) | Test harnesses, sample inputs, and expected outputs. |
| [`Makefile`](Makefile) | Application and test build targets. |

## Testing

Rebuild and run all four included test cases:

```bash
make -B public-test
```

The runner compares each test's output with its corresponding `expected.txt`. The `-B` flag forces recompilation, so the tests use the current source rather than any prebuilt binaries included in the repository.

## Author

**Alexandru Cujba** · [GitHub](https://github.com/AlexandruCujba19)

## License

Distributed under the [MIT License](LICENSE).
