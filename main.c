//
// Created by domenic on 10/5/25.
//

#include "main.h"
#include "autorizzaci.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libssh/libssh.h>
#include <libssh/server.h>

#include <sys/file.h>
#include <sys/types.h>

#include <sys/wait.h>

#include <signal.h>

#define LOCKFILE "/tmp/fork_lockfile"

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

void sklicalecSignalov(int signalizator) {
    printf("Caught signal %d\n",signalizator);

    while (waitpid(-1, NULL, WNOHANG) > 0);

    exit(signalizator);
}

int firstLog(conInformation* information) {
    FILE* writer = fopen("ipList.txt", "a");

    fprintf(writer, "%s\n", information->ip);

    fclose(writer);
    return 0;
}



int testValidity(conInformation* information, poveznik* povezovalec) {
    int rc = 0;
    ssh_channel channel;

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

            printf("Recieved password request: 1\n");

            printf("Auth attempt: user=%s, password=%s\n", information->username, information->password);
            if (strcmp(information->username, "root") == 0 && strcmp(information->password, "root") == 0) {
                ssh_message_auth_reply_success(information->sporocilo, 0);
                logging(information);
                ssh_message_free(information->sporocilo);
                continue;
            }
            logging(information);

        } if (ssh_message_subtype(information->sporocilo) == SSH_CHANNEL_SESSION && ssh_message_type(information->sporocilo) == SSH_REQUEST_CHANNEL_OPEN) {
            printf("Recieved channel request: 2\n");

            channel = ssh_message_channel_request_open_reply_accept(information->sporocilo);
            if (channel == NULL) {
                printf("Failed channel handling 1\n");
                return SSH_ERROR;
            }
            information->channel = channel;
            ssh_message_channel_request_reply_success(information->sporocilo);
            ssh_message_free(information->sporocilo);
            continue;

        }

        if (ssh_message_subtype(information->sporocilo) == SSH_CHANNEL_REQUEST_PTY && ssh_message_type(information->sporocilo) == SSH_REQUEST_CHANNEL) {
            printf("Received terminal reqeust: 3\n");

            ssh_message_channel_request_reply_success(information->sporocilo);
            ssh_message_free(information->sporocilo);
            continue;

        }

        if (ssh_message_subtype(information->sporocilo) == SSH_CHANNEL_REQUEST_SHELL && ssh_message_type(information->sporocilo) == SSH_REQUEST_CHANNEL) {
            printf("Recieved shell reuqest 4:\n");

            ssh_message_channel_request_reply_success(information->sporocilo);
            ssh_message_free(information->sporocilo);
            if (information->channel != NULL) {
                shellRuntime(information);
            }
            continue;

        }


        ssh_message_reply_default(information->sporocilo);
        ssh_message_free(information->sporocilo);
    }

    return rc;
}

int dogojalnik(poveznik* povezovalec) {

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
        conInformation* connection_data = malloc(sizeof(conInformation));
        if (ssh_bind_accept(povezovalec->binding, povezovalec->secija) == SSH_ERROR) {
            printf("WOMP WOMP\n");
            ubijalnikSocketa(povezovalec);
            return -1;
        }

        printf("POLO\n");

        int forky;

        switch (fork()) {
            case -1:
                fprintf(stderr, "Fork returned error: `%d'.\n",-1);
                exit(-1);
            case 0:

                forky = fork();
                switch (forky) {
                    case -1:
                        fprintf(stderr, "Fork returned error: `%d'.\n",-1);
                        exit(-1);
                    case 0:
                        connection_data->pid = getpid();
                        connection_data->currentSes = povezovalec->secija;
                        povezovalec->secija = ssh_new();

                        connection_data->ip = getClientIp(connection_data->currentSes);
                        firstLog(connection_data);
                        connection_data->port = povezovalec->portland;
                        int rpd = testValidity(connection_data, povezovalec);
                        exit(rpd);
                    default:
                        waitpid(forky, NULL, 0);
                        exit(0);
                }

            default:
                continue;
        }
    }

    return 0;

}

int main(int argc, char* args[]) {

    poveznik* povezovalec = calloc(1, sizeof(poveznik));

    povezovalec->connAddr = "0.0.0.0";
    povezovalec->portland = 22;
    povezovalec->verbosity = SSH_LOG_PROTOCOL;

    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGCHLD, SIG_IGN);

    dogojalnik(povezovalec);

    ssh_disconnect(povezovalec->secija);
    ssh_free(povezovalec->secija);
    ssh_bind_free(povezovalec->binding);
    return 0;

}