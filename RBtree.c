#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum { RED, BLACK } Color;

typedef struct Node {
    int key;
    Color color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;


/* 左旋轉（原程式有） */
void leftRotate(Node** root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

/* 右旋轉（原程式有） */
void rightRotate(Node** root, Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

/* 插入修正函式（原程式有） */
void insertFixup(Node** root, Node* z) {
    while (z->parent != NULL && z->parent->color == RED) {
        Node* parent = z->parent;
        Node* grandparent = parent->parent;
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            } else {
                if (z == parent->right) {
                    z = parent;
                    leftRotate(root, z);
                    parent = z->parent;
                    grandparent = parent->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rightRotate(root, grandparent);
            }
        } else {
            Node* uncle = grandparent->left;
            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            } else {
                if (z == parent->left) {
                    z = parent;
                    rightRotate(root, z);
                    parent = z->parent;
                    grandparent = parent->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                leftRotate(root, grandparent);
            }
        }
    }
    (*root)->color = BLACK;
}

/* 插入 (原程式有) */
void insert(Node** root, int data) {
    Node* z = (Node*)malloc(sizeof(Node));
    z->key = data;
    z->left = NULL;
    z->right = NULL;
    z->parent = NULL;
    z->color = RED;

    Node* y = NULL;
    Node* x = *root;
    while (x != NULL) {
        y = x;
        if (data < x->key)    x = x->left;
        else if (data > x->key)    x = x->right;
        else free(z);
    }
    z->parent = y;
    if (y == NULL) {
        *root = z;
    } else if (data < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    // 插入修复
    while (z->parent != NULL && z->parent->color == RED) {
        Node* parent = z->parent;
        Node* grandparent = parent->parent;

        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            } else {
                if (z == parent->right) {
                    z = parent;
                    leftRotate(root, z);
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rightRotate(root, grandparent);
            }
        } else {
            Node* uncle = grandparent->left;
            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            } else {
                if (z == parent->left) {
                    z = parent;
                    rightRotate(root, z);
                }
                parent->color = BLACK;
                grandparent->color = RED;
                leftRotate(root, grandparent);
            }
        }
    }

    (*root)->color = BLACK;
    
}

/* 搜尋 (原程式有) */
Node* search(Node* root, int x) {
    while (root != NULL && root->key != x) {
        if (x < root->key)
            root = root->left;
        else
            root = root->right;
    }
    return root;
}

/* 釋放整棵樹 (原程式有) */
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/* 主程式 (原程式) */
int main() {
    Node* root = NULL;
    int value;
    char operation[20];

    while (1) {
        scanf("%s", operation);
        if (strcmp(operation, "insert") == 0) {
            scanf("%d", &value);
            insert(&root, value);
        } else if (strcmp(operation, "search") == 0) {
            scanf("%d", &value);
            Node* result = search(root, value);
            if (result != NULL) {
                printf("%s\n", (result->color == RED) ? "red" : "black");
            } else {
                printf("Not found\n");
            }
        } else if (strcmp(operation, "quit") == 0) {
            break;
        }
    }

    freeTree(root);
    return 0;
}