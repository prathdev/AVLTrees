
/*Program to perform insertion and deletion operations on an avl tree*/
/* Reference : Data structures and algorithms by GAV Pai */
/* Assignment given was AVL Deletion, AVL insertion and verification have also been coded.*/
#include<stdio.h>
#include<stdlib.h>


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
        printf("No duplicate entries\n");
        size--;
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
                next->bf=cur->bf=0;			//update balance factors
#ifdef DEBUG
                printf("\n   LL case");
#endif
                return root;
            }

            if (flag==2)            		//rotate left for rr case
            {

                leftRotate(root,next);
                if (next==root)
                    root=cur;
                next->bf=cur->bf=0;			//update balance factors
#ifdef DEBUG
                printf("\n   RR case");
#endif
                return root;
            }

            if (flag==3)					//The lr case which requires left rotation at child
            {								//and right rotaton at node
                leftRotate(root,cur);
                rightRotate(root,next);
                if (root==next)
                    root=prev;
                if (prev->bf==1)			//updation of balance factors
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
#ifdef DEBUG
                printf("\n   LR case");
#endif
                return root;
            }

            if (flag==4)						//The lr case which requires right rotation at child
            {									//and left rotaton at node
                rightRotate(root,cur);
                leftRotate(root,next);
                if (root==next)
                    root=prev;

                if (prev->bf==1)
                {
                    next->bf=0;					//updation of balance factors
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
#ifdef DEBUG
                printf("\n   RL case");
#endif
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
        printf("Key not found\n");
        del=1;
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
        if (temp==root) 			//deleting root
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

/*Function to restore height balance after deletion of a node*/
/*The function traverses the tree from the parent of the deleted node upto the root checking for violations*/
/*It returns on reaching a point in the tree where its height is not affected.(Balance factor changed to 1 or -1)*/
/*On encountering a critical node(Balance factor of 2 or -2), the required rotation is performed*/
struct node * delBalance( struct node *root,struct node *temp)
{
    struct node *cur,*next,*parent,*child,*grandchild;
    cur=next=temp;
    int flag=0,flag2=0;
    while (1)
    {

        /*Return if the height of the tree is not affected*/
        if (next->bf==1||next->bf==-1)
        {
            return root;
        }

        /*On encountering a critical node, perform required rotation*/
        if (next->bf==2||next->bf==-2)
        {
            flag=decideDelRotate(next);

            switch (flag)
            {
            case 1:
                child=next->llink;				//R0 case
                rightRotate(root,next);
                if (next==root)
                    root=child;

                if (child->bf==0)
                {
                    next->bf=1;
                    child->bf=-1;
                    flag2=1; 				//updation for R0
#ifdef DEBUG
                    printf("\n   R0 DELETION\n");
#endif
                }
                else
                {
                    next->bf=child->bf=0;  //updation for R1
#ifdef DEBUG
                    printf("\n   R1 DELETION\n");
#endif
                }
                next=cur=child;
                if (child->plink==NULL) return child;
                break;


            case 2:
                child=next->llink;				//R-1 case
                grandchild=child->rlink;
                leftRotate(root,child);
                rightRotate(root,next);
                if (root==next)
                    root=grandchild;
                next->bf=child->bf=0;			//updation
                next=cur=grandchild;
#ifdef DEBUG
                printf("\n   R-1 DELETION\n");
#endif
                if (child->plink==NULL) return child;
                break;

            case 3:
                child=next->rlink;				//L0 case
                leftRotate(root,next);
                if (root==next)
                    root=child;
                if (child->bf==0)
                {
                    next->bf=-1;
                    child->bf=1;
                    flag2=1;				//updation for l0
#ifdef DEBUG
                    printf("\n   L0 DELETION\n");
#endif
                }
                else
                {
                    next->bf=child->bf=0;  //updation for l-1
#ifdef DEBUG
                    printf("\n   L-1 DELETION\n");
#endif
                }
                next=cur=child;
                if (child->plink==NULL) return child;
                break;


            case 4:
                child=next->rlink;			//L1 case
                grandchild=child->llink;
                rightRotate(root,child);
                leftRotate(root,next);
                if (root==next)
                    root=grandchild;

                next->bf=child->bf=0;		//updation of balance factors
                next=cur=grandchild;
#ifdef DEBUG
                printf("\n   L1 DELETION\n");
#endif
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

            if (cur->info<next->info)		//Traversing to the node's parent uptil the root
                next->bf--;					// and updating the balance factors
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
            return 1;	//r1
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

    if (child==parent->llink)
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
int max(int x,int y)
{
    if (x>y)
        return x;
    else
        return y;
}
int verify(struct node *root)            //To verify height balance of AVL tree.It returns 0 if violation..Else returns the height..
{
    int x,y;							//Check in main as a condition if(v==0) Error in tree!!
    if (root==NULL)
        return 0;
    x=verify(root->llink);
    if (x==-1)
        return -1;
    y=verify(root->rlink);
    if ((x-y)==2 || (x-y)==-2)
        return -1;

    return(1+max(x,y));
}

int bstverify(struct node *root)   //to verify bst property of AVL tree. It returns 0 if there's violation
{
    if (root==NULL) return;
    bstverify(root->llink);
    if (root->llink!=NULL)
        if (root->info < root->llink->info ) return 0;
    if (root->rlink!=NULL)
        if (root->info > root->rlink->info ) return 0;
    bstverify(root->rlink);
}
void preorder(struct node *p)
{

    if (p==NULL)
        return;
    printf("%d ",p->info);
    preorder(p->llink);
    preorder(p->rlink);

}

void preorderDotDump ( struct node  *root, FILE* outputFile)
{
    if (root != NULL)
    {
        fprintf (outputFile, "%d [label=\"%d,%d\"color=black];\n",root->info,root->info,root->bf);
        if (root->llink != NULL)
            fprintf (outputFile, "%d -> %d[color=blue] ;\n", root->info, (root->llink)->info);//blue colour indicates left link of node

        if (root->rlink != NULL)
            fprintf (outputFile, "%d -> %d[color=green];\n", root->info, root->rlink->info);//green colour indicates right link of node
        if (root->plink != NULL)
            fprintf (outputFile, "%d -> %d [style = dashed];\n", root->info, (root->plink)->info);
        preorderDotDump (root->rlink, outputFile);
        preorderDotDump (root->llink, outputFile);
    }
}

void dotDump(struct node *root, FILE *outFile)
{
    gName++;
    fprintf (outFile, "digraph AVL {\n",gName);

    preorderDotDump (root, outFile);
    fprintf (outFile, "}\n");
}
FILE *outputFile;


int main(int argc,char **argv)
{
    struct node *root=NULL,*new;
    int ch,i,x,v,s,w;
    FILE *pipe;

    if (argc!=2)
    {
        printf("\nNeeds one argument, Usage: ./a.out 'size'\n");
        exit(0);
    }

    outputFile=fopen("avl.dot","w");
    fclose(outputFile);
    size=atoi(argv[1]);

    for (i=0;i<size;i++)
    {
        x=random()%69883;
        printf("\n   element inserted:%d",x);
        new=getnode(x);
        root=insert(root,root,new);

        root=balance(root,new);
        w=bstverify(root);       //called after every insertion...
        if (w=0)
            printf("Error in tree!!!Violation of BST property\n");
        v=verify(root);          //called after every insertion...
        if (v==0)
            printf("Error in tree!!!Violation in balance factors\n");

        outputFile = fopen ("avl.dot", "a");
        if (outputFile != NULL)
        {

            dotDump (root,  outputFile);
        }
        fclose (outputFile);
    }




    while (1)
    {
        printf("\n---------------------------------------------------------------------------------");
        printf("\nEnter   1.preorder display   2.display dotty   3.delete  4.insert   5.exit ");
        printf("\n---------------------------------------------------------------------------------\n");
        scanf("%d",&ch);

        switch (ch)
        {

        case 1:
            printf("\nPreorder traversal of AVL tree:\n");
            preorder(root);
            printf("\n---------------------------------------------------------------------------------\n");
            break;

        case 2:
            pipe=popen("dot -Tps avl.dot -o avl.ps","w");
            pclose(pipe);
            pipe=popen("evince avl.ps","r");
            pclose(pipe);
            printf("\n---------------------------------------------------------------------------------\n");
            break;

        case 3:
            printf("\n   Enter the element to delete:");
            scanf("%d",&x);
            root=delete(root,x);
            w=bstverify(root);
            if (w=0)
                printf("Error in tree!!!Violation of BST property\n");
            v=verify(root);
            if (v==0)
                printf("Error in tree!!!Violation in balance factors\n");

            if (del!=1)
            {
                outputFile = fopen ("avl.dot", "a");
                dotDump (root,  outputFile);
                fclose (outputFile);


            }
            printf("\n---------------------------------------------------------------------------------\n");
            break;
        case 4:
            printf("\n   Enter the element to insert:");
            scanf("%d",&s);
            new=getnode(s);
            root=insert(root,root,new);
            root=balance(root,new);
            w=bstverify(root);
            if (w=0)
                printf("Error in tree!!!Violation of BST property\n");
            v=verify(root);
            if (v==0)
                printf("Error in tree!!!Violation in balance factors\n");



            outputFile = fopen ("avl.dot", "a");
            if (outputFile != NULL)
            {

                dotDump (root,  outputFile);
                fclose (outputFile);
            }
            printf("\n---------------------------------------------------------------------------------\n");
            break;

        case 5:
            return;


        default:
            exit(0);
        }
    }
}






















