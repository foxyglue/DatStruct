#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
	int key;
	int height;
	Node* right;
	Node* left;
};

Node* createNode(int key){
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->key = key;
	newNode->height = 1; //Jadi leaf karena paling bawah
	newNode->right = newNode->left = NULL;
	return newNode;
}

//height
int getHeight(Node* curr){
	if(!curr){
		return 0;
	}else{
		return curr->height;
	}
}

int max(int a, int b){
	if(a>b){
		return a;
	}else{
		return b;
	}
}


//Get balance factor
int getBalf(Node* curr){ 
	return getHeight(curr->left) - getHeight(curr->right);
}

//left rotate
Node* leftRotate(Node* curr){
	Node* child = curr->right;
	Node* subChild = child->left;
	
	child->left = curr;
	curr->right = subChild;
	
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	child->height = max(getHeight(child->left), getHeight(curr->right)) + 1;
	
	return child;
}

//right rotate
Node* rightRotate(Node* curr){
	Node* child = curr->left;
	Node* subChild = child->right;
	
	child->right = curr;
	curr->left = subChild;
	
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	child->height = max(getHeight(child->left), getHeight(child->right)) +1;
	
	return child;
}

//Insertion
Node* insertion(Node* curr, Node* newNode){
	if(!curr){
		return newNode;
	}else if(newNode->key > curr->key){
		curr->right = insertion(curr->right, newNode);
	}else if(newNode->key < curr->key){
		curr->left = insertion(curr->left, newNode);
	}else{
		return curr;
	}
	
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	
	/*
	Balance factor= Tinggi anak kiri - tinggi anak kanan
	*/
	int balf = getBalf(curr);
	//kanan = -
	//kiri = +
	
	//left-left -> miring ke kiri, putar ke kiri
	if(balf > 1 && getBalf(curr->left) >= 0){
		return rightRotate(curr);
	}
	
	//left-right -> miring ke kiri lalu kanan, putar ke kiri lalu kanan
	else if(balf > 1 && getBalf(curr->left) <0){
		curr->left = leftRotate(curr->left);
		return rightRotate(curr);
	}
	
	//right right -> miring ke kanan, putar ke kanan
	else if(balf < -1 && getBalf(curr->right) <= 0){
		return leftRotate(curr);
	}
	
	//right left -> miring ke kanan lalu kiri, putar ke kanan lalu kiri
	else if(balf < -1 && getBalf(curr->right) > 0){
		curr->right = rightRotate(curr->right);
		return leftRotate(curr);
	}
	
	return curr;
}

//search
Node* searchNode(int target, Node* curr){
	if(!curr){
		return NULL;
	}else if(target > curr->key){
		return searchNode(target, curr->right);
	}else if(target < curr->key){
		return searchNode(target, curr->left);
	}else{ //kalo ketemu
		return curr; 
	}
}


//predecessor
Node* getPredecessor(Node* curr){
	Node* child = curr->left;
	while(child->right){ //selama anak kanan masih ada
		child = child->right;
	}
	return child;
}

//Delete
Node* deleteNode(int target, Node* curr){
	if(!curr){
		return NULL;
	}else if(target > curr->key){
		curr->right = deleteNode(target, curr->right);
	}else if(target < curr->key){
		curr->left = deleteNode(target, curr->left);
	}else{
		//Have no child
		if(!curr->left && !curr->right){
			free(curr);
			curr = NULL;
		}
		//1 child
		else if(!curr->left || !curr->right){
			Node* child = (curr->left)? curr->left : curr->right;
			
			free(curr);
			curr= child;
		}
		//2 child
		else{
			Node* predecessor = getPredecessor(curr);
			curr->key = predecessor->key;
			curr->left = deleteNode(predecessor->key, curr->left);
		}
	}
	if(curr == NULL){
		return curr;
	}
	
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) +1;
	
	int balF = getBalf(curr);
	
	//left left -> putar ke kanan
	if(balF > 1 && getBalf(curr->left) >= 0){
		return rightRotate(curr);
	}
	
	//left right -> putar ke kiri lalu kanan
	else if(balF > 1 && getBalf(curr->left) < 0){
		curr->left = leftRotate(curr->left);
		return rightRotate(curr);
	}
	
	//right right -> putar ke kiri
	else if(balF < -1 && getBalf(curr->right) <= 0){
		return leftRotate(curr);
	}
	
	//right left -> putar ke kanan lalu kiri
	else if(balF < -1 && getBalf(curr->left) > 0){
		curr->right = rightRotate(curr->right);
		leftRotate(curr);
	}
	
	return curr;
}

Node* preOrder(Node* curr){
	if(curr){
		printf("%d ", curr->key);
		preOrder(curr->left);
		preOrder(curr->right);
	}	
}

Node* inOrder(Node* curr){
	if(curr){
		inOrder(curr->left);
		printf("%d ", curr->key);
		inOrder(curr->right);
	}	
}

Node* postOrder(Node* curr){
	if(curr){
		postOrder(curr->left);
		postOrder(curr->right);
		printf("%d ", curr->key);
	}	
}

int main(){
	int opt;
	Node* root = NULL;
	
	menu:
		do{
			puts("1. Insertion");
			puts("2. Deletion");
			puts("3. Traversal");
			puts("4. Exit");
			printf("Choose: ");
			scanf("%d", &opt);
			getchar();
		}while(opt < 1 || opt > 4);
		
		switch(opt){
			case 1:
				{
				int insert;
				
				printf("Insert: ");
				scanf("%d", &insert);
				getchar();
				root = insertion(root, createNode(insert));
				
				goto menu;
				break;
				}
			
			case 2:
				{
				int toDel;
				
				printf("Delete: ");
				scanf("%d", &toDel);
				getchar();
				
				Node* cek = searchNode(toDel, root);
				
				if(cek){
					root = deleteNode(toDel, root);
					puts("Data found!");
					printf("Value %d was deleted\n", toDel);
					
					goto menu;
					break;
				}else{
					puts("Data not found!");
					
					goto menu;
					break;
				}
				}
				
			case 3:
				{
				printf("Preorder: ");
				preOrder(root);
				puts("");
				printf("Inorder: ");
				inOrder(root);
				puts("");
				printf("Postorder: ");
				postOrder(root);
				}
				
			case 4:
				puts("Thank you!");
				return 0;
		}
		
}
