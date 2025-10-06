#include <stdio.h>
#include <term.h>

#define QUESTION "Do you want another transanction"

int get_response(char *);

int main() {
    int response;
    response = get_response(QUESTION);
    return response;
}   

int get_response(char *question) {
    // 0=>YES, 1=>NO
    printf("%s(y/n)?", question);
    while (1) {
        switch(getchar()) {
            case 'Y':
            case 'y': return 0;
            case 'n':
            case 'N':
            case EOF: return 1; 
        }
    }
}
