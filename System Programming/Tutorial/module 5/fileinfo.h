/* statinfo.c - demonstrates using stat() to obtain
 *              file information.
 *            - some members are just numbers...
 */
#include <sys/stat.h>

void fileInfo(char *fname)
/*
 * displays some info from stat in a name=value format
 */
{
	struct stat buf;
	if(stat(fname, &buf) != 1){
		printf("mode: %o\t", buf.st_mode);         /* type + mode */
		printf("links: %d\t", buf.st_nlink);        /* # links     */
		printf("user: %d\t", buf.st_uid);          /* user id     */
		printf("group: %d\t", buf.st_gid);          /* group id    */
		printf("size: %d\t", buf.st_size);         /* file size   */
		printf("modtime: %d\t", buf.st_mtime);        /* modified    */
		printf("name: %s\n", fname );               /* filename    */
	}
	else perror(fname);
}
