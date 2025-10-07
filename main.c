//
// Created by domenic on 10/5/25.
//

#include "main.h"

#include <stdlib.h>
#include <stdio.h>

#include <libssh/libssh.h>
#include <libssh/server.h>

struct poveznik {

    ssh_session secija;
    ssh_bind binding;

    char* connAddr;
    int portland;
    int verbosity;

    char *ecdsa_key;
    char *ed25519_key;
    char *rsa_key;
    char *host_key;


    //char* username; // root
    //char* password; // 12345678

};

int main(int argc, char* args[]) {
    struct poveznik povezovalec;

    povezovalec.portland = 2222;
    povezovalec.verbosity = SSH_LOG_PROTOCOL;

    povezovalec.secija = ssh_new();

    if (povezovalec.secija == NULL) {
        exit(-1);
    }
    povezovalec.binding = ssh_bind_new();

    ssh_bind_options_set(povezovalec.binding, SSH_BIND_OPTIONS_BINDADDR, "localhost");
    ssh_bind_options_set(povezovalec.binding, SSH_BIND_OPTIONS_BINDPORT, &povezovalec.portland);
    ssh_bind_options_set(povezovalec.binding, SSH_BIND_OPTIONS_LOG_VERBOSITY, &povezovalec.verbosity);
    ssh_bind_options_set(povezovalec.binding, SSH_BIND_OPTIONS_HOSTKEY, PUBKEY);


    if (ssh_bind_listen(povezovalec.binding) < 0) {
        printf("Error, zakaj. Zato: %s", ssh_get_error(socket));
        return -1;
    }

    printf("MARKO-\n");


    while (1) {
        if (ssh_bind_accept(povezovalec.binding, povezovalec.secija) == SSH_ERROR) {
            printf("WOMP WOMP\n");
            return -1;
        }

        printf("POLO\n");

        switch (fork()) {
            case -1:
                fprintf(stderr, "Fork returned error: `%d'.\n",-1);
                exit(-1);
            case 0:
                printf("Success");
                break;
            default:
                continue;
        }
    }
    ssh_disconnect(povezovalec.secija);
    ssh_free(povezovalec.secija);
    ssh_bind_free(povezovalec.binding);
    return 0;

}