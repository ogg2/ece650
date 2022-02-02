#include  "my_malloc.h"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
struct Node {
    int used;
    size_t size;
    struct Node * next;
    struct Node * prev;
};

void * p = NULL;
__thread struct Node myList = {0, 0, NULL, NULL};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void * setsbrk(size_t size) {
    pthread_mutex_lock(&lock);
    void * res = sbrk(0);
    sbrk(size);
    pthread_mutex_unlock(&lock);
    struct Node * currNode = res;
    currNode->size = size;
    currNode->used = 1;
    currNode->prev = NULL;
    currNode->next = NULL;

    return res+sizeof(struct Node);
}
 
void mergeToList(struct Node * newNode) {
    struct Node * dummy = &myList;
    struct Node * curr = dummy->next;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

    if (curr == NULL) {
        dummy->next = newNode;
        newNode->prev = dummy;
        newNode->next = NULL;
        return;
    }
    
    int FLAG = 0;
    if((void*)newNode+newNode->size<=(void*)curr){
        if((void*)newNode+newNode->size<(void*)curr){
            curr->prev->next=newNode;
            newNode->next=curr;

            newNode->prev=curr->prev;
            curr->prev=newNode;
        }else{
            newNode->size = newNode->size + curr->size;
 
            newNode->next = curr->next;
            curr->prev->next = newNode;

            newNode->prev = curr->prev;
            if(curr->next!=NULL)curr->next->prev = newNode;
            int a=1;
        }
        return;
    }
    while (curr != NULL && curr->next != NULL) {
        if (((void*)curr + curr->size <= (void*)newNode) && ((void*)newNode + newNode->size <= (void*)curr->next)) {
            //(i)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
            FLAG = 1;
            if (((void*)curr + curr->size < (void*)newNode) && ((void*)newNode + newNode->size < (void*)curr->next)) {
                newNode->next = curr->next;
                curr->next = newNode;
 
                newNode->prev = curr;
                newNode->next->prev = newNode;
                int a=1;
            }
            //(ii)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
            else if (((void*)curr + curr->size < (void*)newNode) && ((void*)newNode + newNode->size == (void*)curr->next)) {
                newNode->size = newNode->size + curr->next->size;
 
                newNode->next = curr->next->next;
                curr->next = newNode;
 
                newNode->prev = curr;
                if(curr->next->next!=NULL)curr->next->next->prev = newNode;
                int a=1;
            }
            //(iii) debug                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
            else if (((void*)curr + curr->size == (void*)newNode) && ((void*)newNode + newNode->size < (void*)curr->next)) {
                curr->size = curr->size + newNode->size;
                int a=1;
            }
            //(iv)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
            else if (((void*)curr + curr->size == (void*)newNode) && ((void*)newNode + newNode->size == (void*)curr->next)) {
                curr->size = curr->size + newNode->size + curr->next->size;
 
                struct Node * temp = curr->next->next;
                curr->next = temp;
                if(temp!=NULL)temp->prev = curr;
                int a=1;
            }
        }
        if(FLAG==1)break;
        curr = curr->next;
    }
    if (FLAG == 1)
        return;
    if ((void*)curr + curr->size == (void*)newNode) {
        curr->size = curr->size + newNode->size;
    }
    else if((void*)curr+curr->size<(void*)newNode){
        curr->next = newNode;
        newNode->prev = curr;
        newNode->next = NULL;
    }else{
        
    }
    return;
}


//first fit malloc
void * ff_malloc(size_t size) {
    if (p == NULL) p = sbrk(0);
    size = size + sizeof(struct Node);

    if (myList.next == NULL) {
        return setsbrk(size);
    }
    else {
        struct Node * dummy = &myList;
        struct Node * curr = dummy->next;

        int FLAG = 0;
        size_t gap = 0;
        //first fit                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
        while (curr != NULL && FLAG == 0) {
            if (curr->size >= size) {
                FLAG = 1;
                gap = curr->size - size;
                break;
            }
            curr = curr->next;
        }

        //(i)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
        if (FLAG == 0) {
            return setsbrk(size);
        }

        //(ii)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
        if (gap <= sizeof(struct Node)) {
            curr->prev->next = curr->next;  //remove curr,update next                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
            if(curr->next!=NULL){curr->next->prev = curr->prev;} //update prev
            curr->prev=NULL;
            curr->next=NULL;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
            return (void*)curr+sizeof(struct Node);
        }
        else {  //(iii)  split                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
            curr->prev->next = curr->next;
            if(curr->next!=NULL){curr->next->prev = curr->prev;}
            curr->prev=NULL;
            curr->next=NULL;
            
            curr->size = size;//
            struct Node * newNode = (void*)curr + size;
            newNode->used = 0;
            newNode->size = gap;
            newNode->next=NULL;
            newNode->prev=NULL;
            mergeToList(newNode);
            return (void*)curr+sizeof(struct Node);
        }
    }
}

