//
// Created by domenic on 10/8/25.
//

#include "autorizzaci.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <libssh/libssh.h>
#include <libssh/server.h>

int logging(conInformation* information) {
    FILE* writer = fopen("loginAttempts.txt", "a");

    fprintf(writer, "%s - %s\n", information->username, information->password);

    fclose(writer);
    return 0;
}

int dataLog(char* fileName, char* buffer) {
    FILE* writer = fopen(fileName, "a");

    if (buffer[0] == 13) {
        fprintf(writer, "\n");
    } else if (buffer[0] == 127) {
        fprintf(writer, "-");
    } else {
        fprintf(writer, "%c", buffer[0]);
    }
    fclose(writer);
    return 0;
}

int shellRuntime(conInformation* information) {
    char fileName[100];
    char buffer[256];
    int nbytes;

    time_t now = time(NULL);
    struct tm *time = localtime(&now);

    strftime(fileName, sizeof(fileName), "operations_%Y%m%d_%H%M%S.txt", time);

    while (ssh_channel_is_open(information->channel) && !ssh_channel_is_eof(information->channel)) {
        nbytes = ssh_channel_read(information->channel, buffer, sizeof(buffer), 0);
        if (nbytes < 0) {
            return SSH_ERROR;
        }
        if (nbytes > 0) {
            if (buffer[0] == 3) {
                ssh_channel_write(information->channel, "\r\nGoodbye\r\n", strlen("\r\nGoodbye\r\n"));
                ssh_channel_close(information->channel);
                ssh_channel_send_eof(information->channel);
                ssh_channel_free(information->channel);
                return 0;
            }

            if (buffer[0] == 13) {
                ssh_channel_write(information->channel, "\r\n", strlen("\r\n"));
            } else if (buffer[0] == 127) {
                ssh_channel_write(information->channel, "\b", strlen("\b"));
            } else {
                ssh_channel_write(information->channel, buffer, nbytes);
            }
            dataLog(fileName, buffer);
            printf("%d\n", buffer[0]);
        }
    }
    return -1;
}




