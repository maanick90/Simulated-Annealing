
#define WIN
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdint.h>
#ifdef WIN
#include<ctime>
#endif

int n;
float T;
char fileName[50], netFileName[50], areaFileName[50], outputFileName[50];
int nodeArraySize = 0, weightArraySize = 0;
int *nodeArray = (int*)malloc(1000000*sizeof(int));
int *weightArray = (int*)malloc(1000000*sizeof(int));
int aSize = 0, bSize = 0;
int *a = (int*)malloc(1000000*sizeof(int));
int *b = (int*)malloc(1000000*sizeof(int));	

/*
	Structure to define linked list which contains the value and pointer to the next node
*/
struct linkedlist
{
       int val;
       struct linkedlist *next;
}*nxt;
typedef struct linkedlist node;

/*
	Declaring all the methods here
*/
void printLinkedList();                                 
void printPartitionArray(int *a, int size);
int getSpaceIndex(char ch[100]);
int getNumberForTheIndex(char ch[100], int start, int end);
void setArea(char ch[100]);
void readArea();
void sortArray();
void read_into_A_and_B_Arrays();
int arrayContains(int val, int *a, int size);
int get_Individual_PSI_Value(node *nxt, int nodeValue, int *a, int *b);
int get_Initial_PSI_Value_Before_Picking_Random(node *nxt, int n, int *a, int *b);
int *swiftArray(int *a, int size, int randValue);
void swapArray(int randValue);
void swapArray1(int randValue);
void get_PSI_value(node *nxt, int n);
void addSubNodes(int subNodesValue, int aIndex);
void setNodeHeadValue();
void swiftArray2(int *c, int size, int randValue);
void setFileNameValues();

/*
	Main method which get called first
*/
int main()
{
	int totalNoOfFiles = 10, fileChoice;
	char fileNamesArray[10][100] = {"ibm01", "ibm03", "ibm04", "ibm06", 			
					"ibm08", "ibm10", "ibm12", "ibm14", 
					"ibm16", "ibm18"};
	int fileNoOfNodes[10] = {12506, 22853, 27220, 32332, 51023, 68685, 70439, 147088, 182980, 210341};
	printf("\nFiles present....");	
	for(int i=0;i<totalNoOfFiles;i++)
		printf("\n%d. %s", i+1, fileNamesArray[i]);
	printf("\n\nEnter your choice...");
	scanf("%d", &fileChoice);
	if(fileChoice < 1 || fileChoice > 10)
	{
		printf("\n\n\nEnter a proper choice.....\nProgram exiting....");
		return 1;
	}

	n = fileNoOfNodes[fileChoice-1];				// finding the n values
	nxt = (node*)malloc(n * sizeof(node));			// allocating the memory to LinkedLists

	strcpy(fileName,fileNamesArray[fileChoice-1]);
	printf("\n\n\n-----------------%s------------------", fileName);
//	printf("\n\n-----------No of Nodes = %d ---------",n);

	printf("\n\nEnter the 'T' value....");
	scanf("%f", &T);
	
	setFileNameValues();			// setting the .net string, .are string and output string

    FILE *fp = fopen(netFileName, "r");                                
    int aIndex=-1, flagP = 0, subNodesValue;
    char ch[100];
    setNodeHeadValue();
    
    if(fp == NULL)                                                                // error in opening the file
    {
          perror("\nError in opening the file...");
          getch();
          exit(EXIT_FAILURE);
    }
    while(fgets(ch,1000,fp)!=NULL)                                                // get 1 line at a time
    {
    	      if(strchr(ch,'s') != NULL)                                          // find the occurence of 's' in the line
    	            if(ch[0] != 'p')									          // for values a0,a1,a2.... 
              		{
						  aIndex = getNumberForTheIndex(ch,1,getSpaceIndex(ch));
						  flagP = 0;
                	}
                    else											              // for values p0,p1...
                    	flagP = 1;									              // setting the flapP = 1 to ignore the following lines 
              else  if(ch[0] == 'a' && flagP == 0)                                // find the occurence of '|', then call the function
              {     
                    subNodesValue = getNumberForTheIndex(ch,1,getSpaceIndex(ch));
                    addSubNodes(subNodesValue, aIndex);
                    addSubNodes(aIndex, subNodesValue);
              }
    }
    fclose(fp);
    printf("\n\nLinked list successfully created...");

//    printLinkedList();                       // print the linked list

	readArea();								// read the area from an another file
	get_PSI_value(nxt,n);					// actual function
	
	printf("\n\nPress any key to continue...");
	getch();
    return 1 ;
}

