#ifndef __SUNVOX_H__
#define __SUNVOX_H__

#define NOTECMD_NOTE_OFF	128
#define NOTECMD_ALL_NOTES_OFF	129 /* notes of all synths off */
#define NOTECMD_CLEAN_SYNTHS	130 /* stop and clean all synths */
#define NOTECMD_STOP		131
#define NOTECMD_PLAY		132

//sv_send_event() parameters:
//  slot;
//  channel_num: from 0 to 7;
//  note: 0 - nothing; 1..127 - note num; 128 - note off; 129, 130... - see NOTECMD_xxx defines;
//  vel: velocity 1..129; 0 - default;
//  module: 0 - nothing; 1..255 - module number;
//  ctl: CCXX. CC - number of controller. XX - std effect;
//  ctl_val: value of controller.

typedef struct sunvox_note
{
    unsigned char	note;           //0 - nothing; 1..127 - note num; 128 - note off; 129, 130... - see NOTECMD_xxx defines
    unsigned char       vel;            //Velocity 1..129; 0 - default
    unsigned char       module;         //0 - nothing; 1..255 - module number
    unsigned char       nothing;
    unsigned short      ctl;            //CCXX. CC - number of controller. XX - std effect
    unsigned short      ctl_val;        //Value of controller
} sunvox_note;

#define SV_INIT_FLAG_NO_DEBUG_OUTPUT 		( 1 << 0 )
#define SV_INIT_FLAG_USER_AUDIO_CALLBACK 	( 1 << 1 )
#define SV_INIT_FLAG_AUDIO_INT16 		( 1 << 2 )
#define SV_INIT_FLAG_AUDIO_FLOAT32 		( 1 << 3 )
#define SV_INIT_FLAG_ONE_THREAD			( 1 << 4 )

#define SV_MODULE_FLAG_EXISTS 1
#define SV_MODULE_FLAG_EFFECT 2
#define SV_MODULE_INPUTS_OFF 16
#define SV_MODULE_INPUTS_MASK ( 255 << SV_MODULE_INPUTS_OFF )
#define SV_MODULE_OUTPUTS_OFF ( 16 + 8 )
#define SV_MODULE_OUTPUTS_MASK ( 255 << SV_MODULE_OUTPUTS_OFF )

#define SV_STYPE_INT16 0
#define SV_STYPE_INT32 1
#define SV_STYPE_FLOAT32 2
#define SV_STYPE_FLOAT64 3

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(__WIN32__)
    #define WIN
    #define LIBNAME "sunvox.dll"
#endif
#if defined(__APPLE__) 
    #define OSX
    #define LIBNAME "sunvox.dylib"
#endif
#if defined(__linux__) || defined(linux)
    #define LINUX
    #define LIBNAME "./sunvox.so"
#endif
#if defined(OSX) || defined(LINUX)
    #define UNIX
#endif

#ifdef WIN
    #define SUNVOX_FN_ATTR __attribute__((stdcall))
#endif
#ifndef SUNVOX_FN_ATTR
    #define SUNVOX_FN_ATTR /**/
#endif

