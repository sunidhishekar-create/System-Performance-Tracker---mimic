// memoryManagement.h

typedef struct NODE
{
    bool allocated;
    int address; 
    int value; // mem stored
    //int occupied; 
    int count;
    struct NODE * left;
    struct NODE * right;
    bool lthread;
    bool rthread;
}node;



void display(void);
void alloc(void);
void dealloc(int);
int to_deallocate(void);
int to_allocate(void);



