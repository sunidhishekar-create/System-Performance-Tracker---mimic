#include <stdio.h>  
#include <stdlib.h>
#include <stdbool.h> 
#include <ctype.h> 
#include "memoryManagement.h"

int ADDRESS_TRACKER = 0;
int MEMORY_TRACKER = 0;
int MAX_MEM = 1000;

/*
    use binary tree.
    each node consists of how much mem is allocated->starting address and block size.
    write a func to take input. and return what input type was.
    another func to alloc memory->return how much was allocated.->mix both 
    add this value to the tree.
*/
node* root = NULL;

void free_tree(node* root) {
    if (root == NULL)
        return;

    // only recurse into real left/right children
    if (!root->lthread)
        free_tree(root->left);
    if (!root->rthread)
        free_tree(root->right);

    // reset node state only
    root->allocated = false;
    root->count = 0;
}

//return NULL on failure
node *CreateNode (int x)
{
    int val;
    if(!x)
        val = to_allocate();
    else
        val = x;

    if(val>0){
        node *p_node;
        
        p_node = (node *)malloc(sizeof(node));  
        if (p_node == NULL)
        {
            //printf ("memory allocation failure\n");
            return NULL;
        }
            
            p_node->allocated = true;  
            //p_node->occupied = val;
            p_node->address = ADDRESS_TRACKER;
            p_node->value = val;
            p_node->count = 1;
            p_node->lthread = true;  
            p_node->rthread = true;
            ADDRESS_TRACKER += val;
            MEMORY_TRACKER += val;
        p_node->left = NULL;
        p_node->right = NULL;
        return p_node;
    }

    else
        return NULL;
}

node *inorderSuccessor(node* ptr){
    if (ptr == NULL) return NULL;
    if (ptr->rthread) return ptr->right;
    ptr = ptr->right;
    while (ptr && !ptr->lthread)
        ptr = ptr->left;
    return ptr;
}


//return 0 on success and -1 on failure
int insert_or_edit(int v){
     if((MEMORY_TRACKER<=MAX_MEM)&&(MAX_MEM-MEMORY_TRACKER >0)){
                
            int retVal = -1;
            node*curr = root;
            node* parent = NULL;
            node* p_node;

            if(!v){
                p_node =  CreateNode(0);
            }
            else{
                p_node = CreateNode(v);
                MEMORY_TRACKER -= p_node->value;
            }

            if(MEMORY_TRACKER>MAX_MEM){
                printf("Allocation failed, not enough memory.\n");
                MEMORY_TRACKER-=p_node->value;
                free(p_node);
                return -1;
                
            }

            if (p_node == NULL) {  
                    return -1;
                }

            if(p_node){
                    if(curr == NULL){
                    root = p_node;
                    return 0;
                }
            
                while(curr != NULL){
                    parent = curr;
                    if(p_node->value == curr->value)
                    {
                    
                        if(!curr->allocated){
                            curr->allocated = true; 
                            curr->count++;
                            //curr->occupied += p_node->value;  

                        }                     
        //check
                        else
                        {
                            curr->count++;
                            //curr->occupied +=p_node->value;
                        }

                    
                        ADDRESS_TRACKER -= p_node->value;
                        free(p_node);                
                        return 0;
                    }
                    else{
                        //ifp_node val is less and curr is free, move on anyway. move on till i get p_node val greater than curr, or, i end up inserting. 
                        if(p_node->value<curr->value)
                            {
                                if(!curr->lthread)
                                    curr = curr->left;
                                else
                                    break;
                            }

                            else
                            {
                                if(!curr->allocated){
                                    curr->allocated = true;
                                    curr->count++;
                                    //curr->occupied += p_node->value;                     
                                    ADDRESS_TRACKER -= (p_node->value-curr->value);
                                    int remaining = (p_node->value-curr->value);
                                    free(p_node);
                                    insert_or_edit(remaining);
                                    return 0;
                                }
                                
                                if(!curr->rthread)
                                    curr = curr->right;
                                else
                                    break;            
                            }
                        }
                    }
                }             
                    
    

                if(p_node->value < parent->value)
                {
                    p_node->left = parent->left;
                    p_node->right = parent;
                    parent->lthread = false;
                    parent->left = p_node;
                    retVal = 0;
                }
                else
                {
                    p_node->right = parent->right;
                    p_node->left = parent;
                    parent->rthread = false;
                    parent->right = p_node;
                    retVal = 0;
                }            
                    

            return retVal;


     }

    else{
        printf("Not enough memory to allocate.\n");
        return -1;
     }

   
    
}
  

