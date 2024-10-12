#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#define MAGIC_NUM_LEN 12
#define PACKET_SIZE 512
#define MAX_NAME_LEN 255

typedef struct {
    int fd;
    bool isOpen;
} dbStatus;


extern int my_write(int fd, const void* buffer, size_t count);

extern int my_read(int fd, void* buffer, size_t count);

extern int initdb(int fd, const char magicNum[MAGIC_NUM_LEN]);

extern int createdb(const char* filename, const char magicNum[MAGIC_NUM_LEN], dbStatus *cur_db);

extern int strfind (const char *word, const char *str);

extern int find_same_object (const char *obj_name, dbStatus *cur_db);

extern int find_object (const char *obj_name, dbStatus *cur_db);

extern int import(const char *filename, const char *obj_name, dbStatus *cur_db, char *db_name);

extern int find (const char *obj_name, dbStatus *cur_db, int full_search);

extern int getdata (int pos_in_file, dbStatus *cur_db);

extern int copydata (dbStatus *cur_db, int newFile_fd, int count);

extern int export (const char *filename, const char *obj_name, dbStatus *cur_db);

extern int getcount (int pos_in_file, dbStatus *cur_db);

extern int my_truncate (int fd, int count);

extern int delete (const char *obj_name, dbStatus *cur_db, const char *db_name);

extern int close_db (dbStatus *cur_db);

extern void quit (dbStatus *cur_db);