//latency - audio latency (in frames);
//out_time - output time (in ticks);
typedef int (*tsv_audio_callback)( void* buf, int frames, int latency, unsigned int out_time ) SUNVOX_FN_ATTR;
typedef int (*tsv_open_slot)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_close_slot)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_init)( const char* dev, int freq, int channels, int flags ) SUNVOX_FN_ATTR;
typedef int (*tsv_deinit)( void ) SUNVOX_FN_ATTR;
//sv_get_sample_type() - get internal sample type of the SunVox engine. Return value: one of the SV_STYPE_xxx defines.
//Use it to get the scope buffer type from get_module_scope() function.
typedef int (*tsv_get_sample_type)( void ) SUNVOX_FN_ATTR;
typedef int (*tsv_load)( int slot, const char* name ) SUNVOX_FN_ATTR;
typedef int (*tsv_play)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_play_from_beginning)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_stop)( int slot ) SUNVOX_FN_ATTR;
//autostop values: 0 - disable autostop; 1 - enable autostop.
//When disabled, song is playing infinitely in the loop.
typedef int (*tsv_set_autostop)( int slot, int autostop ) SUNVOX_FN_ATTR;
//sv_end_of_song() return values: 0 - song is playing now; 1 - stopped.
typedef int (*tsv_end_of_song)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_rewind)( int slot, int t ) SUNVOX_FN_ATTR;
typedef int (*tsv_volume)( int slot, int vol ) SUNVOX_FN_ATTR;
typedef int (*tsv_send_event)( int slot, int channel_num, int note, int vel, int module, int ctl, int ctl_val ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_current_line)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_current_signal_level)( int slot, int channel ) SUNVOX_FN_ATTR;
typedef const char* (*tsv_get_song_name)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_song_bpm)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_song_tpl)( int slot ) SUNVOX_FN_ATTR;
//Frame is one discrete of the sound. Sampling frequency 44100 Hz means, that you hear 44100 frames per second.
typedef unsigned int (*tsv_get_song_length_frames)( int slot ) SUNVOX_FN_ATTR;
typedef unsigned int (*tsv_get_song_length_lines)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_number_of_modules)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_module_flags)( int slot, int mod_num ) SUNVOX_FN_ATTR;
typedef int* (*tsv_get_module_inputs)( int slot, int mod_num ) SUNVOX_FN_ATTR;
typedef int* (*tsv_get_module_outputs)( int slot, int mod_num ) SUNVOX_FN_ATTR;
typedef const char* (*tsv_get_module_name)( int slot, int mod_num ) SUNVOX_FN_ATTR;
typedef unsigned int (*tsv_get_module_xy)( int slot, int mod_num ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_module_color)( int slot, int mod_num ) SUNVOX_FN_ATTR;
typedef void* (*tsv_get_module_scope)( int slot, int mod_num, int channel, int* offset, int* size ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_number_of_patterns)( int slot ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_pattern_x)( int slot, int pat_num ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_pattern_y)( int slot, int pat_num ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_pattern_tracks)( int slot, int pat_num ) SUNVOX_FN_ATTR;
typedef int (*tsv_get_pattern_lines)( int slot, int pat_num ) SUNVOX_FN_ATTR;
typedef sunvox_note* (*tsv_get_pattern_data)( int slot, int pat_num ) SUNVOX_FN_ATTR;
//SunVox engine uses its own time space, measured in ticks.
//Use sv_get_ticks() to get current tick counter (from 0 to 0xFFFFFFFF).
//Use sv_get_ticks_per_second() to get the number of SunVox ticks per second.
typedef unsigned int (*tsv_get_ticks)( void ) SUNVOX_FN_ATTR;
typedef unsigned int (*tsv_get_ticks_per_second)( void ) SUNVOX_FN_ATTR;

#ifdef SUNVOX_MAIN

#ifdef WIN
#define IMPORT( Handle, Type, Function, Store ) \
    Store = (Type)GetProcAddress( Handle, Function );
#define ERROR_MSG( msg ) MessageBox( 0, TEXT("msg"), TEXT("Error"), MB_OK );
#endif

#ifdef UNIX
#define IMPORT( Handle, Type, Function, Store ) \
    Store = (Type)dlsym( Handle, Function );
#define ERROR_MSG( msg ) printf( "ERROR: %s\n", msg );
#endif

tsv_audio_callback sv_audio_callback = 0;
tsv_open_slot sv_open_slot = 0;
tsv_close_slot sv_close_slot = 0;
tsv_init sv_init = 0;
tsv_deinit sv_deinit = 0;
tsv_get_sample_type sv_get_sample_type = 0;
tsv_load sv_load = 0;
tsv_play sv_play = 0;
tsv_play_from_beginning sv_play_from_beginning = 0;
tsv_stop sv_stop = 0;
tsv_set_autostop sv_set_autostop = 0;
tsv_end_of_song sv_end_of_song = 0;
tsv_rewind sv_rewind = 0;
tsv_volume sv_volume = 0;
tsv_send_event sv_send_event = 0;
tsv_get_current_line sv_get_current_line = 0;
tsv_get_current_signal_level sv_get_current_signal_level = 0;
tsv_get_song_name sv_get_song_name = 0;
tsv_get_song_bpm sv_get_song_bpm = 0;
tsv_get_song_tpl sv_get_song_tpl = 0;
tsv_get_song_length_frames sv_get_song_length_frames = 0;
tsv_get_song_length_lines sv_get_song_length_lines = 0;
tsv_get_number_of_modules sv_get_number_of_modules = 0;
tsv_get_module_flags sv_get_module_flags = 0;
tsv_get_module_inputs sv_get_module_inputs = 0;
tsv_get_module_outputs sv_get_module_outputs = 0;
tsv_get_module_name sv_get_module_name = 0;
tsv_get_module_xy sv_get_module_xy = 0;
tsv_get_module_color sv_get_module_color = 0;
tsv_get_module_scope sv_get_module_scope = 0;
tsv_get_number_of_patterns sv_get_number_of_patterns = 0;
tsv_get_pattern_x sv_get_pattern_x = 0;
tsv_get_pattern_y sv_get_pattern_y = 0;
tsv_get_pattern_tracks sv_get_pattern_tracks = 0;
tsv_get_pattern_lines sv_get_pattern_lines = 0;
tsv_get_pattern_data sv_get_pattern_data = 0;
tsv_get_ticks sv_get_ticks = 0;
tsv_get_ticks_per_second sv_get_ticks_per_second = 0;