void display(){
    if(!root){
        printf("nothing to display!\n");
        return;
    }
       
    
    node *cur = root;
    while(!cur->lthread)
        	cur = cur->left;

    	while(cur != NULL) 
	{   
            printf("\n==============================\n");
        	printf("\naddress: %d\nmemory available: %d\ncount: %d\nstatus: %s\n",
            cur->address,
            cur->value,  cur->count,
            cur->allocated ? "allocated" : "free");

        	cur = inorderSuccessor(cur);
    }
    printf("\n==============================\n");
}

void alloc(){
    if((MEMORY_TRACKER<MAX_MEM)&&(MAX_MEM-MEMORY_TRACKER >0)){
        //node* p_node = CreateNode(); put it in insert
        int success = insert_or_edit(0);
        if(success == 0)
            printf("allocation successful. memory used: %d / %d\n", MEMORY_TRACKER, MAX_MEM);

        if(success == -1){
            printf("allocation failed!\n");
            return;
        }            
    }
    else{
        printf("Memory full!\n");
        return;
    }

}

void dealloc(int v){   

    if((MEMORY_TRACKER <= 0)||(root == NULL)){
        MEMORY_TRACKER = 0;
        printf("memory is empty!\n");
        return;
    } 
    int success;
    node* curr = root;
    if(!v){
        success = to_deallocate();
        if(success<=0){
            printf("deallocation failed!\n");
        return;
        }      
        MEMORY_TRACKER -= success;
       
    }
    else  {
        success = v;
        if(success<=0){
            printf("deallocation failed!\n");
        return;
    }      

    }        
//(success >= (MEMORY_TRACKER+success))||
    if (MEMORY_TRACKER <= 0) {
        printf(" Memory shall be freed completely.\n");
        MEMORY_TRACKER = 0;
        free_tree(root);
        
        return;
    }    

    
    while(curr != NULL){

            if(success == curr->value){
                if(curr->count>1){
                    curr->count--;
                    return;
                }
                    
                else if(curr->allocated){
                    curr->allocated = false;
                    curr->count = 0; //same as count-- ->count if <1 = 1.if allocated was true
                    return;
                }               
                                    
            }

            if(success< curr->value){
                if(!curr->lthread)
                     curr = curr->left;
                else
                    break;
            }
               


            else if(success >curr->value){
                if(!curr->rthread)
                     curr = curr->right;
                else
                    break;
            }
    }

        curr = root;
        while (curr!=NULL && !curr->rthread)
            curr = curr->right;

    while(success && curr!= NULL){
        if(curr->allocated){
            if(success>=curr->value*curr->count){        
                        success = success - curr->value*curr->count;
                        curr->count = 0;
                        curr->allocated = false;
                        if(success == 0)
                            break;
                        dealloc(success);
                        break;
                }
            else if(success<curr->value*curr->count){
                    if(success>curr->value){
                            //only enters this if block if count is more than one, otherwise success will be < curr->val
                            success = success- curr->value;
                            curr->count --;
                            //curr->allocated = false;
                            if(success == 0)
                                break;
                        
                        dealloc(success);
                        break;
                    }
                    else{
                        curr->count --;
                        if(curr->count ==0){
                            curr->allocated = false;
                        }                         
                        
                        int left = curr->value - success;
                        insert_or_edit(left);
                        break;
                    }
                    
                }                
                // else if(success<curr->value){
                //       if(curr->count ==1){
                //         curr->count --;
                //         curr->allocated = false;
                //          success = curr->value- success;
                //         if(success == 0)
                //             break;
                //         insert_or_edit(success);
                //           break;
                //     }
                //    else{
                //         curr->count --;
                //         success = curr->value- success;
                //         if(success == 0)
                //             break;
                //         insert_or_edit(success);
                //         break;
                //    } 
                // }
              
           
    }  
    //if it's already free. check the next highest mem node.   
        else{
            curr = curr->left;
        }
}

}

