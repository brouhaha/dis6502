%{
#undef ECHO
#include <string.h>
#include "dis.h"
int lineno = 0;
char *strcpy();
%}

%option nounput

digit		[0-9]
hexdigit	[0-9a-fA-F]
alpha		[a-zA-Z]
alphanum	[0-9a-zA-Z_]

%%

[ \t]			{ ; }

[\n]			{ lineno++; return '\n'; }

\.[Ee][Qq][Uu]		{ return EQ; }

\.[Ee][Qq]		{ return EQ; }

\.[Ll][Ii]		{ return LI; }

\.[Tt][Rr][Aa][Cc][Ee]	{ return TSTART; }

\.[Ss][Tt][Oo][Pp]	{ return TSTOP; }

\.[Rr][Tt][Ss][Tt][Aa][Bb]	{ return TRTSTAB; }

\.[Jj][Tt][Aa][Bb]2	{ return TJTAB2; }

{digit}+		{
			  (void)sscanf(yytext, "%d", &token.ival); 
			  return NUMBER;
			}

\${hexdigit}+		{
			  (void)sscanf(yytext+1, "%x", &token.ival);
			  return NUMBER;
			}

{alpha}{alphanum}*	{
			  token.sval = emalloc((unsigned) strlen(yytext)+1);
			  (void)strcpy((char *)token.sval, (char *)yytext);
			  return NAME;
			}
\*.*			{
			  return COMMENT;
			}
\;.*			{
			  return COMMENT;
			}

.			{ return yytext[0]; }

%%

char *
emalloc(n)
unsigned n;
{
	char *ptr, *malloc();

	if ((ptr = malloc(n)) == (char *) 0) {
		(void) fprintf(stderr,"out of core");
		exit(1);
	}
	return ptr;
}
