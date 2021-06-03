#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#define SIZE 30

typedef struct Node
{
    int data;
    struct Node* child1;
    struct Node* child2;
}Node;

Node* createNode(int num)
{
    Node* node=(Node*)malloc(sizeof(Node));
    if(node!=NULL)
    {
        node->child1=NULL;
        node->child2=NULL;
        node->data=num;
    }
    //printf("%p %d\n",node ,node->data);
    return node;
}

int addChild(Node* node, int val)
{
    if(node!=NULL)
    {
        if( node->child1==NULL ){
            node->child1=createNode(val);
            return 1;
        }
        else if( node->child2==NULL ){
            node->child2=createNode(val);
            return 1;
        }
        else{
            return -1;
        }
    }
    return -1;
}


int maxDepth(Node* root)
{
    if( root==NULL )
    {
        return -1;
    }
    else
    {
        int lDepth=maxDepth(root->child1);
        int rDepth=maxDepth(root->child2);
        return (fmax(lDepth,rDepth)+1);
    }
}

void printTab(int level)
{
    for(int i=0 ; i<level ;i++)
    {
        printf("\t");
    }
}

void printTree(Node* root ,int level)
{
    if(root==NULL){
        return;
        }
    printTab(level);
    printf("%d\n",root->data);
    printTree(root->child1 ,level+1);
    printTree(root->child2 ,level+1);
}



void sortArr(int nums[], int size)
{
    for(int i=0;i<size;i++)
    {
        for(int j=i+1;j<size;j++)
        {
            if(nums[j]<nums[i])
            {
                int temp=nums[i];
                nums[i]=nums[j];
                nums[j]=temp;
            }
        }
    }
}

void printArr(int nums[], int size)
{
    for(int i=0;i<size;i++)
    {
        printf("%d ",nums[i]);
    }
    printf("\n");
}

void fillArr(int nums[] ,int size ,int sort) //Fills the given array with random numbers. Then sorts it according the sort parameter.
{
    srand(time(NULL));
    int sumNum=0;
    for(int i=0;i<SIZE;i++)
    {
        nums[i]=rand() % 100;
        sumNum+=nums[i];
    }
    printf("SUM Arr :%d\n",sumNum);
    if( sort == 1 )
        sortArr(nums ,SIZE);
}

Node* populate(int nums[] ,int left ,int right) //Creates a binary search tree.
{
    if( left > right  ) return NULL;
    int mid = left + (right - left)/2;
    Node* node=createNode(nums[mid]);
    node->child1 = populate(nums ,left ,mid-1);
    node->child2 = populate(nums ,mid+1 ,right);
    return node;
}

Node* fill(int nums[] ,Node* root ,int i ,int size) //Fills tree with a linear approach.
{
    if( i < size )
    {
        Node* temp=createNode(nums[i]);
        root=temp;
        //printf("i:%d\n",i);
        root->child1=fill(nums ,root->child1 ,2*i+1 ,size);
        root->child2=fill(nums ,root->child2 ,2*i+2 ,size);
    }
    return root;
}

Node* search(Node* root ,int key)
{
    if ( root == NULL || root->data == key )
        return root;
    if( root->data < key )
        return search(root->child2, key);
    if( root->data > key )
        return search(root->child1, key);
    return NULL;
}

Node* getParent(Node* root ,int key ,Node* parent)
{
    if ( root == NULL || root->data == key )
        return parent;
    if( root->data < key )
        return getParent(root->child2, key, root);
    if( root->data > key )
        return getParent(root->child1, key, root);
    return parent;
}

Node* smallestNode(Node* root) //Returns the smallest node in given sub-tree
{
    if ( root->child1 == NULL )
        return root;
    else
        return smallestNode(root->child1);
}

