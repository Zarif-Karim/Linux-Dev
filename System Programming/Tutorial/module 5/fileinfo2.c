/* statinfo.c - demonstrates using stat() to obtain
 *              file information.
 *            - some members are just numbers...
 */
//#include <stdio.h>
//#include <sys/types.h>
#include <sys/stat.h>

void show_stat_info(char *fname)
/*
 * displays some info from stat in a name=value format
 */
{
	struct stat buf;
	if(stat(fname, &buf) != 1) {
    	printf("   mode: %o\n", buf.st_mode);         /* type + mode */
  	    printf("  links: %d\n", buf.st_nlink);        /* # links     */
	    printf("   user: %d\n", buf.st_uid);          /* user id     */
	    printf("  group: %d\n", buf.st_gid);          /* group id    */
	    printf("   size: %d\n", buf.st_size);         /* file size   */
	    printf("modtime: %d\n", buf.st_mtime);        /* modified    */
	    //printf("   name: %s\n", fname );               /* filename    */
	}
	else 
		perror(fname);
}
