/*Program to perform insertion operations on an avl tree*/

#include<stdio.h>
#include<stdlib.h>
#include "timer_nrp.h"

/*Each node contains the information, balance factor, pointers to the left and right children and the parent*/
struct node
{
    struct node *llink;
    struct node *rlink;
    struct node *plink;
    int info;
    int bi;
};

void leftRotate(struct node *, struct node *);
void rightRotate(struct node *, struct node *);
struct node * update(struct node *,struct node *);

struct node * getnode(int x)
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp->info=x;
    temp->bi=0;
    temp->llink=NULL;
    temp->rlink=NULL;
    temp->plink=NULL;
    return temp;
}

int s=0;
int ins_rcount=0;
/*The insert function inserts the new node at the correct position and assigns the parent pointer to its parent*/

struct node * insert(struct node *plink,struct node *root,struct node *new)
{
    struct node *temp;
    if (root==NULL)
    {
        temp=plink;
        root=new;
        new->plink=temp;
        return root;
    }

    if (new->info == root->info)
    {
        s--;
        return root;
    }

    if (new->info > root->info)
    {

        root->rlink=insert(root,root->rlink,new);
    }

    if (new->info < root->info)
    {
        root->llink=insert(root,root->llink,new);
    }
    return root;
}


/*The balance function restores height balance to the tree after every insertion and updates the balance factors*/

struct node * balance(struct node *root,struct node *new)
{
    struct node *prev,*next,*cur,*temp,*parent;
    int flag;
    prev=cur=next=new;

    /*The tree is traversed from the new node upto the root*/
    /*Balance factors are incremented or decremented based on whether the new node is a left or right child*/
    /*On encountering a critical node the required rotations are performed to restore balance*/

    while (next->plink!=NULL)
    {
        prev=cur;
        cur=next;
        next=next->plink;
        if (cur==next->llink)
            next->bi++;
        else
            next->bi--;
        if (next->bi==0)
            break;
        /*checking for critical node*/
        if (next->bi==2 || next->bi==-2)
        {
            flag=decideRotate(cur->bi,next->bi);
            ins_rcount++;
            if (flag==1)            		//rotate right for ll case
            {
                rightRotate(root,next);
                if (root==next)
                    root=cur;
                next->bi=cur->bi=0;		//update balance factors

                return root;
            }

            if (flag==2)            		//rotate left for rr case
            {

                leftRotate(root,next);
                if (next==root)
                    root=cur;
                next->bi=cur->bi=0;		//update balance factors
                return root;
            }

            if (flag==3)					//The lr case which requires left rotation at child
            {							//and right rotaton at node
                leftRotate(root,cur);
                rightRotate(root,next);
                if (root==next)
                    root=prev;
                if (prev->bi==1)
                {
                    next->bi=-1;
                    cur->bi= 0;
                    prev->bi=0;
                }
                else if (prev->bi==-1)
                {
                    next->bi=0;
                    cur->bi=1;
                    prev->bi=0;
                }
                else if (prev->bi==0)
                {
                    next->bi=prev->bi=cur->bi=0;
                }
                return root;
            }

            if (flag==4)						//The lr case which requires right rotation at child
            {								//and left rotaton at node
                rightRotate(root,cur);
                leftRotate(root,next);
                if (root==next)
                    root=prev;

                if (prev->bi==1)
                {
                    next->bi=0;
                    cur->bi=-1;
                    prev->bi=0;
                }
                else if (prev->bi==-1)
                {
                    next->bi=1;
                    cur->bi=0;
                    prev->bi=0;
                }
                else if (prev->bi==0)
                {
                    next->bi=prev->bi=cur->bi=0;
                }
                return root;
            }
        }

    }
    return root;
}

int decideRotate(int cbf,int nbf)
{

    if (nbf==2 && cbf==1)
        return 1; 		//checking for ll case and return 1 for ll case
    if (nbf==-2 && cbf==-1)
        return 2; 		//checking for rr case and return 2 for rr case
    if (nbf==2 && cbf==-1)
        return 3; 		//checking for lr case and return 3 for lr case
    if (nbf==-2 && cbf==1)
        return 4; 		//checking for rl case and return 4 for rl case
}

/*Function to perform the left rotate operation*/

void leftRotate(struct node *root,struct node *z)
{
    struct node *x,*y;

    x=z;
    y=x->rlink;
    /*x is the node at which rotation is to be done, y its right child*/
    /*Assign the left child of y(if it exists) as the right child of x*/
    /*Assign x as the left child of y*/
    /*If x is root, make y the new root*/

    if (root==x)
    {
        y->plink=NULL;
        root=y;
    }
    else
    {
        y->plink=x->plink;
        if (x->plink->llink==x)
            x->plink->llink=y;
        else
            x->plink->rlink=y;
    }
    if (y->llink!=NULL)
    {
        x->rlink=y->llink;
        x->rlink->plink=x;
    }
    else
        x->rlink=NULL;
    y->llink=x;
    x->plink=y;

}

/*Function to perform the right rotate operation*/

void rightRotate(struct node *root, struct node *z)
{
    struct node *x,*y;

    x=z;
    y=x->llink;

    /*x is the node at which rotation is to be done, y its left child*/
    /*Assign the right child of y(if it exists) as the left child of x*/
    /*Assign x as the right child of y*/
    /*If x is root, make y the new root*/

    if (x==root)
    {
        y->plink=NULL;
        root=y;
    }
    else
    {
        y->plink=x->plink;
        if (x->plink->llink==x)
            x->plink->llink=y;
        else
            x->plink->rlink=y;
    }
    if (y->rlink!=NULL)
    {
        x->llink=y->rlink;
        x->llink->plink=x;
    }
    else
        x->llink=NULL;
    y->rlink=x;
    x->plink=y;


}

int main(int argc,char **argv)
{
    struct node *root=NULL,*new;
    int ch,i,x,size;

    if (argc!=2)
    {
        printf("\nNeeds one argument, Usage: ./a.out 'size'\n");
        exit(0);
    }

    size=atoi(argv[1]);
    printf("\nInserting %d elements into AVL tree\n",size);

    double insertTime;
    timeStart();

    for (i=0;i<size;i++)
    {
        s++;
        x=random()%1178923;
        new=getnode(x);
        root=insert(root,root,new);

        root=balance(root,new);
    }
    insertTime = timeStop("Insertion finished\n");
    FILE *insFile = fopen("insertData_dup.txt", "a");
    fprintf(insFile, "%d  %f\n", size, insertTime);
    fclose(insFile);
    FILE *ins_File = fopen("insertRot.txt", "a");
    fprintf(ins_File, "%d  %d\n", s, ins_rcount);
    fclose(ins_File);
}
