#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

typedef uint16_t DataType;

typedef struct{
      uint8_t         buff[64];
      uint16_t        len;
      DataType        key;
}Frame_Elem;

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
bool delElemFromEnd( link * head, void **data );
bool isEmptyStack( link *head );
bool stackPop( link *head, void **data );
bool stackPush( link* head, void* data );
bool isEmptyQueue( link *head );
bool deleteQueueByPriority( link *head, void **data, DataType priorityMax, DataType priorityMin );
bool deleteQueue( link *head, void **data );
bool enterQueue( link* head, void* data );
bool deleteQueueByMsg( link * head, void **data, DataType msg );

#define CriticalRegionEnter             //__disable_irq()
#define CriticalRegionExit              //__enable_irq()

link* initLink( void ){
	CriticalRegionEnter;
	link* head = ( link* ) malloc ( sizeof ( link ) );
	if( head ){
		head->next = NULL;
		CriticalRegionExit;
		return head;
	}
	CriticalRegionExit;
	return NULL;
}

bool insertElem( link* head, void* data, uint16_t index ){
	CriticalRegionEnter;
	link * handle = head;
	for( int16_t i = 1; i < index; i++ ){
		if( handle->next == NULL ){
			CriticalRegionExit;
			return false;
		}
		handle = handle->next;
	}
	link* node = ( link* ) malloc ( sizeof ( link ) );
	if( node ){
		node->data = data;
		node->next = handle->next;
		handle->next = node;
		CriticalRegionExit;
		return true;
	}
	CriticalRegionExit;
	return false;

}

bool delElemByIndex( link * head, void **data, int16_t index ){
	if( index == -1 ){
		return false;
	}
	CriticalRegionEnter;
	link* handle = head;
	for( int16_t i = 1; i < index; i++ ){
		if( handle->next == NULL ){
			CriticalRegionExit;
			return false;
		}
		handle = handle->next;
	}
	if( handle->next ){
		link* del = handle->next;
		*data = del->data;
		handle->next = handle->next->next;
		free( del );
		CriticalRegionExit;
		return true;
	}
	CriticalRegionExit;
	return false;
}

int16_t selectElem( link* head, DataType key ){
	CriticalRegionEnter;
	link* handle = head;
	int16_t i = 1;
	while( handle->next ){
		handle = handle->next;
		if( ((Frame_Elem *)handle->data)->key == key ){
			CriticalRegionExit;
			return i;
		}
		i++;
	}
	CriticalRegionExit;
	return -1;
}

bool delElemByKey( link * head, void **data, DataType key ){
	CriticalRegionEnter;
	static uint8_t* index = NULL;
	if( delElemByIndex( head, ( void **)(&index), selectElem( head, key ) ) ){
		*data = index;
		CriticalRegionExit;
		return true;
	}
	CriticalRegionExit;
	return false;
}

bool delElemFromEnd( link * head, void **data ){
	CriticalRegionEnter;
	link* handle = head;

	if( handle->next == NULL ){
		CriticalRegionExit;
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
		CriticalRegionExit;
		return true;
	}
	CriticalRegionExit;
	return false;
}

bool isEmpty( link* head ){
	CriticalRegionEnter;
	if( head->next != NULL ){
		CriticalRegionExit;
		return true;
	}
	CriticalRegionExit;
	return false;
}

/***********************************************************************/

bool enterQueue( link* head, void* data ){
	if( insertElem( head, data, 0 ) ){
		return true;
	}
	return false;
}

bool deleteQueue( link *head, void **data ){
	if( delElemFromEnd( head, data ) ){
		return true;
	}
	return false;
}

bool deleteQueueByMsg( link * head, void **data, DataType msg ){
	if( delElemByKey( head, data, msg ) ){
		return true;
	}
	return false;
}

