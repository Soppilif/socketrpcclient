#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    printf("Trying to connect...\n");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    printf("CONNECTED....\n");
    int choice, n, i, running = 1;
    int *X = NULL, *Y = NULL;
    double r;

    while (running) {
        printf("\n========= ΜΕΝΟΥ =========\n");
        printf("1. Εσωτερικό γινόμενο διανυσμάτων\n");
        printf("2. Μέσος όρος διανυσμάτων\n");
        printf("3. Πολλαπλασιασμός διανύσματος με r\n");
        printf("0. Έξοδος\n");
        printf("=========================\n");
        printf("Δώσε επιλογή: ");
        scanf("%d", &choice);


        if (write(sockfd, &choice, sizeof(int)) < 0) error("ERROR writing to socket");

        while (choice != 0 && choice != 1 && choice != 2 && choice != 3){
            printf("Άκυρη επιλογή!\n");
            printf("Δώσε επιλογή: ");
            scanf("%d", &choice);
        }

        if (choice == 0) {
            printf("Αποσύνδεση.\n");
            break;
        }

        printf("Δώσε το μέγεθος n του διανύσματος: ");
        scanf("%d", &n);
        if (write(sockfd, &n, sizeof(int)) < 0) error("ERROR writing to socket");


        X = malloc(n * sizeof(int));
        if (!X) error("malloc X");
        printf("Δώσε τα %d στοιχεία του διανύσματος X:\n", n);
        for (i = 0; i < n; i++) scanf("%d", &X[i]);
        if (write(sockfd, X, n * sizeof(int)) < 0) error("ERROR writing X");

        if (choice == 1 || choice == 2) {
            Y = malloc(n * sizeof(int));
            if (!Y) error("malloc Y");
            printf("Δώσε τα %d στοιχεία του διανύσματος Y:\n", n);
            for (i = 0; i < n; i++) scanf("%d", &Y[i]);
            if (write(sockfd, Y, n * sizeof(int)) < 0) error("ERROR writing Y");
        }

        if (choice == 3) {
            printf("Δώσε τον αριθμό r: ");
            scanf("%lf", &r);
            if (write(sockfd, &r, sizeof(double)) < 0) error("ERROR writing r");
        }


        if (choice == 1) {
            int res;
            if (read(sockfd, &res, sizeof(int)) != sizeof(int)) error("ERROR reading scalar product");
            printf("Εσωτερικό γινόμενο: %d\n", res);
        } else if (choice == 2) {
            double ex, ey;
            if (read(sockfd, &ex, sizeof(double)) != sizeof(double)) error("ERROR reading ex");
            if (read(sockfd, &ey, sizeof(double)) != sizeof(double)) error("ERROR reading ey");
            printf("Μέσος όρος X: %.2f, Μέσος όρος Y: %.2f\n", ex, ey);
        } else if (choice == 3) {
            double *out = malloc(n * sizeof(double));
            if (!out) error("malloc out");
            if (read(sockfd, out, n * sizeof(double)) != n * (int)sizeof(double)) error("ERROR reading result array");
            printf("Αποτέλεσμα πολλαπλασιασμού:\n");
            for (i = 0; i < n; i++) printf("%.2f ", out[i]);
            printf("\n");
            free(out);
        }


        free(X); X = NULL;
        if (Y) { free(Y); Y = NULL; }
    }

    close(sockfd);
    return 0;
}
