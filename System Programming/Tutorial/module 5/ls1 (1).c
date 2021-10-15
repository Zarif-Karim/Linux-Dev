#include    <stdio.h>
#include    <sys/types.h>
#include    <dirent.h>
#include <sys/stat.h>

void do_ls(char[]);
void show_stat_info(char*, struct stat*);

main(int ac, char *av[])
{
    if (ac == 1)
        do_ls(".");
    else
        while (--ac) 
		{
            printf("%s:\n", *++av);
            do_ls(*av);
        }
	return(0);
}

void do_ls(char dirname[])
/*
 *    list files in directory called dirname
 */
{
    DIR* dir_ptr;        /* the directory */
    struct dirent* direntp;        /* each entry     */
    struct stat info;        /* buffer for file info */

    if ((dir_ptr = opendir(dirname)) == NULL)
    {
        if (stat(dirname, &info) != -1)
            show_stat_info(dirname, &info);
    }
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL)
            if (stat(direntp->d_name, &info) != -1)
                if (S_ISREG(info.st_mode))
                    show_stat_info(direntp->d_name, &info);
				
                else
                    printf("Folder: %s\n\n", direntp->d_name);

        closedir(dir_ptr);
    }
}

void show_stat_info(char* fname, struct stat* buf)
/*
 * displays some info from stat in a name=value format
 */
{
	printf("filename: %s\n", fname);					/* filename */
	printf("    inode: %ld\n", buf->st_ino);		/* inode */
    printf("    mode: %o\n", buf->st_mode);         /* type + mode */
    printf("   links: %ld\n", buf->st_nlink);        /* # links     */
    printf("    user: %d\n", buf->st_uid);          /* user id     */
    printf("   group: %d\n", buf->st_gid);          /* group id    */
    printf("    size: %ld\n", buf->st_size);         /* file size   */
    printf(" modtime: %ld\n", buf->st_mtime);        /* modified    */
	printf("\n");
}
