#include<stdio.h>
#include "list.h"
#include <time.h>

// #include <pthread.h>
// pthread_mutex_t mx_concent = PTHREAD_MUTEX_INITIALIZER;
// #define LOCK				pthread_mutex_lock(&mx_concent)
// #define UNLOCK				pthread_mutex_unlock(&mx_concent)

void main( void )
{
    uint64_t timestamp = 0;
    char key[128] = { 0 };
    DataFrame *data = NULL;

    link* list = initLink();

    if( list == NULL )
    {
        printf("initLink failed !\r\n");
        return;
    }

    for( uint16_t i = 0; i < 1024; i ++ )
    {
        data = NULL;
        data = (DataFrame *)malloc(sizeof(DataFrame));
        if( data )
        {
            memset( data->key, 0, sizeof(data->key) );
            sprintf( data->key, "%04x", i );
            if( enterQueueByRear( list, data ) == false )
            {
                free(data);
            }
        }
    }

    for( uint16_t i = 0; i < 1024; i ++ )
    {
        memset( key, 0, sizeof(key) );
        sprintf( key, "%04x", i );
        //if( deleteQueue( list, (void **)(&data) ) )
        if( deleteQueueByKey( list, (void **)(&data), key ) )
        {
            printf("key[%s] index[%d]\r\n",data->key,i);
            free(data);
        }
    }

    for( uint16_t i = 0; i < 1024; i ++ )
    {
        data = NULL;
        data = (DataFrame *)malloc(sizeof(DataFrame));
        if( data )
        {
            time( &data->timestamp );
            memset( data->key, 0, sizeof(data->key) );
            sprintf( data->key, "%04x", i );
            if( enterQueueByRear( list, data ) == false )
            {
                free(data);
            }
        }
    }

    uint32_t queue_count = 0;
    while ( true )
    {
        queue_count = getQueueCount( list );
        if( queue_count == 0 )
        {
            printf( "Queue is empty, exit\r\n" );
            break;
        }
        
        for( int i = 0; i < queue_count; i ++ )
        {
            if( deleteQueue( list, (void **)(&data) ) )
            {
                time( &timestamp );
                if( timestamp > (data->timestamp+1*60) )
                {
                    printf("KEY[%s] Out of queue\r\n", data->key );
                    free(data);
                }
                else
                {
                    if( enterQueueByRear( list, data ) == false )
                    {
                        free(data);
                    }
                }
            }
        }
        printf( "Scanning\r\n" );
        sleep( 1 );
    }
    









    free( list );

}



