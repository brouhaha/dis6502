
/* 
 * 
 *  dis [-p predefineds] file
 *
 *  The -p option may be repeated.
 */

#include "dis.h"

char *predef[NPREDEF];
int  npredef = 0;
char *file;
char *progname = "dis";
int  bopt = UNKNOWN;
int base_address = 0;
int vector_address = 0x10000;
int asmout = 0;

void usage (void)
{
  fprintf (stderr, "Usage: %s <format> <options> <object-file>\n"
	           "  format:   -r <address>   raw binary file\n"
	           "            -b             Atari boot format\n"
                   "            -l             Atari load format\n"
	           "            -c             Commodore 64\n"
                   "  options:  -a             assembly output\n"
                   "            -p <file>      predefs\n"
	           "            -v <address>   alternate vector address",
	   progname);
  exit (1);
}

initopts(argc,argv)
int argc;
char *argv[];
{
  int ai;
  char *ca;
  char *p;
  int fileset = 0;
  
  progname = argv[0];

  while (--argc) 
    {
      if ((*++argv)[0] == '-') 
	{
	  ca = *argv;
	  for(ai = 1; ca[ai] != '\0'; ai++)
	    switch (ca[ai]) 
	      {
	      case 'a':
		asmout = 1;
		break;
	      case 'p':
		predef[npredef] = *++argv;
		npredef++;
		argc--;
		break;
	      case 'r':
		base_address = strtoul (*++argv, &p, 0);
		if (*p)
		  crash ("base address must be specified");
		bopt = RAW_BINARY;
		argc--;
		break;
	      case 'v':
		vector_address = strtoul (*++argv, &p, 0);
		if (*p)
		  crash ("address required");
		argc--;
		break;
	      case 'l':
		bopt = ATARI_LOAD;
		break;
	      case 'c':
		bopt = C64_LOAD;
		break;
	      case 'b':
		bopt = ATARI_BOOT;
		break;
	      default: crash("Invalid option letter");
	      }
	}
      else if (!fileset)
	{
	  file = *argv;
	  fileset++;
	} 
      else usage ();
    }
  if (!fileset)
    usage ();
}
