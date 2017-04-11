#include <stdio.h>

/*
    TODO:
        - Figure out what commands are needed
        - Install LIRC on rasPi
        - Configure GPIO with LIRC
        - irrecord for config file for samsung remote
        - Test LIRC signal functionality
*/

int main(int argc, char **argv){
    if(argc < 2){
        return -1;
        printf("No command received\n");
    }

    char command[50];


    switch(argv[1][0]){
        case 'v':
            if(argv[1][1] == 'u'){
                strcpy( command, "irsend SEND_ONCE samsung KEY_VOLUMEUP");
                printf("Volume Up\n");
            } else {
                strcpy( command, "irsend SEND_ONCE samsung KEY_VOLUMEDOWN");
                printf("Volume Down\n");
            }
            break;
        case 'c':
            if(argv[1][1] == 'u'){
                strcpy( command, "irsend SEND_ONCE samsung KEY_CHANNELUP");
                printf("Channel Up\n");
            } else {
                strcpy( command, "irsend SEND_ONCE samsung KEY_CHANNELDOWN");
                printf("Channel Down\n");
            }
            break;
        case 'o':
            strcpy( command, "irsend SEND_ONCE samsung KEY_POWER");
            printf("On/Off\n");
            break;
        default:
            printf("Invalid command received\n");
            return -1;
    }
    system(command);
    return 0;
}