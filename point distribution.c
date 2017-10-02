#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define INF 10000
double max(double a,double b)
{	
	if(a>b)
	return a;
	else
	return b;
}
double min(double a,double b)
{	
	if(a<b)
	return a;
	else
	return b;
}
//--------FILE Functions
int countLines(char *s)
{
	int c=0;
	FILE *fp=fopen(s,"rw");
	char ch=getc(fp);
	
	while(ch!=EOF)
	{
		if(ch=='\n')
		c++;
		
		ch=getc(fp);
	}
	
	return c-2;
}
//--------QUADTREE Functions
int height=0;

typedef struct node
{
	struct node *nw;//pointer to North-West neighbour
	struct node *ne;//pointer to North-East neighbour
	struct node *sw;//pointer to South-West neighbour
	struct node *se;//pointer to South-East neighbour
	
	struct node *par;//pointer to the parent node
	
	double lx,ly;//Coordinates of lower left corner of the area
	double hx,hy;//Coordinates of upper right corner of the area
	//double x,y;//Coordinates of boundary/solid point
	
	int level;//Level of node in tree
	int point;
	
}Node;//Structure of node

Node * createNode(double l1,double l2,double h1,double h2,Node *parent)
{
	Node *temp = /*(Node*)*/malloc(sizeof(Node));
	int i,j;
	
	temp->nw=NULL;
	temp->ne=NULL;
	temp->sw=NULL;
	temp->se=NULL;
	temp->par=parent;
	
	temp->point=-999;
	
	temp->lx=l1;
	temp->ly=l2;
	temp->hx=h1;
	temp->hy=h2;
	
	if(parent==NULL)
	temp->level=0;
	else
	temp->level=parent->level+1;
	
	if(temp->level > height)
	height=temp->level;
	
	return temp;
}// Function to create new Node

Node * generateQuadTree(double l1,double l2,double h1,double h2,int n,double arr[][2],Node *parent)
{
	Node * temp=createNode(l1,l2,h1,h2,parent);
	
	int c=0,i;
	
	for(i=0;i<n;i++)
	if(arr[i][0]>=l1 && arr[i][0]<=h1 && arr[i][1]>=l2 && arr[i][1]<=h2)
	c++;
	
	if(c>1)
	{
		temp->nw=generateQuadTree(l1,(l2+h2)*0.5,(l1+h1)*0.5,h2,n,arr,temp);
		temp->ne=generateQuadTree((l1+h1)*0.5,(l2+h2)*0.5,h1,h2,n,arr,temp);
		temp->sw=generateQuadTree(l1,l2,(l1+h1)*0.5,(l2+h2)*0.5,n,arr,temp);
		temp->se=generateQuadTree((l1+h1)*0.5,l2,h1,(l2+h2)*0.5,n,arr,temp);
	}
	else
	return temp;
	
}// Function to generate Quadtree

int isLeafNode(Node *temp)
{
	if(temp->nw==NULL && temp->ne==NULL && temp->sw==NULL && temp->se==NULL)
	return 1;
	else
	return 0;
}

int getEastNeighbourLevel(Node *temp)
{
	int stack[height+1],top=-1;
	
	while( temp->par!=NULL && (temp==temp->par->ne || temp==temp->par->se))
	{
		if(temp->par->ne==temp)
		stack[++top]=2;
		else if(temp->par->nw==temp)
		stack[++top]=1;
		else if(temp->par->sw==temp)
		stack[++top]=4;
		else if(temp->par->se==temp)
		stack[++top]=3;
		
		temp=temp->par;
	}
	
	if(temp->par!=NULL)
	{
		if(temp->par->ne==temp)
		stack[++top]=2;
		else if(temp->par->nw==temp)
		stack[++top]=1;
		else if(temp->par->sw==temp)
		stack[++top]=4;
		else if(temp->par->se==temp)
		stack[++top]=3;
		
		temp=temp->par;
	}
	else if(temp->par==NULL)
	return -1;
	
	
	while((top>=0) &&(isLeafNode(temp)==0))
	{
		if(stack[top]==1)
		temp=temp->ne;
		else if(stack[top]==2)
		temp=temp->nw;
		else if(stack[top]==3)
		temp=temp->se;
		else if(stack[top]==4)
		temp=temp->sw;
		
		top--;
	}
	
	if(isLeafNode(temp)==0)
	{
		if(isLeafNode(temp->sw)==0)
		return temp->sw->sw->level;
		else if(isLeafNode(temp->nw)==0)
		return temp->nw->nw->level;
		else
		return temp->sw->level;
	}
	
	return temp->level;
}

