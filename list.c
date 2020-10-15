#include "list.h"

#define ENTER_CRITICAL		//(__disable_irq())
#define EXIT_CRITICAL		//(__enable_irq())

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

bool insertElem( link* head, void* data, int16_t index ){
	if( index < 0 ){
		return false;
	}
	ENTER_CRITICAL;
    link * handle = head;
    for( int16_t i = 1; i <= index; i++ ){
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
	if( index < 0 ){
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

int16_t selectElemByKey( link* head, uint16_t type ){
	ENTER_CRITICAL;
    link* handle = head;
    int16_t i = 1;
    while( handle->next ){
        handle = handle->next;
        if( ((DataFrame *)handle->data)->key == type ){
			EXIT_CRITICAL;
            return i;
        }
        i++;
    }
	EXIT_CRITICAL;
    return -1;
}

int16_t selectElemByPriority( link* head, uint16_t min_priority, uint16_t max_priority ){
	ENTER_CRITICAL;
	if( head->next == NULL ){
		EXIT_CRITICAL;
		return -1;
	}
	
    link* handle = head;
	int16_t max_priority_index = -1;
	uint16_t current_max_priority;
	int16_t i = 1;
	
	bool stat = true;
	if( max_priority <= min_priority ){
		stat = false;
	}
	
	current_max_priority = ((DataFrame *)handle->next->data)->priority;
	
    while( handle->next ){
        handle = handle->next;
		uint16_t current_priority = ((DataFrame *)handle->data)->priority;
		if( stat ){
			if( current_priority <= max_priority && current_priority >= min_priority ){
				if( current_max_priority <= current_priority ){
					current_max_priority = current_priority;
					max_priority_index = i;
				}
			}
		}else{
			if( current_priority >= max_priority && current_priority <= min_priority ){
				if( current_max_priority >= current_priority ){
					current_max_priority = current_priority;
					max_priority_index = i;
				}
			}
		}
		i ++;
    }
	
	EXIT_CRITICAL;
	
    return max_priority_index;
}

bool delElemByKey( link * head, void **data, uint16_t type ){
	ENTER_CRITICAL;
	uint8_t* handle = NULL;
	if( delElemByIndex( head, ( void **)(&handle), selectElemByKey( head, type ) ) ){
		*data = handle;
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

bool delElemByPriority( link * head, void **data, uint16_t min_priority, uint16_t max_priority ){
	ENTER_CRITICAL;
	uint8_t* handle = NULL;
	if( delElemByIndex( head, ( void **)(&handle), selectElemByPriority( head, min_priority, max_priority ) ) ){
		*data = handle;
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

bool delElemFromFront( link * head, void **data ){
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
	if( getQueueCount( head ) > MAX_QUEUE ){
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
	if( getQueueCount( head ) > MAX_QUEUE ){
		void *data = NULL;
		if( deleteQueueByPriority( head, &data, HIGH_PRIORITY, LOW_PRIORITY ) ){
			free( data );
		}
	}
	EXIT_CRITICAL;
	
	ENTER_CRITICAL;
	uint16_t index = getQueueCount( head );
	if( insertElem( head, data, index < MAX_QUEUE ? index : 0 ) ){
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

bool deleteQueue( link *head, void **data ){
	if( delElemFromFront( head, data ) ){
		return true;
	}
	return false;
}

bool deleteQueueByKey( link * head, void **data, DataType msg ){
	if( delElemByKey( head, data, msg ) ){
		return true;
	}
	return false;
}

bool deleteQueueByPriority( link *head, void **data, DataType priorityMin, DataType priorityMax ){
	ENTER_CRITICAL;
	if( delElemByPriority( head, data, priorityMin, priorityMax ) ){
		EXIT_CRITICAL;
		return true;
	}
	EXIT_CRITICAL;
	return false;
}

uint16_t getQueueCount( link * head ){
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

uint16_t getStackCount( link * head ){
	return getElemSum( head );
}

bool isEmptyStack( link *head ){
	if( isEmpty( head ) ){
		return true;
	}
	return false;
}




