/*
Program Descriptions : 
	c program for avl tree
	there may reside duplicate key values in the avl tree
	predecessor of a key value is a key value exists in the tree which is just less than the given key
	successor of a key value is a key value exists in the tree which is just greater than the given key

*/


#include<stdio.h>
#include<stdlib.h>


// difining structure for a tree node
struct treeNode {
	float data;
	int height;
	struct treeNode *parent;
	struct treeNode *left;
	struct treeNode *right;
};
typedef struct treeNode *tree;


// show a binary tree in 90 degree anti-clockwise rotated format
void printTree(tree root, int level) {
	int c;

	if(root == NULL)
		return;

	printTree(root->right, level + 1); // print right subtree

	for(c = 0; c < level ; c++) // print specified no of tabs
		printf("\t");

	printf("%d\n\n", (int)root->data); // print the data value in integer format for visual simplicity

	printTree(root->left, level + 1); // print the left subtree
}


// get the height of a tree
int getHeight(tree t) {
	if(t == NULL)
		return -1;
	return t->height;
}


// free memory allocated for a tree
void freeTree(tree t) {
	if(t == NULL)
		return;
	freeTree(t->left);
	freeTree(t->right);
	free(t);	
}


//allocate memory for a tree node and return the pointer to the node
tree newTree() {
	tree t = malloc(sizeof(struct treeNode));
	t->left = NULL;
	t->right = NULL;
	t->height = 0;
	t->parent = NULL;
	return t;
}


// find no of occurences of an element in a avl tree
int countNode(tree root, float data) {

	if(root == NULL)
		return 0;

	if(root->data == data) // if data is equal to the current node data then this data may occure in the left or right  subtree also
		return (1 + countNode(root->left, data) + countNode(root->right, data));

	if(root->data < data)
		return countNode(root->right, data);

	return countNode(root->left, data);
}


// find inorder predecessor of an element 'x' in the tree 
int predecessor(tree t, float x) {

	tree temp = NULL, p;

	if(t == NULL) // if tree is empty
		return 0;

	// search the data in the tree and store the node pointer in 'temp', if there is duplicate data find the leftmost in the inorder traversal
	while(t != NULL) { 

		if(t->data == x) {
			temp = t;
			t = t->left;
		}

		else if(x < t->data)
			t = t->left;

		else
			t = t->right;
	}
	
	if(temp == NULL) // if data has not been found
		return 0;

	// if the leftchild of the found node is not empty
	if(temp->left != NULL) {
		temp = temp->left;
		while(temp->right != NULL)
			temp = temp->right;
		return temp->data;
	}

	p = temp->parent;

	while(p != NULL) { // go up the tree until p is a left child of its parent
		if(p->left == temp) {
			temp = p;
			p = p->parent;
		}
		else
			break;
	}

	if(p == NULL) // if there is no predecessor then return 0
		return 0;

	return p->data;	// return the found predecessor
}



// find inorder successor of an element 'x' in the tree 
int successor(tree t, float x) {

	tree temp = NULL, p;

	if(t == NULL) // if tree is empty
		return 0;

	while(t != NULL) { // search the data in the tree and store the node pointer in 'temp'

		if(t->data == x) {
			temp = t;
			t = t->right;
		}

		else if(x < t->data)
			t = t->left;

		else
			t = t->right;
	}
	
	if(temp == NULL) // if data has not been found
		return 0;

	if(temp->right != NULL) {
		temp = temp->right;
		while(temp->left != NULL)
			temp = temp->left;
		return temp->data;
	}

	p = temp->parent;

	while(p != NULL) {
		if(p->right == temp) {
			temp = p;
			p = p->parent;
		}
		else
			break;
	}

	if(p == NULL)
		return 0;

	return p->data;	
}


// rotate right in node g for the avl tree '*root'
void rotateRight(tree *root, tree g) {
	tree p, t;

	t = g->parent;
	p = g->left;

	g->left = p->right;
	p->right = g;

	if(g->left != NULL)
		g->left->parent = g;
	p->parent = g->parent;
	g->parent = p;

	if(t == NULL)
		*root = p;
	else if(t->left == g)
		t->left = p;
	else
		t->right = p;	
}



// rotate left in node g
void rotateLeft(tree *root, tree g) {
	tree p, t;

	t = g->parent;
	p = g->right;

	g->right = p->left;
	p->left = g;

	if(g->right != NULL)
		g->right->parent = g;
	p->parent = g->parent;
	g->parent = p;

	if(t == NULL)
		*root = p;
	else if(t->left == g)
		t->left = p;
	else
		t->right = p;		
}

//return max of two integers
int max(int a, int b) {
	if(a > b)
		return a;
	return b;
}


// return abosolute value of an integer
int abs(int a) {
	if(a >= 0)
		return a;
	return (-a);
}

