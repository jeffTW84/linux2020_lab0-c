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
    /* TODO: What if malloc returned NULL? */
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    list_ele_t *head;

    if (!q) {
        return;
    }
    if (q->size == 0) {
        free(q);
        return;
    }

    head = q->head;
    while (q->head) {
        q->head = q->head->next;
        free(head->value);
        free(head);
        head = q->head;
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
    list_ele_t *newh;
    size_t length;
    /* TODO: What should you do if the q is NULL? */
    if (!q) {
        return false;
    }

    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (!newh) {
        return false;
    }

    length = strlen(s);
    newh->value = malloc(length + 1);
    if (!(newh->value)) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, length);
    newh->value[length] = '\0';
    newh->next = q->head;

    q->head = newh;
    if (!q->tail) {
        q->tail = newh;
    }
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
    list_ele_t *newt;
    size_t length;

    if (!q) {
        return false;
    }

    newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    length = strlen(s);
    newt->value = malloc(length + 1);
    if (!(newt->value)) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, length);
    newt->value[length] = '\0';

    q->tail->next = newt;
    q->tail = newt;
    q->tail->next = NULL;
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || (q->size == 0)) {
        return false;
    }

    list_ele_t *temp = q->head;
    if (sp) {
        strncpy(sp, temp->value, bufsize - 1);
    }

    q->head = q->head->next;
    if (!q->head) {
        q->tail = NULL;
    }

    free(temp->value);
    free(temp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
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
    list_ele_t *head, *middle;

    if (!q || (q->size == 0) || (q->size == 1)) {
        return;
    }

    q->tail = q->head;

    middle = NULL;
    head = q->head;
    q->head = q->head->next;
    while (q->head) {
        middle = head;
        head = q->head;
        q->head = q->head->next;
        head->next = middle;
    }
    q->head = head;
    q->tail->next = NULL;
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    // merge with recursive
    if (!l2)
        return l1;
    if (!l1)
        return l2;

    if (strcmp(l1->value, l2->value)) {
        l2->next = merge(l1, l2->next);
        return l2;
    } else {
        l1->next = merge(l1->next, l2);
        return l1;
    }
}

list_ele_t *merge_sort_list(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    // split list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    // sort each list
    list_ele_t *l1 = merge_sort_list(head);
    list_ele_t *l2 = merge_sort_list(fast);

    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || (q->size == 0) || (q->size == 1)) {
        return;
    }

    q->head = merge_sort_list(q->head);
    list_ele_t *temp = q->head;
    while (temp->next) {
        temp = temp->next;
    }
    q->tail = temp;
}
