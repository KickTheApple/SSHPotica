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

    char* ip;
    char* port;

    char* username;
    char* password;

    char* commands;

} conInformation;

#endif //SSHPOTICA_AUTORIZZACI_H