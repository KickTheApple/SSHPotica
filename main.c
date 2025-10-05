//
// Created by domenic on 10/5/25.
//

#include "main.h"

#include <stdlib.h>
#include <stdio.h>

#include <libssh/libssh.h>
#include <libssh/server.h>

int main(int argc, char* args[]) {

    ssh_session conecctionation;
    int verbosity = SSH_LOG_PROTOCOL;
    int portland = 22;

    int povezava;

    conecctionation = ssh_new();
    if (conecctionation == NULL) {
        exit(-1);
    }

    ssh_options_set(conecctionation, SSH_OPTIONS_HOST, "localhost");
    ssh_options_set(conecctionation, SSH_OPTIONS_PORT, &portland);
    ssh_options_set(conecctionation, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

    povezava = ssh_connect(conecctionation);
    if (povezava != SSH_OK) {
        fprintf(stderr, "ERROR CONNECTING to LOCALHOST: %s\n", ssh_get_error(conecctionation));
        exit(-1);
    }

    ssh_disconnect(conecctionation);
    ssh_free(conecctionation);
    return 0;

}