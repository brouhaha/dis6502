#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <sys/types.h>
#include <unistd.h>

#include "dis.h"

char *strcpy();
char *strcat();


static int has_offset (int i)
{
  return ((i > 0) && (! (f [i] & NAMED)) &&
	  ((f [i-1] & (NAMED | DREF)) == (NAMED | DREF)));
}


static char *lname (int i, int offset_ok)
{
	static char buf[20];
	char t;

	if (f[i] & NAMED) 
		return(get_name(i));
	if (offset_ok && has_offset (i)) {
		(void)strcpy(buf, get_name(i-1));
		(void)strcat(buf, "+1");
		return (buf);
	}
	if (f[i] & SREF)
		t = 'S';
	else if (f[i] & JREF)
		t = 'L';
	else if (f[i] & DREF)
	  {
	    if (i <= 0xff)
	      t = 'Z';
	    else
	      t = 'D';
	  }
	else 
		t = 'X';

	if (i <= 0xff)
	  (void)sprintf(buf, "%c%02x", t, i);
	else
	  (void)sprintf(buf, "%c%04x", t, i);

	return (buf);
}


static int print_label (int i)
{
  if (f[i] & (NAMED | JREF | SREF | DREF)) 
    {
      printf("%s", lname(i, 0));
      return (1);
    }
  else
    return (0);
}

void dumpitout (void)
{
  int i;

  for(i = 0; i<0x10000;) 
    {
      if (f[i] & LOADED) 
	{
	  if ((i == 0) || (! (f[i-1] & LOADED)))
	    printf ("\t.org\t$%04x\n", i);

	  if (f[i] & SREF && f[i] & ISOP)
	    printf("\n");

	  if (! asmout)
	    {
	      printf("%04x  ",i);
	      print_bytes(i);
	    }
	  if (! has_offset (i))
	    {
	      if (print_label(i))
		printf (":");
	    }
	  printf ("\t");
	  if (f[i] & ISOP)
	    i += print_inst(i);
	  else
	    i += print_data(i);
	  printf("\n");

	}
      else 
	{
	  if ((! has_offset (i)) && print_label (i))
	    {
	      if (i <= 0xff)
		printf ("\t.equ\t$%02x\n", i);
	      else
		printf ("\t.equ\t$%04x\n", i);
	    }
	  i++;
	}
    }

  if (! asmout)
    print_refs();
}

int pchar (int c)
{
	if (isascii(c) && isprint(c))
		return(c);
	return('.');
}

void print_bytes (int addr)
{
	register struct info *ip; 

	if ((f[addr] & ISOP) == 0) {
		printf("           ");
		return;
	}

	ip = &optbl[getbyte(addr)];

	switch (ip->nb) {
		case 1:
			printf("%02x         ", getbyte(addr));
			break;
		case 2:
			printf("%02x %02x      ", getbyte(addr), getbyte(addr+1));
			break;
		case 3:
			printf("%02x %02x %02x   ", getbyte(addr), getbyte(addr+1), getbyte(addr+2));
			break;
	}
}
		

int print_inst(int addr)
{
	int opcode;
	register struct info *ip; 
	int operand;

	opcode = getbyte(addr);
	ip = &optbl[opcode];

	printf("%s", ip->opn);

	addr++;

	switch(ip->nb) {
		case 1:
			break;
		case 2:
			operand = getbyte(addr);
			break;
		case 3:
			operand = getword(addr);
			break;
	}

	if (ip->flag & REL) {
		if (operand > 127) 
			operand = (~0xff | operand);
		operand = operand + ip->nb + addr - 1;
	}

	switch (ip->flag & ADRMASK) {
		case IMM:
			printf("\t#$%02x\t; %d %c", operand, operand, pchar(operand));
			break;
		case ACC:
		case IMP:
			break;
		case REL:
		case ABS:
		case ZPG:
			printf("\t%s", lname(operand, 1));
			break;
		case IND:
			printf("\t(%s)", lname(operand, 1));
			break;
		case ABX:
		case ZPX:
			printf("\t%s,X", lname(operand, 1));
			break;
		case ABY:
		case ZPY:
			printf("\t%s,Y", lname(operand, 1));
			break;
		case INX:
			printf("\t(%s,X)", lname(operand, 1));
			break;
		case INY:
			printf("\t(%s),Y", lname(operand, 1));
			break;
		default:
			break;
	}

	return(ip->nb);

}

int print_data (int i)
{
	int count;
	int j;
	int start;

	start = i;
	printf(".byte\t$%02x", getbyte(i));
	count = 1;
	i++;

	for (j = 1; j < 8; j++) {
		if (f[i] & (JREF | SREF | DREF) || ((f[i] & LOADED) == 0)) 
			break;
		else
			printf(",$%02x", getbyte(i));
		i++;
		count++;
	}
	for (j = count; j < 8; j++)
		printf("   ");

	printf("\t; \"");

	for (j = start; j < i ; j++) 
			printf("%c", pchar((int)getbyte(j)));

	printf ("\"");

	return (count);
}

void print_refs (void)
{
	char tname[50];
	char cmd[200];
	FILE *fp;
	register struct ref_chain *rp;
	register int i;
	int npline;

	(void)sprintf(tname, "dis.%d", getpid());
	(void)sprintf(cmd, "sort %s; rm %s", tname, tname);

	fp = fopen(tname, "w");
	if (!fp) 
		crash("Cant open temporary file/n");

	for (i = 0; i<0x10000; i++) {
		if(f[i] & (JREF|SREF|DREF)) {
			rp = get_ref(i);
			if (!rp) {
				fprintf(stderr, "No ref %d\n", i);
				break;
			}

			fprintf(fp, "%-8s  %04x   ", lname(i, 1), i);
			npline = 0;
			while (rp) {
				fprintf(fp, "%04x ", rp->who);
				npline++;
				if (npline == 12) {
					fprintf(fp,"\n");
					fprintf(fp,"%-8s  %04x   ",lname(i, 1),i);
					npline = 0;
				}
				rp = rp->next;
			}
			fprintf(fp, "\n");
		}

	}

	(void)fclose(fp);

	printf("\n\n\n\n\nCross References\n\n");
	printf("%-8s  Value  References\n", "Symbol");
	(void)fflush (stdout);

	(void)system(cmd);
}
