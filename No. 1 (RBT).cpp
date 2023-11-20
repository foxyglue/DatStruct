#include <stdio.h>
#include <stdlib.h>

struct Node {
    int angka;
    char color;
    Node *parent;
    Node *left;
    Node *right;
};

struct RedBlackTree {
    Node *root;
    Node *nil;
};

//createNode
Node *createNode(int angka, char color, Node *parent, Node *left, Node *right) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    
    newNode->angka = angka;
    newNode->color = color;
    newNode->parent = parent;
    newNode->left = left;
    newNode->right = right;
    
    return newNode;
}

//redBlack
RedBlackTree *createRedBlackTree() {
    RedBlackTree *rbTree = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    
    rbTree->nil = createNode(0, 'B', NULL, NULL, NULL);
    rbTree->root = rbTree->nil;
    
    return rbTree;
}

//left rotate
void leftRotate(RedBlackTree *rbTree, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    
    if (y->left != rbTree->nil)
        y->left->parent = x;
        
    y->parent = x->parent;
    
    if (x->parent == rbTree->nil)
        rbTree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
        
    y->left = x;
    x->parent = y;
}

//right rotate
void rightRotate(RedBlackTree *rbTree, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    
    if (x->right != rbTree->nil)
        x->right->parent = y;
        
    x->parent = y->parent;
    
    if (y->parent == rbTree->nil)
        rbTree->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
        
    x->right = y;
    y->parent = x;
}

void rbInsertFixup(RedBlackTree *rbTree, Node *z) {
	//selama parent Red
    while (z->parent->color == 'R') {
    	//jika parent sama dgn sibling kiri
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right; //uncle si z
            
            //jika pamannya z merah
            if (y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                
                z = z->parent->parent;
            
			//jika pamannya z hitam    
            } else {
            	//jika z adl anak kanan
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(rbTree, z);
                }
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                
                rightRotate(rbTree, z->parent->parent);
            }
        
        //jika parent sama dgn sibling kanan
        } else {
            Node *y = z->parent->parent->left; //unclenya z
            
            //jika pamannya z merah
            if (y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            } 
			//jika pamannya z hitam
			else {
				//jika z adl anak kiri
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(rbTree, z);
                }
                
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                leftRotate(rbTree, z->parent->parent);
            }
        }
    }
    rbTree->root->color = 'B';
}

void rbInsert(RedBlackTree *rbTree, int angka) {
    Node *z = createNode(angka, 'R', NULL, rbTree->nil, rbTree->nil); //angka yang dimasukkan
    Node *y = rbTree->nil; //parent z
    Node *x = rbTree->root; //x = current node s/d posisi untuk z ditemukan
    
    //selama x tdk kosong
    while (x != rbTree->nil) {
        y = x; //menggantikan posisi x sblmnya yg tlh pindah sbg parent
        if (z->angka < x->angka) //angka baru lbh kecil dr current angka
            x = x->left;
        else
            x = x->right;
    }
    
    //posisi z untuk memasukkan z ditemukan
    z->parent = y;
    
    if (y == rbTree->nil) //z == root
        rbTree->root = z;
    else if (z->angka < y->angka) //z anak kiri y
        y->left = z;
    else
        y->right = z;
        
    //anak z kosong dan warnanya merah krn node baru
    z->left = rbTree->nil;
    z->right = rbTree->nil;
    z->color = 'R';
    
    rbInsertFixup(rbTree, z); //melakukan perbaikan apabila terjadi violation 
}

void inOrderTraversal(Node *root, Node *nil) {
    if (root != nil) {
        inOrderTraversal(root->left, nil);
        printf("%d ", root->angka);
        inOrderTraversal(root->right, nil);
    }
}

int main() {
    RedBlackTree *rbTree = createRedBlackTree();

    int angka[] = {41, 22, 5, 51, 48, 29, 18, 21, 45, 3};
    int dataSize = sizeof(angka) / sizeof(angka[0]);

    for (int i = 0; i < dataSize; i++) {
        rbInsert(rbTree, angka[i]);
    }

    printf("InOrder Traversal of Created Tree:\n");
    inOrderTraversal(rbTree->root, rbTree->nil);
    printf("\n");

    return 0;
}

