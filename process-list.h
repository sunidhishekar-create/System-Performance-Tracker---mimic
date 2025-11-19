// Process structure
typedef struct Process {
    int pid;                    // Process ID
    char name[50];              // Process name
    int priority;               // Priority (1-10, 10 being highest)
    int cpu_time;               // CPU time in milliseconds
    char state[20];             // State: RUNNING, READY, BLOCKED, TERMINATED
    struct Process *prev;       // Pointer to previous process
    struct Process *next;       // Pointer to next process
} Process;

typedef struct ProcessList {
    Process *head;
    Process *tail;
    int count;
} ProcessList;


ProcessList* createProcessList();
Process* createProcess(int pid, const char *name, int priority, int cpu_time, const char *state);
void addProcess(ProcessList *list, int pid, const char *name, int priority, int cpu_time, const char *state);
void removeProcessByPID(ProcessList *list, int pid);
void removeProcessByName(ProcessList *list, const char *name);
Process* searchProcessByPID(ProcessList *list, int pid);
Process* searchProcessByName(ProcessList *list, const char *name);
void displayProcessDetails(Process *proc);
void displayAllProcesses(ProcessList *list);
void freeProcessList(ProcessList *list);
void displayMenu();

void displayProcessesByState(ProcessList *plist, const char *state);
