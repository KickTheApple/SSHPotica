//
// Created by domenic on 10/5/25.
//

#include "main.h"
#include "autorizzaci.h"

#include <stdlib.h>
#include <stdio.h>

#include <libssh/libssh.h>
#include <libssh/server.h>





void ubijalnikSocketa(poveznik* povezovalec) {
    free(povezovalec->secija);
    free(povezovalec->binding);
    free(povezovalec->connAddr);
    free(povezovalec->ecdsa_key);
    free(povezovalec->ed25519_key);
    free(povezovalec->rsa_key);
    free(povezovalec->host_key);

    free(povezovalec);
}

void ubijalnikConnectiona(conInformation* connection_data) {
    free(connection_data->ip);
    free(connection_data->username);
    free(connection_data->password);
    free(connection_data->commands);

    free(connection_data);
}




int testValidity(conInformation* information) {
    int rc = 0;

    if (ssh_handle_key_exchange(information->currentSes) != SSH_OK) {
        printf("Exhange Failed\n");
        return -1;
    }
    printf("Exchange succeeded\n");

    while (1) {
        information->sporocilo = ssh_message_get(information->currentSes);
        if (information->sporocilo == NULL) {
            break;
        }

        if (ssh_message_subtype(information->sporocilo) == SSH_AUTH_METHOD_PASSWORD) {
            information->username = ssh_message_auth_user(information->sporocilo);
            information->password = ssh_message_auth_password(information->sporocilo);

            printf("Auth attempt: user=%s, password=%s\n", information->username, information->password);
            ssh_message_auth_reply_success(information->sporocilo, 0);

            logging(information);
            shellRuntime(information);

            return 1;
        }

        ssh_message_reply_default(information->sporocilo);
        ssh_message_free(information->sporocilo);
    }

    return rc;
}

int dogajalnik(poveznik* povezovalec) {

    conInformation* connection_data = malloc(sizeof(conInformation));
    povezovalec->secija = ssh_new();


    if (povezovalec->secija == NULL) {
        exit(-1);
    }
    povezovalec->binding = ssh_bind_new();

    ssh_bind_options_set(povezovalec->binding, SSH_BIND_OPTIONS_BINDADDR, povezovalec->connAddr);
    ssh_bind_options_set(povezovalec->binding, SSH_BIND_OPTIONS_BINDPORT, &povezovalec->portland);
    ssh_bind_options_set(povezovalec->binding, SSH_BIND_OPTIONS_LOG_VERBOSITY, &povezovalec->verbosity);
    ssh_bind_options_set(povezovalec->binding, SSH_BIND_OPTIONS_HOSTKEY, PUBKEY);



    if (ssh_bind_listen(povezovalec->binding) < 0) {
        printf("Error, zakaj. Zato: %s", ssh_get_error(socket));
        ubijalnikSocketa(povezovalec);

        return -1;
    }

    printf("MARKO-\n");


    while (1) {
        if (ssh_bind_accept(povezovalec->binding, povezovalec->secija) == SSH_ERROR) {
            printf("WOMP WOMP\n");
            ubijalnikSocketa(povezovalec);
            return -1;
        }

        printf("POLO\n");

        switch (fork()) {
            case -1:
                fprintf(stderr, "Fork returned error: `%d'.\n",-1);
                exit(-1);
            case 0:
                connection_data->currentSes = povezovalec->secija;
                connection_data->ip = povezovalec->connAddr;
                connection_data->port = povezovalec->portland;
                exit(testValidity(connection_data));
            default:
                continue;
        }
    }

    return 0;

}

int main(int argc, char* args[]) {

    poveznik* povezovalec = calloc(1, sizeof(poveznik));

    povezovalec->connAddr = "localhost";
    povezovalec->portland = 2222;
    povezovalec->verbosity = SSH_LOG_PROTOCOL;


    dogajalnik(povezovalec);

    ssh_disconnect(povezovalec->secija);
    ssh_free(povezovalec->secija);
    ssh_bind_free(povezovalec->binding);
    return 0;

}