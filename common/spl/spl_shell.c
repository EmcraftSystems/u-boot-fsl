// * ===============================================================================
// * File         : spl_shell.c
// * Author       : Wang Hairun
// * Created Date : 07/28/2017 | 17:13:19 PM | Friday,July
// * Description  : new created V0.0.0
// * ===============================================================================
#include <common.h>
#include <spl.h>
#include <malloc.h>

// ------------------------------------------------------------------ 
// Desc:  all macros
// ------------------------------------------------------------------ 
#define Shell_Status			int32_t			
#define UCHAR_MAX		(255)       /*UCHAR_MAX*/
#define USHRT_MAX		(65535)     /*USHRT_MAX*/

#define SHELL_OK				(0)			
#define SHELL_ACCEPT			(-1)	
#define SHELL_INVALID			(-2)
#define SHELL_QUEUE_FULL		(-3)
#define SHELL_BUSY				(-4)	
#define SHELL_IGNORE			(-5)	
#define SHELL_ERROR				(-6)	
#define SHELL_NOT_ENOUGH		(-8)
#define SHELL_PERMISSION_DENIED	(-11)			
#define SHELL_EXIT				(-12)	

#define SHELL_ACCEPT_ID_ALL		(-32)			


#define	SHELL_COMMAND_HISTORY	(5)
#define	SHELL_COMMAND_BUFSIZE	(50)
#define	SHELL_COMMAND_MAXENTRY	(20)
#define	SHELL_COMMAND_MAXLENGTH	(48)
#if( SHELL_COMMAND_MAXLENGTH > SHELL_COMMAND_BUFSIZE )
	#error SHELL_COMMAND_MAXLENGTH too large
#endif
#define	SHELL_HELP_MAXLENGTH	(50)
#define	SHELL_LINE_BUFFER		(90)
#define SHELL_SPACE_LIMIT       (0x40000000)

#define SHELL_TYPE_INTERVAL_TIME (20)
// ------------------------------------------------------------------ 
// Desc: all macro functions
// ------------------------------------------------------------------ 
#define shell_scanf( input, size )	shell_scanf_detail( (input), (size), false )

// ------------------------------------------------------------------ 
// Desc: user defined types
// ------------------------------------------------------------------ 
typedef struct
{
    void (*func)( const char* );
    char command[SHELL_COMMAND_MAXLENGTH];
    char help[SHELL_HELP_MAXLENGTH];
}ShellExternalCommand;

// ------------------------------------------------------------------ 
// Desc: global variables
// ------------------------------------------------------------------ 
const char shell_invalidArguments[] = "** invalid arguments detected.**\n";
static const char prompt[]	= "\nspl-> ";
static ShellExternalCommand	shellCommandEntry[SHELL_COMMAND_MAXENTRY];
static bool	initialized = false;
static bool	enabled = false;
static bool cancelEncountered;
static bool exit_tinyshell = false;
static int mmc_dev_cur = -1;


// ------------------------------------------------------------------ 
// Desc:  prototype of private functions
// ------------------------------------------------------------------ 
static void shellPuts( size_t, bool, const char* );
static Shell_Status shell_puts( const char* s );
static Shell_Status shellRx( unsigned long );
static Shell_Status shell_mainViaTty( void );
static void shellExecCommand( const char* );
static int shellString2Uw( const char* const, const size_t, uint32_t* const );
static int shellGetArgument( const char* const, char* const, size_t* const, char** const );
static Shell_Status shell_scanf_history( char* restrict, const size_t, char* restrict, int );
static Shell_Status shell_nprintf( const size_t n, const char* format, ... );
static int shell_snprintf( char* s, const size_t n, const char* format, ... );
static bool shellGetCtrlC( void );
static void shellSetCtrlC( void );
static void shellClearCtrlC( void );
static Shell_Status shellInput( uint8_t* c, uint16_t waitTime );
static uint64_t usec_to_tick(unsigned long usec);


// ------------------------------------------------------------------ 
// Desc: tiny shell internal commands
// ------------------------------------------------------------------ 
static void shellShowHelp( const char* );
static void shellDumpMemory( const char* );
static void shellDumpMemory4( const char* );
static void shellEditMemory( const char* );
static void shellDumpMemory_sub( const size_t, const uint32_t* const );
static bool shellDumpMemory_sub4( const size_t, const uint32_t* const );
static void shellEditMemory_sub( const size_t, const uint32_t* const );
static void shellRegister4( const char* );
static void shellRegister2( const char* );
static void shellRegister1( const char* );
#ifdef CONFIG_SPL_KERMIT_SUPPORT
static void shellloadb( const char* );
static void shellgo( const char* );
#endif
static void shellexit( const char* );
#ifdef CONFIG_SPL_DDRSCAN_SUPPORT
static void shellscan( const char* );
#endif
#ifdef CONFIG_SPL_UT_GPT_INT
static void test_gpt_i( const char* arguments );
#endif
#ifdef CONFIG_SPL_UT_LPDDR4_RETENTION
static void test_lpddr4_retention( const char* arguments );
#endif
#ifdef CONFIG_SPL_UT_TZC380
static void test_tzc380( const char* arguments );
#endif
#ifdef CONFIG_SPL_UT_DSM
static void test_dsm( const char* arguments );
#endif
#ifdef CONFIG_SPL_UT_USDHC
static void test_usdhc( const char* arguments );
#endif
#ifdef CONFIG_SPL_MMC_CMD_SUPPORT
static void shell_mmc_dev(const char *args);
static void shell_mmc_info(const char *args);
static void shell_mmc_read(const char *args);
static void shell_mmc_write(const char *args);
static void shell_mmc_erase(const char *args);
#endif
#ifdef CONFIG_SPL_THERM_SUPPORT
static void shell_therm(const char *args);
#endif

// ------------------------------------------------------------------ 
// Desc: prototype of public functions
// ------------------------------------------------------------------ 
Shell_Status spl_shell_init(void);
Shell_Status shell_addCommand( const char* command, const char* help, void (*func)( const char* arguments ) );


/**************************************************************************
 Desc: shell entry
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:20:46 PM | Friday,July
 **************************************************************************/
Shell_Status spl_shell_init(void)
{
								  /* 1		  10		20		  30		40		  50 */
	shell_addCommand( "?",			"show this help.",										shellShowHelp		);
	shell_addCommand( "m",			"modify memory.(arg:[start] [unit(1,2,4)])",			shellEditMemory		);
	shell_addCommand( "d",			"dump memory.(arg:[start] [end])",						shellDumpMemory		);
	shell_addCommand( "d4",			"4byte-boundary dump.(arg:[start] [end])",				shellDumpMemory4	);
	shell_addCommand( "reg4",		"read/write 4byte register.(arg:<address> [value])",	shellRegister4		);
	shell_addCommand( "reg2",		"read/write 2byte register.(arg:<address> [value])",	shellRegister2		);
	shell_addCommand( "reg1",		"read/write 1byte register.(arg:<address> [value])",	shellRegister1		);
#ifdef CONFIG_SPL_KERMIT_SUPPORT
	shell_addCommand( "loadb",		"load binary by kermit protocol.(arg:loadb <addr>)",	shellloadb  		);
	shell_addCommand( "go",		    "jump to address without return.(arg:go <addr>)",	    shellgo  		    );
#endif
#ifdef CONFIG_SPL_DDRSCAN_SUPPORT
	shell_addCommand( "scan",       "scan ddr.(arg:scan <a1> <a2> <p> <t>)",	            shellscan  		    );
#endif
	shell_addCommand( "exit",		"exit spl tinyshell.(arg:exit)",	                    shellexit  		    );
#ifdef CONFIG_SPL_UT_GPT_INT
    shell_addCommand( "gpt_i",		"test gpt interrupt.(arg:<address> [value])",		    test_gpt_i	        );
#endif
#ifdef CONFIG_SPL_UT_LPDDR4_RETENTION
    shell_addCommand( "lp_ret",		"test lpddr4 retention.(arg:<address> [value])",		test_lpddr4_retention);
#endif
#ifdef CONFIG_SPL_UT_TZC380
    shell_addCommand( "tzc380",		"test tzc380 .(arg:<address> [value])",		            test_tzc380         );
#endif
#ifdef CONFIG_SPL_UT_DSM
    shell_addCommand( "dsm",		"test tzc380 .(arg:<address> [value])",		            test_dsm            );
#endif
#ifdef CONFIG_SPL_UT_USDHC
    shell_addCommand( "usdhc",		"test usdhc .(arg:<address> [value])",		            test_usdhc          );
#endif

#ifdef CONFIG_SPL_MMC_CMD_SUPPORT
	shell_addCommand("mmc.dev",	"show or set current mmc device",	shell_mmc_dev);
	shell_addCommand("mmc.info",	"display info of the cur MMC device",	shell_mmc_info);
	shell_addCommand("mmc.read",	"read block (arg: [blk#])",		shell_mmc_read);
	shell_addCommand("mmc.write",	"write block (arg: [blk#] [ival])",	shell_mmc_write);
	shell_addCommand("mmc.erase",	"erase block (arg: [blk#])",		shell_mmc_erase);
#endif
#ifdef CONFIG_SPL_THERM_SUPPORT
	shell_addCommand("therm",	"get temperature",			shell_therm);
#endif

	initialized = true;

    if(shellRx(0) == SHELL_EXIT)
        return (SHELL_EXIT);

	return( SHELL_OK );
}

