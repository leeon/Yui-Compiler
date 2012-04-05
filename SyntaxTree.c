#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "state.h"
#include "ylib.h"

/*
Table.c
@author octobershiner
@version 0.1
2011 12 15
Description:  The file define the data structure of the syntax tree
*/

/*Initialize the root of the syntax tree*/
struct TreeNode* initTree(struct TreeNode* h)
{
	char name[10] = "root";
	h = addTree(h,0,VN,name);
	printf("Syntax Tree built successfully.\n");
	return h;
	
}

/*Function : add a new node to the tree*/
struct TreeNode* addTree(struct TreeNode* currentTree,int p_order,int p_value,char* p_name)
{
	struct TreeNode* newTreeNode= NULL;
	int children_num = 0;
	int i = 0;
	newTreeNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	if(newTreeNode == NULL)
	{
		printf("fail in creating syntax tree node .\n");
		exit(0);
	}
	
	newTreeNode->order = p_order;
	newTreeNode->value = p_value;
	strcpy(newTreeNode->name,p_name);
	/*initialize the children of a node */
	for (children_num = 0; children_num <15; children_num++)
	{
		newTreeNode->children[children_num] = NULL;
	}
	
	if(currentTree == NULL)
	{
		newTreeNode->parent = NULL;
		currentTree = newTreeNode;	
	}
	else
	{
		newTreeNode->parent = currentTree;
		while(currentTree->children[i] != NULL)
		i++;
		currentTree->children[i] = newTreeNode;
		/*指针向子节点移动 非终结符的时候不回退*/
		if(newTreeNode->value == VN)
		{
		currentTree = currentTree->children[i];
		}
	}
	return currentTree;
}


/*递归遍历语法分析树*/
void traverse(struct TreeNode* h,FILE*gf)
{
	int i =0;
	struct TreeNode * temp = h;
	
	int m = 0;
	/*write to the graphviz file*/
	while(temp->children[m] != NULL)
	{
		fprintf(gf, "\t\"%d %s\" -> \"%d %s\";\n", temp->order, temp->name,temp->children[m]->order,temp->children[m]->name);
		m++;
	}
	
    for(temp = h->children[i];temp !=NULL;temp = h->children[++i])
	traverse(temp,gf);
}
/*利用graph visualization 绘制语法分析树*/
void graphviz()
{
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("YuiSem-> drawing the syntax tree....please wait......\n");
	printf("YuiSem-> Done^_^.\n");
	FILE* gf= NULL;
	 if ( ( gf = fopen("syntax.dot","w+") ) == NULL)
	 {
			printf("YuiSem->Error: Create file fail.\n");
			exit(0);
	 } 
	 
	fputs("digraph vis {\n", gf);
	fputs("\tsize=\"26,26\";\n", gf);
	fputs("\tnode [color=lightblue2, style=filled];\n", gf);

	traverse(SyntaxTree,gf);
	fputs("}\n", gf);
	fclose(gf);
	
}