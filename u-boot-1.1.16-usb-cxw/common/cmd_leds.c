/* leds command is used to control leds light or dim
   Code writed by cxw
 */
#include <common.h>
#include <s3c2410.h>

DECLARE_GLOBAL_DATA_PTR;

#if (CONFIG_COMMANDS & CFG_CMD_LEDS)
#include <command.h>
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)
int do_leds (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong whichled, islight, rcode = 0;
	char *endp;
  //GPBCON	 = 0x00015400;
  switch (argc) {
  case 3:
    whichled = simple_strtoul(argv[1], &endp, 10);
    //if ((*endp != '\0') && (whichled == 0) || whichled > 3) {
    if ((*endp != '\0') || (whichled > 3)) {
      printf ("arg1 error\n");
      printf ("Usage:\n%s\n", cmdtp->help);
      return 1;
    }
    islight  = simple_strtoul(argv[2], &endp, 10);
    //if (((*endp != '\0') && (islight == 0)) || islight > 1) {
    if ((*endp != '\0') || (islight > 1)) {
      printf ("arg2 error\n");
      printf ("Usage:\n%s\n", cmdtp->help);
      return 1;
    }
    if (!islight)
      GPBDAT = (1<<(whichled+5)) | GPBDAT;
    else
      GPBDAT = ~(1<<(whichled+5)) & GPBDAT;
    break;
  default:
    printf ("Usage:\n%s\n", cmdtp->usage);
    rcode = 1;
  }
  return rcode;
}

/***************************************************/

U_BOOT_CMD(
	leds,	3,	1,	do_leds,
	"leds    - control led0-3 light or dim\n",
	"leds arg1 arg2\n"
	"  - arg1 indicate number of led, from 0 to 3\n"
	"  - arg2=0 means dim, arg2=1 means light\n"
);

int do_machnum (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
  ulong machnum;
  char *endp;
  ulong rcode = 0;
  switch (argc) {
  case 1:
    printf("machine number is %d\n", gd->bd->bi_arch_number);
    break;
  case 2:
    machnum = simple_strtoul(argv[1], &endp, 10);
    if (*endp != '\0') {
      printf ("arg1 error\n");
      printf ("Usage:\n%s\n", cmdtp->help);
      return 1;
    }
    gd->bd->bi_arch_number = machnum;
    printf("machine number is %d\n", gd->bd->bi_arch_number);
    break;
  default:
    printf ("Usage:\n%s\n", cmdtp->usage);
    rcode = 1;
  }
  return rcode;
}

/***************************************************/

U_BOOT_CMD(
	machnum,	2,	1,	do_machnum,
	"machnum    - change and display machine type number\n",
	"machnum or machnum arg1\n"
	"  - arg1 indicate new machine type number\n"
);

//#define S3C2440_UPLL_48MHZ  ((0x38<<12)|(0x02<<4)|(0x02))
int do_upll (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
  ulong upllval;
  char *endp;
  ulong rcode = 0;
  S3C24X0_CLOCK_POWER * const clk_power = S3C24X0_GetBase_CLOCK_POWER();
  switch (argc) {
  case 1:
    printf("UPLLCON value should be 0x38022\n");
    printf("UPLLCON value now is %d\n", get_UCLK());
    //printf("UPLLCON value now is %d\n", clk_power->UPLLCON);
    break;
  case 2:
    upllval = simple_strtoul(argv[1], &endp, 10);
    if (*endp != '\0') {
      printf ("arg1 error\n");
      printf ("Usage:\n%s\n", cmdtp->help);
      return 1;
    }
    //clk_power->UPLLCON = S3C2440_UPLL_48MHZ;
    clk_power->UPLLCON = upllval;
    udelay (8000);
    printf("UPLLCON value now is 0x%x\n", clk_power->UPLLCON);  	
    break;
  default:
    printf ("Usage:\n%s\n", cmdtp->help);
    rcode = 1;
  }
  return rcode;
}
U_BOOT_CMD(
	upll,	2,	1,	do_upll,
	"upll    - set and display UPLLCON value\n",
	"upll or upll arg1\n"
	"  - arg1 indicate the UPLLCON value you want to set\n"
);
#endif /* (CONFIG_COMMANDS & CFG_CMD_LEDS) */
