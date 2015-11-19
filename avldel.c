
/*Program to perform insertion and deletion operations on an avl tree*/

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
    int bf;
};

void leftRotate(struct node *, struct node *);
void rightRotate(struct node *, struct node *);
struct node * delBalance(struct node *,struct node *);
struct node * update(struct node *,struct node *);

static int gName=0;
int size=0;
int del=0;
int del_rcount=0;

struct node * getnode(int x)
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp->info=x;
    temp->bf=0;
    temp->llink=NULL;
    temp->rlink=NULL;
    temp->plink=NULL;
    return temp;
}

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
        //printf("No duplicate entries\n");
        //size--;
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
            next->bf++;
        else
            next->bf--;
        if (next->bf==0)
            break;
        /*checking for critical node*/
        if (next->bf==2 || next->bf==-2)
        {
            flag=decideRotate(cur->bf,next->bf);

            if (flag==1)            		//rotate right for ll case
            {
                rightRotate(root,next);
                if (root==next)
                    root=cur;
                next->bf=cur->bf=0;		//update balance factors

                return root;
            }

            if (flag==2)            		//rotate left for rr case
            {

                leftRotate(root,next);
                if (next==root)
                    root=cur;
                next->bf=cur->bf=0;		//update balance factors
                return root;
            }

            if (flag==3)					//The lr case which requires left rotation at child
            {							//and right rotaton at node
                leftRotate(root,cur);
                rightRotate(root,next);
                if (root==next)
                    root=prev;
                if (prev->bf==1)
                {
                    next->bf=-1;
                    cur->bf= 0;
                    prev->bf=0;
                }
                else if (prev->bf==-1)
                {
                    next->bf=0;
                    cur->bf=1;
                    prev->bf=0;
                }
                else if (prev->bf==0)
                {
                    next->bf=prev->bf=cur->bf=0;
                }
                return root;
            }

            if (flag==4)						//The lr case which requires right rotation at child
            {								//and left rotaton at node
                rightRotate(root,cur);
                leftRotate(root,next);
                if (root==next)
                    root=prev;

                if (prev->bf==1)
                {
                    next->bf=0;
                    cur->bf=-1;
                    prev->bf=0;
                }
                else if (prev->bf==-1)
                {
                    next->bf=1;
                    cur->bf=0;
                    prev->bf=0;
                }
                else if (prev->bf==0)
                {
                    next->bf=prev->bf=cur->bf=0;
                }
                return root;
            }
        }

    }
    return root;
}

/* Function to delete a node based in the key*/
/*The tree is traversed to find the node, the balance factor of its parent is updated*/
/*The node is deleted and its parent is passed to the delBalance function to restore height balance*/

struct node *delete(struct node *root,int key)
{
    if (root==NULL)
        return root;
    struct node *temp=root,*parent;
    while (temp!=NULL)
    {
        if (temp->info==key)
            break;
        if (temp->info>key)
            temp=temp->llink;
        else
            temp=temp->rlink;
    }
    if (temp==NULL)
    {
        // printf("node not found\n");
        //del=1;
        return root;
    }

    /*If the node has two children*/
    /*The inorder successor of the node is found and the node's data is replaced with it*/
    /*The inorder successor is now deleted*/
    /*The case falls throught into the case of single child or leaf*/

    if (temp->rlink!=NULL && temp->llink!=NULL)
    {
        struct node *t=temp->rlink;
        while (t->llink!=NULL)
            t=t->llink;
        temp->info=t->info;
        temp=t;
    }
    /*To delete a leaf node,the left or right pointer of its parent is assigned null*/
    if (temp->llink==NULL && temp->rlink==NULL)
    {
        if (temp==root) 		//deleting root
        {
            root=NULL;
            return NULL;
        }

        temp->plink=update(temp,temp->plink);
        if (temp->plink->llink==temp) temp->plink->llink=NULL;
        else temp->plink->rlink=NULL;
        parent=temp->plink;
        free(temp);
        root=delBalance(root,parent);
        return root;
    }

