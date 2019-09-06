#ifndef __LIST__H__
#define __LIST__H__
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

#define MAX_QUEUE		10

#define	HIGH_PRIORITY		0
#define MEDIUM_PRIORITY		1
#define LOW_PRIORITY		2


typedef uint16_t DataType;

typedef struct RadioFrame_t{
	bool duplicate;
	bool confirmFrame;
	uint16_t key;
	uint16_t priority;
	uint8_t	buff[16];
	uint32_t len;
}RadioFrame;

typedef struct Link{
    void *data;
    struct Link *next;
}link,queue;

link * initLink( void );
bool insertElem( link * p, void* data, uint16_t index );
bool delElemByIndex( link * p, void **data, int16_t index );
bool delElemByKey( link * head, void **data, DataType key );
int16_t selectElem( link* p, DataType data );
bool isEmpty( link * p );
bool delElemFromRear( link * head, void **data );
bool isEmptyStack( link *head );
bool stackPop( link *head, void **data );
bool stackPush( link* head, void* data );
bool isEmptyQueue( link *head );
bool deleteQueue( link *head, void **data );
bool enterQueueByRear( link* head, void* data );
bool deleteQueueByKey( link * head, void **data, DataType msg );
bool deleteQueueByPriority( link *head, void **data, DataType priorityMax, DataType priorityMin );
uint16_t getQueueSum( link * head );
uint16_t getStackSum( link * head );
bool enterQueueByFront( link* head, void* data );
#endif

#include "list.h"

#define ENTER_CRITICAL		(__disable_irq())
#define EXIT_CRITICAL		(__enable_irq())

link* initLink( void ){
	ENTER_CRITICAL;
    link* head = ( link* ) malloc ( sizeof ( link ) );
	if( head ){
		head->next = NULL;
		EXIT_CRITICAL;
		return head;
	}
	EXIT_CRITICAL;
	return NULL;
}

bool insertElem( link* head, void* data, uint16_t index ){
	ENTER_CRITICAL;
    link * handle = head;
    for( int16_t i = 1; i < index; i++ ){
        if( handle->next == NULL ){
			EXIT_CRITICAL;
            return false;
        }
        handle = handle->next;
    }
	link* node = ( link* ) malloc ( sizeof ( link ) );
	if( node ){
		node->data = data;
		node->next = handle->next;
		handle->next = node;
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;

}

bool delElemByIndex( link * head, void **data, int16_t index ){
	if( index == -1 ){
		return false;
	}
	ENTER_CRITICAL;
    link* handle = head;
    for( int16_t i = 1; i < index; i++ ){
        if( handle->next == NULL ){
			EXIT_CRITICAL;
            return false;
        }
        handle = handle->next;
    }
	if( handle->next ){
		link* del = handle->next;
		*data = del->data;
		handle->next = handle->next->next;
		free( del );
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

int16_t selectElemByKeyOrPriority( link* head, bool iskey, uint16_t type ){
	ENTER_CRITICAL;
    link* handle = head;
    int16_t i = 1;
    while( handle->next ){
        handle = handle->next;
        if( iskey ? (((RadioFrame *)handle->data)->key == type) : (((RadioFrame *)handle->data)->priority == type) ){
			EXIT_CRITICAL;
            return i;
        }
        i++;
    }
	EXIT_CRITICAL;
    return -1;
}

bool delElemByKeyOrPriority( link * head, bool iskey, void **data, uint16_t type ){
	ENTER_CRITICAL;
	uint8_t* index = NULL;
	if( delElemByIndex( head, ( void **)(&index), selectElemByKeyOrPriority( head, iskey, type ) ) ){
		*data = index;
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

bool delElemFromRear( link * head, void **data ){
	ENTER_CRITICAL;
    link* handle = head;
	
	if( handle->next == NULL ){
		EXIT_CRITICAL;
		return false;
	}
	
    while( true ){
        if( handle->next != NULL && handle->next->next == NULL ){
			break;
        }
        handle = handle->next;
    }
	
	if( handle->next ){
		link* del = handle->next;
		*data = del->data;
		handle->next = handle->next->next;
		free( del );
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

uint16_t getElemSum( link * head ){
	ENTER_CRITICAL;
	uint16_t sum = 0;
    link* handle = head;
	
	if( handle->next == NULL ){
		EXIT_CRITICAL;
		return 0;
	}
	
    while( true ){
		sum ++;
        if( handle->next != NULL && handle->next->next == NULL ){
			break;
        }
        handle = handle->next;
    }
	
	if( handle->next ){
		EXIT_CRITICAL;
		return sum;
	}
	EXIT_CRITICAL;
	return 0;
}

bool isEmpty( link* head ){
	ENTER_CRITICAL;
	if( head->next == NULL ){
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

/***********************************************************************/

bool enterQueueByRear( link* head, void* data ){
	
	ENTER_CRITICAL;
	if( getQueueSum( head ) > MAX_QUEUE ){
		void *data = NULL;
		if( deleteQueueByPriority( head, &data, HIGH_PRIORITY, LOW_PRIORITY ) ){
			free( data );
		}
	}
	EXIT_CRITICAL;
	
	if( insertElem( head, data, 0 ) ){
		return true;
	}
	return false;
}

bool enterQueueByFront( link* head, void* data ){
	ENTER_CRITICAL;
	uint16_t index = getQueueSum( head );
	if( insertElem( head, data, index < MAX_QUEUE ? index : 0 ) ){
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}


bool deleteQueue( link *head, void **data ){
	if( delElemFromRear( head, data ) ){
		return true;
	}
	return false;
}

bool deleteQueueByKey( link * head, void **data, DataType msg ){
	if( delElemByKeyOrPriority( head, true, data, msg ) ){
		return true;
	}
	return false;
}

bool deleteQueueByPriority( link *head, void **data, DataType priorityMin, DataType priorityMax ){
	ENTER_CRITICAL;
	if( priorityMax >= priorityMin ){
		for( int32_t i = priorityMax; i >= priorityMin; i -- ){
			if( delElemByKeyOrPriority( head, false, data, i ) ){
				EXIT_CRITICAL;
				return true;
			}
		}
	}else{
		for( int32_t i = priorityMax; i <= priorityMin; i ++ ){
			if( delElemByKeyOrPriority( head, false, data, i ) ){
				EXIT_CRITICAL;
				return true;
			}
		}
	}
	EXIT_CRITICAL;
	return false;
}

uint16_t getQueueSum( link * head ){
	return getElemSum( head );
}

bool isEmptyQueue( link *head ){
	if( isEmpty( head ) ){
		return true;
	}
	return false;
}

/***********************************************************************/

bool stackPush( link* head, void* data ){
	if( insertElem( head, data, 0 ) ){
		return true;
	}
	return false;
}

bool stackPop( link *head, void **data ){
	if( delElemByIndex( head, data, 0 ) ){
		return true;
	}
	return false;
}

uint16_t getStackSum( link * head ){
	return getElemSum( head );
}

bool isEmptyStack( link *head ){
	if( isEmpty( head ) ){
		return true;
	}
	return false;
}




