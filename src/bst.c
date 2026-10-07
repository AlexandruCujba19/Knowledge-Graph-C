#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bst.h"

/* =========================================================
 * bst.c
 * BST indexat după numele entității; nodurile conțin pointeri
 * în tabloul de noduri al grafului (fără duplicarea datelor).
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții auxiliare interne (statice, neexportate)
 * ---------------------------------------------------------- */

/* Eliberează recursiv nodii BSTNode (nu GraphNode-urile spre care
 * pointează — acelea aparțin Grafului). */
static void bst_free_recursive(BSTNode *node)
    __attribute__((unused));
static void bst_free_recursive(BSTNode *node)
{
    /* TODO: traversare post-ordine pentru eliberarea fiecărui BSTNode */
    (void)node;
     if (!node) return;
    bst_free_recursive(node->left);
    bst_free_recursive(node->right);
    free(node);
}

/* Inserare recursivă; returnează rădăcina (posibil nouă) a subarbore. */
static BSTNode *bst_insert_recursive(BSTNode *node, GraphNode *gn)
    __attribute__((unused));
static BSTNode *bst_insert_recursive(BSTNode *node, GraphNode *gn)
{
    /* TODO: compară numele, recursie stânga/dreapta, alocă la NULL */
    (void)node;
    (void)gn;
    
    if (!node) {
        BSTNode *n = malloc(sizeof(BSTNode));
        if (!n) return NULL;
        n->graph_node = gn;
        n->left       = NULL;
        n->right      = NULL;
        return n;
    }
    int cmp = strcmp(gn->entity.name, node->graph_node->entity.name);
    if (cmp < 0)
        node->left  = bst_insert_recursive(node->left,  gn);
    else if (cmp > 0)
        node->right = bst_insert_recursive(node->right, gn);
    return node;
}

/* Căutare recursivă; returnează GraphNode* sau NULL. */
static GraphNode *bst_search_recursive(const BSTNode *node, const char *name)
    __attribute__((unused));
static GraphNode *bst_search_recursive(const BSTNode *node, const char *name)
{
    /* TODO: compară name, recursie stânga/dreapta */
    (void)node;
    (void)name;
       if (!node) return NULL;
    int cmp = strcmp(name, node->graph_node->entity.name);
    if (cmp == 0) return node->graph_node;
    if (cmp < 0)  return bst_search_recursive(node->left,  name);
    return bst_search_recursive(node->right, name);
}

/* Traversare inordine pentru afișare. */
static void bst_inorder_recursive(const BSTNode *node)
    __attribute__((unused));
static void bst_inorder_recursive(const BSTNode *node)
{
    /* TODO: stânga -> vizitare -> dreapta */
    (void)node;
     if (!node) return;
    bst_inorder_recursive(node->left);
    printf("%d %s %s\n",
           node->graph_node->entity.id,
           node->graph_node->entity.name,
           entity_type_to_str(node->graph_node->entity.type));
    bst_inorder_recursive(node->right);
}


/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

/* Alocă și inițializează un BST gol. Returnează NULL la eșec. */
BST *bst_create(void)
{
    /* TODO: alocă structura BST și inițializează root-ul cu NULL */
   BST *tree = malloc(sizeof(BST));
    if (!tree) return NULL;
    tree->root = NULL;
    return tree;
}

/* Eliberează recursiv toți nodii BST, apoi structura BST. */
void bst_free(BST *tree)
{
    /* TODO: traversare post-ordine pentru eliberarea memoriei */
     if (!tree) return;
    bst_free_recursive(tree->root);
    free(tree);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int bst_insert(BST *tree, GraphNode *graph_node)
{
    /* TODO: deleghează la helper-ul recursiv, actualizează tree->root */
    (void)tree;
    (void)graph_node;
    if (!tree || !graph_node) return -1;
    tree->root = bst_insert_recursive(tree->root, graph_node);
    return 0;
}

/* ----------------------------------------------------------
 * Interogare
 * ---------------------------------------------------------- */

GraphNode *bst_search(const BST *tree, const char *name)
{
    /* TODO: deleghează la helper-ul recursiv */
    (void)tree;
    (void)name;
    if (!tree) return NULL;
    return bst_search_recursive(tree->root, name);
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void bst_inorder_print(const BST *tree)
{
    /* TODO: deleghează la helper-ul recursiv */
    (void)tree;
     if (!tree) return;
    bst_inorder_recursive(tree->root);
}
