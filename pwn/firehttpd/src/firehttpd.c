/* J. David's webserver */
/* This is a simple webserver.
 * Created November 1999 by J. David Blackstone.
 * CSE 4344 (Network concepts), Prof. Zeigler
 * University of Texas at Arlington
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: FireHTTPD/0.0.1\r\n"

void accept_request(int);
void bad_request(int);
void cat(int, FILE *);
void error_die(const char *);
int get_line(int, char *, int);
void headers(int, const char *, const char *);
void not_found(int);
void serve_file(int, const char *);
int startup(u_short *);
void unimplemented(int);
void readBufferBeforeSend(int);

void accept_request(int connfd)
{
    char buf[1024];
    int numchars;
    char method[255];
    char url[255];
    char path[512];
    size_t i, j;
    struct stat st;

    numchars = get_line(connfd, buf, sizeof(buf));

    i = 0; j = 0;
    while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
    {
        method[i] = buf[j];
        i++; j++;
    }
    method[i] = '\0';

    if (strcasecmp(method, "GET"))
    {
        readBufferBeforeSend(connfd);

        unimplemented(connfd);

        close(connfd);

        return;
    }

    i = 0;

    while (ISspace(buf[j]) && (j < sizeof(buf)))
        j++;

    while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
    {
        url[i] = buf[j];
        i++; j++;
    }
    url[i] = '\0';

    sprintf(path, "www%s", url);

    if (path[strlen(path) - 1] == '/') {
        strcat(path, "index.html");
    }

    if (stat(path, &st) == -1) {
        readBufferBeforeSend(connfd);

        not_found(connfd);
    } else {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path, "/index.html");

        serve_file(connfd, path);
    }

    close(connfd);
}

void bad_request(int connfd)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(connfd, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(connfd, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(connfd, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(connfd, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(connfd, buf, sizeof(buf), 0);
}

void cat(int connfd, FILE *resource)
{
    char buf[1024];

    fgets(buf, sizeof(buf), resource);
    while (!feof(resource))
    {
        send(connfd, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), resource);
    }
}

void error_die(const char *sc)
{
    perror(sc);
    exit(1);
}

int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        if (n > 0) {
            if (c == '\r') {

                n = recv(sock, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }

            buf[i] = c;
            i++;
        } else {
            c = '\n';
        }
    }

    buf[i] = '\0';

    return (i);
}

void readBufferBeforeSend(int connfd)
{
    int numchars = 1;
    char buf[1024];

    int content_length = -1;
    int i;
    char c;

    buf[0] = 'A'; buf[1] = '\0';

    while ((numchars > 0) && strcmp("\n", buf))
    {
        buf[15] = '\0';
        if (strcasecmp(buf, "Content-Length:") == 0) {
            content_length = atoi(&(buf[16]));
        }
        numchars = get_line(connfd, buf, sizeof(buf));
    }

    if(content_length != -1){
        for (i = 0; i < content_length; i++) {
            recv(connfd, &c, 1, 0);
        }
    }
}

void headers(int connfd, const char *filename, const char *referer)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(connfd, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(connfd, buf, strlen(buf), 0);

    if (strlen(referer) > 0) {
        send(connfd, referer, strlen(referer), 0);
    }

    strcpy(buf, "\r\n");
    send(connfd, buf, strlen(buf), 0);
}

void not_found(int connfd)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(connfd, buf, strlen(buf), 0);
}

void serve_file(int connfd, const char *filename)
{
    FILE *resource = NULL;
    int numchars = 1;
    char buf[1024];
    char referer[1024] = {0};

    buf[0] = 'A'; buf[1] = '\0';

    // Avoid directory transversal
    char *dots = "..";
    char *ret = strstr(filename, dots);

    while ((numchars > 0) && strcmp("\n", buf)) {
        numchars = get_line(connfd, buf, sizeof(buf));

        if (strncmp(buf, "Referer: ", 9) == 0) {
            sprintf(referer, buf);
        }
    }

    if (access(filename, F_OK) == -1 || ret != NULL) {
        not_found(connfd);
    } else {
        headers(connfd, filename, referer);
        resource = fopen(filename, "r");
        cat(connfd, resource);
        fclose(resource);
    }
}

int startup(u_short *port)
{
    int httpd = 0;
    struct sockaddr_in name;

    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1)
        error_die("socket");

    int reuse = 1;
    if (setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        error_die("setsockopet");
    }

    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;

    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        error_die("bind");

    if (*port == 0)
    {
        int namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, (socklen_t *)&namelen) == -1)
            error_die("getsockname");
        *port = ntohs(name.sin_port);
    }


    if (listen(httpd, 5) < 0)
        error_die("listen");
    return (httpd);
}

void unimplemented(int connfd)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(connfd, buf, strlen(buf), 0);
}

/**********************************************************************/

int main(void)
{
    int listenfd = -1;
    u_short port = 1337;
    int connfd = -1;

    struct sockaddr_in client;
    int client_len = sizeof(client);

    listenfd = startup(&port);
    printf("Running on port %d\n", port);

    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t *)&client_len);
        if (connfd == -1) {
            error_die("accept");
        }

        accept_request(connfd);
    }

    close(listenfd);

    return (0);
}
