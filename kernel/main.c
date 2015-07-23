
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"


/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");
	disp_str("- begins-----\n");
	struct task* p_task;
	struct proc* p_proc= proc_table;
	char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16   selector_ldt = SELECTOR_LDT_FIRST;
        u8    privilege;
        u8    rpl;
	int   eflags;
	int   i, j;
	int   prio;
	for (i = 0; i < NR_TASKS+NR_PROCS; i++) {
	        if (i < NR_TASKS) {     /* 任务 */
                        p_task    = task_table + i;
                        privilege = PRIVILEGE_TASK;
                        rpl       = RPL_TASK;
                        eflags    = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
			prio      = 15;
                }
                else {                  /* 用户进程 */
                        p_task    = user_proc_table + (i - NR_TASKS);
                        privilege = PRIVILEGE_USER;
                        rpl       = RPL_USER;
                        eflags    = 0x202; /* IF=1, bit 2 is always 1 */
			prio      = 5;
                }

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid = i;			/* pid */

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = eflags;

		/* p_proc->nr_tty		= 0; */

		p_proc->p_flags = 0;
		p_proc->p_msg = 0;
		p_proc->p_recvfrom = NO_TASK;
		p_proc->p_sendto = NO_TASK;
		p_proc->has_int_msg = 0;
		p_proc->q_sending = 0;
		p_proc->next_sending = 0;

		for (j = 0; j < NR_FILES; j++)
			p_proc->filp[j] = 0;

		p_proc->ticks = p_proc->priority = prio;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

        /* proc_table[NR_TASKS + 0].nr_tty = 0; */
        /* proc_table[NR_TASKS + 1].nr_tty = 1; */
        /* proc_table[NR_TASKS + 2].nr_tty = 1; */

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

	init_clock();
        init_keyboard();

	restart();

	while(1){}
}


/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}


/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int fd;
	int i, n;

	char tty_name[] = "/dev_tty0";

	char rdbuf[128];


	int fd_stdin  = open(tty_name, O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

//	char filename[MAX_FILENAME_LEN+1] = "zsp01";
	const char bufw[80] = {0};
//	const int rd_bytes = 3;
//	char bufr[rd_bytes];

	clear();
	long int ii = 9999;

printf("                                                                           \n");while(ii-->0);ii=9999;
printf("    ##      D##                     ###           .##                      \n");
printf("    ##      D##                     W##           :##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      D##      L###K          ###           .##           ##W# .     \n");
printf("    ##      G##    j#######i        ###           .##        .########     \n");
printf("    ###########    ##t  .W##        W##           .##        ###   W##D    \n");
printf("    ###########   ##D     ###       W##           .##       D##      ##    \n");
printf("    ##      G##  .##      ###       W##           .##       ##E      ##.   \n");
printf("    ##      G##  .###########       W##           .##       ##       ##E   \n");
printf("    ##      D##  :##                W##           .##       ##       ##E   \n");
printf("    ##      G##  .##                W##           .##       ##       ##G   \n");
printf("    ##      G##   ##.     f##       W##           .##       ##K      ##    \n");
printf("    ##      G##   ###.    ###       W##           .##       i#W     ;##    \n");
printf("    ##      G##    ###,  ###.       W##           .##        #### .###:    \n");
printf("    ##      D##     #######:        W##           :##         #######:     \n");
printf("                      K##           . .             .           i##  .     \n\n");
	startAnimate();
	
	clear();
	disp_color_str(" \n\n                   ==================================\n",5);
	disp_color_str("                                   MyTinix v1.0.2             \n",2);
	disp_color_str("                                 Kernel on Orange's \n\n",3);
	disp_color_str("                                     Welcome !\n",4);
	disp_color_str("                        ====================================",5);

	

	//int ii =1000;
	//while(ii-->0) sleep();
	while (1) {           
		printf("\n\n\n\n\n\n command:");


		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		//show();
        if (strcmp(rdbuf, "process") == 0)
        {
			ProcessManage();
        }
		else if (strcmp(rdbuf, "filemng") == 0)
		{
			printf("File Manager is already running on CONSOLE-1 ! \n");

			continue;

		}else if (strcmp(rdbuf, "help") == 0)
		{
			help();
		}
		else if (strcmp(rdbuf, "timer") == 0)
		{

			timer(fd_stdin, fd_stdout);
		}
		
		else if (strcmp(rdbuf, "clear") == 0)
		{
			clear();
			printf("                        ==================================\n");
			printf("                                   MyTinix v1.0.2             \n");
			printf("                                 Kernel on Orange's \n\n");
			printf("                                     Welcome !\n");
			printf("                        ==================================\n");
		}
		

		else
			printf("Command not found, please check!\n");
	}
}

void timer(int fd_stdin,int fd_stdout)
{

	
		printf("How many seconds do you want to set?\n");
		char rdbuf[128];
		int tmp = 0;
		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf,&tmp);

		int i = tmp;
