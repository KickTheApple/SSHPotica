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

    ssh_channel channel;
    int rc;
    char buffer[256];
    int nbytes;

    channel = ssh_message_channel_request_open_reply_accept(information->sporocilo);
    if (channel == NULL) {
        printf("PROBLEM s SHELL1\n");
        return SSH_ERROR;
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK)
    {
        printf("PROBLEM s SHELL2\n");
        ssh_channel_free(channel);
        return rc;
    }

    rc = ssh_channel_request_pty(channel);
    if (rc != SSH_OK) {
        printf("PROBLEM s SHELL3\n");
        return rc;
    }
    rc = ssh_channel_change_pty_size(channel, 80, 24);
    if (rc != SSH_OK) {
        printf("PROBLEM s SHELL4\n");
        return rc;
    }

    rc = ssh_channel_request_shell(channel);
    if (rc != SSH_OK) {
        printf("PROBLEM s SHELL5\n");
        return rc;
    }

    while (ssh_channel_is_open(channel) &&
           !ssh_channel_is_eof(channel))
    {
        printf("PROBLEM s SHELL6\n");

        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
        if (nbytes < 0)
            return SSH_ERROR;

        if (nbytes > 0)
            write(1, buffer, nbytes);
    }1


    ssh_channel_close(channel);
    ssh_channel_send_eof(channel);
    ssh_channel_free(channel);

    return SSH_OK;

}