int getWestNeighbourLevel(Node *temp)
{
	int stack[height+1],top=-1;
	
	while( temp->par!=NULL && (temp==temp->par->nw || temp==temp->par->sw))
	{
		if(temp->par->ne==temp)
		stack[++top]=2;
		else if(temp->par->nw==temp)
		stack[++top]=1;
		else if(temp->par->sw==temp)
		stack[++top]=4;
		else if(temp->par->se==temp)
		stack[++top]=3;
		
		temp=temp->par;
	}
	
	if(temp->par!=NULL)
	{
		if(temp->par->ne==temp)
		stack[++top]=2;
		else if(temp->par->nw==temp)
		stack[++top]=1;
		else if(temp->par->sw==temp)
		stack[++top]=4;
		else if(temp->par->se==temp)
		stack[++top]=3;
		
		temp=temp->par;
	}
	else if(temp->par==NULL)
	return -1;
	
	
	while((top>=0) &&(isLeafNode(temp)==0))
	{
		if(stack[top]==1)
		temp=temp->ne;
		else if(stack[top]==2)
		temp=temp->nw;
		else if(stack[top]==3)
		temp=temp->se;
		else if(stack[top]==4)
		temp=temp->sw;
		
		top--;
	}
	
	if(isLeafNode(temp)==0)
	{
		if(isLeafNode(temp->se)==0)
		return temp->se->se->level;
		else if(isLeafNode(temp->ne)==0)
		return temp->ne->ne->level;
		else
		return temp->se->level;
	}
	
	return temp->level;
}

int getNorthNeighbourLevel(Node *temp)
{
	int stack[height+1],top=-1;
	
	while( temp->par!=NULL && (temp==temp->par->ne || temp==temp->par->nw))
	{
		if(temp->par->ne==temp)
		stack[++top]=3;
		else if(temp->par->nw==temp)
		stack[++top]=4;
		else if(temp->par->sw==temp)
		stack[++top]=1;
		else if(temp->par->se==temp)
		stack[++top]=2;
		
		temp=temp->par;
	}
	
	if(temp->par!=NULL)
	{
		if(temp->par->ne==temp)
		stack[++top]=3;
		else if(temp->par->nw==temp)
		stack[++top]=4;
		else if(temp->par->sw==temp)
		stack[++top]=1;
		else if(temp->par->se==temp)
		stack[++top]=2;
		
		temp=temp->par;
	}
	else if(temp->par==NULL)
	return -1;
	
	
	while((top>=0) &&(isLeafNode(temp)==0))
	{
		if(stack[top]==1)
		temp=temp->ne;
		else if(stack[top]==2)
		temp=temp->nw;
		else if(stack[top]==3)
		temp=temp->se;
		else if(stack[top]==4)
		temp=temp->sw;
		
		top--;
	}
	
	if(isLeafNode(temp)==0)
	{
		if(isLeafNode(temp->se)==0)
		return temp->se->se->level;
		else if(isLeafNode(temp->sw)==0)
		return temp->sw->sw->level;
		else
		return temp->sw->level;
	}
	
	return temp->level;
}

int getSouthNeighbourLevel(Node *temp)
{
	int stack[height+1],top=-1;
	
	while( temp->par!=NULL && (temp==temp->par->se || temp==temp->par->sw))
	{
		if(temp->par->ne==temp)
		stack[++top]=3;
		else if(temp->par->nw==temp)
		stack[++top]=4;
		else if(temp->par->sw==temp)
		stack[++top]=1;
		else if(temp->par->se==temp)
		stack[++top]=2;
		
		temp=temp->par;
	}
	
	if(temp->par!=NULL)
	{
		if(temp->par->ne==temp)
		stack[++top]=3;
		else if(temp->par->nw==temp)
		stack[++top]=4;
		else if(temp->par->sw==temp)
		stack[++top]=1;
		else if(temp->par->se==temp)
		stack[++top]=2;
		
		temp=temp->par;
	}
	else if(temp->par==NULL)
	return -1;
	
	
	while((top>=0) &&(isLeafNode(temp)==0))
	{
		if(stack[top]==1)
		temp=temp->ne;
		else if(stack[top]==2)
		temp=temp->nw;
		else if(stack[top]==3)
		temp=temp->se;
		else if(stack[top]==4)
		temp=temp->sw;
		
		top--;
	}
	
	if(isLeafNode(temp)==0)
	{
		if(isLeafNode(temp->ne)==0)
		return temp->ne->ne->level;
		else if(isLeafNode(temp->nw)==0)
		return temp->nw->nw->level;
		else
		return temp->nw->level;
	}
	
	return temp->level;
}

