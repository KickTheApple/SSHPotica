//
// Created by domenic on 11/2/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pty.h>

int basher2_MoreBashass(int *master) {

    int masterPd;
    int forky = forkpty(&masterPd, NULL, NULL, NULL);

    if (forky == -1) {
        printf("Problem with Smoking Pipes\n");
        return -1;
    }

    if (forky == 0) {

        execl("/usr/bin/docker", "docker", "run", "-ti", "--rm", "--entrypoint", "/bin/sh", "--net", "none", "bash", "-i", (char *) NULL);
        printf("bin bang bash error\n");
        exit(1);

    }

    *master = masterPd;
    return forky;

}