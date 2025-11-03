//
// Created by domenic on 11/2/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int basher2_MoreBashass(int kidPair[2], int parentPair[2]) {

    if (pipe(kidPair) == -1 || pipe(parentPair) == -1) {
        printf("Problem with Smoking Pipes\n");
        return -1;
    }

    int forky = fork();

    if (forky == -1) {
        printf("Problem with Smoking Pipes\n");
        return -1;
    }

    if (forky == 0) {

        close(kidPair[1]);
        close(parentPair[0]);

        dup2(kidPair[0], STDIN_FILENO);
        dup2(parentPair[1], STDOUT_FILENO);
        dup2(parentPair[1], STDERR_FILENO);

        execl("/usr/bin/docker", "docker", "run", "-i", "--rm", "--entrypoint", "/bin/sh", "--net", "none", "bash", (char *) NULL);
        printf("bin bang bash error\n");
        exit(1);

    }

    return forky;

}