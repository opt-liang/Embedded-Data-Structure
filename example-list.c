#include<stdio.h>
#include "list.h"

void main( void )
{
    link* list = initLink();
    if( list == NULL )
    {
        printf("initLink failed !\r\n");
    }
    DataFrame *data = NULL;
    for( uint16_t i = 0; i < 1024; i ++ )
    {
        data = NULL;
        data = (DataFrame *)malloc(sizeof(DataFrame));
        if( data )
        {
            data->key = "hello";
            if( enterQueueByRear( list, data ) == false )
            {
                free(data);
            }
        }
        else
        {
            break;
        }
    }

    for( uint16_t i = 0; i < 1024; i ++ )
    {
        //if( deleteQueue( list, (void **)(&data) ) )
        if( deleteQueueByKey( list, (void **)(&data), "hello" ) )
        {
            printf("key[%s] index[%d]\r\n",data->key,i);
            free(data);
        }
    }
    free( list );

}



