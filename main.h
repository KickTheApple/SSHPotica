//
// Created by domenic on 10/5/25.
//
#ifndef SSHPOTICA_MAIN_H
#define SSHPOTICA_MAIN_H

#include <libssh/libssh.h>
#include <libssh/server.h>

#define ALGO "ssh-rsa"
#define PUBKEY "./ssh_key"

typedef struct poveznik {

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

} poveznik;



#endif //SSHPOTICA_MAIN_H