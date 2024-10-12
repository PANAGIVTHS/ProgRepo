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

int my_write(int fd, const void* buffer, size_t count) {
    int curWritten = 0, total = 0;

    do {
        curWritten = write(fd, buffer + curWritten, count);
        if (curWritten == -1){
            return(-1);
        }
        if (curWritten == 0) return(total);
        total += curWritten;
        count -= curWritten;
    } while((count > 0));

    return(total);
}

int my_read(int fd, void* buffer, size_t count) {
    int curRead = 0, total = 0;

    do {
        curRead = read(fd, buffer + curRead, count);
        if (curRead == -1){
            return(-1);
        }
        total += curRead;
        if (curRead == 0) return(total);
        count -= curRead;
    } while(count > 0);
    
    return(total);
}

int initdb(int fd, const char magicNum[MAGIC_NUM_LEN]) {
    if(my_write(fd, magicNum, MAGIC_NUM_LEN) == -1) return(-1);

    return(0);
}

int createdb(const char* filename, const char magicNum[MAGIC_NUM_LEN], dbStatus *cur_db) {
    char magicNumBuffer[MAGIC_NUM_LEN];

    if (cur_db->isOpen != 0) {
        close(cur_db->fd);
        printf("Closed old db. Opening new...\n");
    }
    int fd = open(filename, O_RDWR | O_EXCL | O_CREAT, 0700); 
    // File exists
    if (fd == -1) {
        // Try opening existing file
        fd = open(filename, O_RDONLY);
        // Some weird cpu bussiness 
        if (fd == -1) {
            printf("Error opening %s\n", filename);
            cur_db->isOpen = false;
            return(-2);
        }
        else {
            // Opened file 
            read(fd, magicNumBuffer, MAGIC_NUM_LEN);
            if (strcmp(magicNumBuffer, magicNum) != 0) {
                close(fd);
                // Not database
                printf("Invalid db file %s\n", filename);
                cur_db->isOpen = false;
                return(-1);
            }
            else {
                // Is database
                cur_db->isOpen = true;
                cur_db->fd = fd;
                return(fd);
            }
        }
    }
    // Initialise created file
    initdb(fd, magicNum);
    cur_db->isOpen = true;
    cur_db->fd = fd;
    
    return(fd);
}


int strfind (const char *word, const char *str) {
    int str_len = strlen(str);
    int word_len = strlen(word);
    bool same_word = true;

    char *temp = (char*) malloc(word_len+1);
    strcpy(temp, word);

    if (word_len < str_len) return(0);


    do {
        temp = strchr(temp, str[0]);
        if (temp == NULL) return(0);
        if (temp[1] == '\0') return(1);

        int i;
        for(i = 1; i < str_len; i++) {
            if(temp[i] != str[i]) {
                same_word = false;
                break;
            }
        }
        if ((same_word) && (str_len == word_len)) return(2);
        else if ((i >= str_len-1)) return(1);
    
    } while (1);

    return(1);
}

// how many bytes till found )not counting magic number)

