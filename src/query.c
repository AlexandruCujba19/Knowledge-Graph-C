#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "../include/query.h"
#include "../include/heap.h"

/* =========================================================
 * query.c
 * Parsarea și dispatcharea interogărilor de graf:
 *   EXISTS, EDGE, NEIGHBORS, PATH (BFS), DIJKSTRA
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper de parsare
 * ---------------------------------------------------------- */

QueryType parse_query_type(const char *line)
{
    /* TODO: potrivește cuvântul cheie de la începutul liniei */
    (void)line;
     if (strncmp(line, "EXISTS",    6) == 0) return Q_EXISTS;
    if (strncmp(line, "EDGE",      4) == 0) return Q_EDGE;
    if (strncmp(line, "NEIGHBORS", 9) == 0) return Q_NEIGHBORS;
    if (strncmp(line, "DIJKSTRA",  8) == 0) return Q_DIJKSTRA;
    if (strncmp(line, "PATH",      4) == 0) return Q_PATH;
    return Q_UNKNOWN;
}

/* ----------------------------------------------------------
 * EXISTS
 * ---------------------------------------------------------- */

void process_exists(const BST *tree, const char *name)
{
    /* TODO: caută în BST, afișează DA/NU */
    (void)tree;
    (void)name;
    GraphNode *gn = bst_search(tree, name);
    printf("EXISTS %s: %s\n", name, gn ? "DA" : "NU");
}

/* ----------------------------------------------------------
 * EDGE
 * ---------------------------------------------------------- */

void process_edge(const Graph *g, const BST *tree,
                  const char *src_name, const char *dest_name)
{
    /* TODO: găsește ambele noduri via BST, scanează lista de muchii
     *       a sursei pentru destinație */
    (void)g;
    (void)tree;
    (void)src_name;
    (void)dest_name;
     GraphNode *src = bst_search(tree, src_name);
    GraphNode *dst = bst_search(tree, dest_name);
    if (!src || !dst) {
        printf("EDGE %s %s: NU\n", src_name, dest_name);
        return;
    }
    EdgeNode *e = src->edges;
    while (e) {
        if (e->dest_id == dst->entity.id) {
            printf("EDGE %s %s: DA\n", src_name, dest_name);
            return;
        }
        e = e->next;
    }
    printf("EDGE %s %s: NU\n", src_name, dest_name);
}

/* ----------------------------------------------------------
 * NEIGHBORS
 * ---------------------------------------------------------- */

void process_neighbors(const Graph *g, const BST *tree,
                       const char *name)
{
    /* TODO: găsește nodul, iterează lista de muchii, afișează
     *       numele vecinilor; dacă nu există muchii de ieșire
     *       afișează "NEIGHBORS <name>: NULL" */
    (void)g;
    (void)tree;
    (void)name;
     GraphNode *src = bst_search(tree, name);
    if (!src || !src->edges) {
        printf("NEIGHBORS %s: NULL\n", name);
        return;
    }
    printf("NEIGHBORS %s:", name);
    EdgeNode *e = src->edges;
    while (e) {
        GraphNode *dest = graph_get_node(g, e->dest_id);
        if (dest) printf(" %s", dest->entity.name);
        e = e->next;
    }
    printf("\n");
}

/* ----------------------------------------------------------
 * PATH (BFS)
 * ---------------------------------------------------------- */

void process_path_bfs(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: BFS de la src la dest, reconstruiește și afișează calea;
     *       dacă nu există cale afișează "PATH <src> <dest>: NU" */
    (void)g;
    (void)tree;
    (void)src_name;
    (void)dest_name;
       GraphNode *src = bst_search(tree, src_name);
    GraphNode *dst = bst_search(tree, dest_name);
    if (!src || !dst) {
        printf("PATH %s %s: NU\n", src_name, dest_name);
        return;
    }
    int n = g->size;
    int *visited = calloc(n, sizeof(int));
    int *parent  = malloc(n * sizeof(int));
    if (!visited || !parent) { free(visited); free(parent); return; }
    for (int i = 0; i < n; i++) parent[i] = -1;

    /* BFS cu coada simpla */
    int *bfs_q  = malloc(n * sizeof(int));
    int  front  = 0, rear = 0;
    int  src_id = src->entity.id;
    int  dst_id = dst->entity.id;

    visited[src_id] = 1;
    bfs_q[rear++]   = src_id;
    int found = 0;

    while (front < rear && !found) {
        int cur = bfs_q[front++];
        EdgeNode *e = g->nodes[cur].edges;
        while (e) {
            int nxt = e->dest_id;
            if (!visited[nxt]) {
                visited[nxt] = 1;
                parent[nxt]  = cur;
                if (nxt == dst_id) { found = 1; break; }
                bfs_q[rear++] = nxt;
            }
            e = e->next;
        }
    }

    if (!found && src_id != dst_id) {
        printf("PATH %s %s: NU\n", src_name, dest_name);
    } else {
        /* Reconstruieste calea */
        int *path = malloc(n * sizeof(int));
        int  len  = 0;
        int  cur  = dst_id;
        while (cur != -1) { path[len++] = cur; cur = parent[cur]; }
        printf("PATH %s %s:", src_name, dest_name);
        for (int i = len - 1; i >= 0; i--) {
            printf(" %s", g->nodes[path[i]].entity.name);
            if (i > 0) printf(" ->");
        }
        printf("\n");
        free(path);
    }
    free(visited); free(parent); free(bfs_q);
}

