#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/graph.h"

/* =========================================================
 * graph.c
 * Implementarea grafului direcționat ponderat cu liste de
 * adiacență.
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru relații
 * ---------------------------------------------------------- */

const char *relation_type_to_str(RelationType type)
{
    /* TODO: returnează șir cu litere mici pentru fiecare enum de relație */
    (void)type;
      switch (type) {
        case WORKS_AT:        return "works_at";
        case FRIEND_OF:       return "friend_of";
        case LOCATED_IN:      return "located_in";
        case PARTICIPATES_IN: return "participates_in";
        default:              return "unknown";
    }
    return "unknown";
}

RelationType str_to_relation_type(const char *str)
{
    /* TODO: parsează șirul relației la valoarea enum */
    (void)str;
    if (strcmp(str, "works_at")        == 0) return WORKS_AT;
    if (strcmp(str, "friend_of")       == 0) return FRIEND_OF;
    if (strcmp(str, "located_in")      == 0) return LOCATED_IN;
    if (strcmp(str, "participates_in") == 0) return PARTICIPATES_IN;
    return (RelationType)-1;
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Graph *graph_create(int initial_capacity)
{
    /* TODO: alocă structura Graph și tabloul de noduri */
    (void)initial_capacity;
    Graph *g = malloc(sizeof(Graph));
    if (!g) return NULL;
    g->nodes = malloc(sizeof(GraphNode) * initial_capacity);
    if (!g->nodes) { free(g); return NULL; }
    g->size     = 0;
    g->capacity = initial_capacity;
    return g;
}

void graph_free(Graph *g)
{
    /* TODO: eliberează șirul de nume al fiecărui nod și lista de muchii,
     *       apoi tabloul și structura */
    (void)g;
     if (!g) return;
    for (int i = 0; i < g->size; i++) {
        free(g->nodes[i].entity.name);
        EdgeNode *e = g->nodes[i].edges;
        while (e) {
            EdgeNode *next = e->next;
            free(e);
            e = next;
        }
    }
    free(g->nodes);
    free(g);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int graph_add_node(Graph *g, const char *name, EntityType type)
{
    /* TODO: extinde tabloul dacă e necesar, inițializează noul GraphNode,
     *       returnează id-ul */
    (void)g;
    (void)name;
    (void)type;
    if (g->size == g->capacity) {
        int new_cap = g->capacity * 2;
        GraphNode *tmp = realloc(g->nodes, sizeof(GraphNode) * new_cap);
        if (!tmp) return -1;
        g->nodes    = tmp;
        g->capacity = new_cap;
    }
    int id = g->size;
    g->nodes[id].entity.name = strdup(name);
    if (!g->nodes[id].entity.name) return -1;
    g->nodes[id].entity.type = type;
    g->nodes[id].entity.id   = id;
    g->nodes[id].edges        = NULL;
    g->size++;
    return id;
}

int graph_add_edge(Graph *g, int src_id, int dest_id,
                   RelationType type, float cost)
{
    /* TODO: alocă EdgeNode, adaugă la finalul listei de muchii a sursei
     *       pentru a păstra ordinea de inserare */
    (void)g;
    (void)src_id;
    (void)dest_id;
    (void)type;
    (void)cost;
    if (src_id < 0 || src_id >= g->size) return -1;
    EdgeNode *e = malloc(sizeof(EdgeNode));
    if (!e) return -1;
    e->dest_id = dest_id;
    e->type    = type;
    e->cost    = cost;
    e->next    = NULL;

    /* Adaugă la finalul listei pentru a păstra ordinea de inserare */
    EdgeNode **cur = &g->nodes[src_id].edges;
    while (*cur) cur = &(*cur)->next;
    *cur = e;
    return 0;
}

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru interogare
 * ---------------------------------------------------------- */

int graph_find_id(const Graph *g, const char *name)
{
    /* TODO: scanare liniară returnând id-ul potrivit sau -1 */
    (void)g;
    (void)name;
    for (int i = 0; i < g->size; i++)
        if (strcmp(g->nodes[i].entity.name, name) == 0)
            return i;
    return -1;
}

GraphNode *graph_get_node(const Graph *g, int id)
{
    /* TODO: verifică limitele și returnează pointerul */
    (void)g;
    (void)id;
    if (id < 0 || id >= g->size) return NULL;
    return &g->nodes[id];
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void graph_print(const Graph *g)
{
    /* TODO: afișează fiecare nod și lista sa de muchii */
    (void)g;
     for (int i = 0; i < g->size; i++) {
        const GraphNode *nd = &g->nodes[i];
        printf("%d %s:", nd->entity.id, nd->entity.name);
        EdgeNode *e = nd->edges;
        while (e) {
            printf(" [%d %s %.2f]",
                   e->dest_id,
                   relation_type_to_str(e->type),
                   e->cost);
            e = e->next;
        }
        printf("\n");
    }
}
