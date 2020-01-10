#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFSIZE 1024

struct tree                                                                                                                                                                         
{
    char * data ;
    int wordCount;
    struct tree * left ;
    struct tree * right ;
};

struct Data
{
    char* data;
    int wordCount;
};

typedef struct tree tree;
typedef struct Data Data;

tree* tree_node(char* x);
void treeSort(int arr[], int n);
void storeSorted(tree* root, int arr[], int* i); 
void tree_insert(struct tree ** l, int* treeCount, char * x);   // insert with lexicaly order
void tree_insertWC(struct tree ** l, Data x);   // insert by word count
void tree_traversal(struct tree* l);    // traverse and display node
void node_display(tree* node);
void array_insert(tree *node, Data* arr, int *i);   // tree->array insert
void tree_clean(tree** l);  // delete all nodes

bool isSpace(char c);
void textDisplay(char* line);

struct tree * tree_search(struct tree *l, char * x);                                                                                                                                                                                                                                                                                                                        
struct tree * tree_min(struct tree * l);                                                                            
struct tree * tree_max(struct tree * l);

int main (int argc, char* argv)
{
    struct tree* root = NULL;
    int treeCount = 0;
// 1 file read and insert
    char* fileName = (char*) calloc(100, sizeof(char));
    char* buffer = (char*) calloc(BUFFSIZE, sizeof(char));
    char c; // temp char for input
    int i; // counter
    int white = 0;
    FILE* fp;
    printf("podaj nazwe pliku z danymi\n");
    scanf("%s", fileName);
    fp = fopen(fileName, "r");
    free(fileName);
    if (fp != NULL){
        textDisplay("Odczyt z pliku");
        c = fgetc(fp);
        fseek(fp, 0 , SEEK_CUR - 1);
        while (c != EOF)
        {
            for (i = 0; i < BUFFSIZE && c != EOF; i++)
            {
                c = fgetc(fp);

                if(!isSpace(c))
                    white = 1; // on space mode
                else if (isSpace(c))
                    break;

                *(buffer + i) = c;
                *(buffer + i + 1) = '\0';
            }
            if (white){
                printf("%s\n", buffer);
                tree_insert(&root, &treeCount, buffer);
                white = 0;  // off space mode
            }
        }
        free(buffer);
        fclose(fp);
    } else{
        printf("error - file opening\n");
    }

// 2 traverse and dispaly [inserted by lexical superiority]
    textDisplay("Sorted by lexical order");
    tree_traversal(root);
    printf("correct traverse\n");
// 3 sort by number of occurences
    i = 0;
    Data* tempArray = (Data*) malloc(treeCount * sizeof(Data));
    array_insert(root, tempArray, &i);
    textDisplay("In array");
    tree_clean(&root);  // delete old tree
    root = NULL;
    printf("correct clean\n");
    for (i = 0; i < treeCount; i++){
        tree_insertWC(&root, *(tempArray + i)); // creating new sorted by wordCount tree
    }
// 4 traverse and display [sorted by number of ocurences]
    textDisplay("Sorted by number of occurances order");
    tree_traversal(root);
    printf("correct traverse\n");
// 5 clean tree
    treeCount = 0;
    tree_clean(&root);
    for(i = 0; i < treeCount; i++)
        free((tempArray + i)->data);
    free(tempArray);
    textDisplay("Koniec Programu");

    return 0;
}

// git
void tree_insert(struct tree ** l, int* treeCount, char * x)
{
            if(*l == NULL){
                (*treeCount)++;
                *l = tree_node(x);
                return;
            }
            if( (strcmp(x, (*l)->data) == 0) ){    // word exist so increment count
                (*l)->wordCount++;
                return;
            }
            if(strcmp(x, (*l)->data) < 0)
                return tree_insert(&(*l)->left, treeCount, x) ;
            if(strcmp(x, (*l)->data) > 0)
                return tree_insert(&(*l)->right, treeCount, x) ;
}

void tree_insertWC(struct tree ** l, Data x)
{           
    if (*l == NULL){  
        *l = (tree*) malloc(sizeof(tree));
        (*l)->data = strdup(x.data);
        (*l)->wordCount = x.wordCount;
        (*l)->left = (*l)->right = NULL;
        return;
    }
    if (x.wordCount > (*l)->wordCount)
        return tree_insertWC(&(*l)->left, x) ;
    if (x.wordCount <= (*l)->wordCount)  // if count is equal to partent send right
        return tree_insertWC(&(*l)->right, x) ;
}

tree* tree_node(char* x)   // init node
{
    struct tree * p = (tree*) malloc(sizeof(tree));
    p->data = strdup(x);
    p->wordCount = 1;
    p->right = p->left = NULL;
    return p;
}

void tree_clean(tree** l)
{
    if((*l)->left != NULL)
        tree_clean(&(*l)->left);
    if((*l)->left == NULL && (*l)->right == NULL){
        free((*l)->data);
        free(*l);
    }    
    if((*l)->right != NULL)
        tree_clean(&(*l)->right);    
}

void array_insert(tree *node, Data* arr, int *i)
{
    if(node == NULL)
        return ;

    arr[*i].data = strdup(node->data);
    arr[*i].wordCount = node->wordCount;
    ++*i;
    array_insert(node->left, arr, i);
    array_insert(node->right, arr, i);
}

void node_display(tree* node)
{
    printf("||word: %10s || number of ocurrances: %d||\n", node->data, node->wordCount);
}

bool isSpace(char c)
{
    if(c == ' ' || c == '\n' || c == '\t')
        return true;
    else
        return false;
}

void textDisplay(char* line)
{
    int i = 0;
    printf("||");
    for(i = 0; i < (strlen(line) + 2); i++)
        printf("-");
    printf("||\n");
    printf("|| %s ||\n||", line);
    for(i = 0; i < (strlen(line) + 2); i++)
        printf("-");
    printf("||\n");
}

void tree_traversal(struct tree* l) // display full tree
{
    if(l != NULL)
    {
        if (l->left != NULL)
            tree_traversal(l->left) ;
        /* here occures data operation */
        node_display(l);
        if (l->right != NULL)
            tree_traversal(l->right) ;
    }
}

// not used in program but standard tree functions for library
struct tree * tree_search(struct tree *l, char * x)
{   
    if(strcmp(x, l->data) == 0)
        return l ;
    if(l == NULL)
        return NULL ;
    if(strcmp(x, l->data) < 0)
        tree_search(l->left, x) ;
    else if(strcmp(x, l->data) > 0)
        tree_search(l->right, x) ;
}


struct tree * tree_min(struct tree * l)
{
    if(l == NULL)
        return NULL ;
    struct tree * min = l ;
    while(min->left != NULL)
        min = min->left ;
    return min ;
}

struct tree * tree_max(struct tree * l)
{
    if(l == NULL)
        return NULL ;
    struct tree * max = l ;
    while(max->right != NULL)
        max = max->right ;
    return max ;
}