#ifdef UNIX
    void* g_sv_dll = 0;
#endif

#ifdef WIN
    HMODULE g_sv_dll = 0;
#endif

int sv_load_dll( void )
{
#ifdef WIN
    g_sv_dll = LoadLibrary( TEXT(LIBNAME) );
    if( g_sv_dll == 0 ) 
    {
        ERROR_MSG( "sunvox.dll not found" );
        return 1;
    }
#endif
#ifdef UNIX
    g_sv_dll = dlopen( LIBNAME, RTLD_NOW );
    if( g_sv_dll == 0 )
    {
	printf( "%s\n", dlerror() );
        return 1;
    }
#endif
    IMPORT( g_sv_dll, tsv_audio_callback, "sv_audio_callback", sv_audio_callback );
    IMPORT( g_sv_dll, tsv_open_slot, "sv_open_slot", sv_open_slot );
    IMPORT( g_sv_dll, tsv_close_slot, "sv_close_slot", sv_close_slot );
    IMPORT( g_sv_dll, tsv_init, "sv_init", sv_init );
    IMPORT( g_sv_dll, tsv_deinit, "sv_deinit", sv_deinit );
    IMPORT( g_sv_dll, tsv_get_sample_type, "sv_get_sample_type", sv_get_sample_type );
    IMPORT( g_sv_dll, tsv_load, "sv_load", sv_load );
    IMPORT( g_sv_dll, tsv_play, "sv_play", sv_play );
    IMPORT( g_sv_dll, tsv_play_from_beginning, "sv_play_from_beginning", sv_play_from_beginning );
    IMPORT( g_sv_dll, tsv_stop, "sv_stop", sv_stop );
    IMPORT( g_sv_dll, tsv_set_autostop, "sv_set_autostop", sv_set_autostop );
    IMPORT( g_sv_dll, tsv_end_of_song, "sv_end_of_song", sv_end_of_song );
    IMPORT( g_sv_dll, tsv_rewind, "sv_rewind", sv_rewind );
    IMPORT( g_sv_dll, tsv_volume, "sv_volume", sv_volume );
    IMPORT( g_sv_dll, tsv_send_event, "sv_send_event", sv_send_event );
    IMPORT( g_sv_dll, tsv_get_current_line, "sv_get_current_line", sv_get_current_line );
    IMPORT( g_sv_dll, tsv_get_current_signal_level, "sv_get_current_signal_level", sv_get_current_signal_level );
    IMPORT( g_sv_dll, tsv_get_song_name, "sv_get_song_name", sv_get_song_name );
    IMPORT( g_sv_dll, tsv_get_song_bpm, "sv_get_song_bpm", sv_get_song_bpm );
    IMPORT( g_sv_dll, tsv_get_song_tpl, "sv_get_song_tpl", sv_get_song_tpl );
    IMPORT( g_sv_dll, tsv_get_song_length_frames, "sv_get_song_length_frames", sv_get_song_length_frames );
    IMPORT( g_sv_dll, tsv_get_song_length_lines, "sv_get_song_length_lines", sv_get_song_length_lines );
    IMPORT( g_sv_dll, tsv_get_number_of_modules, "sv_get_number_of_modules", sv_get_number_of_modules );
    IMPORT( g_sv_dll, tsv_get_module_flags, "sv_get_module_flags", sv_get_module_flags );
    IMPORT( g_sv_dll, tsv_get_module_inputs, "sv_get_module_inputs", sv_get_module_inputs );
    IMPORT( g_sv_dll, tsv_get_module_outputs, "sv_get_module_outputs", sv_get_module_outputs );
    IMPORT( g_sv_dll, tsv_get_module_name, "sv_get_module_name", sv_get_module_name );
    IMPORT( g_sv_dll, tsv_get_module_xy, "sv_get_module_xy", sv_get_module_xy );
    IMPORT( g_sv_dll, tsv_get_module_color, "sv_get_module_color", sv_get_module_color );
    IMPORT( g_sv_dll, tsv_get_module_scope, "sv_get_module_scope", sv_get_module_scope );
    IMPORT( g_sv_dll, tsv_get_number_of_patterns, "sv_get_number_of_patterns", sv_get_number_of_patterns );
    IMPORT( g_sv_dll, tsv_get_pattern_x, "sv_get_pattern_x", sv_get_pattern_x );
    IMPORT( g_sv_dll, tsv_get_pattern_y, "sv_get_pattern_y", sv_get_pattern_y );
    IMPORT( g_sv_dll, tsv_get_pattern_tracks, "sv_get_pattern_tracks", sv_get_pattern_tracks );
    IMPORT( g_sv_dll, tsv_get_pattern_lines, "sv_get_pattern_lines", sv_get_pattern_lines );
    IMPORT( g_sv_dll, tsv_get_pattern_data, "sv_get_pattern_data", sv_get_pattern_data );
    IMPORT( g_sv_dll, tsv_get_ticks, "sv_get_ticks", sv_get_ticks );
    IMPORT( g_sv_dll, tsv_get_ticks_per_second, "sv_get_ticks_per_second", sv_get_ticks_per_second );

    int nf = 0;
    while( 1 )
    {
	if( sv_audio_callback == 0 ) { nf = 1; break; }
	if( sv_open_slot == 0 ) { nf = 2; break; }
	if( sv_close_slot == 0 ) { nf = 3; break; }
	if( sv_init == 0 ) { nf = 4; break; }
	if( sv_deinit == 0 ) { nf = 5; break; }
	if( sv_get_sample_type == 0 ) { nf = 6; break; }
	if( sv_load == 0 ) { nf = 7; break; }
	if( sv_play == 0 ) { nf = 8; break; }
	if( sv_play_from_beginning == 0 ) { nf = 9; break; }
	if( sv_stop == 0 ) { nf = 10; break; }
	if( sv_set_autostop == 0 ) { nf = 11; break; }
	if( sv_end_of_song == 0 ) { nf = 12; break; }
	if( sv_rewind == 0 ) { nf = 13; break; }
	if( sv_volume == 0 ) { nf = 14; break; }
	if( sv_send_event == 0 ) { nf = 15; break; }
	if( sv_get_song_name == 0 ) { nf = 16; break; }
	if( sv_get_song_bpm == 0 ) { nf = 17; break; }
	if( sv_get_song_tpl == 0 ) { nf = 18; break; }
	if( sv_get_song_length_frames == 0 ) { nf = 19; break; }
	if( sv_get_song_length_lines == 0 ) { nf = 20; break; }
	if( sv_get_number_of_modules == 0 ) { nf = 21; break; }
	if( sv_get_module_flags == 0 ) { nf = 22; break; }
	if( sv_get_module_inputs == 0 ) { nf = 23; break; }
	if( sv_get_module_outputs == 0 ) { nf = 24; break; }
	if( sv_get_module_name == 0 ) { nf = 25; break; }
	if( sv_get_module_xy == 0 ) { nf = 26; break; }
	if( sv_get_module_color == 0 ) { nf = 27; break; }
	if( sv_get_module_scope == 0 ) { nf = 28; break; }
	if( sv_get_number_of_patterns == 0 ) { nf = 29; break; }
	if( sv_get_pattern_x == 0 ) { nf = 30; break; }
	if( sv_get_pattern_y == 0 ) { nf = 31; break; }
	if( sv_get_pattern_tracks == 0 ) { nf = 32; break; }
	if( sv_get_pattern_lines == 0 ) { nf = 33; break; }
	if( sv_get_pattern_data == 0 ) { nf = 34; break; }
	if( sv_get_current_line == 0 ) { nf = 35; break; }
	if( sv_get_current_signal_level == 0 ) { nf = 36; break; }
	if( sv_get_ticks == 0 ) { nf = 37; break; }
	if( sv_get_ticks_per_second == 0 ) { nf = 38; break; }
	break;
    }
    if( nf )
    {
	char ts[ 256 ];
	sprintf( ts, "sunvox lib: some functions not found (%d)", nf );
	ERROR_MSG( ts );
	return nf;
    }
    
    return 0;
}

int sv_unload_dll( void )
{
#ifdef UNIX
    if( g_sv_dll ) dlclose( g_sv_dll );
#endif
    return 0;
}

#endif

#endif
