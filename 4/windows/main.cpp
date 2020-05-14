
#include <windows.h>

#include <stdio.h>

HANDLE semaphore;
int a[5];
HANDLE thread;
unsigned long thread_id;

typedef struct Stack
{
	HANDLE value;
	struct Stack *next;
}Stack;

HANDLE pop(Stack **head);
void push(HANDLE value,Stack** head);

void Thread(void* pParams, Stack **head )
{ 
	int i, num = 0;
	while (TRUE)
	{ 
		int id = GetCurrentThreadId();

		WaitForSingleObject(semaphore, INFINITE);

		printf("My id is:%d\n", id);

		ReleaseSemaphore(semaphore, 1, NULL);
		Sleep(500);
	}
}



int main()
{ 
	TCHAR name[]=TEXT("Semaphore");

	semaphore = CreateSemaphore(NULL, 1, 1, name);

	char symbol;

	Stack *stack=NULL;

	while(TRUE)
	{
		symbol = getchar();
		if('+' == symbol)
		{
			thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, NULL, 0, &thread_id);
			push(thread,&stack);
		}

		if('-' == symbol)
		{
			if(stack != NULL)
			{
				TerminateThread(pop(&stack),0);
			}
			else
			{
				puts("Thread stack is empty");
			}
		}
		if('q' == symbol)
		{
			while(NULL != stack)
			{
				TerminateThread(pop(&stack),0);
			}

			return 0;
		}
	}
	return 0;
}

void push(HANDLE value,Stack** head)
{
	Stack *temp =(Stack*) malloc(sizeof(Stack));	

	if(NULL == temp)
	{
		perror("push");
		exit(EXIT_FAILURE);
	}

	temp->next = *head;

	temp->value = value;

	*head = temp;
}

HANDLE pop(Stack **head)
{

	Stack *popped;

	HANDLE value;

	popped = *head;

	*head = (*head)->next;

	value = popped->value;

	free(popped);

	return value;

}