bool deleteQueueByPriority( link *head, void **data, DataType priorityMin, DataType priorityMax ){
	if( priorityMax >= priorityMin ){
		for( int32_t i = priorityMax; i >= priorityMin; i -- ){
			if( delElemByKey( head, data, i ) ){
				return true;
			}
		}
	}else{
		for( int32_t i = priorityMax; i <= priorityMin; i ++ ){
			if( delElemByKey( head, data, i ) ){
				return true;
			}
		}
	}

	return false;
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

bool isEmptyStack( link *head ){
	if( isEmpty( head ) ){
		return true;
	}
	return false;
}

/***********************************************************************/

int main(void)
{
		static Frame_Elem *index;
		link* LINK;
		LINK = initLink();
		index = NULL;

		for( uint8_t i = 0; i < 100; i ++ ){
			index = ( Frame_Elem *)malloc( sizeof(Frame_Elem) );
			if( index ){
				index->key = i;
				if( !insertElem( LINK, index, 0 ) ){
					free( index );
				}
			}
		}

        static DataType temp = 0;
        for( uint8_t i = 50; i <= 70; i ++ ){
            temp = i;
            if( delElemByKey( LINK, (void **)&index, temp ) ){
                free( index );
            }
        }

        for( uint8_t i = 0; i < 100; i ++ ){
			index = NULL;
			if( delElemByIndex( LINK, ( void **)(&index), 0 ) ){
				printf( "delete elem[%d]\r\n", index->key );
				free( index );
			}
        }

        while( true ){

        #if 0

			for( uint8_t i = 0; i < 100; i ++ ){
				index = ( Frame_Elem *)malloc( sizeof(Frame_Elem) );
				if( index ){
					index->key = i;
					if( !insertElem( LINK, index, 0 ) ){
						free( index );
					}
				}
			}

			for( uint8_t i = 0; i < 100; i ++ ){
				index = NULL;
				if( delElemByIndex( LINK, ( void **)(&index), 0 ) ){
					printf( "delete elem[%d]\r\n", index->key );
					free( index );
				}
			}
			
			for( uint8_t i = 0; i < 100; i ++ ){
				index = ( Frame_Elem *)malloc( sizeof(Frame_Elem) );
				if( index ){
					index->key = i;
					if( !insertElem( LINK, index, 0 ) ){
						free( index );
					}
				}
			}
			
			for( uint8_t i = 0; i < 100; i ++ ){
				index = NULL;
				if( delElemFromEnd( LINK, ( void **)(&index) ) ){
					printf( "delete elem[%d]\r\n", index->key );
					free( index );
				}
			}

         #elif 1

			for( uint8_t i = 0; i < 100; i ++ ){
				index = ( Frame_Elem *)malloc( sizeof(Frame_Elem) );
				if( index ){
					index->key = i;
					if( !enterQueue( LINK, index ) ){
						free( index );
					}
				}
			}

			for( uint8_t i = 0; i < 100; i ++ ){
				 index = NULL;
				 if( deleteQueue( LINK, ( void **)(&index) ) ){
					 printf( "delete elem[%d]\r\n", index->key );
					 free( index );
				 }
			}
			 
			for( uint8_t i = 0; i < 100; i ++ ){
				index = ( Frame_Elem *)malloc( sizeof(Frame_Elem) );
				if( index ){
					index->key = i;
					if( !enterQueue( LINK, index ) ){
						free( index );
					}
				}
			}
			
			for( int8_t i = 0; i <= 100; i ++ ){
				 index = NULL;
				 if( deleteQueueByMsg( LINK, ( void **)(&index), i ) ){
					 printf( "delete elem[%d]\r\n", index->key );
					 free( index );
				 }
			}
			 
			for( uint8_t i = 0; i < 100; i ++ ){
				index = ( Frame_Elem *)malloc( sizeof(Frame_Elem) );
				if( index ){
					index->key = i;
					if( !enterQueue( LINK, index ) ){
							free( index );
					}
				}
			}
			
			for( int8_t i = 0; i <= 100; i ++ ){
				index = NULL;
				if( deleteQueueByPriority( LINK, ( void **)(&index), 0, 100 ) ){
					printf( "delete elem[%d]\r\n", index->key );
					free( index );
				}
			}

        #else

			for( uint8_t i = 0; i < 100; i ++ ){
				index = ( Frame_Elem *)malloc( sizeof(Frame_Elem) );
				if( index ){
					index->key = i;
					if( !stackPush( LINK, index ) ){
						free( index );
					}
				}
			}

			for( int8_t i = 100; i >= 0; i -- ){
				index = NULL;
				if( stackPop( LINK, ( void **)(&index) ) ){
					printf( "delete elem[%d]\r\n", index->key );
					free( index );
				}
			}

        #endif


        }
}