//frist fit free                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
void ff_free(void * ptr) {
    struct Node * curr = ptr - sizeof(struct Node);
    mergeToList(curr);
}

//best fit malloc
void * bf_malloc(size_t size) {
    if (p == NULL) p = sbrk(0);
    size = size + sizeof(struct Node);

    if (myList.next == NULL) {
        return setsbrk(size);
    }
    else {
        struct Node * dummy = &myList;
        struct Node * curr = dummy->next;

        int FLAG = 0;
        size_t gap = -1;
        struct Node*helper=NULL;
        //first fit - > best fit                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
        while (curr != NULL) {
            if (curr->size == size) {
                FLAG = 1;
                gap = curr->size - size;
                break;
            }else if(curr->size > size){
                FLAG = 2;
                
                if(gap == -1 || gap > curr->size-size){
                    gap = curr->size - size;
                    helper = curr;
                }
            }
            curr = curr->next;
        }

        //(i)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
        if (FLAG == 0) {
            return setsbrk(size);
        }

        //(ii)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
        //if{ (gap <= sizeof(struct Node)) {
        if(FLAG == 1){
            curr->prev->next = curr->next;  //remove curr,update next                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
            if(curr->next!=NULL){curr->next->prev = curr->prev;} //update prev
            curr->prev=NULL;
            curr->next=NULL;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
            return (void*)curr+sizeof(struct Node);
        }
        if(FLAG==2){
            curr=helper;
            if(gap <= sizeof(struct Node)){
                curr->prev->next = curr->next;  //remove curr,update next                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
                if(curr->next!=NULL){curr->next->prev = curr->prev;} //update prev
                curr->prev=NULL;
                curr->next=NULL;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                return (void*)curr+sizeof(struct Node);
            }else{ //(iii)  split 
                curr->prev->next = curr->next;
                if(curr->next!=NULL){curr->next->prev = curr->prev;}
                curr->prev=NULL;
                curr->next=NULL;
            
                curr->size = size;//
                struct Node * newNode = (void*)curr + size;
                newNode->used = 0;
                newNode->size = gap;
                newNode->next=NULL;
                newNode->prev=NULL;
                mergeToList(newNode);
                return (void*)curr+sizeof(struct Node);
            }
        }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

    }
}


//best fit free
void bf_free(void * ptr) {
    struct Node * curr = ptr - sizeof(struct Node);
    mergeToList(curr);
}


/*
unsigned long get_data_segment_size(){
    return sbrk(0) - p;
}
unsigned long get_data_segment_free_space_size(){
    long sum = 0;
    struct Node* cur = dummy->next;
    while (cur != NULL) {
        sum += cur->size;
        cur = cur->next;
    }
    return sum;
}
*/



/* no lock version */
void *ts_malloc_nolock(size_t size) {
    void * res = bf_malloc(size);
    return res;
}
void ts_free_nolock(void *ptr) {
    bf_free(ptr);
}


//=============================================================================================================================




void * p2 = NULL;
struct Node myList2 = {0, 0, NULL, NULL};
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void * setsbrk2(size_t size) {
    void * res = sbrk(0);
    sbrk(size);
    struct Node * currNode = res;
    currNode->size = size;
    currNode->used = 1;
    currNode->prev = NULL;
    currNode->next = NULL;

    return res+sizeof(struct Node);
}
 
