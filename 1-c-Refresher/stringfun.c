#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here

int setup_buff(char *buff, char *user_str, int len) {
    if (!buff || !user_str) {
        return -2;
    }

    int string_len = 0;
    int last_char;

    while (*user_str != '\0' && string_len < len) { 
        if (*user_str != ' ' || (string_len > 0 && *(buff + string_len - 1) != ' ')) {
            *(buff + string_len) = *user_str;
            string_len++;
            last_char = (*user_str == ' ');
        }
        user_str++;
    }

    if (last_char) {
        string_len--;
    }

    for (int i = string_len; i < len; i++) {
        *(buff + i) = '.';
    }

    if (*user_str != '\0') {
        return -1;
    }

    return string_len;
}



void print_buff(char *buff, int len) {
    printf("Buffer:  [");
    for (int i = 0; i < len; i++) {
        putchar(*(buff + i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename) {
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len, int str_len) {
    int count = 0;
    int limit = len < str_len ? len : str_len;
    int in_word = 0;
    char *p = buff;

    for (int i = 0; i < limit; i++, p++) {
        if (*p != ' ' && !in_word) {
            count++;
            in_word = 1;
        } else if (*p == ' ') {
            in_word = 0;
        }
    }

    return count;
}

void reverse_string(char *buff, int len) {
    for (int i = 0; i < len / 2; i++) {

        char frontLetter = *(buff + i);
        *(buff + i) = *(buff + len - i - 1);
        *(buff + len - i - 1) = frontLetter;

    }

}

void print_word(char *buff, int len) {
    
    int word_length = 0;
    int count = 1;

    for (int i = 0; i < len; i++) {

        if (*(buff + i) != ' ' && *(buff + i) >= 32 && *(buff + i) <= 126) {
            word_length++; 
        } else {
                printf("%d. %.*s(%d)\n", count, word_length, buff + i - word_length, word_length);
                word_length = 0; 
                count++;
        }
    }

        printf("%d. %.*s(%d)\n", count, word_length, buff + len - word_length, word_length);

    
}



//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]) {

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if argv[1] does not exist?
    // PLACE A COMMENT BLOCK HERE EXPLAINING
    // This is safe because we check if argc is less than 2 or if argv[1] doesn't start with '-', 
    // in which case, the programs exit with error code 1. 
    if ((argc < 2) || (*argv[1] != '-')) {
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1] + 1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h') {
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    // PLACE A COMMENT BLOCK HERE EXPLAINING
    // The if statement checks if there are less than 3 arguments, which means no string to process.
    if (argc < 3) {
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = (char *) malloc(BUFFER_SZ);
    if (buff == NULL) {
        exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0) {
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt) {
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0) {
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r':
            reverse_string(buff, user_str_len);
            break;

        case 'w':
            rc = count_words(buff, BUFFER_SZ, user_str_len);

            if (rc < 0) {
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Print\n");
            printf("----------\n");
            print_word(buff, user_str_len);
            printf("\nNumber of words returned: %d\n", rc);
            break;

        case 'x':
            printf("Not Implemented!\n");
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Don't forget to free your buffer before exiting
    print_buff(buff, BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          This is to ensure safe code execution, for example you can check if the length of the buff matches the length
//          of the actual string or if the length of the string exeed the length of the buffer.