// delete a node with a certain value iteratively
int delete (tree *root, float data) {
	tree t = *root, temp = NULL, p, left, right;
	int leftHeight,rightHeight, maxHeight, hdiff, originalHeight, h;

	if(t == NULL) // if tree is empty
		return 0;

	if(t->left == NULL && t->right == NULL) { // if tree has only one node
		if(t->data == data) {
			*root = NULL;
			free(t);
			return 1;
		}
		else
			return 0;
	}

	while(t != NULL) { // search the data in the tree and store the node pointer in 'temp'

		if(t->data == data) {
			temp = t;
			t = t->left;
		}

		else if(data < t->data)
			t = t->left;

		else
			t = t->right;
	}
	
	if(temp == NULL) // if data has not been found
		return 0;

	else if(temp->left == NULL && temp->right == NULL);

	else if(temp->left == NULL && temp->right != NULL) { // if left subtree of deleting node is empty
		temp->data = temp->right->data;
		temp = temp->right;
	}

	else if(temp->left != NULL && temp->right == NULL) {// if right subtree of deleting node is empty
		temp->data = temp->left->data;
		temp = temp->left;
	}

	else{
		t = temp;
		temp = temp->right;

		while(temp->left != NULL) // find the successor
			temp = temp->left;

		t->data = temp->data;// replace the deleting node data with successor data

		if(temp->right != NULL) { // if the successor have one right child node that is eventually a leave node
			temp->data = temp->right->data;
			temp = temp->right;
		}
	}

	// At last 'temp' node is the deleting node which is a leave node
	t = temp;
	p= temp->parent;

	if(t == p->left) //delete the node 
		p->left = NULL;
	else 
		p->right = NULL;

	free(t); // deallocate memory for the deleting node

	while(p != NULL) {
		left = p->left;
		right = p->right;

		leftHeight = getHeight(left);
		rightHeight = getHeight(right);

		h = p->height;
		hdiff = abs(leftHeight - rightHeight);

		if(hdiff <= 1) { // if this node is balanced

			maxHeight = max(leftHeight, rightHeight);

			if( (maxHeight+1) == h) // if height is matched with the parent
				return 1;

			p->height -= 1; // otherwise decrease height
			p = p->parent;
		}
		else {
			if(leftHeight > rightHeight) { // left case
				temp = p->left;
				if(getHeight(temp->left) >= getHeight(temp->right)) { // left left case
					rotateRight(root, p);

					originalHeight = p->height;
					leftHeight =getHeight(p->left);
					rightHeight = getHeight(p->right);

					p->height = 1 + max(leftHeight, rightHeight);
					temp->height = 1 + p->height;

					if(temp->height == originalHeight)
						return 1;
					p = temp->parent;
				}
				else { // left right case
					rotateLeft(root, temp);
					rotateRight(root, p);
					p->height -= 2 ;
					temp->height -= 1;
					temp->parent->height += 1;
					p = temp->parent->parent;
				}
			}
			else { // right case
				temp = p->right;
				if(getHeight(temp->right) >= getHeight(temp->left)) { // left left case
					rotateLeft(root, p);

					originalHeight = p->height;
					leftHeight = getHeight(p->right);
					rightHeight = getHeight(p->left);

					p->height = 1 + max(leftHeight, rightHeight);
					temp->height = 1 + p->height;

					if(temp->height == originalHeight)
						return 1;
					p = temp->parent;
				}
				else { // left right case
					rotateRight(root, temp);
					rotateLeft(root, p);
					p->height -= 2 ;
					temp->height -= 1;
					temp->parent->height += 1;
					p = temp->parent->parent;
				}
			}
		}	
	}

	return 1;	
}


// insert data into the tree iteratively
int insert(tree *root, float data) {

	tree t = *root, temp, p, leftChild, rightChild;
	int leftHeight, rightHeight;
	
	temp = newTree();
	temp->data = data;

	if(t == NULL) { // if the tree is empty
		*root = temp;
		return 1;
	}

	while(1) { // find the insertion point

		if(data <= t->data) {
			if(t->left == NULL) {
				t->left = temp;
				temp->parent = t; 
				break;
			}
			else
				t = t->left;
		}

		else {
			if(t->right == NULL) {
				t->right = temp;
				temp->parent = t; 
				break;
			}
			else
				t = t->right;
		}
	}
	p = temp->parent;

	// change height of nodes if necessary from bottom to up and if there is any unbalance situation then rotate left or right
	while(p != NULL) {
		if(p->left == temp) { // left case
			rightChild = p->right;
			leftHeight = getHeight(temp);
			rightHeight = getHeight(rightChild);

			if( leftHeight == rightHeight) // if tree remains balanced
				return 1;

			if( (leftHeight+1) == rightHeight)
				return 1;

			if( leftHeight == (rightHeight+1) ) { 
				p->height += 1;
				temp = p;
				p = p->parent;
			}
			else { // leftHeight == (rightHeight+2)
				leftHeight = getHeight(temp->left);
				rightHeight = getHeight(temp->right);

				if(leftHeight > rightHeight) { // left left case
					rotateRight(root, p);
					p->height -= 1;

				}
				else { // left right case
					rotateLeft(root, temp);
					rotateRight(root, p);
					p->height -= 1;
					temp->height -= 1;
					temp->parent->height = p->height + 1;

				}
				return 1;

			}
			
		}

		else { // right case
			rightChild = p->left;
			leftHeight = getHeight(temp);
			rightHeight = getHeight(rightChild);

			if( leftHeight == rightHeight)
				return 1;

			if( leftHeight == (rightHeight+1) ) { 
				p->height += 1;
				temp = p;
				p = p->parent;
			}
			else { // leftHeight == (rightHeight+2)
				leftHeight = getHeight(temp->right);
				rightHeight = getHeight(temp->left);

				if(leftHeight > rightHeight) { // right right case
					rotateLeft(root, p);
					p->height -= 1;

				}
				else { // right left  case
					rotateRight(root, temp);
					rotateLeft(root, p);
					p->height -= 1;
					temp->height -= 1;
					temp->parent->height = p->height + 1;

				}
				return 1;

			}
			
		}
		
	}
}


