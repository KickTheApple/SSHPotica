//
// Created by domenic on 10/8/25.
//

#ifndef SSHPOTICA_AUTORIZZACI_H
#define SSHPOTICA_AUTORIZZACI_H

#include <libssh/libssh.h>
#include <libssh/server.h>

typedef struct connectionData {

    ssh_session currentSes;
    ssh_message sporocilo;
    ssh_channel channel;

    char* ip;
    int port;

    char* username;
    char* password;
    char* pubkey;

    char* commands;

} conInformation;

int logging(conInformation*);
int shellRuntime(conInformation*);

#endif //SSHPOTICA_AUTORIZZACI_H