/*
	Method to create the linked list
*/
void addSubNodes(int subNodesValue, int aIndex)   // creating the linked list
{
     node *temp = (node*)malloc(sizeof(node));
     temp->val = subNodesValue;                                                  // set the next node value as j
     temp->next = NULL;                                            
     if(nxt[aIndex].next == NULL)                                                // gets executed only for the 1st next node
           nxt[aIndex].next = temp;
     else
     {
            node *nxt1 = nxt[aIndex].next;
            while(nxt1->next != NULL)
                             nxt1 = nxt1->next;
            nxt1->next = temp;
     }
}

/*
	Method which set the head value of each node
*/
void setNodeHeadValue()         // setting the head value of each nxt[i]
{
     for(int i=0;i<n;i++)
     {
             nxt[i].val = i;
             nxt[i].next = NULL;
     }
}

/*
Method to check in which list the given value is contained 
*/
int arrayContains(int val, int *a, int size)
{
	for(int i=0;i<size;i++)
	{
		if(val == a[i])
			return 1;
	}
	return 0;
}

/*
	Method to get the individual PSI value of each node
*/
int get_Individual_PSI_Value(node *nxt, int nodeValue, int *a, int *b)      
{
	int sum = 0;
	while(nxt != NULL)
	{
		if( arrayContains(nodeValue,a,aSize) == arrayContains(nxt->val,b,bSize))
			sum++;
		nxt = nxt->next;
	}
	return sum;
}

/*
	Method to function to calculate PSI_NOT
*/
int get_Initial_PSI_Value_Before_Picking_Random(node *nxt, int n, int *a, int *b)		
{
	int PSI_NOT = 0;
	
	for(int i=0;i<n;i++)
	{
		PSI_NOT = PSI_NOT + get_Individual_PSI_Value(nxt[i].next, nxt[i].val, a,b);		// sum up the individual value of PSI
	}
	return PSI_NOT;
}

/*
	Method to switch the values
*/
void swiftArray2(int *c, int size, int randValue)
{
	int i, cSize = 0;
	for(i=0;i<size;i++)
	{
		if(c[i] != randValue)
		{
			c[cSize] = c[i];
			cSize = cSize + 1;
		}
	}
}

/*
	Method to swift the array that actualy contains the randValue
*/
int *swiftArray(int *c, int size, int randValue)		
{
	int *temp = (int*)malloc((1000000) * sizeof(int));
	int tempSize = 0,i;
	for(i=0;i<size;i++)
	{
		if(c[i] != randValue)
		{
			temp[tempSize] = c[i];
			tempSize = tempSize + 1;
			temp[tempSize] = (int)malloc(sizeof(int));
		}
	}
	return temp;
	
}

/*
	Method to swap array by taking 1 node from A to B (or) from B to A
*/
void swapArray(int randValue)           
{
	if(arrayContains(randValue,a,aSize))
	{
		swiftArray2(a, aSize, randValue);
		aSize--;
		b[bSize] = (int)malloc(sizeof(int));				// add from 'a' to 'b'
		b[bSize++] = randValue;
	}
	 
	else if(arrayContains(randValue,b,bSize))
	{
		swiftArray2(b, bSize, randValue);
		bSize--;
		a[aSize] = (int)malloc(sizeof(int));				// add from 'b' to 'a'
		a[aSize++] = randValue;
	}
}

/*
	Method to get the PSI Value
*/
void get_PSI_value(node *nxt, int n)      
{
	int i,j,k,PSI_NOT,minCutSetValue;
	int randValue, PSI_NEW = 0, delE = 0, PSI_PREV;
	int rand_0_1;
	float propability;
	double tempRand;																// temp variable to get the random number
	srand ( time(NULL) );	                                                        // used to get the random number for 'each time'
    
    FILE *fpW = fopen(outputFileName,"w");
    if(fpW == NULL)
    {
           perror("\nError in writing the file...\nProgram Exiting....");
           getch();
           exit(EXIT_FAILURE);
    }		                                            	
    
	
	PSI_NOT = get_Initial_PSI_Value_Before_Picking_Random(nxt,n,a,b)/2;				// get the PSI_NOT value before choosing randon number
	printf("\n\nPSI_NOT...%d\n",PSI_NOT);
	fprintf(fpW, "\nPSI_NOT...%d\n",PSI_NOT);
	PSI_NEW = PSI_NOT;
	minCutSetValue = PSI_NOT;
	while(T>=1)
	{
		for (k=0;k<50;k++)
		{
		printf("\n\n T....%f",T);
		randValue = rand() % n;				                                    	// random value to be picked between 0 and n-1
		PSI_PREV = PSI_NEW;
		
		PSI_NEW = PSI_PREV - get_Individual_PSI_Value(nxt[randValue].next, nxt[randValue].val, a,b);
		swapArray(randValue);
		PSI_NEW = PSI_NEW + get_Individual_PSI_Value(nxt[randValue].next, nxt[randValue].val, a,b);
		
		delE = PSI_NOT - PSI_NEW;
		propability = exp (delE/T);

		if(delE>0) {
			PSI_NOT = PSI_NEW;
		}
		else if(propability > 0.999 ) {
			PSI_NOT = PSI_NEW;
		}
		else {
			PSI_NEW = PSI_PREV;
			swapArray(randValue);
		
		}
		if(PSI_NEW<minCutSetValue)
			minCutSetValue=PSI_NEW;
		}
		T = T-0.1;
	}
	fprintf(fpW,"\n\n minimum cut set value is %d",minCutSetValue);
	fclose(fpW);
}

