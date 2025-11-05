#include "process-list.h"
#include <stdlib.h>
#include <stdio.h>

void displayMenu() {
    printf("\nPROCESS LIST MANAGEMENT SYSTEM\n");
    printf("1. Add Process\n");
    printf("2. Delete Process\n");  // Unified delete option
    printf("3. Search Process\n");  // Unified search option
    printf("4. Display All Processes\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

// Create an empty process list
ProcessList *createProcessList()
{
    ProcessList *list = (ProcessList *)malloc(sizeof(ProcessList));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

static void mystrncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    // Copy characters from src to dest
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }

    // Pad with null characters if src is shorter than n
    for (; i < n; i++)
    {
        dest[i] = '\0';
    }
}

int mystrcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// Create a new process node
Process *createProcess(int pid, const char *name, int priority, int cpu_time, const char *state)
{
    Process *newProcess = (Process *)malloc(sizeof(Process));
    newProcess->pid = pid;
    mystrncpy(newProcess->name, name, 49);
    newProcess->name[49] = '\0';
    newProcess->priority = priority;
    newProcess->cpu_time = cpu_time;
    mystrncpy(newProcess->state, state, 19);
    newProcess->state[19] = '\0';
    newProcess->prev = NULL;
    newProcess->next = NULL;
    return newProcess;
}

// Add a process to the list
void addProcess(ProcessList *list, int pid, const char *name, int priority, int cpu_time, const char *state)
{
    if (searchProcessByPID(list, pid))
    {
        printf("Error: Process with PID %d already exists!\n", pid);
        return;
    }

    Process *newProcess = createProcess(pid, name, priority, cpu_time, state);
    if (!list->head)
    {
        list->head = list->tail = newProcess;
    }
    else
    {
        list->tail->next = newProcess;
        newProcess->prev = list->tail;
        list->tail = newProcess;
    }
    list->count++;
    printf("Process added: PID=%d, Name=%s\n", pid, name);
}

// Remove process by PID
void removeProcessByPID(ProcessList *list, int pid)
{
    Process *proc = searchProcessByPID(list, pid);
    if (!proc)
    {
        printf("Error: Process with PID %d not found!\n", pid);
        return;
    }

    if (proc == list->head && proc == list->tail)
    {
        list->head = list->tail = NULL;
    }
    else if (proc == list->head)
    {
        list->head = proc->next;
        list->head->prev = NULL;
    }
    else if (proc == list->tail)
    {
        list->tail = proc->prev;
        list->tail->next = NULL;
    }
    else
    {
        proc->prev->next = proc->next;
        proc->next->prev = proc->prev;
    }

    printf("Process removed: PID=%d, Name=%s\n", pid, proc->name);
    free(proc);
    list->count--;
}

// Remove process by name
void removeProcessByName(ProcessList *list, const char *name)
{
    Process *proc = searchProcessByName(list, name);
    if (!proc)
    {
        printf("Error: Process with name '%s' not found!\n", name);
        return;
    }

    if (proc == list->head && proc == list->tail)
    {
        list->head = list->tail = NULL;
    }
    else if (proc == list->head)
    {
        list->head = proc->next;
        list->head->prev = NULL;
    }
    else if (proc == list->tail)
    {
        list->tail = proc->prev;
        list->tail->next = NULL;
    }
    else
    {
        proc->prev->next = proc->next;
        proc->next->prev = proc->prev;
    }

    printf("Process removed: PID=%d, Name=%s\n", proc->pid, proc->name);
    free(proc);
    list->count--;
}

// Search by PID
Process *searchProcessByPID(ProcessList *list, int pid)
{
    Process *current = list->head;
    while (current)
    {
        if (current->pid == pid)
            return current;
        current = current->next;
    }
    return NULL;
}

// Search by name
Process *searchProcessByName(ProcessList *list, const char *name)
{
    Process *current = list->head;
    while (current)
    {
        if (mystrcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// Display details of a single process
void displayProcessDetails(Process *proc)
{
    printf("\nProcess Details:\n");
    printf("PID       : %d\n", proc->pid);
    printf("Name      : %s\n", proc->name);
    printf("Priority  : %d\n", proc->priority);
    printf("CPU Time  : %d ms\n", proc->cpu_time);
    printf("State     : %s\n\n", proc->state);
}

// Display all processes
void displayAllProcesses(ProcessList *list)
{
    if (!list->head)
    {
        printf("\nNo processes in the list.\n\n");
        return;
    }

    printf("\nPROCESS LIST (Total: %d)\n", list->count);
    printf("-------------------------------------------------------------\n");
    printf("PID\tName\t\tPriority\tCPU Time\tState\n");
    printf("-------------------------------------------------------------\n");

    Process *current = list->head;
    while (current)
    {
        printf("%d\t%-15s\t%d\t\t%d ms\t\t%s\n",
               current->pid, current->name, current->priority,
               current->cpu_time, current->state);
        current = current->next;
    }

    printf("-------------------------------------------------------------\n\n");
}

// Free all memory
void freeProcessList(ProcessList *list)
{
    Process *current = list->head;
    while (current)
    {
        Process *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}