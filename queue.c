#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q != NULL) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *temp;
        temp = q->head;
        q->head = q->head->next;
        free(temp->value);
        free(temp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    char *str_val = malloc((strlen(s) + 1) * sizeof(char));
    if (!str_val) {
        free(newh);
        return false;
    }
    memset(str_val, '\0', strlen(s) + 1);
    strcpy(str_val, s);
    newh->value = str_val;
    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    char *str_val = malloc((strlen(s) + 1) * sizeof(char));
    if (!str_val) {
        free(newt);
        return false;
    }
    memset(str_val, '\0', strlen(s) + 1);
    strcpy(str_val, s);
    newt->value = str_val;
    newt->next = NULL;
    q->size++;
    if (!q->head) {
        q->head = newt;
        q->tail = newt;
        return true;
    }
    q->tail->next = newt;
    q->tail = newt;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * ok! (up to a maximum of bufsize-1 characters, plus a null terminator.) The
 * space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;
    list_ele_t *node;
    node = q->head;
    if (sp != NULL) {
        memset(sp, '\0', bufsize);
        strncpy(sp, node->value, bufsize - 1);
    }
    q->head = q->head->next;
    free(node->value);
    free(node);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size == 0)
        return;
    list_ele_t *t1, *t2;
    q->tail = q->head;
    t1 = q->head->next;
    t2 = q->tail;
    while (t1) {
        q->head = t1;
        t1 = t1->next;
        q->head->next = t2;
        t2 = q->head;
    }
    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

void q_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    q->head = mergeSortList(q->head);
    list_ele_t *temp = q->head;
    int i;
    for (i = 1; i < q->size; i++)
        temp = temp->next;
    q->tail = temp;
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (!l2)
        return l1;
    if (!l1)
        return l2;

    if (strcmp(l1->value, l2->value) < 0) {
        l1->next = merge(l1->next, l2);
        return l1;
    } else {
        l2->next = merge(l1, l2->next);
        return l2;
    }
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = mergeSortList(head);
    list_ele_t *l2 = mergeSortList(fast);

    return merge(l1, l2);
}
