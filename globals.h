#include <windows.h>

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "80"
#define path "C:\\oneir\\"
#define path_to_ini_file  "C:\\oneir\\ONEIR.ini"
#define telnet98 "C:\\ONEIR\\Dev3-Cenedex.etx"
#define terminal "C:\\ONEIR\\terminal.etx"
#define username 0
#define session_file_1 1
#define session_file_2 2
#define session_file_3 3
#define CREDENTIAL_SIZE_LIMIT 4
#define CREDENTIAL_SPACE 100
#define REMOTE_HOST_IP "shielded-hollows-65276.herokuapp.com"
using namespace std;
// global variable
int choice = 0;
int CREDENTIALS_SIZE = 0;
char* credentials[CREDENTIAL_SIZE_LIMIT];
// Macros
#define _ETNAPI_H
#define ETNBUFSIZ       0x2000

HWND erik98 = NULL;
