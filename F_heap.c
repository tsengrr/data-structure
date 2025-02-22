#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Child {
    struct Child* next;
    struct Node* data;
};


struct Node {
    int key;
    int degree;
    int mark;
    int root;
    struct Node* parent;
    struct Child* child_head;

};

struct Node* hashMap[10001] = {NULL};


struct F_heap {
    struct F_heap* nextNode;
    struct Node* data;
};

struct F_heap* F_heap_head = NULL;

int qsort_key(const void* a, const void*b) {
    struct F_heap* x = *(struct F_heap**)a;
    struct F_heap* y = *(struct F_heap**)b;
    return x->data->key - y->data->key;
}

int qsort_degree(const void* a, const void*b) {
    struct F_heap* x = *(struct F_heap**)a;
    struct F_heap* y = *(struct F_heap**)b;
    return x->data->degree - y->data->degree;
}

int qsort_child(const void* a, const void*b) {
    struct Child* x = *(struct Child**)a;
    struct Child* y = *(struct Child**)b;
    return x->data->key - y->data->key;
}

void addToList(struct Node* children) {
    children->root = 1;
    children->parent = NULL;
    struct F_heap* now = F_heap_head;
    while(now->nextNode != NULL) now = now->nextNode;
    struct F_heap* newRoot = (struct F_heap*)calloc(1, sizeof(struct F_heap));
    newRoot->data = children;
    newRoot->nextNode = NULL;

    if(F_heap_head == NULL) F_heap_head = newRoot;
    else {
        now->nextNode = newRoot;
    }

    return;
}

void CascadingCut(struct Node* target) {
    struct Node* dad = target->parent;
    struct Child* now = dad->child_head;
    struct Child* last = NULL;

    while(now->data->key != target->key) {
        last = now;
        now = now->next;
    }

    if(last == NULL) {
        if(now->next == NULL) dad->child_head = NULL;
        else {
            dad->child_head = now->next;
        }
    } 

    else last->next = now->next;

    dad->degree -= 1;

    if(target->parent->mark == 1) CascadingCut(target->parent);

    target->mark = 0;
    addToList(target);

    return;
}

void merge(struct F_heap* H1, struct F_heap* H2) {
    struct Node* x = H1->data;
    struct Node* y = H2->data;
    x->root = 0;
    y->degree += 1;

    struct Child* now = y->child_head;

    while(now != NULL && now->next != NULL) now = now->next;

    struct Child* baby = (struct Child*)calloc(1, sizeof(struct Child));
    baby->data = x;
    baby->next = NULL;

    if(y->child_head == NULL) y->child_head = baby;
    else now->next = baby;

    struct F_heap* now_ptr = F_heap_head;
    if(F_heap_head == H1) F_heap_head = now_ptr->nextNode;
    else {
        struct F_heap* last = NULL;
        while(now_ptr != H1) {
            last = now_ptr;
            now_ptr = now_ptr->nextNode;
        }
        last->nextNode = now_ptr->nextNode;
    }

    x->parent = y;
    return;

}


void RootList() {
    int ListLen = 0;
    static struct F_heap* RootMap[10001];
    struct F_heap* now = F_heap_head;
    while(now != NULL) {
        RootMap[ListLen++] = now;
        now = now->nextNode;
    }

    qsort(RootMap, ListLen, sizeof(struct F_heap*), qsort_key);
    for(int i = 0; i < ListLen; i++) RootMap[i]->nextNode = RootMap[i+1];
    RootMap[ListLen - 1]->nextNode = NULL;
    F_heap_head = RootMap[0];
    return;
}

void DegreeList() {
    int ListLen = 0;
    static struct F_heap* RootMap[10001];
    struct F_heap* now = F_heap_head;
    while(now != NULL) {
        RootMap[ListLen++] = now;
        now = now->nextNode;
    }

    qsort(RootMap, ListLen, sizeof(struct F_heap*), qsort_degree);
    for(int i = 0; i < ListLen; i++) RootMap[i]->nextNode = RootMap[i+1];
    RootMap[ListLen - 1]->nextNode = NULL;
    F_heap_head = RootMap[0];
    return;
}

void consolidate() {
    RootList();
    struct F_heap* now = F_heap_head;
    struct F_heap** hash;
    hash = (struct F_heap**)calloc(10001, sizeof(struct F_heap*));

    while(now != NULL) {
        if(hash[now->data->degree] != NULL) {
            merge(now, hash[now->data->degree]);
            free(hash);
            hash = (struct F_heap**)calloc(10001, sizeof(struct F_heap*));
            RootList();
            now = F_heap_head;
            continue;
        }
        else hash[now->data->degree] = now;

        now = now->nextNode;
    }
}