int deleteNode(Node* root ,int num)
{
    Node* temp=(Node*)malloc(sizeof(Node));
    Node* parent=(Node*)malloc(sizeof(Node));
    if( root==NULL )
        return -1;
    else if( search(root ,num)==NULL )
    {
        return -1;
    }
    else
    {
        temp=search(root ,num);
        if( temp->child1==NULL && temp->child2==NULL )  //Deleting a leaf node
        {
            parent = getParent(root ,temp->data ,NULL);
            if( parent->child1 == temp ) //Leaf is child1 1.1
            {
                parent->child1 = NULL;
                free(temp);
                return 1;
            }
            else //Leaf is child2 1.2
            {
                parent->child2 = NULL;
                free(temp);
                return 1;
            }
        }
        else if( temp->child1==NULL && temp->child2!=NULL ) // Deleting a node with a only right subtree.(child2 exist)
        {
            parent = getParent(root ,num ,NULL);
            if( parent->child1 == temp )  //Node will be deleted is left node of parent 2.1.1
            {
                parent->child1 = temp->child2;
                free(temp);
                temp = NULL;
                return 1;
            }
            else  //Node will be deleted is right node of parent 2.1.2
            {
                parent->child2 = temp->child2;
                free(temp);
                temp = NULL;
                return 1;
            }
        }
        else if( temp->child1!=NULL && temp->child2 == NULL )  // Deleting a node with a only left subtree(child1 exist)
        {
            parent = getParent(root ,num ,NULL);
            if( parent->child1 == temp )  //Node will be deleted is left node of parent 2.2.1
            {
                parent->child1 = temp->child1;
                free(temp);
                temp = NULL;
                return 1;
            }
            else  //Node will be deleted is right node of parent 2.2.2
            {
                parent->child2 = temp->child1;
                free(temp);
                temp = NULL;
                return 1;
            }
        }
        else //Node to delete has both sub trees
        {
            Node* leftMost=(Node*)malloc(sizeof(Node));
            leftMost = smallestNode(temp->child2);
            //parent = getParent(root ,leftMost->data ,NULL);
            if( leftMost->child2 == NULL ) // Most left node under deleted node does not has a right sub tree 3.1
            {
                int tempNum=leftMost->data;
                deleteNode(root, leftMost->data); //Basically leaf
                temp->data = tempNum;
                return 1;
            }
            else // Most left node under deleted node has a right sub tree 3.2
            {
                int tempNum=leftMost->data;
                deleteNode(root ,leftMost->data);
                temp->data = tempNum;
                return 1;
            }
        }
    }
    return -1;
}

void insertNode(Node* root ,int val)
{
    if( root == NULL )
    {
        root=createNode(val);
        return;
    }
    Node* temp=(Node*)malloc(sizeof(Node));
    temp=root;
    while( 1 )
    {
        if( val == temp->data )
            break;
        if( val < temp->data )
        {
            if( temp->child1!=NULL ) {
                temp = temp->child1;
            }
            else {
                temp->child1 = createNode(val);
                break;
            }
        }
        else
        {
            if( temp->child2!=NULL ) {
                temp = temp->child2;
            }
            else {
                temp->child2 = createNode(val);
                break;
            }
        }
    }
}

void preorderTraversal(Node* root)
{
    if( root==NULL)
        return;
    preorderTraversal(root->child1);
    printf("%d ",root->data);
    preorderTraversal(root->child2);
}

int isBST(Node* node, int min, int max) {
	if (node == NULL)
		return 1;
	if (node -> data < min)
		return 0;
  if (node -> data > max)
		return 0;
	return ( isBST(node -> child2, node -> data, max) && isBST(node -> child1, min, node -> data) );
}

int sumOfAll(Node* root)
{
    if( root!=NULL )
        return root->data + sumOfAll(root->child1) + sumOfAll(root->child2);
    return 0;
}

void printGivenLevel(Node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        printf("%d ", root->data);
    else if (level > 1)
    {
        printGivenLevel(root->child1, level-1);
        printGivenLevel(root->child2, level-1);
    }
}

void printLevelOrder(Node* root)
{
    int h = maxDepth(root);
    int i;
    for (i=1; i<=h; i++)
        printGivenLevel(root, i);
}

void test1(int nums[], int size) //BST constructed with populate function.
{
    printf("-------- TEST 1 --------BST\n");
    Node* root=createNode(nums[0]);
    root=populate(nums ,0 ,SIZE-1);
    printTree(root ,0);
    printf("DEPTH :%d\n",maxDepth(root));
}

void test2(int nums[], int size) //Balanced binary tree constructed with fill function.
{
    printf("-------- TEST 2 --------Balanced BT\n");
    Node* root=createNode(nums[0]);
    root=fill(nums ,root ,0 ,SIZE);
    printTree(root ,0);
    int sum=sumOfAll(root);
    printf("SUM :%d\n", sum);
    printf("DEPTH :%d\n",maxDepth(root));
    printf("-----------\n");
}

void test3(int nums[], int size) //Instead of using fill function we use insertNode for every value which results in a skewed binary tree.
{
    printf("-------- TEST 3 --------\n");
    Node* root=createNode(nums[0]);
    for(int i=0;i<size;i++)
    {
        insertNode(root ,nums[i]);
    }
    printTree(root ,0);
    printf("DEPTH :%d\n",maxDepth(root));
}

int main() // Right > Parent || Left < Parent || (right=child2)
{
    int nums[SIZE];
    fillArr(nums ,SIZE ,1); //1 for random arr ,0 for unsorted arr.
    test1(nums ,SIZE);
    test2(nums ,SIZE);
    //test3(nums ,SIZE);
}