int balanceQuadTree(Node * temp)
{
	if(isLeafNode(temp)==1)
	{
		if((height-(temp->level)) > 1)
		{
			int Ln,Le,Lw,Ls,l1,l2,h1,h2;
			int h=temp->level;
			Ln=getNorthNeighbourLevel(temp);
			Le=getEastNeighbourLevel(temp);
			Lw=getWestNeighbourLevel(temp);
			Ls=getSouthNeighbourLevel(temp);
			
			if((Ln-h>1) || (Le-h>1) || (Lw-h>1) || (Ls-h>1))
			{
				l1=temp->lx;
				l2=temp->ly;
				h1=temp->hx;
				h2=temp->hy;
				
				temp->nw=createNode(l1,(l2+h2)*0.5,(l1+h1)*0.5,h2,temp);
				temp->ne=createNode((l1+h1)*0.5,(l2+h2)*0.5,h1,h2,temp);
				temp->sw=createNode(l1,l2,(l1+h1)*0.5,(l2+h2)*0.5,temp);
				temp->se=createNode((l1+h1)*0.5,l2,h1,(l2+h2)*0.5,temp);
			}
			
		}
	}
	
	if(isLeafNode(temp)==0)
	{
		balanceQuadTree(temp->nw);
		balanceQuadTree(temp->ne);
		balanceQuadTree(temp->sw);
		balanceQuadTree(temp->se);
	}
	
	return 0;
}

int getPoints(Node *temp,double arr[][2],int n)
{
	if(isLeafNode(temp)==1)
	{
		int i,flag=0;
		
		for(i=0;i<n;i++)
		{
			if(arr[i][0]>=temp->lx && arr[i][1]>=temp->ly && arr[i][0]<=temp->hx && arr[i][1]<=temp->hy)
			{
				temp->point=i;
				flag=1;
				break;
			}
		}
		
		if(flag!=1)
		temp->point=-1;
		
	}
	else
	{
		getPoints(temp->nw,arr,n);
		getPoints(temp->ne,arr,n);
		getPoints(temp->sw,arr,n);
		getPoints(temp->se,arr,n);
	}
	
	return 0;
	
}
int onSegment(double px,double py,double qx,double qy,double rx,double ry)
{
	if(qx <= max(px,rx) && qx>=min(px,rx) && qy <= max(py,ry) && qy>=min(py,ry))
	return 1;
	else
	return 0;
}

int orientation(double px,double py,double qx,double qy,double rx,double ry)
{
	double val=(qy-py)*(rx-qx)-(qx-px)*(ry-qy);
	
	if(val==0)
	return 0;
	else if(val>0)
	return 1;
	else return 2;
}

int doIntersect(double p1x,double p1y,double q1x,double q1y,double p2x,double p2y,double q2x,double q2y)
{
	int o1=orientation(p1x,p1y,q1x,q1y,p2x,p2y);
	int o2=orientation(p1x,p1y,q1x,q1y,q2x,q2y);
	int o3=orientation(p2x,p2y,q2x,q2y,p1x,p1y);
	int o4=orientation(p2x,p2y,q2x,q2y,q1x,q1y);
	
	if(o1!=o2 && o3!=o4)
	return 1;
	
	if(o1==0 && onSegment(p1x,p1y,p2x,p2y,q1x,q1y)==1)
	return 1;
	if(o2==0 && onSegment(p1x,p1y,q2x,q2y,q1x,q1y)==1)
	return 1;
	if(o3==0 && onSegment(p2x,p2y,p1x,p1y,q2x,q2y)==1)
	return 1;
	if(o4==0 && onSegment(p2x,p2y,q1x,q1y,q2x,q2y)==1)
	return 1;
	
	return 0;
}