void insert(int key){
    struct F_heap* newListNode = (struct F_heap*)calloc(1, sizeof(struct F_heap));
    if(F_heap_head == NULL) {
        F_heap_head = newListNode;
    }
    else {
        struct F_heap* now = F_heap_head;
        while(now->nextNode != NULL) {
            now = now->nextNode;
        }

        now->nextNode = newListNode;
    }

    newListNode->data = (struct Node*)calloc(1, sizeof(struct Node));
    newListNode->data->degree = 0;
    newListNode->data->mark = 0;
    newListNode->data->root = 1;
    newListNode->data->key = key;
    newListNode->data->parent = NULL;
    newListNode->data->child_head = NULL;
    hashMap[key] = newListNode->data;

    return;


}

struct Node* findMin() {
    struct F_heap* now = F_heap_head;
    struct Node* target = NULL;
    int mn = 1e5;

    while(now != NULL) {
        if(now->data->key < mn) {
            mn = now->data->key;
            target = now->data;
        }
        now = now->nextNode;
    }

    return target;
    
}

void delete(int key) {
    struct Node* target = hashMap[key];
    if(target->parent != NULL) {
        struct Child* now = target->parent->child_head;
        struct Child* last = NULL;
        while(now->data->key != key) {
            last = now;
            now = now->next;
        } 
        if(last != NULL){
            last->next = now->next;
        }
        else {
            if(now->next == NULL) {
                target->parent->child_head = NULL;
            }
            else {
                target->parent->child_head = now->next;
            }
        }

        target->parent->degree -=1;

        if(target->parent->mark == 0) target->parent->mark = 1;
        else if (target->parent->root == 0) {
            CascadingCut(target->parent);
        }

        hashMap[key] = NULL;

    }

    struct Child* child = target->child_head;
    while(child != NULL) {
        addToList(child->data);
        child = child->next;
    }

    if(target->parent == NULL) {
        struct F_heap* now = F_heap_head;
        struct F_heap* last = NULL;
        while(now->data != target) {
            last = now;
            now = now->nextNode;
        }
        if(last == NULL) F_heap_head = now->nextNode;
        else {
            last->nextNode = now->nextNode;
        }
    }

    return;
}

void decreaseKey(int key, int value) {
    struct Node* target = hashMap[key];
    target->key -= value;
    hashMap[key] = NULL;
    hashMap[key-value] = target;

    if(target->parent != NULL && target->parent->key > target->key) {
        struct Child* now = target->parent->child_head;

        if(now->data == target) {
            if(now->next != NULL) target->parent->child_head = now->next;
            else target->parent->child_head = NULL;
        }

        else {
            struct Child* last = NULL;
            while(now->data != target) {
                last = now;
                now = now->next;
            }
            last->next = now->next;
        }

        if(target->parent->mark == 1) {
            target->parent->degree -= 1;
            addToList(target);
            CascadingCut(target->parent);
        }
        else {
            target->parent->mark = 1;
            target->parent->degree -= 1;
            addToList(target);
        }
    }
}
void bfs(struct Node* target) {
    struct Node* queue[10001];
    int idx = 1, now_idx = 0;
    queue[0] = target;
    while( now_idx != idx ) {
        printf("%d ", queue[now_idx] -> key);
        struct Child* now_ptr = queue[now_idx] -> child_head;
        int index = 0;
        struct Child* tmp[10001];
        while( now_ptr != NULL ) tmp[index++] = now_ptr, now_ptr = now_ptr -> next;
        qsort(tmp, index, sizeof(struct Child*), qsort_child);
        for(int i=0;i<index;i++) queue[idx++] = tmp[i] -> data;
        now_idx++;
    }
    printf("\n");
}
void printHeap() {
    DegreeList();
    struct F_heap* now = F_heap_head;
    while(now != NULL) {
        bfs(now->data);
        now = now->nextNode;
    }

    return;
}

struct OperatorType {
    int insert;
    int delete;
    int decrease;
    int extract_min;
    int exit;
};

struct InputData {
    struct OperatorType operator;
    int key, value;
};


int main() {
    while (1) {
        char opt[20] = {};
        struct InputData input_data = {0};  // 初始化结构体，避免随机值
        
        scanf("%s", opt);
        
        if (strcmp(opt, "insert") == 0) {
            input_data.operator.insert = 1;
            scanf("%d", &(input_data.key));
            insert(input_data.key);
        } else if (strcmp(opt, "delete") == 0) {
            input_data.operator.delete = 1;
            scanf("%d", &(input_data.key));
            delete(input_data.key);
            consolidate();
        } else if (strcmp(opt, "decrease") == 0) {
            input_data.operator.decrease = 1;
            scanf("%d %d", &(input_data.key), &(input_data.value));
            decreaseKey(input_data.key, input_data.value);
        } else if (strcmp(opt, "extract-min") == 0) {
            input_data.operator.extract_min = 1;
            if (findMin() != NULL) {
                delete(findMin()->key);
                consolidate();
            } else {
                fprintf(stderr, "Error: Heap is empty.\n");
            }
        } else if (strcmp(opt, "exit") == 0) {
            input_data.operator.exit = 1;
            break;
        } 
    }

    printHeap();  // 打印堆的最终状态
    return 0;
}
