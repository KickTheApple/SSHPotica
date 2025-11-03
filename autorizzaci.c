//
// Created by domenic on 10/8/25.
//

#include "autorizzaci.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <basher.h>
#include <pthread.h>
#include <libssh/libssh.h>
#include <libssh/server.h>
#include <arpa/inet.h>
#include <sys/wait.h>

char* getClientIp(ssh_session session) {

    struct sockaddr_storage tmp;
    struct sockaddr_in *sock;
    unsigned int len = 100;
    char ip[100] = "\0";

    getpeername(ssh_get_fd(session), (struct sockaddr*)&tmp, &len);
    sock = (struct sockaddr_in *)&tmp;
    inet_ntop(AF_INET, &sock->sin_addr, ip, len);

    char* ip_str = malloc(sizeof(ip) + 1);
    strcpy(ip_str, ip);

    return ip_str;
}

int logging(conInformation* information) {
    FILE* writer = fopen("loginAttempts.txt", "a");

    fprintf(writer, "%s - %s - %s\n", information->username, information->password, information->ip);

    fclose(writer);
    return 0;
}

int dataLog(char* fileName, char* buffer) {
    FILE* writer = fopen(fileName, "a");

    if (buffer[0] == 13) {
        fprintf(writer, "\n");
    } else if (buffer[0] == 127) {
        fprintf(writer, "-");
    } else {
        fprintf(writer, "%c", buffer[0]);
    }
    fclose(writer);
    return 0;
}

void *bashStran(void *zadeve) {
    struct latchBatch* sadge = zadeve;

    ssize_t n;
    char bashBuffer[4096];
    printf("printf\n");
    while ((n = read(sadge->pipca[0], bashBuffer, sizeof(bashBuffer))) > 0) {
        bashBuffer[n] = '\0';
        ssh_channel_write(sadge->aKanal, bashBuffer, n);
        printf("%s", bashBuffer);
    }
    printf("printf\n");


    return NULL;
}

int shellRuntime(conInformation* information) {
    char fileName[100];
    char buffer[256];
    int nbytes;

    int childSide[2];
    int daddySide[2];
    int forky = basher2_MoreBashass(childSide, daddySide);

    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    pthread_t pthread;

    strftime(fileName, sizeof(fileName), "operations_%Y%m%d_%H%M%S.txt", time);

    if (ssh_channel_is_open(information->channel) && !ssh_channel_is_eof(information->channel)) {
        ssh_channel_write(information->channel, "Welcome user: root\r\n", strlen("Welcome user: root\r\n"));
    }
    printf("Test\n");

    struct latchBatch* ananasBalls = malloc(sizeof(struct latchBatch));
    ananasBalls->aKanal = information->channel;
    ananasBalls->pipca = daddySide;
    pthread_create(&pthread, NULL, bashStran, ananasBalls);
    printf("Test\n");

    while (ssh_channel_is_open(information->channel) && !ssh_channel_is_eof(information->channel)) {
        nbytes = ssh_channel_read(information->channel, buffer, sizeof(buffer), 0);
        if (nbytes < 0) {
            break;
        }
        if (nbytes > 0) {
            buffer[nbytes] = '\0';
            ssh_channel_write(information->channel, buffer, nbytes);
            write(childSide[1], buffer, strlen(buffer));
            write(childSide[1], "\n", strlen("\n"));
            dataLog(fileName, buffer);
            printf("%s - %d\n", buffer, nbytes);
        }
    }

    printf("End of ends\n");
    close(childSide[0]);
    close(childSide[1]);
    close(daddySide[0]);
    close(daddySide[1]);
    free(ananasBalls);

    kill(forky, 15);
    waitpid(forky, NULL, 0);
    ssh_channel_close(information->channel);
    ssh_channel_send_eof(information->channel);
    ssh_channel_free(information->channel);
    return -1;
}