void mergeToList2(struct Node * newNode) {
    struct Node * dummy = &myList;
    struct Node * curr = dummy->next;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

    if (curr == NULL) {
        dummy->next = newNode;
        newNode->prev = dummy;
        newNode->next = NULL;
        return;
    }
    
    int FLAG = 0;
    if((void*)newNode+newNode->size<=(void*)curr){
        if((void*)newNode+newNode->size<(void*)curr){
            curr->prev->next=newNode;
            newNode->next=curr;

            newNode->prev=curr->prev;
            curr->prev=newNode;
        }else{
            newNode->size = newNode->size + curr->size;
 
            newNode->next = curr->next;
            curr->prev->next = newNode;

            newNode->prev = curr->prev;
            if(curr->next!=NULL)curr->next->prev = newNode;
            int a=1;
        }
        return;
    }
    while (curr != NULL && curr->next != NULL) {
        if (((void*)curr + curr->size <= (void*)newNode) && ((void*)newNode + newNode->size <= (void*)curr->next)) {
            //(i)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
            FLAG = 1;
            if (((void*)curr + curr->size < (void*)newNode) && ((void*)newNode + newNode->size < (void*)curr->next)) {
                newNode->next = curr->next;
                curr->next = newNode;
 
                newNode->prev = curr;
                newNode->next->prev = newNode;
                int a=1;
            }
            //(ii)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
            else if (((void*)curr + curr->size < (void*)newNode) && ((void*)newNode + newNode->size == (void*)curr->next)) {
                newNode->size = newNode->size + curr->next->size;
 
                newNode->next = curr->next->next;
                curr->next = newNode;
 
                newNode->prev = curr;
                if(curr->next->next!=NULL)curr->next->next->prev = newNode;
                int a=1;
            }
            //(iii) debug                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
            else if (((void*)curr + curr->size == (void*)newNode) && ((void*)newNode + newNode->size < (void*)curr->next)) {
                curr->size = curr->size + newNode->size;
                int a=1;
            }
            //(iv)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
            else if (((void*)curr + curr->size == (void*)newNode) && ((void*)newNode + newNode->size == (void*)curr->next)) {
                curr->size = curr->size + newNode->size + curr->next->size;
 
                struct Node * temp = curr->next->next;
                curr->next = temp;
                if(temp!=NULL)temp->prev = curr;
                int a=1;
            }
        }
        if(FLAG==1)break;
        curr = curr->next;
    }
    if (FLAG == 1)
        return;
    if ((void*)curr + curr->size == (void*)newNode) {
        curr->size = curr->size + newNode->size;
    }
    else if((void*)curr+curr->size<(void*)newNode){
        curr->next = newNode;
        newNode->prev = curr;
        newNode->next = NULL;
    }else{
        
    }
    return;
}


//best fit malloc
void * bf_malloc2(size_t size) {
    if (p2 == NULL) p2 = sbrk(0);
    size = size + sizeof(struct Node);

    if (myList2.next == NULL) {
        return setsbrk2(size);
    }
    else {
        struct Node * dummy = &myList2;
        struct Node * curr = dummy->next;

        int FLAG = 0;
        size_t gap = -1;
        struct Node*helper=NULL;
        //first fit - > best fit                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
        while (curr != NULL) {
            if (curr->size == size) {
                FLAG = 1;
                gap = curr->size - size;
                break;
            }else if(curr->size > size){
                FLAG = 2;
                
                if(gap == -1 || gap > curr->size-size){
                    gap = curr->size - size;
                    helper = curr;
                }
            }
            curr = curr->next;
        }

        //(i)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
        if (FLAG == 0) {
            return setsbrk2(size);
        }

        //(ii)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
        //if{ (gap <= sizeof(struct Node)) {
        if(FLAG == 1){
            curr->prev->next = curr->next;  //remove curr,update next                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
            if(curr->next!=NULL){curr->next->prev = curr->prev;} //update prev
            curr->prev=NULL;
            curr->next=NULL;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
            return (void*)curr+sizeof(struct Node);
        }
        if(FLAG==2){
            curr=helper;
            if(gap <= sizeof(struct Node)){
                curr->prev->next = curr->next;  //remove curr,update next                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
                if(curr->next!=NULL){curr->next->prev = curr->prev;} //update prev
                curr->prev=NULL;
                curr->next=NULL;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                return (void*)curr+sizeof(struct Node);
            }else{ //(iii)  split 
                curr->prev->next = curr->next;
                if(curr->next!=NULL){curr->next->prev = curr->prev;}
                curr->prev=NULL;
                curr->next=NULL;
            
                curr->size = size;//
                struct Node * newNode = (void*)curr + size;
                newNode->used = 0;
                newNode->size = gap;
                newNode->next=NULL;
                newNode->prev=NULL;
                mergeToList2(newNode);
                return (void*)curr+sizeof(struct Node);
            }
        }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

    }
}


//best fit free
void bf_free2(void * ptr) {
    struct Node * curr = ptr - sizeof(struct Node);
    mergeToList2(curr);
}








/* lock version */

void *ts_malloc_lock(size_t size){
    pthread_mutex_lock(&lock);
    void * res = bf_malloc2(size);
    pthread_mutex_unlock(&lock);
    return res;
}
void ts_free_lock(void*ptr){
    pthread_mutex_lock(&lock);
    bf_free2(ptr);
    pthread_mutex_unlock(&lock);
}





