/* ----------------------------------------------------------
 * DIJKSTRA
 * ---------------------------------------------------------- */

void process_dijkstra(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: Dijkstra cu min-heap, afișează costul și calea;
     *       dacă nu există cale afișează "DIJKSTRA <src> <dest>: NU" */
    (void)g;
    (void)tree;
    (void)src_name;
    (void)dest_name;
    GraphNode *src = bst_search(tree, src_name);
    GraphNode *dst = bst_search(tree, dest_name);
    if (!src || !dst) {
        printf("DIJKSTRA %s %s: NU\n", src_name, dest_name);
        return;
    }
    int n = g->size;
    float *dist   = malloc(n * sizeof(float));
    int   *parent = malloc(n * sizeof(int));
    int   *done   = calloc(n, sizeof(int));
    if (!dist || !parent || !done) {
        free(dist); free(parent); free(done); return;
    }
    for (int i = 0; i < n; i++) { dist[i] = FLT_MAX; parent[i] = -1; }

    int src_id = src->entity.id;
    int dst_id = dst->entity.id;
    dist[src_id] = 0.0f;

    MinHeap *h = heap_create(n + 1);
    heap_push(h, src_id, 0.0f);

    while (!heap_is_empty(h)) {
        HeapNode hn = heap_pop(h);
        int u = hn.node_id;
        if (done[u]) continue;
        done[u] = 1;
        if (u == dst_id) break;
        EdgeNode *e = g->nodes[u].edges;
        while (e) {
            int v = e->dest_id;
            float nd = dist[u] + e->cost;
            if (nd < dist[v]) {
                dist[v]   = nd;
                parent[v] = u;
                heap_push(h, v, nd);
            }
            e = e->next;
        }
    }
    heap_free(h);

    if (dist[dst_id] == FLT_MAX) {
        printf("DIJKSTRA %s %s: NU\n", src_name, dest_name);
    } else {
        int *path = malloc(n * sizeof(int));
        int  len  = 0, cur = dst_id;
        while (cur != -1) { path[len++] = cur; cur = parent[cur]; }
        printf("DIJKSTRA %s %s: COST = %.2f; DRUM =", src_name, dest_name, dist[dst_id]);
        for (int i = len - 1; i >= 0; i--) {
            printf(" %s", g->nodes[path[i]].entity.name);
            if (i > 0) printf(" ->");
        }
        printf("\n");
        free(path);
    }
    free(dist); free(parent); free(done);
}

/* ----------------------------------------------------------
 * Dispatcher batch
 * ---------------------------------------------------------- */

void process_all_queries(Queue *q, const Graph *g, const BST *tree)
{
    /* TODO: extrage fiecare linie, parsează tipul, dispatchează
     *       la handlerul corespunzător */
    (void)q;
    (void)g;
    (void)tree;
    while (!queue_is_empty(q)) {
        char *line = queue_dequeue(q);
        if (!line) continue;

        QueryType qt = parse_query_type(line);
        /* Copiem linia pentru tokenizare (strtok modifica sirul) */
        char *tmp = strdup(line);
        free(line);
        if (!tmp) continue;

        char *token = strtok(tmp, " ");  /* tipul interogarii */

        switch (qt) {
            case Q_EXISTS: {
                char *name = strtok(NULL, " ");
                if (name) process_exists(tree, name);
                break;
            }
            case Q_EDGE: {
                char *src  = strtok(NULL, " ");
                char *dest = strtok(NULL, " ");
                if (src && dest) process_edge(g, tree, src, dest);
                break;
            }
            case Q_NEIGHBORS: {
                char *name = strtok(NULL, " ");
                if (name) process_neighbors(g, tree, name);
                break;
            }
            case Q_PATH: {
                char *src  = strtok(NULL, " ");
                char *dest = strtok(NULL, " ");
                if (src && dest) process_path_bfs(g, tree, src, dest);
                break;
            }
            case Q_DIJKSTRA: {
                char *src  = strtok(NULL, " ");
                char *dest = strtok(NULL, " ");
                if (src && dest) process_dijkstra(g, tree, src, dest);
                break;
            }
            default:
                break;
        }
        (void)token;
        free(tmp);
    }
}