int find_object (const char *obj_name, dbStatus *cur_db, int *isSame) {
    int obj_name_size, data_size, total = 0;
    char obj_buf[MAX_NAME_LEN], mag_num_buf[MAGIC_NUM_LEN];
    int readRes, findRes;

    if (cur_db->isOpen != true) return(-1);

    // If at start of a db skip magicnum
    readRes = my_read(cur_db->fd, mag_num_buf, MAGIC_NUM_LEN);
    mag_num_buf[MAGIC_NUM_LEN-1] = '\0';
    if (strcmp(mag_num_buf, "__prog2db__\0") != 0) {
        lseek(cur_db->fd, -MAGIC_NUM_LEN, SEEK_CUR);
        total = lseek(cur_db->fd, 0, SEEK_CUR);
    }
    else {
        total = 12;
    }

    do {
        readRes = my_read(cur_db->fd, &obj_name_size, 4);
        if (readRes != -1) total += sizeof(int);
        if (readRes == 0) return(0);
        if (readRes == -1) return(-1);
        readRes = my_read(cur_db->fd, obj_buf, obj_name_size);
        if (readRes != -1) total += obj_name_size;
        if (readRes == -1) return(-1);
        else {
            obj_buf[readRes] = '\0';
            findRes = strfind(obj_buf, obj_name);
            if (findRes != 0) {
                if (findRes == 2) *isSame = 1;
                
                // move fd to next object
                readRes = my_read(cur_db->fd, &data_size, 4);
                if (readRes == -1) return(-1);
                lseek(cur_db->fd, (off_t)data_size, SEEK_CUR);

                return(total - obj_name_size - sizeof(int));
            }
            if (readRes != -1) total += sizeof(int);
        }
        readRes = my_read(cur_db->fd, &data_size, 4);
        if (readRes == -1) return(-1);
        if (readRes != -1) total += sizeof(int);

        lseek(cur_db->fd, (off_t)data_size, SEEK_CUR);
    } while (1);

    return(0);
}

int import(const char *filename, const char *obj_name, dbStatus *cur_db, char *db_name) {
    int fd = open(filename, O_RDWR);
    int readRes, findRes , isSame = 0;
    int file_len;
    char buffer[PACKET_SIZE];
    dbStatus dup_db;

    dup_db.isOpen = false;
    int obj_len = strlen(obj_name);    
    
    createdb(db_name, "__prog2db__\0", &dup_db);
    lseek(dup_db.fd, 0, 0);
    do {
        findRes = find_object(obj_name, &dup_db, &isSame);
        if (findRes == -1) break;
    } while ((findRes != 0) && (isSame == 0));
    
    if (isSame) {
        close(dup_db.fd);
        return(-2);
    } 
    close(dup_db.fd);


    if (fd == -1) return(-1);
    if (cur_db->isOpen != true) {
        printf("false\n");
        return(-1);
    }

    file_len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    my_write(cur_db->fd, &obj_len, sizeof(int));
    my_write(cur_db->fd, obj_name, strlen(obj_name));
    my_write(cur_db->fd, &file_len, sizeof(int));

    do {
        readRes = my_read(fd, buffer, PACKET_SIZE);
        if (readRes == -1) return(-1);
        if (my_write(cur_db->fd, buffer, readRes) == -1) return(-1);
    } while (readRes != 0);

    close(fd);
    return(0);
}

int find (const char *obj_name, dbStatus *cur_db) {
    int findRes, isSame, readRes, obj_name_size;
    char obj_buf[MAX_NAME_LEN];

    do {
        findRes = find_object(obj_name, cur_db, &isSame);
        if (findRes == -1) break;
            printf("%d", findRes);

        if (findRes != 0) {
            readRes = my_read(cur_db->fd, &obj_name_size, 4);
            if (readRes == 0) return(0);
            if (readRes == -1) return(-1);
            readRes = my_read(cur_db->fd, obj_buf, obj_name_size);
            if (readRes == -1) return(-1);
            obj_buf[readRes] = '\0';
            printf("%s\n", (char*)obj_buf);
            printf("WTF IS HAPPENIGN");
        } 
        else break;
    } while (1);

    return(false);
}

int main() {
    dbStatus cur_db;
    cur_db.isOpen = false;


    createdb("test", "__prog2db__\0", &cur_db);
    // void *buffer = malloc(PACKET_SIZE);
    import("wow1", "objc", &cur_db, "test");
    import("wow1", "objb", &cur_db, "test");
    import("wow1", "objb", &cur_db, "test");
    // import("wow", "obja", &cur_db, buffer);
    // import("wow1", "objb", &cur_db, buffer);
    // import("wow1", "obja", &cur_db, buffer);

    find("obj", &cur_db);
    printf("I AM HERE\n");
    close(cur_db.fd);
    // free(buffer);

    printf("%d",strfind("ss", "n"));

    return(0);
}