int isInside(double arr[][2],int n,double px,double py)
{
	if(n<3)
	return 0;
	
	double exX=INF,exY=py;
	
	int count=0,i=0;
	
	do
	{
		int next=(i+1)%n;
		//printf("%lf %lf - %lf %lf\n",arr[i][0],arr[i][1],arr[next][0],arr[next][1]);
		
		if(doIntersect(arr[i][0],arr[i][1],arr[next][0],arr[next][1],px,py,exX,exY)==1)
		{
			//printf("%d %d\n",i,next);
			if(orientation(arr[i][0],arr[i][1],px,py,arr[next][0],arr[next][1])==0)
			return onSegment(arr[i][0],arr[i][1],px,py,arr[next][0],arr[next][1]);
			
			count++;
		}
		
		i=next;
		
	}while (i!=0);
	
	//printf("%lf %lf count=%d\n",px,py,count);
	
	if(count%2==1)
	return 1;
	else
	return 0;
}

void removeInteriorPoints(Node *temp,double arr[][2],int n)
{
	if(isLeafNode(temp)==1)
	{
		if(temp->point==-1)
		{
			//printf("success\n");
			if(isInside(arr,n,(temp->lx+temp->hx)*0.5,(temp->ly+temp->hy)*0.5)==1)
			{
				temp->point=-2;
				//printf("success\n");
			}
		}
	}
	else
	{
		removeInteriorPoints(temp->nw,arr,n);
		removeInteriorPoints(temp->ne,arr,n);
		removeInteriorPoints(temp->sw,arr,n);
		removeInteriorPoints(temp->se,arr,n);
	}
	return;
}

int printPoints(Node* temp,double arr[][2],int n)
{
	if(isLeafNode(temp)==1)
	{
		if(temp->point>=0)
		printf("%lf %lf\n",arr[temp->point][0],arr[temp->point][1]);
		else if(temp->point==-1)
		printf("%lf %lf\n",(temp->lx+temp->hx)*0.5,(temp->ly+temp->hy)*0.5);
	}
	else
	{
		printPoints(temp->nw,arr,n);
		printPoints(temp->ne,arr,n);
		printPoints(temp->sw,arr,n);
		printPoints(temp->se,arr,n);
	}
	
	return 0;
}

int printQuadTree(Node * temp)
{
	int i,j;
	
	printf("%d (%lf,%lf) - (%lf,%lf)\n",temp->level,temp->lx,temp->ly,temp->hx,temp->hy);
	
	if (temp->nw!=NULL)
	printQuadTree(temp->nw);
	if (temp->ne!=NULL)
	printQuadTree(temp->ne);
	if (temp->sw!=NULL)
	printQuadTree(temp->sw);
	if (temp->se!=NULL)
	printQuadTree(temp->se);
	
	return 0;
}//Function to print Quadtree

int main()
{
	double lx,ly,hx,hy;
	int n,i,dummy;
	char fname[100];
	
	printf("Enter File Name: ");
	scanf("%s",&fname);

	FILE *fp;
	fp=fopen(fname,"r");
	
	if(fp==NULL)
	{
		printf("Error in opening File %s!\nPress any key to exit",fname);
		getc(stdin);
		return 0;
	}
	//printf("ENTER THE COORDINATES OF THE LOWER LEFT CORNER: ");
	fscanf(fp,"%lf %lf",&lx,&ly);
	
	//printf("ENTER THE COORDINATES OF THE UPPER RIGHT CORNER: ");
	fscanf(fp,"%lf %lf",&hx,&hy);
	
	printf("Number of points: ");
	n=countLines(fname);
	printf("%d\n",n);
	
	double arr[n][2];
	
	//printf("Enter coordinates of points:\n");
	for(i=0;i<n;i++)
	{
		//printf("%d. ",i+1);
		fscanf(fp,"%lf %lf",&arr[i][0],&arr[i][1]);
	}
	
	Node *root;
	
	root=generateQuadTree(lx,ly,hx,hy,n,arr,NULL);
	//getPoints(root,arr,n);
	//printPoints(root,arr,n);
	//printQuadTree(root);
	//printf("************************\n");
	//balanceQuadTree(root);
	//printQuadTree(root);
	getPoints(root,arr,n);
	removeInteriorPoints(root,arr,n);
	printPoints(root,arr,n);
	
	printf("Height=%d\n",height);
	
	return 0;
}