/**************************************************************************
 Desc: get input command and execute
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:21:13 PM | Friday,July
 **************************************************************************/
static Shell_Status shell_mainViaTty( void )
{
    static const char log[] = "tinyshell execute following command:\n";
    static char	command[sizeof(log)-1+SHELL_COMMAND_BUFSIZE];
    static char	history[SHELL_COMMAND_BUFSIZE*SHELL_COMMAND_HISTORY];

    shell_puts( prompt );
    Shell_Status result = shell_scanf_history( &command[sizeof(log)-1], SHELL_COMMAND_BUFSIZE, history, SHELL_COMMAND_HISTORY );

    if( ( SHELL_OK == result ) && ( '\0' != command[sizeof(log)-1] ) )
    {
        shell_puts( "\n" );

        memcpy( command, log, sizeof(log)-1 );

        shellExecCommand( &command[sizeof(log)-1] );
    }
    else if (result == (SHELL_EXIT))
        return result;

    return (SHELL_OK);
}

/**************************************************************************
 Desc: add user defined command
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:22:06 PM | Friday,July
 **************************************************************************/
Shell_Status shell_addCommand( const char* command, const char* help, void (*func)( const char* arguments ) )
{
	size_t	entry	= 0;		
	size_t	current	= 0;		
	char*	target	= NULL;
	ShellExternalCommand*	empty = NULL;

	for( entry=0; entry<SHELL_COMMAND_MAXENTRY; entry++ )
	{
		if( NULL == shellCommandEntry[entry].func )
		{
			if( NULL == empty )
			{
				empty = &shellCommandEntry[entry];
			}

			continue;
		}

		target = shellCommandEntry[entry].command;

		for( current=0; current<SHELL_COMMAND_MAXLENGTH; current++ )
		{
			if( ( '\0' == target[current]  ) &&
				( '\0' == command[current] ) )
			{
				return( SHELL_IGNORE );
			}

			if( command[current] != target[current] )
			{
				break;
			}
		}
	}

	if( NULL == empty )
	{
		return( SHELL_QUEUE_FULL );
	}

	if( NULL == help )
	{
		empty->help[0] = '\0';
	}
	else
	{
		memcpy( empty->help, help, SHELL_HELP_MAXLENGTH-1 );
		empty->help[SHELL_HELP_MAXLENGTH-1] = '\0';
	}
	memcpy( empty->command, command, sizeof( empty->command ) );
	empty->func = func;

	return( SHELL_OK );
}

/**************************************************************************
 Desc: get parameters from input
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:22:25 PM | Friday,July
 **************************************************************************/
void shell_getArgumentList( const char* const command,
							const size_t maxNumberOfArguments,
							uint32_t* argumentList,
							size_t* numberOfArguments )
{
	char   argument[SHELL_COMMAND_BUFSIZE];
	size_t argumentLength;				
	size_t currentArgument;				
	char*  nextCandidature = NULL;		

	if( NULL == argumentList )
	{
		return;	
	}

	if( NULL == numberOfArguments )
	{
		return;
	}

	nextCandidature    = (char*)command;
	*numberOfArguments = 0;
	memset( argument, 0, sizeof(argument) );

	for( currentArgument=0; currentArgument<maxNumberOfArguments; currentArgument++ )
	{
		if( SHELL_OK != shellGetArgument( nextCandidature, argument, &argumentLength, &nextCandidature ) || 
			SHELL_OK != shellString2Uw( argument, argumentLength, argumentList )						 )	
		{
			break;
		}

		argumentList++;
		(*numberOfArguments)++;
	}
}

/**************************************************************************
 Desc: receive input characters
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:22:50 PM | Friday,July
 **************************************************************************/
Shell_Status shell_scanf_detail( char* input, const size_t size, const bool blind )
{
	uint8_t	c = '\0';
	size_t	position = 0;
	bool	extend	 = false;
    Shell_Status res;

	if( !initialized )
	{
		return( SHELL_IGNORE );
	}

	if( !enabled )
	{
		return( SHELL_IGNORE );
	}

	if( NULL == input )
	{
		
		return( SHELL_INVALID );
	}

	if( 0 == size )
	{
		
		return( SHELL_INVALID );
	}

	for(;;)
	{
        res = shellInput( &c, SHELL_TYPE_INTERVAL_TIME );
        if(res == SHELL_EXIT)
            return res;
        else if(res != SHELL_OK)
            continue;

		switch( c )
		{
			case 0x03:	/* EX(^C) */
				shell_puts( "^C\n" );
				input[position]='\0';
				return( SHELL_IGNORE );

			case 0x1A:	/* SB(^Z) */
				shell_puts( "^Z\n" );
				input[position]='\0';
				return( SHELL_IGNORE );

			case 0x00:
			case 0x01:	/* SH(^A) */
			case 0x02:	/* SX(^B) */
			case 0x04:	/* ET(^D) */
			case 0x05:	/* EQ(^E) */
			case 0x06:	/* AK(^F) */
			case 0x07:	/* BL(^G) */
			case 0x09:	/* HT(TAB)(^I) */
			case 0x0B:	/* HM(^K) */
			case 0x0C:	/* CL(^L) */
			case '\n':	/* CR(^M) */
			case 0x0E:	/* SO(^N) */
			case 0x0F:	/* SI(^O) */
			case 0x10:	/* DE(^P) */
			case 0x11:	/* D1(^Q) */
			case 0x12:	/* D2(^R) */
			case 0x13:	/* D3(^S) */
			case 0x14:	/* D4(^T) */
			case 0x15:	/* NK(^U) */
			case 0x16:	/* SN(^V) */
			case 0x17:	/* EB(^W) */
			case 0x18:	/* CN(^X) */
			case 0x19:	/* EM(^Y) */
			case 0x1C:	/* ¨ */
			case 0x1D:	/* © */
			case 0x1E:	/* ª */
			case 0x1F:	/* « */
				break;

            case 0x7F:
			case '\b':	/* BS(^H) */
				if( position > 0 )
				{
					shell_puts( "\b \b" );
					position--;
				}
				break;

			case '\r':	/* LF(^J) */
				input[position]='\0';
				return( SHELL_OK );

			case 0x1B:	/* ESC */
				extend = true;	
				break;

			case '~':	
			case 0x41:	
			case 0x42:	
			case 0x43:	
			case 0x44:	
			case 0x50:	
			case 0x51:	
			case 0x52:	
			case 0x53:	
				if( !extend )
				{
					if( size-1 > position )
					{
						shell_nprintf( 2, "%c", blind ? '*' : c );
						input[position]=c;
						position++;
					}
				}
				extend = false;	
				break;

			default:
				if( !extend )	
				{
					if( size-1 > position )
					{
						shell_nprintf( 2, "%c", blind ? '*' : c );
						input[position]=c;
						position++;
					}
				}
				break;
		}
	}
}

