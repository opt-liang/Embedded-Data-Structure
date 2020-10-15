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
    for( uint8_t i = 0; i < 255; i ++ )
    {
        data = NULL;
        data = (DataFrame *)malloc(sizeof(DataFrame));
        if( data )
        {
            data->key = i;
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

    for( uint8_t i = 0; i < 255; i ++ )
    {
        if( deleteQueue( list, (void **)(&data) ) )
        {
            printf("key[%d]\r\n",data->key);
            free(data);
        }
    }
    free( list );

}



