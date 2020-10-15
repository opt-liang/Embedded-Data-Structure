#ifndef __LIST__H__
#define __LIST__H__
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

#define MAX_QUEUE		1000

#define	HIGH_PRIORITY		0
#define MEDIUM_PRIORITY		1
#define LOW_PRIORITY		2

#define MAX_PRIORITY		HIGH_PRIORITY
#define MIN_PRIORITY		LOW_PRIORITY

typedef uint16_t DataType;

typedef struct DataFrame_t{
	bool duplicate;
	bool confirmFrame;
	uint16_t key;
	uint16_t priority;
	uint8_t	buff[16];
	uint32_t len;
}DataFrame;

typedef struct Link{
    void *data;
    struct Link *next;
}link,queue;

link * initLink( void );
bool insertElem( link * p, void* data, int16_t index );
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
uint16_t getQueueCount( link * head );
uint16_t getStackCount( link * head );
bool enterQueueByFront( link* head, void* data );
#endif