int main() {
	tree root = NULL;
	int choice, n, count, i;
	char enter;
	float data, pdata, sdata;

	do {	
		printf("\nEnter Your Choice Number from the list below and press enter key:\n\n");
		printf("1. Create an avl tree from a set of real numbers\n");
		printf("2. Insert an element into the tree\n");
		printf("3. Search an element in the tree\n");
		printf("4. Delete elements with particular value from the tree\n");
		printf("5. Find predecessor and successor of an element\n");
		printf("6. Print the avl tree\n");
		printf("0. Exit\n\n");
		printf("Choice Number :$");
		
		fflush(stdin);
		if( (scanf("%d%c", &choice, &enter)) != 2 || enter != '\n' || choice < 0 || choice > 6 ) {
				printf("\nYour choice is not valid for any type of operations !\n\n");
				exit(1);
		}

		else {
			switch(choice) {

				case 1: 
					freeTree(root);// FREE MEMORY ALLOCATED FOR THE TREE NODES
					root = NULL; //reset the tree to empty
					printf("How many elements are there in the set:$");
					if( (scanf("%d%c", &n, &enter)) != 2 || enter != '\n' || n <= 0 )
						printf("\nInvalid input !\n\n");
					else {
						for(i = 0; i< n ; i++) {
							printf("Enter %d-th element:$",i+1);
							if( (scanf("%f%c", &data, &enter)) != 2 || enter != '\n' || data <= 0 ) {
								printf("\nInvalid input !, although previous elements has been inserted.\n\n");
								exit(1);
							}
							insert(&root,data);
						}
					}
					printf("Insertion completed successfully\n");
					break;

				
				case 2: 
					printf("Enter the element:$");
					if( (scanf("%f%c", &data, &enter)) != 2 || enter != '\n' || data <= 0 ) {
						printf("\nInvalid input !\n\n");
						break;
					}
					count = insert(&root,data);
					if(count == 1)
						printf("Inserted successfully\n");
					else
						printf("Inserted Failed\n");
					break;
				
				case 3: 
					printf("Enter the element:$");
					if( (scanf("%f%c", &data, &enter)) != 2 || enter != '\n' || data <= 0 ) {
						printf("\nInvalid input !\n\n");
						break;
					}
					count = countNode(root,data);
					printf("\nThe element %f occures in the tree %d times\n",data, count);
					break;
				case 5: 
					printf("Enter the element:$");
					if( (scanf("%f%c", &data, &enter)) != 2 || enter != '\n' || data <= 0 ) {
						printf("\nInvalid input !\n\n");
						break;
					}
					pdata = predecessor(root, data);
					sdata = successor(root, data);
					count = countNode(root,data);
					printf("If there is no predecessor or successor for an element then 0.0 will be printed\n");
					printf("\nFor The element %f, The predecessor is %f and the successor is %f\n",data, pdata, sdata);
					break;
				case 4: 
					printf("Enter the element:$");
					if( (scanf("%f%c", &data, &enter)) != 2 || enter != '\n' || data <= 0 ) {
						printf("\nInvalid input !\n\n");
						break;
					}
					count = countNode(root,data);
					for(i = 0 ; i < count ; i++)
						delete(&root,data);
					if(count >= 1)
						printf("%d elements deleted successfully.\n", count);
					else
						printf("Data not found !\n");
					break;
				case 6: 
					if(root == NULL)
						printf("Tree is empty\n");
					else {
						printf("For the sake of simple diagram the integral part of the real numbers will be printed.\n");
						printf("\nDiagram of the tree ignoring the edges in 90 degree anti-clockwise rotated format:\n");
						printTree(root, 0);
					}
					break;

				case 0:	//exit from program
					printf("\nProgram terminates successfully\n\n");
					freeTree(root); // FREE MEMORY ALLOCATED FOR THE TREE NODES
					exit(0); // if choice is '0' program terminates
			}
		}
		printf("-------------------------\n") ;
	}while(1);
	
	return 0;
}
