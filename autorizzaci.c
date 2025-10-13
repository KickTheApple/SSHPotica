//
// Created by domenic on 10/8/25.
//

#include "autorizzaci.h"
#include <stdlib.h>
#include <stdio.h>

#include <libssh/libssh.h>
#include <libssh/server.h>

int logging(conInformation* information) {
    FILE* writer = fopen("loginAttempts.txt", "a");

    fprintf(writer, "%s - %s\n", information->username, information->password);

    return 0;
}

int shellRuntime(conInformation* information) {
    char buffer[256];
    int nbytes;

    printf("WOMPUS\n");

    nbytes = ssh_channel_read(information->channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0)
    {
        if (fwrite(buffer, 1, nbytes, stdout) != nbytes)
        {
            ssh_channel_close(information->channel);
            ssh_channel_free(information->channel);
            return SSH_ERROR;
        }
        if (nbytes > 0) {
            ssh_channel_write(information->channel, buffer, nbytes);
        }
        nbytes = ssh_channel_read(information->channel, buffer, sizeof(buffer), 0);
    }

    if (nbytes < 0)
    {
        printf("LOl4\n");

        ssh_channel_close(information->channel);
        ssh_channel_free(information->channel);
        return SSH_ERROR;
    }
}




