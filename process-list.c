#include "process-list.h"
#include <stdlib.h>
#include <stdio.h>

/* ----------------------------------------------------------
   STRING FUNCTIONS
---------------------------------------------------------- */

static void mystrncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
}

int mystrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

/* ----------------------------------------------------------
   MENU
---------------------------------------------------------- */

void displayMenu() {
    printf("\nPROCESS LIST MANAGEMENT SYSTEM\n");
    printf("1. Add Process\n");
    printf("2. Delete Process\n");
    printf("3. Search Process\n");
    printf("4. Display All Processes\n");
    printf("10. Search Processes by State\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

/* ----------------------------------------------------------
   VALID STATES + VALIDATION FUNCTIONS
---------------------------------------------------------- */

static const char *validStates[] = {
    "RUNNING",
    "READY",
    "BLOCKED",
    "TERMINATED"
};

static const int NUM_STATES = 4;

int isValidState(const char *state) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (mystrcmp(state, validStates[i]) == 0)
            return 1;
    }
    return 0;
}

int isValidPriority(int priority) {
    return (priority >= 1 && priority <= 10);
}

/* ----------------------------------------------------------
   PROCESS LIST CREATION
---------------------------------------------------------- */

ProcessList *createProcessList() {
    ProcessList *list = (ProcessList *)malloc(sizeof(ProcessList));
    if (!list) {
        fprintf(stderr, "Memory allocation failed for ProcessList\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

/* ----------------------------------------------------------
   PROCESS CREATION
---------------------------------------------------------- */

Process *createProcess(int pid, const char *name, int priority, int cpu_time, const char *state) {
    Process *newProcess = (Process *)malloc(sizeof(Process));
    if (!newProcess) {
        fprintf(stderr, "Memory allocation failed for Process\n");
        return NULL;
    }

    newProcess->pid = pid;

    mystrncpy(newProcess->name, name, sizeof(newProcess->name) - 1);
    newProcess->name[sizeof(newProcess->name) - 1] = '\0';

    newProcess->priority = priority;
    newProcess->cpu_time = cpu_time;

    mystrncpy(newProcess->state, state, sizeof(newProcess->state) - 1);
    newProcess->state[sizeof(newProcess->state) - 1] = '\0';

    newProcess->prev = NULL;
    newProcess->next = NULL;

    return newProcess;
}

/* ----------------------------------------------------------
   ADD PROCESS
---------------------------------------------------------- */

void addProcess(ProcessList *list, int pid, const char *name, int priority, int cpu_time, const char *state) {
    if (!list) {
        printf("Error: process list is NULL\n");
        return;
    }

    if (!isValidPriority(priority)) {
        printf("Error: Invalid priority %d. Valid range is 1–10.\n", priority);
        return;
    }

    if (!isValidState(state)) {
        printf("Error: Invalid state '%s'. Valid states: ", state);
        for (int i = 0; i < NUM_STATES; i++) {
            printf("%s ", validStates[i]);
        }
        printf("\n");
        return;
    }

    if (searchProcessByPID(list, pid)) {
        printf("Error: Process with PID %d already exists!\n", pid);
        return;
    }

    Process *newProcess = createProcess(pid, name, priority, cpu_time, state);
    if (!newProcess) return;

    if (!list->head) {
        list->head = list->tail = newProcess;
    } else {
        list->tail->next = newProcess;
        newProcess->prev = list->tail;
        list->tail = newProcess;
    }

    list->count++;
    printf("Process added: PID=%d, Name=%s\n", pid, name);
}

/* ----------------------------------------------------------
   REMOVE PROCESS BY PID
---------------------------------------------------------- */

void removeProcessByPID(ProcessList *list, int pid) {
    if (!list) return;

    Process *proc = searchProcessByPID(list, pid);

    if (!proc) {
        printf("Error: Process with PID %d not found!\n", pid);
        return;
    }

    if (proc == list->head && proc == list->tail) {
        list->head = list->tail = NULL;
    } else if (proc == list->head) {
        list->head = proc->next;
        if (list->head) list->head->prev = NULL;
    } else if (proc == list->tail) {
        list->tail = proc->prev;
        if (list->tail) list->tail->next = NULL;
    } else {
        proc->prev->next = proc->next;
        proc->next->prev = proc->prev;
    }

    printf("Process removed: PID=%d, Name=%s\n", pid, proc->name);
    free(proc);
    list->count--;
}

/* ----------------------------------------------------------
   REMOVE PROCESS BY NAME
---------------------------------------------------------- */

void removeProcessByName(ProcessList *list, const char *name) {
    if (!list) return;

    Process *proc = searchProcessByName(list, name);

    if (!proc) {
        printf("Error: Process with name '%s' not found!\n", name);
        return;
    }

    if (proc == list->head && proc == list->tail) {
        list->head = list->tail = NULL;
    } else if (proc == list->head) {
        list->head = proc->next;
        if (list->head) list->head->prev = NULL;
    } else if (proc == list->tail) {
        list->tail = proc->prev;
        if (list->tail) list->tail->next = NULL;
    } else {
        proc->prev->next = proc->next;
        proc->next->prev = proc->prev;
    }

    printf("Process removed: PID=%d, Name=%s\n", proc->pid, proc->name);
    free(proc);
    list->count--;
}

/* ----------------------------------------------------------
   SEARCH
---------------------------------------------------------- */

Process *searchProcessByPID(ProcessList *list, int pid) {
    if (!list) return NULL;
    Process *current = list->head;
    while (current) {
        if (current->pid == pid)
            return current;
        current = current->next;
    }
    return NULL;
}

Process *searchProcessByName(ProcessList *list, const char *name) {
    if (!list) return NULL;
    Process *current = list->head;
    while (current) {
        if (mystrcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

/* ----------------------------------------------------------
   DISPLAY PROCESSES BY STATE (returns all matches)
---------------------------------------------------------- */

void displayProcessesByState(ProcessList *plist, const char *state) {
    if (!plist) {
        printf("Error: process list is NULL\n");
        return;
    }

    if (!isValidState(state)) {
        printf("Error: '%s' is not a valid state.\n", state);
        return;
    }

    Process *current = plist->head;
    int found = 0;

    while (current != NULL) {
        if (mystrcmp(current->state, state) == 0) {
            displayProcessDetails(current);
            printf("-----------------------------------\n");
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No processes found with state '%s'.\n", state);
    }
}

/* ----------------------------------------------------------
   DISPLAY
---------------------------------------------------------- */

void displayProcessDetails(Process *proc) {
    if (!proc) return;
    printf("\nProcess Details:\n");
    printf("PID       : %d\n", proc->pid);
    printf("Name      : %s\n", proc->name);
    printf("Priority  : %d\n", proc->priority);
    printf("CPU Time  : %d ms\n", proc->cpu_time);
    printf("State     : %s\n\n", proc->state);
}

void displayAllProcesses(ProcessList *list) {
    if (!list || !list->head) {
        printf("\nNo processes in the list.\n\n");
        return;
    }

    printf("\nPROCESS LIST (Total: %d)\n", list->count);
    printf("-------------------------------------------------------------\n");
    printf("PID\tName\t\tPriority\tCPU Time\tState\n");
    printf("-------------------------------------------------------------\n");

    Process *current = list->head;
    while (current) {
        printf("%d\t%-15s\t%d\t\t%d ms\t\t%s\n",
               current->pid, current->name, current->priority,
               current->cpu_time, current->state);
        current = current->next;
    }

    printf("-------------------------------------------------------------\n\n");
}

/* ----------------------------------------------------------
   FREE MEMORY
---------------------------------------------------------- */

void freeProcessList(ProcessList *list) {
    if (!list) return;
    Process *current = list->head;
    while (current) {
        Process *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
