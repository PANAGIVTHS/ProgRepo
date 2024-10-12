
#include "objdb.h"

#ifndef __MGDEF__
#define  __MGDEF__
#define MAGICNUM "__prog2db__\0"
#endif

void clearStdin() {
    char binInput;

    do {
        scanf("%c", &binInput);
    } while (binInput != '\n');
}

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
    int curRead = 0, total = 0 , i=0;

    do {
        i++;
        curRead = read(fd, buffer + curRead, count);
        if (curRead == -1){
            //printf("READ :: Returning #%d: %d\n", i,-1);
            return(-1);
        }
        total += curRead;
        if (curRead == 0){
            //printf("READ :: Returning #%d: %d\n", i,total);
            return(total);
        }
        count -= curRead;
    } while(count > 0);
    
    //printf("READ :: Returning #%d: %d\n", i,total);
    return(total);
}

int initdb(int fd) {
    if(my_write(fd, MAGICNUM, MAGIC_NUM_LEN) == -1) return(-1);

    return(0);
}

int createdb(const char* filename, dbStatus *cur_db) {
    char magicNumBuffer[MAGIC_NUM_LEN];
    errno = 0;
    
    if (cur_db->isOpen != 0) close(cur_db->fd);

    int fd = open(filename, O_RDWR | O_EXCL | O_CREAT, 0700); 
    // File exists
    if (fd == -1 && errno == EEXIST) {
        // Try opening existing file
        fd = open(filename, O_RDWR);
        // Some weird cpu bussiness 
        if (fd == -1) {
            cur_db->isOpen = false;
            return(-2);
        }
        else {
            // Opened file 
            read(fd, magicNumBuffer, MAGIC_NUM_LEN);
            if (strcmp(magicNumBuffer, MAGICNUM) != 0) {
                close(fd);
                // Not database
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
    else if (fd == -1) {
        return(-2);
    }
    // Initialise created file
    initdb(fd);
    cur_db->isOpen = true;
    cur_db->fd = fd;
    
    return(fd);
}

int strfind (const char *word, const char *str) {
    int str_len = strlen(str);
    int word_len = strlen(word);
    bool same_word = true;
    char *temp2, *temp3;
    
    char *temp = (char*) malloc(word_len+1);
    if (temp == NULL) return(-1);
    temp3 = temp;

    strcpy(temp, word);

    if (word_len < str_len) {
        free(temp3);
        return(0);
    }

    do {
        temp2 = strchr(temp, str[0]);
        if (temp2 == NULL) {
            free(temp3);
            return(0);
        }
        else {
            temp = temp2;
        }
        if (temp[1] == '\0') {
            free(temp3);
            return(1);
        }
        int i;
        for(i = 1; i < str_len; i++) {
            if(temp[i] != str[i]) {
                same_word = false;
                break;
            }
        }
        if (!same_word) {
            free(temp3);
            return(0);
        }
        if ((same_word) && (str_len == word_len)) {
            free(temp3);
            return(2);
        }
        else if ((i >= str_len-1)) {
            free(temp3);
            return(1);
        }
        else {
            temp++;
        }
    
    } while (1);

    free(temp3);
    return(0);
}

int find_same_object (const char *obj_name, dbStatus *cur_db) {
    char buffer[PACKET_SIZE];
    int name_size = 0, data_size = 0, int_size = 0;
    int res, find_res, total = 0;
        
    //printf("Calling read <fos>\n");
    res = my_read(cur_db->fd, buffer, MAGIC_NUM_LEN);
    buffer[MAGIC_NUM_LEN-1] = '\0';
    if (res == 0) return(-2);
    if (res == -1) return(-1);
    if (strcmp(buffer, MAGICNUM) != 0) lseek(cur_db->fd, -MAGIC_NUM_LEN, SEEK_CUR);
    
    do {
        total += (2*int_size + name_size + data_size);
        //printf("Calling read <fo02>\n");
        res = my_read(cur_db->fd, &name_size, sizeof(int));
        if (res == 0) return(-2);
        if (res == -1) return(-1);
        //printf("Calling read <fo02>\n");
        res = my_read(cur_db->fd, buffer, name_size);
        if (res == 0) return(-2);
        if (res == -1) return(-1);
        buffer[name_size] = '\0';
        find_res = strfind(buffer, obj_name);
        //printf("Calling read <fo03>\n");
        res = my_read(cur_db->fd, &data_size, sizeof(int));
        if (res == 0) return(-2);
        if (res == -1) return(-1);
        lseek(cur_db->fd, data_size, SEEK_CUR);
        int_size = sizeof(int);
    } while (find_res != 2);

    return(total);
}

int append (outBuf *out, const char *word) {
    int word_len = strlen(word);
    char *temp = (char *)realloc(out->string, out->len + word_len+1);

    if (temp == NULL) return(-1);
    out->string = temp;

    strcpy(&(out->string[out->len]), word);
    out->len += word_len+1;
    
    return(0);
}

int find_object (const char *obj_name, dbStatus *cur_db, outBuf *out) {
    char buffer[PACKET_SIZE];
    int name_size = 0, data_size = 0, int_size = 0;
    int res, find_res = 0, total = 0;
    bool print_all = false;


    //printf("Calling read <fo>\n");
    res = my_read(cur_db->fd, buffer, MAGIC_NUM_LEN);
    buffer[MAGIC_NUM_LEN-1] = '\0';
    if (res == 0) return(-2);
    if (res == -1) return(-1);
    if (strcmp(buffer, MAGICNUM) != 0) lseek(cur_db->fd, -MAGIC_NUM_LEN, SEEK_CUR);
    if (strcmp("*", obj_name) == 0) print_all = true;
    
    do {
        total += (2*int_size + name_size + data_size);
        //printf("Calling read <fo1>\n");
        res = my_read(cur_db->fd, &name_size, sizeof(int));
        if (res == 0) return(-2);
        if (res == -1) return(-1);
        //printf("Calling read <fo2>\n");
        res = my_read(cur_db->fd, buffer, name_size);
        if (res == 0) return(-2);
        if (res == -1) return(-1);
        buffer[name_size] = '\0';
        if (!print_all) 
            find_res = strfind(buffer, obj_name);
        if (print_all || find_res != 0) 
            append(out, buffer);
        //printf("Calling read <fo3>\n");
        res = my_read(cur_db->fd, &data_size, sizeof(int));
        if (res == 0) return(-2);
        if (res == -1) return(-1);
        lseek(cur_db->fd, data_size, SEEK_CUR);
        int_size = sizeof(int);
    } while (res == 0 || find_res == 0);

    return(total);
}

int import(const char *filename, const char *obj_name, dbStatus *cur_db, char *db_name) {
    int readRes;
    int file_len;
    char buffer[PACKET_SIZE];
    dbStatus dup_db;

    dup_db.isOpen = false;
    int obj_len = strlen(obj_name);    
    
    // no open
    if (cur_db->isOpen != true) {
        return(-2);
    }

    int fd = open(filename, O_RDWR);
    
    createdb(db_name, &dup_db);
    lseek(dup_db.fd, 0, 0);
    // Not found
    if (fd == -1) return(-4);
    // Already
    if (find_same_object(obj_name, &dup_db) >= 0) {
        close(dup_db.fd);
        return(-3);
    } 
    close(dup_db.fd);

    file_len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    my_write(cur_db->fd, &obj_len, sizeof(int));
    my_write(cur_db->fd, obj_name, strlen(obj_name));
    my_write(cur_db->fd, &file_len, sizeof(int));

    do {
        //printf("Calling read <import>\n");
        readRes = my_read(fd, buffer, PACKET_SIZE);
        if (readRes == -1) return(-1);
        if (my_write(cur_db->fd, buffer, readRes) == -1) return(-1);
    } while (readRes != 0);

    close(fd);
    return(0);
}

int find (const char *obj_name, dbStatus *cur_db, outBuf *out, int full_search) {
    int find_res = 0;
    int total = MAGIC_NUM_LEN;

    if (cur_db->isOpen == false) return(-3);

    lseek(cur_db->fd, 0, 0);
    if (full_search) {
        do {
            find_res = find_object(obj_name, cur_db, out);
            if (find_res == -2) return(-2);
            if (find_res == -1) return(-1);
            total += find_res;
        } while (1);
    }
    else {
        find_res = find_same_object(obj_name, cur_db);
        if (find_res == -2) return(-2);
        if (find_res == -1) return(-1);
        total += find_res;
    }

    return(total);
}

// Moves fd to data position + returns data memory

int getdata (int pos_in_file, dbStatus *cur_db) {
    int res = 0, name_size = 0, data_size = 0;
    
    lseek(cur_db->fd, pos_in_file, SEEK_SET);
    res = my_read(cur_db->fd, &name_size, sizeof(int));
    if (res == 0) return(-2);
    if (res == -1) return(-1);
    lseek(cur_db->fd, name_size, SEEK_CUR);
    res = my_read(cur_db->fd, &data_size, sizeof(int));
    if (res == 0) return(-2);
    if (res == -1) return(-1);

    return(data_size);
}

int copydata (dbStatus *cur_db, int newFile_fd, int count) {
    void *buffer = NULL;
    int res, total = 0; 

    buffer = malloc(PACKET_SIZE);
    if (buffer == NULL) return(-3);

    do {
        res = my_read(cur_db->fd, buffer, count > PACKET_SIZE ? PACKET_SIZE : count);
        if (res == 0) {
            free(buffer);
            return(-2);
        }
        if (res == -1) {
            free(buffer);
            return(-1);
        }
        count -= res;
        total += res;
        my_write(newFile_fd, buffer, res);
    } while (count > 0);

    free(buffer);
    return(total);
}

int export (const char *filename, const char *obj_name, dbStatus *cur_db) {
    int find_res = 0, data_len = 0;
    outBuf bin;

    bin.len = 0;
    bin.string = NULL;

    //  No open
    if (cur_db->isOpen == false) return(-4);

    int fd = open(filename, O_RDWR | O_EXCL | O_CREAT, 0700); 
    // cannt open
    if (fd == -1) return(-3);

    find_res = find(obj_name, cur_db, &bin, 0);
    if (find_res == -1) {
        unlink(filename);
        close(fd); 
        return(-1);
    }

    // not found
    if (find_res == -2) {
        unlink(filename);
        close(fd); 
        return(-2);
    }

    data_len = getdata(find_res, cur_db);
    int ret_value = copydata(cur_db, fd, data_len);
    close(fd);

    return(ret_value);
}

int getcount (int pos_in_file, dbStatus *cur_db) {
    int res = 0, name_size = 0, data_size = 0;
    
    lseek(cur_db->fd, pos_in_file, SEEK_SET);
    res = my_read(cur_db->fd, &name_size, sizeof(int));
    if (res == 0) return(-2);
    if (res == -1) return(-1);
    lseek(cur_db->fd, name_size, SEEK_CUR);
    res = my_read(cur_db->fd, &data_size, sizeof(int));
    if (res == 0) return(-2);
    if (res == -1) return(-1);
    lseek(cur_db->fd, -(2*sizeof(int) + name_size), SEEK_CUR);

    return(2*sizeof(int) + name_size + data_size);
}

int my_truncate (int fd, int count) {
    int res = 0, total = 0, del_size = 0, fd_cur = 0, eof = 0; 
    
    do {
        del_size = count > PACKET_SIZE ? PACKET_SIZE : count;
        fd_cur = lseek(fd, 0, SEEK_CUR);
        eof = lseek(fd, 0, SEEK_END);
        lseek(fd, fd_cur, 0);
        res = ftruncate(fd, eof - del_size);
        if (res == -1) return(-1);
        count -= del_size;
        total += del_size;
    } while (count > 0);
  
    return(total);
}

int delete (const char *obj_name, dbStatus *cur_db, const char *db_name) {
    dbStatus dup;
    dup.isOpen = false;
    int find_res, obj_size, mem_until_eof, eof;
    outBuf bin;

    if (cur_db->isOpen == false) return(-3);

    createdb(db_name, &dup);
    find_res = find(obj_name, cur_db, &bin, 0);
    if (find_res == -1) return(-1);
    if (find_res == -2) return(-2);
    obj_size = getcount(find_res, &dup);
    lseek(cur_db->fd, find_res, 0);
    eof = lseek(cur_db->fd, 0, SEEK_END);
    mem_until_eof = eof - find_res;
    lseek(cur_db->fd, find_res, 0);

    if (mem_until_eof == obj_size) {
        // my truncate bt mem_until_eof
        return(my_truncate(cur_db->fd, eof - find_res));
    }
    lseek(dup.fd, find_res + obj_size, 0);
    copydata(&dup, cur_db->fd, mem_until_eof - obj_size);
    // truncate by obj_size

    return(my_truncate(cur_db->fd, obj_size));
}

int close_db (dbStatus *cur_db) {
    
    if (cur_db->isOpen == false) return(-2);
    cur_db->isOpen = false;

    return(close(cur_db->fd));
}

void quit (dbStatus *cur_db) {
    close_db(cur_db);
    exit(0);
}