/*
	Method to print the linked list
*/
void printLinkedList()          
{
     int i;
     for(i=0;i<n;i++)
     {
                     printf("\nThe adjacent nodes of %d....", nxt[i].val);
                     node *nxt1 = nxt[i].next;
                     while(nxt1!=NULL)
                     {
                                     printf("%d\t", nxt1->val);
                                     nxt1 = nxt1->next;
                     }
     }
}

/*
	Method to get the n-digit number in 'ch' between the start and end index
*/
int getNumberForTheIndex(char ch[100], int start, int end)		
{
	double sum = 0, powIndex = 0;
	for(int i=end-1;i>=start;i--)
		if(ch[i]>=48 && ch[i]<=57)					                               	// check if the char is 'number' or not
			{
				sum = sum + (ch[i]-48)*pow(10,powIndex);		                    // for calculation, keep 'sum' as 'double'
				powIndex++;
			}
	return (int)sum;
}

/*
	Method to reads area of each node from .are file
*/
void readArea()            
{
	FILE *fp = fopen(areaFileName,"r");
	if(fp == NULL)
	{
		perror("Error in opening the file:");
		exit(EXIT_FAILURE);
	}
	
	char ch[100];
	while(fgets(ch,1000,fp)!=NULL)
		if(ch[0]=='a')
			setArea(ch);					// once each line in the file is encountered, read those values into 'nodeArrays' and 'weightArrays'
	
	sortArray();							// sort the 'nodeArrays' and 'weightArrays'
	read_into_A_and_B_Arrays();				// read 'nodeArrays' into 'a' and 'b'
}

/*
	Method to set the area in the given array
*/
void setArea(char ch[100])       
{
	int nodeValue = getNumberForTheIndex(ch,1,getSpaceIndex(ch));				       	//get the nodeValues and weightValues
	int weightNodeValue = getNumberForTheIndex(ch, getSpaceIndex(ch),strlen(ch));	
	
	nodeArray[nodeArraySize++] = nodeValue;								          		// enter those values into respective arrays
	weightArray[weightArraySize++] = weightNodeValue; 	           
}

/*
	Method to find the index of the space
*/
int getSpaceIndex(char ch[100])			
{
	int i;
	for(i=0;ch[i]!=' ';i++);
	return i;
}

/*
	Method to sort the nodeArray and weightArray using BUBBLE SORT
*/
void sortArray()						
{
	for(int i=0;i<nodeArraySize-1;i++)
		for(int j=i+1;j<nodeArraySize;j++)
			if(weightArray[i] > weightArray[j]) 
			{
				int temp = weightArray[i];
				weightArray[i] = weightArray[j];
				weightArray[j] = temp;
				temp = nodeArray[i];
				nodeArray[i] = nodeArray[j];
				nodeArray[j] = temp;
			}
}

/*
	Method to read from nodeArrays into 'a' and 'b' arrays
*/
void read_into_A_and_B_Arrays()				
{
	int i=0,j=nodeArraySize-1, flag=1;
	for(;i<nodeArraySize/2;i++,j--,flag++)
	{
		if((flag/2)%2==0)					                                     // constraint to read into 'a' with flag = 1 initially
		{
			a[aSize++] = nodeArray[i];
			a[aSize++] = nodeArray[j];
		}
		else
		{
			b[bSize++] = nodeArray[i];
			b[bSize++] = nodeArray[j];
		}
	}
	if(i==j)												                      // if odd no of nodes exists
		a[aSize++] = nodeArray[i];
}

/*
	Method to print any array like nodeArray, weightArray, 'a', 'b'
*/
void printPartitionArray(int *a, int size)			
{
	printf("\nPrinting the given Partition Array...");
	for(int i=0;i<size;i++)
		printf("%d\t",a[i]);

}

/*
	Method to set the file name value
*/
void setFileNameValues()
{
	strcpy(netFileName, fileName);
	strcpy(areaFileName, fileName);
	strcpy(outputFileName, fileName);

	strcat(netFileName,".net");
	strcat(areaFileName,".are");
	strcat(outputFileName,".txt");

	char tempOutput[50] = "Output_";
	strcat(tempOutput,outputFileName);
	strcpy(outputFileName, tempOutput);
}