printf("the time you set is %d\nNow begin....\n",tmp);
		for(;i >=0;i--){
			printf("%d left\n",i);
			milli_delay(10000);
		}
		printf("Time up!");

		
	
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	char tty_name[] = "/dev_tty1";

	int fd_stdin  = open(tty_name, O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

	char rdbuf[128];
	char cmd[8];
	char filename[120];
	char buf[1024];
	int m,n;
	printf("                        ==================================\n");
	printf("                                    File Manager           \n");
	printf("                                 Kernel on Orange's \n\n");
	printf("                        ==================================\n");
	while (1) {
		printf("$ ");
		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		

		if (strcmp(rdbuf, "help") == 0)
		{
			printf("=============================================================================\n");
			printf("Command List     :\n");
			printf("1. create [filename]       : Create a new file \n");
			printf("2. read [filename]         : Read the file\n");
			printf("3. write [filename]        : Write at the end of the file\n");
			printf("4. delete [filename]       : Delete the file\n");
			printf("5. help                    : Display the help message\n");
			printf("==============================================================================\n");		
		}
		else if (strcmp(rdbuf, "help") == 0)
		{
			
		}
		else
		{
			int fd;
			int i = 0;
			int j = 0;
			char temp = -1;
			while(rdbuf[i]!=' ')
			{
				cmd[i] = rdbuf[i];
				i++;
			}
			cmd[i++] = 0;
			while(rdbuf[i] != 0)
			{
				filename[j] = rdbuf[i];
				i++;
				j++;
			}
			filename[j] = 0;

			if (strcmp(cmd, "create") == 0)
			{
				fd = open(filename, O_CREAT | O_RDWR);
				if (fd == -1)
				{
					printf("Failed to create file! Please check the fileaname!\n");
					continue ;
				}
				buf[0] = 0;
				write(fd, buf, 1);
				printf("File created: %s (fd %d)\n", filename, fd);
				close(fd);
			}
			else if (strcmp(cmd, "read") == 0)
			{
				fd = open(filename, O_RDWR);
				if (fd == -1)
				{
					printf("Failed to open file! Please check the fileaname!\n");
					continue ;
				}
				
				n = read(fd, buf, 1024);
				
				printf("%s\n", buf);
				close(fd);

			}
			else if (strcmp(cmd, "write") == 0)
			{
				fd = open(filename, O_RDWR);
				if (fd == -1)
				{
					printf("Failed to open file! Please check the fileaname!\n");
					continue ;
				}

				m = read(fd_stdin, rdbuf,80);
				rdbuf[m] = 0;
				
				n = write(fd, rdbuf, m+1);
				close(fd);
			}
			else if (strcmp(cmd, "delete") == 0)
			{
				m=unlink(filename);
				if (m == 0)
				{
					printf("File deleted!\n");
					continue;
				}
				else
				{
					printf("Failed to delete file! Please check the fileaname!\n");
					continue;
				}

			}
			else 
			{
				printf("Command not found, Please check!\n");
				continue;
			}

			
			
		}
		
			
	}

	assert(0); /* never arrive here */
}




void TestC()
{
	spin("TestC");
	printf("this is c");
	disp_color_str("c");
}


/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

void clear()
{
	clear_screen(0,console_table[current_console].cursor);
	console_table[current_console].crtc_start = 0;
	console_table[current_console].cursor = 0;
	
}

//void show()
//{
//	printf("%d  %d  %d  %d",console_table[current_console].con_size, console_table[current_console].crtc_start, console_table[current_console].cursor, console_table[current_console].orig);
//}

void help()
{
	printf("=============================================================================\n");
	printf("Command List     :\n");
	printf("1. process       : A process manage,show you all process-info here\n");
	printf("2. filemng       : Run the file manager\n");
	printf("3. clear         : Clear the screen\n");
	printf("4. help          : Show this help message\n");
//	printf("5. taskmanager   : Run a task manager,you can add or kill a process here\n");
	printf("5. runttt        : Run a small game on this OS\n");
	printf("==============================================================================\n");		
}
void startAnimate(){
	long int ii = 9999;
	printf("System is loading...");

	while(ii-->0);ii=2999999;
	printf("................");
	while(ii-->0);ii=2999999;
	printf("......");
	while(ii-->0);ii=999999;
	printf("..");
	while(ii-->0);ii=6999999;
	printf("..............................");
	while(ii-->0);ii=9999;
	printf("......");
	while(ii-->0);ii=999999;
	printf("..");
	while(ii-->0);ii=9999;
	printf("..");
	while(ii-->0);ii=999;
	printf(".......");
}
void ProcessManage()
{
	int i;
	printf("=============================================================================\n");
	printf("      myID      |    name       | spriority    | running?\n");
	//进程号，进程名，优先级，是否是系统进程，是否在运行
	printf("-----------------------------------------------------------------------------\n");
	for ( i = 0 ; i < NR_TASKS + NR_PROCS ; ++i )//逐个遍历
	{
//		if ( proc_table[i].priority == 0) continue;//系统资源跳过
		printf("        %d           %s            %d                yes\n", proc_table[i].pid, proc_table[i].name, proc_table[i].priority);
	}
	printf("=============================================================================\n");
}
