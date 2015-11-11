#include <stdio.h>

#define resourceTypeQuan 3
#define processQuan 5

int i = 0;//Switch on C99 mode or we cannot initialize variable in for loop
int j = 0;

//available, max, allocation, need
int availResourceVector [resourceTypeQuan];
int allocMatrix [processQuan][resourceTypeQuan] = {{1,1,4},{1,3,0},{0,0,2},{4,1,1},{0,2,0}};
int maxMatrix [processQuan][resourceTypeQuan] = {{3,3,4},{3,3,3},{3,3,3},{4,1,1},{2,2,2}};
int needMatrix [processQuan][resourceTypeQuan];

int requestResource(int processID,int requestVector[]);
int releaseResource(int processID,int releaseVector[]);
int ifGreaterThanNeed(int processID,int requestVector[]);
int ifEnoughToRelease(int processID,int releaseVector[]);
int ifInSafeMode();
int ifEnoughToAlloc();
void printNeedMatrix();
void printAllocMatrix();
void printAvailable();

int main(int argc, char const *argv[])
{
	if(argc != resourceTypeQuan + 1)
	{
		printf("Quantity of parameter is not correct.\n");
		return -1;
	}
	for(i = 0; i < resourceTypeQuan; i++)
	{
		availResourceVector[i] = atoi(argv[i+1]);//argv[0] is name of program
	}

	//initialize needMatrix
	for (i = 0; i < processQuan; ++i)
	{
		for (j = 0; j < resourceTypeQuan; ++j)
		{
			needMatrix[i][j] = maxMatrix[i][j] - allocMatrix[i][j];
		}
	}

	printf("Available resources vector is:\n");
	printAvailable();

	printf("Initial allocation matrix is:\n");
	printAllocMatrix();

	printf("Initial need matrix is:\n");
	printNeedMatrix();

	int processID;

	printf("Please type in process ID which you request resources for(range from 0 to 4)\n");
	scanf("%d",&processID);
	if (processID > processQuan - 1 || processID < 0)
	{
		printf("Illegal process ID.\n");
		return -1;
	}

	printf("Please type in your request vector likes \"1 1 1\"\n");

	int requestVector[resourceTypeQuan];
	for (i = 0; i < resourceTypeQuan; ++i)
	{
		scanf("%d",&requestVector[i]);
	}

	requestResource(processID,requestVector);


	return 0;
}

int requestResource(int processID,int requestVector[])
{
	//whether request number of resources is greater than needed
	if (ifGreaterThanNeed(processID,requestVector) == -1)
	{
		printf("requested resources is bigger than needed.\n");
		return -1;
	}
	printf("Requested resources are not more than needed.\nPretend to allocate...\n");

	//whether request number of resources is greater than needed
	if(ifEnoughToAlloc(requestVector) == -1)
	{
		printf("There is not enough resources for this process.\n");
		return -1;
	}

	//pretend allocated
	for (i = 0; i < resourceTypeQuan; ++i)
	{
		needMatrix[processID][i] -= requestVector[i];
		allocMatrix[processID][i] += requestVector[i];
		availResourceVector[i] -= requestVector[i];
	}
	printf("Checking if it is still safe...\n");
	
	//check if still in safe status
	if (ifInSafeMode() == 0)
	{
		printf("Allocated successfully.\nNow available resources vector is:\n");
		printAvailable();
		printf("Now allocated matrix is:\n");
		printAllocMatrix();
		printf("Now need matrix is:\n");
		printNeedMatrix();
		return 0;
	}
	else
	{
		printf("It is not safe. Rolling back.\n");
		for (i = 0; i < resourceTypeQuan; ++i)
		{
			needMatrix[processID][i] += requestVector[i];
			allocMatrix[processID][i] -= requestVector[i];
		}
		printf("Rolled back successfully.\n");
		return -1;
	}
}

int releaseResource(int processID,int releaseVector[])
{
	if(ifEnoughToRelease() == -1)
	{
		printf("The process do not own enough resources to release.\n");
		return -1;
	}
	for(i = 0; i < resourceTypeQuan; i++)
	{
		allocMatrix[processID][i] -= releaseVector[i];
		needMatrix[processID][i] += releaseVector[i];
		availResourceVector[i] += releaseVector[i];
	}
	return 0;
}

int ifEnoughToRelease(int processID,int releaseVector[])
{
	for (i = 0; i < resourceTypeQuan; ++i)
	{
		if (releaseVector[i] <= allocMatrix[processID][i])
		{
			continue;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}
int ifGreaterThanNeed(int processID,int requestVector[])
{
	for (i = 0; i < resourceTypeQuan; ++i)
	{
		if (requestVector[i] <= needMatrix[processID][i])
		{
			continue;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

int ifEnoughToAlloc(int requestVector[])
{
	//first element of requestVector is processID
	for (i = 0; i < resourceTypeQuan; ++i)
	{
		if (requestVector[i] <= availResourceVector[i])
		{
			continue;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

void printNeedMatrix()
{
	for (i = 0; i < processQuan; ++i)
	{
		printf("{ ");
		for (j = 0; j < resourceTypeQuan; ++j)
		{
			printf("%d, ", needMatrix[i][j]);
		}
		printf("}\n");
	}
	return;
}

void printAllocMatrix()
{
	for (i = 0; i < processQuan; ++i)
	{
		printf("{ ");
		for (j = 0; j < resourceTypeQuan; ++j)
		{
			printf("%d, ", allocMatrix[i][j]);
		}
		printf("}\n");
	}
	return;
}

void printAvailable()
{
	for (i = 0; i < resourceTypeQuan; ++i)
	{
		printf("%d, ",availResourceVector[i]);
	}
	printf("\n");
	return;
}

int ifInSafeMode()
{
	int ifFinish[processQuan] = {0};//there is no bool type in old C
	int work[resourceTypeQuan];//temporary available resources vector
	for(i = 0; i < resourceTypeQuan; i++)
	{
		work[i] = availResourceVector[i];
	}
	int k;
	for(i = 0; i < processQuan; i++)
	{
		if (ifFinish[i] == 0)
		{
			for(j = 0; j < resourceTypeQuan; j++)
			{
				if(needMatrix[i][j] < work[j])
				{
					if(j == resourceTypeQuan - 1)//means we checked whole vector, so this process can execute
					{
						ifFinish[i] = 1;
						for (k = 0; k < resourceTypeQuan; ++k)
						{
							work[k] += needMatrix[i][k];
							//execute and release resources
						}
						//if we use break, it will not check all process, so we should reset i to let it check from beginning
						i = -1;//at the end of this loop, i++, so -1++ = 0
						break;//in loop j, break to loop i and check next runnable process
					}
					else//not finished checking all resource, but this kind resources is enough
					{
						continue;
					}
				}
				else//resources not enough, break to loop i for next process
				{
					//because there is no change happened, so we do not need to reset i in this condition.
					break;
				}
			}
		}
		else
		{
			continue;
		}
	}
	//there are two condition if we finish loop i
	//1. there is no process can run in this condition.
	//2. all processes are runned, which means it is in safe status.
	for(i = 0; i < processQuan; i++)
	{
		if (ifFinish[i] == 0)
		{
			//not all processes are runned, so it is condition 1.
			return -1;
		}
		else
		{
			continue;
		}
	}
	//finished loop, so it is condition 2
	return 0;
}