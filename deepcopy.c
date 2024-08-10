#include <stdio.h>
#include <stdlib.h>

struct Node {
    int val;

    struct Node *next;
    struct Node *random;
};

struct Node *create_node(int val) {
    struct Node *n = (struct Node *)malloc(sizeof(struct Node));
    if (n == NULL) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }

    n->val    = val;
    n->random = NULL;
    n->next   = NULL;

    return n;
}

struct Node *copy_list(struct Node *head) {
    struct Node *copy = NULL;
    struct Node *ptr  = head;

    while (ptr) {
        struct Node *node = create_node(ptr->val);

        if (!copy) 
            copy = node;
        else 
            for (struct Node *p = copy; p != NULL; p = p->next) 
                if (!p->next) {
                    p->next = node;
                    break;
                }

        ptr = ptr->next;
    }

    return copy;
}

void connect_random(struct Node *head, struct Node *n, int index) {
    struct Node *ptr = head;
    int itr          = 0;

    while (ptr) {
        if (itr == index) {
            n->random = ptr;
            return;
        }

        itr++;
        ptr = ptr->next;
    }
}

int get_index(struct Node *head, struct Node *node) {
    if (!node->random) 
        return -1;
    
    int index = 0;
    for (struct Node *ptr = head; ptr != NULL; ptr = ptr->next) {
        if (node->random == ptr) 
            return index;

        index++;
    }

    return index;
}

struct Node *deep_copy(struct Node *head) {
    struct Node *copy = copy_list(head);

    for (
            struct Node *p_1 = head, *p_2 = copy; 
            p_1 != NULL && p_2 != NULL; 
            p_1 = p_1->next, p_2 = p_2->next
        ) {
        
        if (!p_1->random) 
            p_2->random = NULL;
        else {
            int index = get_index(head, p_1);
            
            if (index == -1) 
                p_2->random = NULL;
            else 
                connect_random(copy, p_2, index);
        }
    }

    return copy;
}

void freeList(struct Node *head) {
    struct Node *ptr = head;
    
    while (ptr) {
        struct Node *temp = ptr;
        ptr               = ptr->next;
        
        free(temp);
    }
    
    free(head);
}

void printList(struct Node *head) {
    struct Node *ptr = head;
    
    while (ptr) {
        int val;

        if (!ptr->random) 
            val = -1;
        else 
            val = ptr->random->val;

        printf("[%i, %i]\n", ptr->val, val);
        ptr = ptr->next;
    }
}

int main() {
    int arr[] = {7, -1, 13, 0, 11, 4, 10, 2, 1, 0};
    int size  = sizeof(arr) / (sizeof(int));

    struct Node *list = NULL;

    for (int i = 0; i < size; i+=2) {
        struct Node *node = create_node(arr[i]);

        if (!list) 
            list = node;
        else 
            for (struct Node *ptr = list; ptr != NULL; ptr = ptr->next) 
                if (!ptr->next) {
                    ptr->next = node;
                    break;
                }
    }

    int i = 1;
    for (struct Node *ptr = list; ptr != NULL; ptr = ptr->next) {
        if (arr[i] == -1) 
            ptr->random = NULL;
        else {
            struct Node *p = list;
            int j          = 0;
        
            while (p) {
                if (j == arr[i]) {
                    ptr->random = p;
                    break;
                }

                j++;
                p = p->next;
            }
        }

        i += 2;
    } 

    printList(list);

    struct Node *copy = deep_copy(list);
    
    printList(copy);

    free(list);
    free(copy);

    return 0;
}
