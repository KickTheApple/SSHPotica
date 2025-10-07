//
// Created by domenic on 10/5/25.
//
#ifndef SSHPOTICA_MAIN_H
#define SSHPOTICA_MAIN_H

#define ALGO "ssh-rsa"
#define PUBKEY "/etc/ssh/ssh_host_rsa_key"
#define SUBKEY "/etc/ssh/ssh_host_ecdsa_key"

struct connectionData {

    char* ip;
    char* port;

    char* username;
    char* password;

    char* commands;

};



#endif //SSHPOTICA_MAIN_H