#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/queue.h"

/* =========================================================
 * queue.c
 * Coadă FIFO susținută de o listă simplu înlănțuită.
 * Stochează copii alocate pe heap ale șirurilor.
 * ========================================================= */

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Queue *queue_create(void)
{
    /* TODO: alocă și inițializează cu zero o structură Queue */
    Queue *q = calloc(1, sizeof(Queue));
    return q;
}

void queue_free(Queue *q)
{
    /* TODO: golește coada eliberând fiecare nod și datele sale */
    (void)q;
    if (!q) return;
    QueueNode *cur = q->front;
    while (cur) {
        QueueNode *next = cur->next;
        free(cur->data);
        free(cur);
        cur = next;
    }
    free(q);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int queue_enqueue(Queue *q, const char *data)
{
    /* TODO: duplică data, alocă QueueNode, adaugă la capătul rear */
    (void)q;
    (void)data;
    if (!q) return -1;
    QueueNode *n = malloc(sizeof(QueueNode));
    if (!n) return -1;
    n->data = strdup(data);
    if (!n->data) { free(n); return -1; }
    n->next = NULL;
    if (!q->rear) {
        q->front = q->rear = n;
    } else {
        q->rear->next = n;
        q->rear       = n;
    }
    q->size++;
    return 0;
}

char *queue_dequeue(Queue *q)
{
    /* TODO: elimină nodul din față, returnează datele sale (apelantul
     *       eliberează) */
    (void)q;
    if (!q || !q->front) return NULL;
    QueueNode *n    = q->front;
    char      *data = n->data;
    q->front = n->next;
    if (!q->front) q->rear = NULL;
    free(n);
    q->size--;
    return data;
}

/* ----------------------------------------------------------
 * Inspecție
 * ---------------------------------------------------------- */

int queue_is_empty(const Queue *q)
{
    /* TODO: returnează 1 când size == 0 */
    (void)q;
    return !q || q->size == 0;
}

void queue_print(const Queue *q)
{
    /* TODO: iterează de la front la rear, afișează fiecare șir de date */
    (void)q;
     if (!q) return;
    const QueueNode *cur = q->front;
    while (cur) {
        printf("%s\n", cur->data);
        cur = cur->next;
    }
}
