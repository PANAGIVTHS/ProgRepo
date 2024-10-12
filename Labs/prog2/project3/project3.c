
#include "objdb.h"

int main(int argc, char *argv[]) {
    char command_in;
    dbStatus db;
    char *filename, *obj_name;
    char *db_name = NULL, *temp = NULL;
    outBuf out;
    int find_res, imp_res, exp_res;

    db.isOpen = false;
    out.string = NULL;
    out.len = 0;

    do {
        scanf(" %c", &command_in);
        switch (command_in) {
            case 'o':{

                scanf(" %ms", &temp);
                if (temp == NULL) {
                    return(-1);
                }
                int open_res = createdb(temp, &db);
                if (open_res == -2) fprintf(stderr, "\nError opening %s.\n", db_name);
                if (open_res == -1) fprintf(stderr, "\nInvalid db file %s.\n", db_name);
                if ((open_res != -1) && (open_res != -2)) {
                    free(db_name);
                    db_name = temp;
                }
                break;
            }
            case 'i':{
                scanf(" %ms %ms", &filename, &obj_name);
                imp_res = import(filename, obj_name, &db, db_name);
                if (imp_res == -3) fprintf(stderr, "\nObject %s already in db.\n", obj_name);
                if (imp_res == -4) fprintf(stderr, "\nFile %s not found.\n", filename);
                if (imp_res == -2) fprintf(stderr, "\nNo open db file.\n");
                if (imp_res == -1) {
                    perror("\n");
                    free(filename);
                    free(obj_name);
                    free(db_name);
                    close_db(&db);
                    exit(42);
                }
                free(filename);
                free(obj_name);
                break;
            } 
            case 'f':{
                scanf(" %ms", &obj_name);

                find_res = find(obj_name, &db, &out, 1);
                if (find_res == -3) fprintf(stderr, "\nNo open db file.\n");
                if (find_res == -1) {
                    perror("\n");
                    out.string = NULL;
                    free(out.string);
                    free(obj_name);
                    out.len = 0;
                    free(db_name);
                    exit(42);
                }
                
                printf("\n##\n");
                
                for (int i = 0; i < out.len-1; i++) {
                    printf("%s\n", &(out.string[i]));
                    i += strlen(&(out.string[i]));
                }

                free(out.string);
                out.string = NULL;
                out.len = 0;
                free(obj_name);
                break;
            } 
            case 'e':{
                scanf(" %ms %ms", &obj_name, &filename);
                if (obj_name == NULL || filename == NULL) {
                    free(obj_name);
                    free(filename);
                    break;
                }
                exp_res = export(filename, obj_name, &db);
                if (exp_res == -1) {
                    perror("\n");
                    free(filename);
                    free(obj_name);
                    free(db_name);
                    exit(42);
                }
                if (exp_res == -2) fprintf(stderr, "\nObject %s not in db.\n", obj_name);
                if (exp_res == -3) fprintf(stderr, "\nCannot open file %s.\n", filename);
                if (exp_res == -4) fprintf(stderr, "\nNo open db file.\n");
                out.string = NULL;
                out.len = 0;
                free(filename);
                free(obj_name);

                break;
            } 
            case 'd':{
                scanf(" %ms", &obj_name);
                if (obj_name == NULL) break;
                int del_res = delete(obj_name, &db, db_name);
                if (del_res == -1) {
                    perror("\n");
                    free(obj_name);
                    free(db_name);
                    exit(42);
                }                
                if (del_res == -2) fprintf(stderr, "\nObject %s not in db.\n", obj_name);
                if (del_res == -3) fprintf(stderr, "\nNo open db file.\n");
                
                free(obj_name);

                break;
            } 
            case 'c':{
                if (close_db(&db) == -2) fprintf(stderr, "\nNo open db file.\n");
                break;
            } 
            case 'q':{
                free(db_name);
                quit(&db);
                break;
            } 
            default:{
                fprintf(stderr, "\nInvalid command.\n");
            }
            clearStdin();
        } 
    } while (1);


    free(db_name);
    close_db(&db);

    return(0);
}
