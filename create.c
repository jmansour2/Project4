/**
 * @file create.c
 * @provides create, newpid, userret
 *
 *OSC 3250 - Project #2
 * This code is helping xinu keep secerts from his babymama
 *  * @author [JJ Mansour, Steve McDonough]
 *   * Instructor [Dennis Brylow]
 *    *    * TA-BOT:MAILTO [jack.mansour@marquette.edu, stephen.mcdonough@marquette.edu]
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    /* round up to even boundary    */
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];
	
	// TODO: Setup PCB entry for new process.
	ppcb -> state = PRSUSP;
	ppcb -> stkbase = (ulong*)((ulong)(saddr)- ssize);
	ppcb -> stklen = ssize;
	ppcb -> stkptr = NULL;
	strncpy(ppcb -> name,name, PNMLEN);
    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / 4) * 4;
    }
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
	 ulong *saddrVar = saddr;

    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }
	
/*       for(i =0; i < pads; i++){
                *--saddr = 0;
}*/
	ppcb -> stkptr = saddr;	
	// TODO: Initialize process context.
		saddr[CTX_LR] = (int) funcaddr;
		saddr[CTX_SP] = (int)userret;
	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.
//maybe:  int should be ulong 
	va_start(ap,nargs);
	for(i =0; i < nargs; i++){
	
		if(i < 4){
			saddr[i] = va_arg(ap, int);
		
		}else{
			saddrVar[i-4] = va_arg(ap,int);

		}

	}
	va_end(ap);

    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    kill(currpid);
}
