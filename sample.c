#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#define N 6


int queue[N]={0};
int rear=0,front=0;

void insert(void);
void del(void);
void disp(void);
void cre(void);

void main()
{
    int input = 0;
    while (input != 4)
    {
	system("cls");
        printf("\n\n\n\t\t\t THE SIZE OF QUEUE IS %d",N);
        printf("\n\t 1.INSERT");
        printf("\n\t 2.DELETE");
        printf("\n\t 3.DISPLAY");
        printf("\n\t 4.EXIT");
        printf("\n\t 5.CREATE\n");
   
		printf("\n\t -> ");
        scanf("%d",&input);

        switch(input)
        {
	    system("cls");
            case 1:
                insert();
                break;
            case 2:
                del();
                break;
            case 3:
                disp();
                break;
            case 4:
                printf("\n\t THANK U\n\n");
                break;
            case 5:
                cre();
                break;
	    default:
		break;
        }

        getch();
	
      }

}

/********************* insert ********************/
void insert(void)
{
    int value=0;

    if(rear < N)
    {

        printf("\n\t ENTER A VALUE IN QUEUE :: ");
        scanf("%d",&value);
        queue[rear]=value;
        rear++;
		printf("\t PRESS ENTER KEY");
    }

    else
    {

        printf("\n\t Q OVERFLOW!!!!!!!!!!!!!!!");

    }
}

/********************* delete ********************/
void del(void)
{
    printf("\n\t %d gets deleted.........\n",queue[front]);
    queue[front]=0;
    	front++;
	printf("\t PRESS ENTER KEY");
}

/********************* display ********************/
void disp(void)
{
	int i=0;
	printf("\n\t");
    for(i = front ; i < rear ; i++)
        printf(" %d ", queue[i]);	
	printf("\n");
	printf("\t PRESS ENTER KEY");

}

/********************* create ********************/
void cre(void)
{
    int t=0;
    printf("\n\t ENTER A VALUE IN QUEUE :: ");
    scanf("%d",&t);
    front=0;
    queue[front]=t;
    rear=front+1;
	printf("\t PRESS ENTER KEY");
}
