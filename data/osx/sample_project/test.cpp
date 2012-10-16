#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

#define SUNVOX_MAIN
#include "../../headers/sunvox.h"

int main()
{
    if( sv_load_dll() )
	return 1;
    
    int ver = sv_init( 0, 44100, 2, 0 );
    if( ver >= 0 )
    {
	int major = ( ver >> 16 ) & 255;
	int minor1 = ( ver >> 8 ) & 255;
	int minor2 = ( ver ) & 255;
	printf( "SunVox lib version: %d.%d.%d\n", major, minor1, minor2 );
	
	sv_open_slot( 0 );
	
	sv_load( 0, "test.sunvox" );
	sv_volume( 0, 256 );
	
	//Send two events (Note ON):
	sv_send_event( 0, 0, 64, 128, 7, 0, 0 );
	sleep( 1 );
	sv_send_event( 0, 0, 64, 128, 7, 0, 0 );
	sleep( 1 );
	
	sv_play( 0 );
		
	while( 1 )
	{
	    printf( "Line counter: %d\n", sv_get_current_line( 0 ) );
	    sleep( 1 );
	}
	
	sv_stop( 0 );
	
	sv_close_slot( 0 );
	
	sv_deinit();
    }
    else 
    {
	printf( "sv_init() error %d\n", ver );
    }
    
    sv_unload_dll();
    
    return 0;
}
