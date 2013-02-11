#include "sysutil.h"

/**
 * tcp_server 启动 TCP 服务器
 * @host 服务器 IP 地址或服务器主机名
 * @port 服务器端口
 * 成功返回监听套接字
 */
 int tcp_server(const char *host, unsigned short port) {
 	int listenfd;
 	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
 		ERR_EXIT("tcp_server");
 	}

 	struct sockaddr_in servaddr;
 	memset(&servaddr, 0, sizeof(servaddr));
 	servaddr.sin_family = AF_INET;
 	if (host != NULL) {
 		if (inet_aton(host, &servaddr.sin_addr) == 0) {
 			struct hostent *hp = gethostbyname(host);
 			if (hp == NULL) {
 				ERR_EXIT("gethostbyname");
 			}
 			servaddr.sin_addr = *(struct in_addr*)hp->h_addr;
 		}
 	} else {
 		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 	}

 	servaddr.sin_port = htons(port);

 	int on = 1;
 	if ((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on))) < 0) {
 		ERR_EXIT("set socket opt");
 	}
 	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
 		ERR_EXIT("bind");
 	}
 	if (listen(listenfd, SOMAXCONN) < 0) {
 		ERR_EXIT("listen");
 	}

 	return listenfd;
 }

 int getlocalip(char *ip) {
 	char host[100] = {0};
 	if (gethostname(host, sizeof(host)) < 0) {
 		return -1;
 	}
 	struct hostent *hp;
 	if ((hp = gethostbyname(host)) == NULL) {
 		return -1;
 	}
 	strcpy(ip, inet_ntoa(*(struct in_addr*)hp->h_addr));
 	return 0;
 }