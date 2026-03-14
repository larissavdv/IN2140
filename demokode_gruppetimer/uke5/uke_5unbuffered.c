#define BUFSIZE 255
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void) {
    char buf[BUFSIZE + 1];
    const char *msg = "Dette er gruppe 2!";
    size_t msg_len = strlen(msg);

    int fd = open("unbuffered.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    size_t wc = write(fd, msg, msg_len);
    if (wc == -1) {
        perror("write");
        close(fd);
        return -1;
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return -1;
    }

    size_t rc = read(fd, buf, BUFSIZE);
    if (rc == -1) {
        perror("read");
        close(fd);
        return -1;
    }

    buf[rc] = '\0';

    printf("I bufferet ligger det: %s\n", buf);

    close(fd);
    return 0;
}