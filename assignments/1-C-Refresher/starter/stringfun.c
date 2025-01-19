#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int count_words(char *, int, int);
int reverse_string(char *, int, int);
int individual_words(char *, int, int);
int word_search_replace(char *, int, char *, char *);

//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    int str_len = 0;
    char *ptr = user_str;
    while (*ptr != '\0'){
        str_len++;
        ptr++;
    }
    if (str_len > len){
        fprintf(stderr, "Error: user string too long\n");
        exit(3);
    }

    // Copy the user string into the buffer
    char *dest = buff;
    char *src = user_str;
    while (*src != '\0'){
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';
    return str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
    int in_word = 0; // False

    for (int i=0; i < str_len; i++){
        char c = *(buff+i);
        // If the character is a space
        if (c == ' '){
            in_word = 0;
        } else {
            if (!in_word){
                word_count++;
                in_word = 1; // True
            }
        }
    }
    return word_count;
}

int reverse_string(char *buff, int len, int str_len){
    char *start = buff;
    char *end = buff + str_len - 1;
    char temp;

    // Pointers start and end move towards each other, swapping characters
    while (start < end){
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int individual_words(char *buff, int len, int str_len){
    int word_count = 0;
    int in_word = 0; // False
    int word_len = 0;
    char *word_start = buff;
    
    printf("Word Print\n");
    printf("----------\n");

    for (int i=0; i < str_len; i++){
        char c = *(buff + i);

        if (c == ' ' || c == '\0') {
            // End of a word
            if (in_word) {
                word_count++;
                printf("%d. ", word_count); // Print word number
                for (char *ptr = word_start; ptr < word_start + word_len; ptr++) { // Print word
                    putchar(*ptr);
                }
                printf(" (%d)\n", word_len); // Print word length
                in_word = 0;
                word_len = 0;
            }
        } else {
            // Inside a word
            if (!in_word) {
                word_start = buff + i;
                in_word = 1;
            }
            word_len++;
        }
    }

    if (in_word) {
        word_count++;
        printf("%d. ", word_count);
        for (char *ptr = word_start; ptr < word_start + word_len; ptr++) {
            putchar(*ptr);
        }
        printf(" (%d)\n", word_len);
    }
}
int word_search_replace(char *buff, int len, char *search_word, char *replace_word) {
    char temp_buff[BUFFER_SZ];
    char *write_ptr = temp_buff;
    char *read_ptr = buff;
    char *search_ptr, *replace_ptr;
    int search_len = 0;
    int replace_len = 0;

    for (search_ptr = search_word; *search_ptr != '\0'; search_ptr++) {
        search_len++;
    }
    for (replace_ptr = replace_word; *replace_ptr != '\0'; replace_ptr++) {
        replace_len++;
    }

    while (*read_ptr != '\0') {
        // Check if the current substring matches search_word
        char *match_ptr = read_ptr;
        search_ptr = search_word;
        while (*match_ptr == *search_ptr && *search_ptr != '\0') {
            match_ptr++;
            search_ptr++;
        }

        // If a match is found, replace it with replace_word
        if (search_ptr == search_word + search_len && (*match_ptr == ' ' || *match_ptr == '\0')) {
            replace_ptr = replace_word;
            while (*replace_ptr != '\0') {
                *write_ptr = *replace_ptr;
                write_ptr++;
                replace_ptr++;
            }
            read_ptr += search_len; // Skip over the matched search_word
        } else {
            // Copy the current character to temp_buff
            *write_ptr = *read_ptr;
            write_ptr++;
            read_ptr++;
        }
    }

    *write_ptr = '\0';

    char *src = temp_buff;
    char *dst = buff;
    while (*src != '\0' && (dst - buff) < len - 1) {
        *dst = *src;
        src++;
        dst++;
    }

    *dst = '\0';
    return 0;
}



//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    /*
        -- WHY IS THIS SAFE, aka what if arv[1] does not exist?
        The left hand of the OR statement is checked first, if it evalutes to to true, the right hand side is not checked.
        If argc is less than 2 (aka. argv[1] Does Not Exist), then the program will exit with an error code of 1.
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    /*
        -- #2 Document the purpose of the if statement below
        This if statement checks if the number of arguments is less than 3, 
        if it is, the program will print the usage and exit with an error code of 1.
    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //          Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL){
        printf("Error allocating buffer, error = 99");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r':
            rc = reverse_string(buff, BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error reversing string, rc = %d", rc);
                exit(2);
            }
            printf("Reversed String: ");
            for (int i = 0; i < user_str_len; i++) {
                putchar(buff[i]);
            }
            putchar('\n');
            break;

        case 'w':
            rc = individual_words(buff, BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error printing individual words, rc = %d", rc);
                exit(2);
            }
            break;
        
        case 'x':
            if (argc != 5) {
                printf("Error: Missing arguments for word search and replace.\n");
                usage(argv[0]);
                exit(1);
            }

            rc = word_search_replace(buff, BUFFER_SZ, argv[3], argv[4]);
            if (rc < 0) {
                printf("Error replacing words, rc = %d\n", rc);
                exit(2);
            }

            printf("Modified String: ");
            print_buff(buff, BUFFER_SZ);
            break;


        default:
            usage(argv[0]);
            exit(1);
    }

    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE

/*
    Doing so ensures safety, flexibility, and reusability. 
    - It prevents buffer overflows by keeping operations within bounds and allows the functions to handle buffers of varying sizes
    - It improves code readability and error handling by explicitly communicating the expected buffer size
*/