    /*To delete a node with a single child*/
    /*If the node is a left child, its parent's left link is assigned to the node's child.*/
    /*If not the right link is assigned to the node's child*/
    if (temp->rlink==NULL)
    {
        if (temp==root)
        {
            root->llink->plink=NULL;
            root=root->llink;
            temp->plink=update(temp,temp->plink);
            parent=temp->plink;
            free(temp);
            root=delBalance(root,parent);
            return root;
        }
        if (temp==temp->plink->llink)
        {
            temp->plink->llink=temp->llink;
            temp->llink->plink=temp->plink;
        }
        else
        {
            temp->plink->rlink=temp->llink;
            temp->llink->plink=temp->plink;
        }
        temp->plink=update(temp,temp->plink);
        parent=temp->plink;
        free(temp);
        root=delBalance(root,parent);
        return root;

    }
    if (temp->llink==NULL)
    {
        if (root==temp)
        {
            root->rlink->plink=NULL;
            root=root->rlink;
            temp->plink=update(temp,temp->plink);
            parent=temp->plink;
            free(temp);
            root=delBalance(root,parent);
            return root;
        }
        if (temp==temp->plink->llink)
        {
            temp->plink->llink=temp->rlink;
            temp->rlink->plink=temp->plink;
        }
        else
        {
            temp->plink->rlink=temp->rlink;
            temp->rlink->plink=temp->plink;
        }
        temp->plink=update(temp,temp->plink);
        parent=temp->plink;
        free(temp);
        root=delBalance(root,parent);
        return root;
    }


}
int leaf=0;
struct node * delBalance( struct node *root,struct node *temp)
{
    struct node *cur,*next,*parent,*child,*grandchild;
    cur=next=temp;
    int flag=0,flag2=0;
    while (1)
    {
        if (next->llink==NULL && next->rlink==NULL && next->bf!=0)
            leaf=1;
        /*Return if the height of the tree is not affected*/
        if (next->bf==1||next->bf==-1)
        {
            return root;
        }

        /*On encountering a critical node, perform required rotation*/
        if (next->bf==2||next->bf==-2)
        {
            flag=decideDelRotate(next);
            del_rcount++;
            switch (flag)
            {
            case 1:
                child=next->llink;
                rightRotate(root,next);
                if (next==root)
                    root=child;
                if (child->bf==0)
                {
                    next->bf=1;
                    child->bf=-1;
                    flag2=1; //updation for r 0
                }
                else
                {
                    next->bf=child->bf=0;  //updation for r1
                }
                next=cur=child;
                if (child->plink==NULL) return child;
                break;


            case 2:
                child=next->llink;
                grandchild=child->rlink;
                leftRotate(root,child);
                rightRotate(root,next);
                if (root==next)
                    root=grandchild;
                if (grandchild->bf==0)
                    next->bf=child->bf=grandchild->bf=0;
                else if (grandchild->bf==-1)
                {
                    next->bf=0;
                    child->bf=1;
                    grandchild->bf=0;
                }
                else
                {
                    next->bf=-1;
                    child->bf=0;
                    grandchild->bf=0;
                }
                next=cur=grandchild;
                if (child->plink==NULL) return child;
                break;

            case 3:
                child=next->rlink;
                leftRotate(root,next);
                if (root==next)
                    root=child;
                if (child->bf==0)
                {
                    next->bf=-1;
                    child->bf=1;
                    flag2=1;//updation for l0
                }
                else
                {
                    next->bf=child->bf=0;  //updation for l-1
                }
                next=cur=child;
                if (child->plink==NULL) return child;
                break;


            case 4:
                child=next->rlink;
                grandchild=child->llink;
                rightRotate(root,child);
                leftRotate(root,next);
                if (root==next)
                    root=grandchild;
                if (grandchild->bf==0)
                    next->bf=child->bf=grandchild->bf=0;
                else if (grandchild->bf==-1)
                {
                    next->bf=1;
                    child->bf=0;
                    grandchild->bf=0;
                }
                else
                {
                    next->bf=0;
                    child->bf=-1;
                    grandchild->bf=0;
                }
                next=cur=grandchild;
                if (child->plink==NULL) return child;
                break;


            }
        }

        cur=next;
        if (flag2==1)
            return root;
        if (next!=root)
        {
            next=next->plink;

            if (cur->info<next->info)
                next->bf--;
            else
            {
                next->bf++;
            }
        }
        if (next==root && (next->bf!=2 || next->bf!=-2))
            return root;

    }
    return root;
}

/*Function to decide the type of rotation required after deletion*/

int decideDelRotate(struct node *next)
{
    int factor;
    if (next->bf==2)
    {
        if (next->llink!=NULL)
            factor=next->llink->bf;
        switch (factor)
        {
        case 0:
            return 1;	 //r0
        case 1:
            return 1;//r1
        case -1:
            return 2; //r-1
        }
    }
    else
    {
        if (next->rlink!=NULL)
            factor=next->rlink->bf;
        switch (factor)
        {
        case 0:
            return 3; //l0
        case -1:
            return 3; //l-1
        case 1:
            return 4; //l1
        }
    }
}

/*Function to update the balance factors*/
struct node * update(struct node *child,struct node *parent)
{

    if (child->info < parent->info)
        parent->bf--;
    else
        parent->bf++;
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
    int ch,i,x;
    size=atoi(argv[1]);
    int arr[size];

    if (argc!=2)
    {
        printf("\nNeeds one argument, Usage: ./a.out 'size'\n");
        exit(0);
    }


    for (i=0;i<1000000;i++)
    {
        x=random()%1169883;
        if (i<size)
            arr[i]=x;

        new=getnode(x);
        root=insert(root,root,new);

        root=balance(root,new);
    }

    printf("\nDeleting %d elements from AVL tree\n",size);
    double time;
    timeStart();

    for (i=0;i<size;i++)
    {
        root=delete(root,arr[i]);
    }
    time=timeStop("Deletion finished\n");

    FILE *del_File = fopen("delrot.txt", "a");
    fprintf(del_File, "%d  %f  %d\n", size,time, del_rcount);
    fclose(del_File);
}

















