//
// Created by domenic on 10/8/25.
//

#include "autorizzaci.h"
#include <stdlib.h>
#include <stdio.h>

#include <libssh/libssh.h>
#include <libssh/server.h>

int logging(conInformation information) {
    FILE* writer = fopen("loginAttempts.txt", "a");

    fprintf(writer, "%s - %s\n", information.username, information.password);

    return 0;
}

int shellRuntime(conInformation information) {

    ssh_channel aKanal;
    int rc = 0;

    aKanal = ssh_channel_new(information.currentSes);

    return 0;

}