// int type_of_data(){
//         int choice;
//         int retVal;
//         printf("choose data type:\n\
//         1. integer\n\
//         2. character\n\
//         3. long\n\
//         4. Dynamic memory allocation\n");
//         int check = scanf("%d", &choice);
//         if(check !=1){
//             while ((choice = getchar()) != '\n' && choice != EOF);
//             printf("invalid input");
//         }
//         else{
//              switch (choice)
//             {
//             case 1:
//                 retVal = 1;
//                 break;
//             case 2:
//                 retVal = 2;
//                 break;
//             case 3:
//                 retVal = 3;
//                 break;
//             case 4:
//                 retVal = 4;
//                 break;               
//             default:
//                 retVal = 0;
//                 break;
//         }
//         return retVal;
// }
// }
//return how much memory allocated

//return val on success and -1 on failure
int to_allocate(){

    int retVal = -1;
    printf("\nhow much memory to allocate: ");

    int n;
    int check1 = scanf("%d", &n);
    if(check1 !=1){
        int ch;
        while (( ch = getchar()) != '\n' && ch != EOF);
        printf("invalid input\n");
        retVal = -1;
    }
    else
        retVal = n;

    return retVal;
   
}

int to_deallocate(){
    int retVal = -1;
    printf("\nhow much memory to deallocate: ");

    int n;
    int check1 = scanf("%d", &n);
    if(check1 !=1){
        int ch;
        while (( ch = getchar()) != '\n' && ch != EOF);
        printf("invalid input\n");
        retVal = -1;
    }
    else
        retVal = n;

    return retVal;

}


// int to_allocate(){
//     int retVal;
//     int choice = type_of_data();            
//     switch (choice)
//         {
//         case 1:
//             retVal = 4;
//             break;
//         case 2:
//             retVal = 1;
//             break;
//         case 3:
//             retVal = 8;
//             break;
//         case 4:
//             retVal = for_dynamic_mem_alloc();
//             break;
//         default:
//             retVal = 0;
//             printf("pleace choose from the list");
//             break;
//         }
// }
    
// int for_dynamic_mem_alloc(){
//         int choice, n;
//         int retVal;
//         printf("allocating as (typeOfData * n)\n\
//         choose data type:\n\
//         1. integer\n\
//         2. character\n\
//         3. long\n");
//         int check = scanf("%d", &choice);
//         if(check !=1){
//             while ((choice = getchar()) != '\n' && choice != EOF);
//             printf("invalid input");
//         }
//         printf("enter n: ");
//         int check2 = scanf("%d", &n);
//         if(check2 !=1){
//             while ((choice = getchar()) != '\n' && choice != EOF);
//             printf("invalid input");
//         }
//         else{
//              switch (choice)
//             {
//             case 1:
//                 retVal = 4*n;                
//                 break;
//             case 2:
//                 retVal = n;
//                 break;
//             case 3:
//                 retVal = 8*n;
//                 break;             
//             default:
//                 retVal = 0;
//                 printf("pleace choose from the list");
//                 break;
//         }
//         return retVal;
// }
// }

// node *delete_node(node *root, int dkey);  
// node *case_a(node *root, node *par,node *ptr);  
// node *case_b(node *root,node *par,node *ptr);  
// node *case_c(node *root, node *par,node *ptr); 