/**************************************************************************
 Desc: receive input characters
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:23:33 PM | Friday,July
 **************************************************************************/
static Shell_Status shell_scanf_history( char*		   input, const size_t size, char*			history, int numberOfHistories )
{
#define SHELL_VT100_WINDOW_WIDTH	(80)
#define SHELL_PROMPT_LENGTH			(sizeof(prompt)-2)

    Shell_Status res;
    uint8_t	c = '\0';
    size_t	position = 0;
    bool	extend	 = false;
    int		currentCommand = -1;
    char	eraser[20];			

    if( !initialized )
    {
        return( SHELL_IGNORE );
    }

    if( !enabled )
    {
        return( SHELL_IGNORE );
    }

    if( NULL == input )
    {

        return( SHELL_INVALID );
    }

    if( 0 == size )
    {

        return( SHELL_INVALID );
    }

    if( NULL == history )
    {

        return( SHELL_INVALID );
    }

    if( 0 == numberOfHistories )
    {

        return( SHELL_INVALID );
    }

    shell_puts( "\x1b[s" );	
    strncpy( eraser, "\x1b[u\x1b[0J", sizeof(eraser) );

    for(;;)
    {
        res = shellInput( &c, SHELL_TYPE_INTERVAL_TIME );
        if(res == SHELL_EXIT)
            return res;
        else if(res != SHELL_OK)
            continue;

        switch( c )
        {
        case 0x00:
        case 0x01:	/* SH(^A) */
        case 0x02:	/* SX(^B) */
        case 0x03:	/* EX(^C) */
        case 0x04:	/* ET(^D) */
        case 0x05:	/* EQ(^E) */
        case 0x06:	/* AK(^F) */
        case 0x07:	/* BL(^G) */
        case 0x09:	/* HT(TAB)(^I) */
        case 0x0B:	/* HM(^K) */
        case 0x0C:	/* CL(^L) */
        case '\n':	/* CR(^M) */
            break;

        case 0x0E:	/* SO(^N) */
            if( currentCommand > 0 )
            {
                shell_puts( eraser );
                currentCommand--;
                memcpy( input, &history[currentCommand*size], size );
                shell_puts( input );	
                position = strlen(input);

                if( 0 == (position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH )
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uG\x1b[0J",SHELL_PROMPT_LENGTH+1 );
                }
                else
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uA\x1b[%uG\x1b[0J",(position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH,SHELL_PROMPT_LENGTH+1 );	/* ´_Í1,1ÈÌÅ+1·é */
                }
            }
            break;

        case 0x0F:	/* SI(^O) */
            break;

        case 0x10:	/* DE(^P) */
            if( ( currentCommand < numberOfHistories	   ) &&
                ( '\0' != history[(currentCommand+1)*size] ) )
            {
                shell_puts( eraser );
                currentCommand++;
                memcpy( input, &history[currentCommand*size], size );
                shell_puts( input );
                position = strlen(input);

                if( 0 == (position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH )
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uG\x1b[0J",SHELL_PROMPT_LENGTH+1 );
                }
                else
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uA\x1b[%uG\x1b[0J",(position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH,SHELL_PROMPT_LENGTH+1 );	/* ´_Í1,1ÈÌÅ+1·é */
                }
            }
            break;

        case 0x11:	/* D1(^Q) */
        case 0x12:	/* D2(^R) */
        case 0x13:	/* D3(^S) */
        case 0x14:	/* D4(^T) */
        case 0x15:	/* NK(^U) */
        case 0x16:	/* SN(^V) */
        case 0x17:	/* EB(^W) */
        case 0x18:	/* CN(^X) */
        case 0x19:	/* EM(^Y) */
        case 0x1A:	/* SB(^Z) */
        case 0x1C:	/* ¨ */
        case 0x1D:	/* © */
        case 0x1E:	/* ª */
        case 0x1F:	/* « */
            break;

        case 0x7F:
        case '\b':	/* BS(^H) */
            if( position > 0 )
            {
                shell_puts( "\b \b" );
                position--;
            }
            break;

        case '\r':	/* LF(^J) */
            input[position]='\0';
            if( position > 0 )
            {
                memmove( &history[size], history, size*(numberOfHistories-1) );
                memcpy( history, input, size );
            }
            return( SHELL_OK );

        case 0x1B:	/* ESC */
            extend = true;
            break;

        case '~':	
        case 0x43:	
        case 0x44:	
        case 0x50:	
        case 0x51:	
        case 0x52:	
        case 0x53:	
            if( !extend )	
            {
                if( size-1 > position )
                {
                    shell_nprintf( 2, "%c",c );
                    input[position]=c;
                    position++;
                }
            }
            extend = false;	
            break;

        case 0x41:	
            if( !extend )	
            {
                if( size-1 > position )
                {
                    shell_nprintf( 2, "%c",c );
                    input[position]=c;
                    position++;
                }
            }
            else if( ( currentCommand < numberOfHistories		) &&
                     ( '\0' != history[(currentCommand+1)*size] ) )
            {
                shell_puts( eraser );
                currentCommand++;
                memcpy( input, &history[currentCommand*size], size );
                shell_puts( input );
                position = strlen(input);

                if( 0 == (position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH )
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uG\x1b[0J",SHELL_PROMPT_LENGTH+1 );
                }
                else
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uA\x1b[%uG\x1b[0J",(position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH,SHELL_PROMPT_LENGTH+1 );	/* ´_Í1,1ÈÌÅ+1·é */
                }
            }
            extend = false;
            break;

        case 0x42:	
            if( !extend )	
            {
                if( size-1 > position )
                {
                    shell_nprintf( 2, "%c",c );
                    input[position]=c;
                    position++;
                }
            }
            else if( currentCommand > 0 )
            {
                shell_puts( eraser );
                currentCommand--;
                memcpy( input, &history[currentCommand*size], size );
                shell_puts( input );
                position = strlen(input);

                if( 0 == (position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH )
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uG\x1b[0J",SHELL_PROMPT_LENGTH+1 );
                }
                else
                {
                    shell_snprintf( eraser,sizeof(eraser),"\x1b[%uA\x1b[%uG\x1b[0J",(position+SHELL_PROMPT_LENGTH-1)/SHELL_VT100_WINDOW_WIDTH,SHELL_PROMPT_LENGTH+1 );	/* ´_Í1,1ÈÌÅ+1·é */
                }
            }
            extend = false;
            break;

        default:
            if( !extend )
            {
                if( size-1 > position )
                {
                    shell_nprintf( 2, "%c",c );
                    input[position]=c;
                    position++;
                }
            }
            break;
        }
    }
}

/**************************************************************************
 Desc: puts a string
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:23:49 PM | Friday,July
 **************************************************************************/
Shell_Status shell_puts( const char* s )
{
	char*	buf = NULL;

	size_t	n = strlen(s)+1;

	if( !initialized )
	{
		return( SHELL_IGNORE );
	}

	if( !enabled )
	{
		return( SHELL_IGNORE );
	}

    buf = (char*)malloc(n);

	memcpy( buf, s, n );
	shellPuts( n, true, (const char*)buf );
    free(buf);

	return( SHELL_OK );
}

/**************************************************************************
 Desc: put input characters to a byte pool and echo on the screen
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:24:09 PM | Friday,July
 **************************************************************************/
static Shell_Status shell_nprintf( const size_t n, const char* format, ... )
{
    char *buf = NULL;
    va_list	arguments;

    if( !initialized )
    {
        return( SHELL_IGNORE );
    }

    if( !enabled )
    {
        return( SHELL_IGNORE );
    }

    if( 0 == n )
    {
        return( SHELL_INVALID );
    }

    buf = (char*)malloc(n);

    va_start( arguments, format );
    vsnprintf( buf, n, format, arguments );
    va_end( arguments );

    shellPuts( n, true, (const char*)buf );

    free(buf);
    return( SHELL_OK );
}

/**************************************************************************
 Desc: printf to buf
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:25:08 PM | Friday,July
 **************************************************************************/
static int shell_snprintf( char* 		 s, const size_t n, const char* 		 format, ... )
{
	va_list	arg;
	int result;

	va_start( arg, format );
	result = vsnprintf( s, n, format, arg );
	va_end( arg );

	return result;
}

/**************************************************************************
 Desc: get argument list
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:25:28 PM | Friday,July
 **************************************************************************/
static int shellGetArgument( const char* const command, char* const resultArgument, size_t* const argumentLength, char** const nextCandidature )
{
	size_t commandDigit  = 0;
	size_t argumentDigit = 0;

	if( NULL == command )
	{
		return( SHELL_ERROR );
	}

	if( NULL == resultArgument )
	{
		return( SHELL_ERROR );
	}

	for( ; '\0'!=command[commandDigit]; commandDigit++ )
	{
		if( ' '!=command[commandDigit] )
		{
			break;
		}
	}

	for( ; '\0'!=command[commandDigit]; commandDigit++ )
	{
		if( ' '==command[commandDigit] )
		{
			break;
		}

		resultArgument[argumentDigit] = command[commandDigit];
		argumentDigit++;
	}

	resultArgument[argumentDigit] = '\0';

	if( NULL != argumentLength )
	{
		*argumentLength  = argumentDigit;
	}

	if( NULL != nextCandidature )
	{
		*nextCandidature = (char*)command+commandDigit;
	}

	return( SHELL_OK );
}

/**************************************************************************
 Desc: parse arguments
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:25:48 PM | Friday,July
 **************************************************************************/
static int shellString2Uw( const char* const string, const size_t length, uint32_t* const resultInt )
{
	typedef enum
	{
		HEX=16,	/* 16*/
		DEC=10,	/* 10*/
		OCT=8,	/* 8*/
		BIN=2	/* 2*/
	}Base;

	Base base = DEC;	
	bool sign = false;	

	size_t digit   = 0;	
	int    result  = 0;
	int    current = 0;

	if( NULL == string )
	{
		return( SHELL_ERROR );	
	}

	if( 0 == length )
	{
		return( SHELL_ERROR );	
	}

	if( NULL == resultInt )
	{
		return( SHELL_ERROR );	
	}

	if( '@' == string[0] )
	{
		switch( string[1] )
		{
			case 'd':	base = DEC;	digit = 2;	break;
			case 'o':	base = OCT;	digit = 2;	break;
			case 'b':	base = BIN;	digit = 2;	break;
			default :	return( SHELL_ERROR );
		}
	}
	else if( ( '0' == string[0] ) &&
			 ( 'x' == string[1] ) )
	{
		base = HEX;
		digit = 2;
	}

	for( ; digit<length; digit++ )
	{
		switch( string[digit] )
		{
			case '0': current=0; break;
			case '1': current=1; break;
			case '2': current=2; break;
			case '3': current=3; break;
			case '4': current=4; break;
			case '5': current=5; break;
			case '6': current=6; break;
			case '7': current=7; break;
			case '8': current=8; break;
			case '9': current=9; break;
			case 'A':
			case 'a': current=10; break;
			case 'B':
			case 'b': current=11; break;
			case 'C':
			case 'c': current=12; break;
			case 'D':
			case 'd': current=13; break;
			case 'E':
			case 'e': current=14; break;
			case 'F':
			case 'f': current=15; break;
			case '-': sign=true;  continue;

			default : return( SHELL_ERROR );
		}

		if( ( BIN == base	 ) &&
			( 2   <= current ) )
		{
			return( SHELL_ERROR );	
		}

		if( ( OCT == base	 ) &&
			( 8   <= current ) )
		{
			return( SHELL_ERROR );	
		}

		if( ( DEC == base	 ) &&
			( 10  <= current ) )
		{
			return( SHELL_ERROR );	
		}

		result*=base;
		result+=current;
	}

	*resultInt = ( sign && ( 0 < result ) ) ? (0xFFFFFFFF-(result-1)) : result;

	return( SHELL_OK );
}

/**************************************************************************
 Desc: execute command 
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:26:10 PM | Friday,July
 **************************************************************************/
static void shellExecCommand( const char* command )
{
	static const char commandNotFound[] = " command not found. \n";	

	size_t	entry	= 0;	
	size_t	current = 0;	
	char*	target	= NULL;

	enum{
		MISMATCH,			
		HAVE_ARGUMENT,		
		NO_ARGUMENT			
	}matching = MISMATCH;


	for( entry=0; entry<SHELL_COMMAND_MAXENTRY; entry++ )
	{
		target = shellCommandEntry[entry].command;	

		for( current=0; current<SHELL_COMMAND_MAXLENGTH; current++ )
		{
			if( '\0' == target[current] )
			{
				switch( command[current] )
				{
					case ' ':	
						matching = HAVE_ARGUMENT;
						break;

					case '\0':	
						matching = NO_ARGUMENT;
						break;

					default:	
						;
				}

				break;
			}

			if( command[current] != target[current] )
			{
				break;	
			}
		}

		if( MISMATCH != matching )
		{
			break;
		}
	}

	if( MISMATCH == matching )
	{
		shell_puts( commandNotFound );
		return;
	}

	if( NULL == shellCommandEntry[entry].func )
	{
		shell_puts( commandNotFound );
		return;
	}

	(shellCommandEntry[entry].func)( ( NO_ARGUMENT == matching ) ? NULL : &command[current+1] );
}

/**************************************************************************
 Desc: shell rx entry
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:26:26 PM | Friday,July
 **************************************************************************/
static Shell_Status shellRx( unsigned long arguments )
{
	enabled = true;

	shell_puts( "\n===========================================================\n" );
	shell_puts( "\tWelcome to spl tinyshell.\n" );
	shell_puts( "\n===========================================================\n" );

	for(;;)
	{
		if(shell_mainViaTty() == SHELL_EXIT)
            return (SHELL_EXIT);
	}
}

/**************************************************************************
 Desc: put characters by writing tx fifo
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:26:43 PM | Friday,July
 **************************************************************************/
static void shellPuts( size_t size, bool useBytePool, const char* string )
{
	if( 0 == size )
	{
		return;
	}

    puts(string);
}

static void shellShowHelp( const char* arguments )
{
	size_t entry;

	shell_puts( "==tiny shell commands==\n" );

	for( entry=0; entry<SHELL_COMMAND_MAXENTRY; entry++ )
	{
		if( NULL != shellCommandEntry[entry].func )
		{
			shell_nprintf( 80," %-24s: %s\n",shellCommandEntry[entry].command, shellCommandEntry[entry].help );
		}
	}
}

/**************************************************************************
 Desc: dump memory  1byte
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:27:14 PM | Friday,July
 **************************************************************************/
static void shellDumpMemory( const char* arguments )
{
	uint32_t	argumentList[2];
	size_t		numberOfArguments = 0;

	shell_getArgumentList( arguments, 2, argumentList, &numberOfArguments );

	shellDumpMemory_sub( numberOfArguments, argumentList );
}

/**************************************************************************
 Desc: dump memory  4bytes
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:27:26 PM | Friday,July
 **************************************************************************/
static void shellDumpMemory4( const char* arguments )
{
	uint32_t		argumentList[2];
	size_t	numberOfArguments = 0;

	shell_getArgumentList( arguments, 2, argumentList, &numberOfArguments );

	shellDumpMemory_sub4( numberOfArguments, argumentList );
}

/**************************************************************************
 Desc: edit memory
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:28:14 PM | Friday,July
 **************************************************************************/
static void shellEditMemory( const char* arguments )
{
	uint32_t		argumentList[2];
	size_t	numberOfArguments = 0;

	shell_getArgumentList( arguments, 2, argumentList, &numberOfArguments );

	shellEditMemory_sub( numberOfArguments, argumentList );
}

/**************************************************************************
 Desc: sub function of dump memory 1byte
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:28:25 PM | Friday,July
 **************************************************************************/
static void shellDumpMemory_sub( const size_t numberOfArguments, const uint32_t* const argumentList )
{
	static uint32_t startAddress	 = 0;
	static uint32_t endAddress	 = 0xFF;
	volatile char* currentAddress = 0;
	char	result[SHELL_LINE_BUFFER];
	char*	current;
	int		offset=0;

	enum
	{
		START_ADDRESS,
		END_ADDRESS
	};

	memset( result, 0, sizeof(result) );


	switch( numberOfArguments )
	{
		case 0:	
			endAddress	 = startAddress+0xFF;	
			break;

		case 1:	
			startAddress = argumentList[START_ADDRESS];
			endAddress	 = startAddress+0xFF;	
			break;

		case 2:	
			if( argumentList[START_ADDRESS] > argumentList[END_ADDRESS] )
			{
				shell_puts( shell_invalidArguments );
				return;
			}
			startAddress = argumentList[START_ADDRESS];
			endAddress	 = argumentList[END_ADDRESS];
			break;

		default:
			shell_puts( "*too many arguments.*\n" );
			return;
	}

	shell_puts( "          + 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n----------------------------------------------------------\n" );

	shellClearCtrlC();	

	for( currentAddress=(volatile char*)(startAddress&(~0xF)); currentAddress<=(volatile char*)endAddress; currentAddress+=16 )
	{
		shell_snprintf( result, 12, "%08p:  ",currentAddress );
		current=result+11;

		for( offset=0; offset<16; offset++ )
		{
			if( ( currentAddress < (volatile char*)startAddress ) ||
				( currentAddress > (volatile char*)endAddress	) )
			{
				shell_snprintf( current, 4, "   " );
			}
			else
			{
				shell_snprintf( current, 4, "%02x ",*(currentAddress)&0xFF );
			}
			current=current+3;
			currentAddress++;
		}

		currentAddress -= offset;
		shell_snprintf( current, 3, " *" );
		current+=2;

		for( offset=0; offset<16; offset++ )
		{
			if( ( currentAddress < (volatile char*)startAddress ) ||
				( currentAddress > (volatile char*)endAddress	) )
			{
				shell_snprintf( current, 2, " " );
			}
			else
			{
				shell_snprintf( current, 2, "%c", ( ' ' <= *(currentAddress) && '~' >= *(currentAddress) ) ? *(currentAddress) : '.' );
			}
			current++;
			currentAddress++;
		}

		currentAddress -= offset;
		shell_snprintf( current, 4, "*\n\0" );
		shell_puts( result );

		if( shellGetCtrlC() )
		{
			shellClearCtrlC();
			shell_puts( "^C\n" );
			break;
		}
	}

	startAddress=endAddress+1;
}

/**************************************************************************
 Desc: sub function of dump memory 4byte
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:28:43 PM | Friday,July
 **************************************************************************/
static bool shellDumpMemory_sub4( const size_t numberOfArguments, const uint32_t* const argumentList )
{
	static uint32_t startAddress = 0;			
	static uint32_t endAddress	 = 0xFF;		
	volatile unsigned int* currentAddress = 0;	
	char	result[SHELL_LINE_BUFFER];
	char*	current;
	int		offset=0;

	enum
	{
		START_ADDRESS,
		END_ADDRESS
	};

	memset( result, 0, sizeof(result) );

	switch( numberOfArguments )
	{
		case 0:	
			endAddress	 = startAddress+0xFF;	
			break;

		case 1:	
			startAddress = argumentList[START_ADDRESS]&(~3);
			endAddress	 = startAddress+0xFF;	
			break;

		case 2:	
			if( argumentList[START_ADDRESS] > argumentList[END_ADDRESS] )
			{
				shell_puts( shell_invalidArguments );
				return( true );
			}
			startAddress = argumentList[START_ADDRESS]&(~3);
			endAddress	 = argumentList[END_ADDRESS]&(~3);
			break;

		default:
			shell_puts( "*too many arguments.*\n" );
			return( true );
	}


	shell_puts( "          + 3 2 1 0  7 6 5 4  B A 9 8  F E D C\n----------------------------------------------\n" );

	shellClearCtrlC();	

	for( currentAddress=(volatile unsigned int*)(startAddress&(~0xF)); currentAddress<=(volatile unsigned int*)endAddress; currentAddress+=4 )
	{
		shell_snprintf( result, 12, "%08p:  ",currentAddress );
		current=result+11;

		for( offset=0; offset<4; offset++ )
		{
			if( ( currentAddress < (volatile unsigned int*)startAddress ) ||
				( currentAddress > (volatile unsigned int*)endAddress	) )
			{
				shell_snprintf( current, 10, "         " );
			}
			else
			{
				shell_snprintf( current, 10, "%08x ",*(currentAddress) );
			}
			current+=9;
			currentAddress++;
		}
		currentAddress -= offset;

		shell_snprintf( current, 3, "\n\0" );
		shell_puts( result );

		if( shellGetCtrlC() )
		{
			shellClearCtrlC();
			shell_puts( "^C\n" );
			return( false );
		}
	}

	startAddress=endAddress+1;

	return( true );
}

/**************************************************************************
 Desc: edit memory sub function
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:29:18 PM | Friday,July
 **************************************************************************/
static void shellEditMemory_sub( const size_t numberOfArguments, const uint32_t* const argumentList )
{
	static	 uint32_t startAddress = 0;		
	volatile uint8_t* currentAddress = 0;
	uint32_t		  unit = 2;			

	enum
	{
		START_ADDRESS,
		UNIT
	};

	switch( numberOfArguments )
	{
		case 2:	
			switch( argumentList[UNIT] )
			{
				case 1:
				case 2:
				case 4:
					unit = argumentList[UNIT];
					break;
			}

		case 1:	
			startAddress = argumentList[START_ADDRESS];
			break;

		case 0:	
			break;

		default:
			shell_puts( shell_invalidArguments );
			return;
	}


	{
		uint32_t modifyData[1];

		char input[SHELL_COMMAND_BUFSIZE];		
		memset( input, 0, sizeof(input) );

		currentAddress=(volatile uint8_t*)((startAddress/unit)*unit);

		for( ;; )
		{
			shell_nprintf( 20,"%08X:  ",currentAddress );

			switch( unit )
			{
				case 1:
					shell_nprintf( 20,"%02x-",*currentAddress );
					break;

				case 2:
					shell_nprintf( 20,"%04x-",*(volatile uint16_t*)currentAddress );
					break;

				case 4:
					shell_nprintf( 20,"%08x-",*(volatile uint32_t*)currentAddress );
					break;
			}

			if( SHELL_OK != shell_scanf( input, sizeof(input) ) )
			{
				return;
			}

			shell_puts("\n");

			if( '\0' == input[0] )
			{
				goto nextAddress;	
			}

			if( ( '\\'== input[0] ) &&
				( '\0'== input[1] ) )
			{
				if( currentAddress >= (volatile uint8_t*)unit )	
				{
					currentAddress-=unit;
				}

				continue;
			}

			if( ( '.' == input[0] ) &&
				( '\0'== input[1] ) )
			{
				break;
			}

			{
				size_t	args = 0;

				shell_getArgumentList( input, 1, modifyData, &args );
				if( 1 != args )
				{
					shell_puts( shell_invalidArguments );
				}
			}

			switch( unit )
			{
				case 1:
					if( UCHAR_MAX < modifyData[0] )
					{
						shell_puts( shell_invalidArguments );
					}
					else
					{
						*currentAddress = modifyData[0];
					}
					break;

				case 2:
					if( USHRT_MAX < modifyData[0] )
					{
						shell_puts( shell_invalidArguments );
					}
					else
					{
						*(volatile uint16_t*)currentAddress = modifyData[0];
					}
					break;

				case 4:
					*(volatile uint32_t*)currentAddress = modifyData[0];
					break;
			}
nextAddress:
			currentAddress+=unit;
		}
	}

	startAddress=(uint32_t)currentAddress;
}

/**************************************************************************
 Desc: register operation with 4 byte
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:29:32 PM | Friday,July
 **************************************************************************/
static void shellRegister4( const char* arguments )
{
	enum
	{
		 ADDRESS
		,VALUE
	};

	uint32_t	argumentList[2];
	size_t		numberOfArguments = 0;

	shell_getArgumentList( arguments, 2, argumentList, &numberOfArguments );

	if( 0 == numberOfArguments )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

	if( ( SHELL_SPACE_LIMIT < argumentList[ADDRESS] ) ||
		( argumentList[ADDRESS] & 3 ) )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

	if( 1 == numberOfArguments )
	{
		shell_nprintf( 40,"0x%08X: 0x%08X\n", argumentList[ADDRESS], *((volatile uint32_t*)argumentList[ADDRESS]) );
	}
	else
	{
		*((volatile uint32_t*)argumentList[ADDRESS]) = argumentList[VALUE];
	}
}

/**************************************************************************
 Desc: register operation with 2 byte
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:30:10 PM | Friday,July
 **************************************************************************/
static void shellRegister2( const char* arguments )
{
	enum
	{
		 ADDRESS
		,VALUE
	};

	uint32_t	argumentList[2];
	size_t		numberOfArguments = 0;

	shell_getArgumentList( arguments, 2, argumentList, &numberOfArguments );

	if( 0 == numberOfArguments )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

	if( ( SHELL_SPACE_LIMIT < argumentList[ADDRESS] ) ||
		( argumentList[ADDRESS] & 1 ) )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

	if( 1 == numberOfArguments )
	{
		shell_nprintf( 40,"0x%08X: 0x%04X\n", argumentList[ADDRESS], *((volatile uint16_t*)argumentList[ADDRESS]) );
	}
	else
	{
		*((volatile uint16_t*)argumentList[ADDRESS]) = (uint16_t)argumentList[VALUE];
	}
}

/**************************************************************************
 Desc: register operation with 1 byte
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:30:27 PM | Friday,July
 **************************************************************************/
static void shellRegister1( const char* arguments )
{
	enum
	{
		 ADDRESS
		,VALUE
	};

	uint32_t	argumentList[2];
	size_t		numberOfArguments = 0;

	shell_getArgumentList( arguments, 2, argumentList, &numberOfArguments );

	if( 0 == numberOfArguments )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

	if( SHELL_SPACE_LIMIT < argumentList[ADDRESS] )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

	if( 1 == numberOfArguments )
	{
		shell_nprintf( 40,"0x%08X: 0x%02X\n", argumentList[ADDRESS], *((volatile uint8_t*)argumentList[ADDRESS]) );
	}
	else
	{
		*((volatile uint8_t*)argumentList[ADDRESS]) = (uint8_t)argumentList[VALUE];
	}
}

#ifdef CONFIG_SPL_KERMIT_SUPPORT
/**************************************************************************
 Desc: load binary by kermit
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:36:05 PM | Friday,July
 **************************************************************************/
static void shellloadb( const char* arguments )
{
	enum
	{
		ADDRESS
	};

	uint32_t	argumentList[1];
	size_t		numberOfArguments = 0;

	shell_getArgumentList( arguments, 1, argumentList, &numberOfArguments );

	if( 0 == numberOfArguments )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

    printf("load binary to address 0x%08X\n",argumentList[ADDRESS]);

    spl_kermit_load_binary(argumentList[ADDRESS]);
}

/**************************************************************************
 Desc:jump to address and run without return
 
 Author: Wang Hairun
 
 date: 07/29/2017 | 09:03:38 AM | Saturday,July
 **************************************************************************/
static void shellgo( const char* arguments )
{
	enum
	{
		ADDRESS
	};

	typedef void __noreturn (*image_entry_noargs_t)(void);
	uint32_t	argumentList[1];
	size_t		numberOfArguments = 0;

	shell_getArgumentList( arguments, 1, argumentList, &numberOfArguments );

	if( 0 == numberOfArguments )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

    printf("jump to address 0x%08X\n",argumentList[ADDRESS]);


	image_entry_noargs_t image_entry =
		(image_entry_noargs_t)argumentList[ADDRESS];

	image_entry();
}
#endif


/**************************************************************************
 Desc:
 
 Author: Wang Hairun
 
 date: 07/29/2017 | 13:01:20 PM | Saturday,July
 **************************************************************************/
static void shellexit( const char* arguments )
{
    exit_tinyshell = true;
}

#ifdef CONFIG_SPL_DDRSCAN_SUPPORT
/**************************************************************************
 Desc:
 
 Author: Wang Hairun
 
 date: 07/29/2017 | 13:16:33 PM | Saturday,July
 **************************************************************************/
static void shellscan( const char* arguments )
{
	enum
	{
		ADDRESS1,
		ADDRESS2,
        PATTEN,
        TIMES
	};

	uint32_t	argumentList[4];
	size_t		numberOfArguments = 0;

	shell_getArgumentList( arguments, 4, argumentList, &numberOfArguments );

	if( numberOfArguments < 4 )
	{
		shell_puts( shell_invalidArguments );
		return;
	}

    if(argumentList[PATTEN] == 0x52ad)
    {
        u32 i;
        while(argumentList[TIMES]--)
        {
            srand(timer_get_us()) ;  
            argumentList[PATTEN] = rand()%0xC0000000;
            dram_scan(argumentList[ADDRESS1],argumentList[ADDRESS2], argumentList[PATTEN], 1);
        }
        return;
    }

    dram_scan(argumentList[ADDRESS1],argumentList[ADDRESS2], argumentList[PATTEN], argumentList[TIMES]);
}
#endif

/**************************************************************************
 Desc:
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:30:43 PM | Friday,July
 **************************************************************************/
static bool shellGetCtrlC( void )
{
	return( cancelEncountered );
}

static void shellSetCtrlC( void )
{
	cancelEncountered = true;
}

/**************************************************************************
 Desc:
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:30:49 PM | Friday,July
 **************************************************************************/
static void shellClearCtrlC( void )
{
	cancelEncountered = false;
}

/**************************************************************************
 Desc:
 
 Author: Wang Hairun
 
 date: 07/29/2017 | 10:46:24 AM | Saturday,July
 **************************************************************************/
static uint64_t usec_to_tick(unsigned long usec)
{
	uint64_t tick = usec;
	tick *= get_tbclk();
	do_div(tick, 1000000);
	return tick;
}

/**************************************************************************
 Desc:
 
 Author: Wang Hairun
 
 date: 07/28/2017 | 17:30:53 PM | Friday,July
 **************************************************************************/
static Shell_Status shellInput( uint8_t* c, uint16_t waitTime )
{
#if 0
    uint64_t tmp;
    uint8_t in_buf[2];
    uint8_t i = 0;
    in_buf[0] = '\0';
    in_buf[1] = '\0';
    Shell_Status res = SHELL_OK;
    i=0;
    do 
    {
        if(tstc())
        {
            *c = getc();
            in_buf[i++] = *c;
            if(i>1)
                i=0;
	        tmp = get_ticks() + usec_to_tick(waitTime*1000);
        }
    }while(get_ticks()<tmp+1);

    if((in_buf[0]=='e')&&(in_buf[1]=='d'))
    {
        res = SHELL_EXIT;
    }

    in_buf[0] = '\0';
    in_buf[1] = '\0';
    return res;
#else

    if(exit_tinyshell)
        return SHELL_EXIT;

    while(!tstc());
    *c = getc();
    return SHELL_OK;
#endif
}

#ifdef CONFIG_SPL_UT_GPT_INT
/**************************************************************************
 Desc: test gpt interrupt
 
 Author: Wang Hairun
 
 date: 09/25/2017 | 17:50:20 PM | Monday,September
 **************************************************************************/
static void test_gpt_i( const char* arguments )
{
    test_gpt_interrupt();
}
#endif

#ifdef CONFIG_SPL_UT_LPDDR4_RETENTION

/**************************************************************************
 Desc: test ddr 2d retention
 
 Author: Wang Hairun
 
 date: 09/30/2017 | 14:43:37 PM | Saturday,September
 **************************************************************************/
static void test_lpddr4_retention( const char* arguments )
{
    if(arguments[0] == 's')
        lpddr4_enter_retention();
    else if(arguments[0] == 'e')
        lpddr4_exit_retention();
    else
    {
        printf("wrong arg\n");
    }
}
#endif

#ifdef CONFIG_SPL_UT_TZC380
/**************************************************************************
 Desc: test tzc380
 
 Author: Wang Hairun
 
 date: 10/12/2017 | 17:15:21 PM | Thursday,October
 **************************************************************************/
static void test_tzc380( const char* arguments )
{
    printf("test tzc380\n");
    test_tzc380_int();
}
#endif


#ifdef CONFIG_SPL_UT_DSM
/**************************************************************************
 Desc: test dsm
 
 Author: Wang Hairun
 
 date: 10/13/2017 | 16:28:09 PM | Friday,October
 **************************************************************************/
static void test_dsm( const char* arguments )
{
    printf("test dsm\n");
    enter_dsm();
}
#endif

#ifdef CONFIG_SPL_UT_USDHC
/**************************************************************************
 Desc: test usdhc after retention
 
 Author: Wang Hairun
 
 date: 10/16/2017 | 16:36:05 PM | Monday,October
 **************************************************************************/
static void test_usdhc( const char* arguments )
{
    printf("test usdhc\n");
    test_usdhc_after_lpddr4_retention();
}
#endif

#ifdef CONFIG_SPL_MMC_CMD_SUPPORT
/*
 * MMC test commands
 */

#include <mmc.h>

static struct mmc *init_mmc_device(int dev, bool force_init)
{
	struct mmc *mmc;
	int rv;

	rv = mmc_initialize(NULL);
	if (rv) {
		printf("can't init mmc (%d)\n", rv);
		return NULL;
	}

	mmc = find_mmc_device(dev);
	if (!mmc) {
		printf("no mmc device at slot %x\n", dev);
		return NULL;
	}

	if (force_init)
		mmc->has_init = 0;

	if (mmc_init(mmc))
		return NULL;

	return mmc;
}

static void print_mmcinfo(struct mmc *mmc)
{
	int i;

	printf("Device: %s\n", mmc->cfg->name);
	printf("Manufacturer ID: %x\n", mmc->cid[0] >> 24);
	printf("OEM: %x\n", (mmc->cid[0] >> 8) & 0xffff);
	printf("Name: %c%c%c%c%c \n", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);

	printf("Bus Speed: %d\n", mmc->clock);
	printf("Mode : %s\n", mmc_mode_name(mmc->selected_mode));
	printf("Rd Block Len: %d\n", mmc->read_bl_len);

	printf("%s version %d.%d", IS_SD(mmc) ? "SD" : "MMC",
			EXTRACT_SDMMC_MAJOR_VERSION(mmc->version),
			EXTRACT_SDMMC_MINOR_VERSION(mmc->version));
	if (EXTRACT_SDMMC_CHANGE_VERSION(mmc->version) != 0)
		printf(".%d", EXTRACT_SDMMC_CHANGE_VERSION(mmc->version));
	printf("\n");

	printf("High Capacity: %s\n", mmc->high_capacity ? "Yes" : "No");
	puts("Capacity: ");
	print_size(mmc->capacity, "\n");

	printf("Bus Width: %d-bit%s\n", mmc->bus_width,
			mmc->ddr_mode ? " DDR" : "");

	puts("Erase Group Size: ");
	print_size(((u64)mmc->erase_grp_size) << 9, "\n");

	if (!IS_SD(mmc) && mmc->version >= MMC_VERSION_4_41) {
		bool has_enh = (mmc->part_support & ENHNCD_SUPPORT) != 0;
		bool usr_enh = has_enh && (mmc->part_attr & EXT_CSD_ENH_USR);

		puts("HC WP Group Size: ");
		print_size(((u64)mmc->hc_wp_grp_size) << 9, "\n");

		puts("User Capacity: ");
		print_size(mmc->capacity_user, usr_enh ? " ENH" : "");
		if (mmc->wr_rel_set & EXT_CSD_WR_DATA_REL_USR)
			puts(" WRREL\n");
		else
			putc('\n');
		if (usr_enh) {
			puts("User Enhanced Start: ");
			print_size(mmc->enh_user_start, "\n");
			puts("User Enhanced Size: ");
			print_size(mmc->enh_user_size, "\n");
		}
		puts("Boot Capacity: ");
		print_size(mmc->capacity_boot, has_enh ? " ENH\n" : "\n");
		puts("RPMB Capacity: ");
		print_size(mmc->capacity_rpmb, has_enh ? " ENH\n" : "\n");

		for (i = 0; i < ARRAY_SIZE(mmc->capacity_gp); i++) {
			bool is_enh = has_enh &&
				(mmc->part_attr & EXT_CSD_ENH_GP(i));
			if (mmc->capacity_gp[i]) {
				printf("GP%i Capacity: ", i+1);
				print_size(mmc->capacity_gp[i],
					   is_enh ? " ENH" : "");
				if (mmc->wr_rel_set & EXT_CSD_WR_DATA_REL_GP(i))
					puts(" WRREL\n");
				else
					putc('\n');
			}
		}
	}
}

static void shell_mmc_dev(const char *args)
{
	size_t arg_num = 0;
	u32 arg_list[1], dev;

	shell_getArgumentList(args, ARRAY_SIZE(arg_list), arg_list, &arg_num);
	if (!arg_num) {
		printf("Current MMC device: %d\n", mmc_dev_cur);
		return;
	}

	dev = arg_list[0];
	if (!init_mmc_device(dev, true)) {
		printf("Failed init MMC%d\n", dev);
		return;
	}

	mmc_dev_cur = dev;
}

static void shell_mmc_info(const char *args)
{
	struct mmc *mmc;

	if (mmc_dev_cur < 0) {
		printf("MMC device isn't selected\n");
		return;
	}

	mmc = init_mmc_device(mmc_dev_cur, false);
	if (!mmc) {
		printf("MMC device init failure\n");
		return;
	}

	print_mmcinfo(mmc);
}

static void shell_mmc_read(const char *args)
{
	struct mmc *mmc;
	size_t arg_num = 0;
	u32 arg_list[1], mmc_buf[128];
	int i, rv;

	shell_getArgumentList(args, ARRAY_SIZE(arg_list), arg_list, &arg_num);
	if (!arg_num) {
		printf("Bad parameters specified\n");
		return;
	}

	if (mmc_dev_cur < 0) {
		printf("MMC device isn't selected\n");
		return;
	}

	mmc = init_mmc_device(mmc_dev_cur, false);
	if (!mmc) {
		printf("MMC device init failure\n");
		return;
	}

	rv = blk_dread(mmc_get_blk_desc(mmc), arg_list[0],
			sizeof(mmc_buf) / 512, mmc_buf);
	if (rv != 1) {
		printf("Read MMC blk %d read fail (%d)\n", arg_list[0], rv);
		return;
	}

	for (i = 0; i < ARRAY_SIZE(mmc_buf); i++)
		printf("%08x%s", mmc_buf[i], (i + 1) % 4 ? " " : "\n");
}

static void shell_mmc_write(const char *args)
{
	struct mmc *mmc;
	size_t arg_num = 0;
	u32 arg_list[2], mmc_buf[128];
	int i, rv;

	shell_getArgumentList(args, ARRAY_SIZE(arg_list), arg_list, &arg_num);
	if (!arg_num) {
		printf("No parameters specified\n");
		return;
	}

	if (mmc_dev_cur < 0) {
		printf("MMC device isn't selected\n");
		return;
	}

	mmc = init_mmc_device(mmc_dev_cur, false);
	if (!mmc) {
		printf("MMC device init failure\n");
		return;
	}

	if (mmc_getwp(mmc) == 1) {
		printf("MMC device is write protected!\n");
		return;
	}

	for (i = 0; i < ARRAY_SIZE(mmc_buf); i++)
		mmc_buf[i] = arg_list[1] + i;

	rv = blk_dwrite(mmc_get_blk_desc(mmc), arg_list[0],
			sizeof(mmc_buf) / 512, mmc_buf);
	if (rv != 1)
		printf("Write MMC blk %d write fail (%d)\n", arg_list[0], rv);
}

static void shell_mmc_erase(const char *args)
{
	struct mmc *mmc;
	size_t arg_num = 0;
	u32 arg_list[1];
	int i, rv;

	shell_getArgumentList(args, ARRAY_SIZE(arg_list), arg_list, &arg_num);
	if (!arg_num) {
		printf("No parameters specified\n");
		return;
	}

	if (mmc_dev_cur < 0) {
		printf("MMC device isn't selected\n");
		return;
	}

	mmc = init_mmc_device(mmc_dev_cur, false);
	if (!mmc) {
		printf("MMC device init failure\n");
		return;
	}

	if (mmc_getwp(mmc) == 1) {
		printf("MMC device is write protected!\n");
		return;
	}

	rv = blk_derase(mmc_get_blk_desc(mmc), arg_list[0], 1);
	if (rv != 1)
		printf("Write MMC blk %d erase fail (%d)\n", arg_list[0], rv);
}

#endif /* CONFIG_SPL_MMC_CMD_SUPPORT */

#ifdef CONFIG_SPL_THERM_SUPPORT

#define TMU_REGS_BASE	0x30260000
#define TMU_SENSOR_ID	0

#define __REG(x)	(*((volatile u32 *)(&(x))))

/*
 * QorIQ TMU Registers
 */
struct qoriq_tmu_site_regs {
#define TRITSR_DATA	0xFF
#define TRITSR_READY	(1 << 31)
	u32 tritsr;		/* Immediate Temperature Site Register */
	u32 tratsr;		/* Average Temperature Site Register */
	u8 res0[0x8];
};

struct qoriq_tmu_regs {
	u32 tmr;		/* Mode Register */
#define TMR_DISABLE	0x0
#define TMR_ME		0x80000000
#define TMR_ALPF	0x0c000000
	u32 tsr;		/* Status Register */
	u32 tmtmir;		/* Temperature measurement interval Register */
#define TMTMIR_DEFAULT	0x0000000f
	u8 res0[0x14];
	u32 tier;		/* Interrupt Enable Register */
#define TIER_DISABLE	0x0
	u32 tidr;		/* Interrupt Detect Register */
	u32 tiscr;		/* Interrupt Site Capture Register */
	u32 ticscr;		/* Interrupt Critical Site Capture Register */
	u8 res1[0x10];
	u32 tmhtcrh;		/* High Temperature Capture Register */
	u32 tmhtcrl;		/* Low Temperature Capture Register */
	u8 res2[0x8];
	u32 tmhtitr;		/* High Temperature Immediate Threshold */
	u32 tmhtatr;		/* High Temperature Average Threshold */
	u32 tmhtactr;	/* High Temperature Average Crit Threshold */
	u8 res3[0x24];
	u32 ttcfgr;		/* Temperature Configuration Register */
	u32 tscfgr;		/* Sensor Configuration Register */
	u8 res4[0x78];
	struct qoriq_tmu_site_regs site[16];
	u8 res5[0x9f8];
	u32 ipbrr0;		/* IP Block Revision Register 0 */
	u32 ipbrr1;		/* IP Block Revision Register 1 */
	u8 res6[0x310];
	u32 ttrxcr[4];		/* Temperature Range 0-3 Control Registers */
};

static void shell_therm(const char *args)
{
	static int tmu_inited;

	volatile struct qoriq_tmu_regs *regs = (void *)TMU_REGS_BASE;
	uint32_t val;
	int i;

	/*
	 * If cmd is running for the first time, then init and calibrate
	 */
	if (!tmu_inited) {
		static uint32_t rng[] = {
			0xb0000, 0xa0026, 0x80048, 0x70061
		};
		static uint32_t clb[] = {
			0x00000000, 0x00000023,
			0x00000001, 0x00000029,
			0x00000002, 0x0000002f,
			0x00000003, 0x00000035,
			0x00000004, 0x0000003d,
			0x00000005, 0x00000043,
			0x00000006, 0x0000004b,
			0x00000007, 0x00000051,
			0x00000008, 0x00000057,
			0x00000009, 0x0000005f,
			0x0000000a, 0x00000067,
			0x0000000b, 0x0000006f,

			0x00010000, 0x0000001b,
			0x00010001, 0x00000023,
			0x00010002, 0x0000002b,
			0x00010003, 0x00000033,
			0x00010004, 0x0000003b,
			0x00010005, 0x00000043,
			0x00010006, 0x0000004b,
			0x00010007, 0x00000055,
			0x00010008, 0x0000005d,
			0x00010009, 0x00000067,
			0x0001000a, 0x00000070,

			0x00020000, 0x00000017,
			0x00020001, 0x00000023,
			0x00020002, 0x0000002d,
			0x00020003, 0x00000037,
			0x00020004, 0x00000041,
			0x00020005, 0x0000004b,
			0x00020006, 0x00000057,
			0x00020007, 0x00000063,
			0x00020008, 0x0000006f,

			0x00030000, 0x00000015,
			0x00030001, 0x00000021,
			0x00030002, 0x0000002d,
			0x00030003, 0x00000039,
			0x00030004, 0x00000045,
			0x00030005, 0x00000053,
			0x00030006, 0x0000005f,
			0x00030007, 0x00000071
		};

		__REG(regs->tier) = TIER_DISABLE;
		__REG(regs->tmtmir) = TMTMIR_DEFAULT;
		__REG(regs->tmr) = TMR_DISABLE;

		for (i = 0; i < ARRAY_SIZE(regs->ttrxcr); i++)
			__REG(regs->ttrxcr[i]) = rng[i];

		for (i = 0; i < ARRAY_SIZE(clb); ) {
			__REG(regs->ttcfgr) = clb[i++];
			__REG(regs->tscfgr) = clb[i++];
		}

		__REG(regs->tmr) = (0x1 << (15 - TMU_SENSOR_ID)) | TMR_ME |
				   TMR_ALPF;
		tmu_inited = 1;
	}

	/*
	 * Get therm values
	 */
	i = 0;
	do {
		val = __REG(regs->site[TMU_SENSOR_ID].tritsr);
	} while (!(val & TRITSR_READY) && (i++ < 1000000));

	if (val & TRITSR_READY)
		printf("T=%3d*C\n", val & TRITSR_DATA);
}

#endif /* CONFIG_SPL_THERM_SUPPORT */
