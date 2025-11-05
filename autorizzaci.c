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
        fprintf(writer, "%s", buffer);
    }
    fclose(writer);
    return 0;
}

int theGreatReplacer2(char* data, int capacity, char origin, char replacee) {
    char datus[strlen(data)*2];
    printf("%s\n", data);

    int zamikovalec = 0;
    for (int i = 0; i < strlen(data); i++) {
        datus[i+zamikovalec] = data[i];
        if (data[i] == origin) {
            zamikovalec += 1;
            datus[i+zamikovalec] = replacee;
        }
    }
    datus[strlen(data)+zamikovalec] = '\0';
    printf("%s\n", datus);
    memcpy(data, datus, capacity);
    data[capacity-1] = '\0';
    return 0;
}

// beseda\nxdxdxd -> beseda\r\nxdxdxd

int theGreatReplacer(char* data, int capacity, char origin, char replacee) {
    char datus[strlen(data)*2];
    int zamikovalec = 0;
    for (int i = 0; i < strlen(data); i++) {
        if (data[i] == origin) {
            datus[i+zamikovalec] = replacee;
            zamikovalec += 1;
        }
        datus[i+zamikovalec] = data[i];
    }
    datus[strlen(data)+zamikovalec] = '\0';
    memcpy(data, datus, capacity);
    data[capacity-1] = '\0';
    return 0;
}

int theGreatNotReplacer(char* data, char origin, char replacee) {
    char *datus = data;
    while ((datus = strchr(datus, origin)) != NULL) {
        *datus = replacee;
        datus++;
    }
    return 0;
}

void *bashStran(void *zadeve) {
    struct latchBatch* sadge = zadeve;

    ssize_t n;
    char bashBuffer[4096];
    while ((n = read(sadge->pipca, bashBuffer, sizeof(bashBuffer))) > 0) {
        bashBuffer[n] = '\0';
        printf("%s", bashBuffer);
        dataLog(sadge->fileName, bashBuffer);
        ssh_channel_write(sadge->aKanal, bashBuffer, n);
    }
    printf("bananas\n");

    return NULL;
}

int shellRuntime(conInformation* information) {
    char fileName[100];
    char buffer[256];
    int nbytes;

    int master;
    int forky = basher2_MoreBashass(&master);

    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    pthread_t pthread;

    strftime(fileName, sizeof(fileName), "operations_%Y%m%d_%H%M%S.txt", time);

    struct latchBatch* ananasBalls = malloc(sizeof(struct latchBatch));
    ananasBalls->aKanal = information->channel;
    ananasBalls->pipca = master;
    memcpy(ananasBalls->fileName, fileName, sizeof(fileName));
    pthread_create(&pthread, NULL, bashStran, ananasBalls);

    while (ssh_channel_is_open(information->channel) && !ssh_channel_is_eof(information->channel)) {
        nbytes = ssh_channel_read(information->channel, buffer, sizeof(buffer), 0);
        if (nbytes < 0) {
            break;
        }
        if (nbytes > 0) {
            if (buffer[0] == 4) {
                break;
            }
            buffer[nbytes] = '\0';
            write(master, buffer, strlen(buffer));
        }
    }

    printf("End of ends\n");
    close(master);
    close(forky);
    free(ananasBalls);

    kill(forky, 15);
    waitpid(forky, NULL, 0);
    ssh_channel_close(information->channel);
    ssh_channel_send_eof(information->channel);
    ssh_channel_free(information->channel);
    return -1;
}




