#include <tests.h>
#include <lld_control.h>


void testDriverControlRoutine( void )
{
    lldControlInit( );

    while( true )
    {

       chThdSleepMilliseconds(500);
    }
}
