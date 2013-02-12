#include "session.h"
#include "privparent.h"
#include "ftpproto.h"

void begin_session(session_t *sess) {
    struct passwd *pw = getpwnam("nobody");
    if (pw == NULL) {
        return;
    }

    // 把当前进程用户设置为 nobody 用户
    if (setegid(pw->pw_gid) < 0) {
        ERR_EXIT("setegid");
    }
    if (seteuid(pw->pw_uid) < 0) {
        ERR_EXIT("seteuid");
    }

    int sockfds[2];
    if (socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds) < 0) {
        ERR_EXIT("socketpair");
    }

    pid_t pid = fork();
    if (pid < 0) {
        ERR_EXIT("fork");
    }
    if (pid == 0) {
        // FTP 服务进程
        close(sockfds[0]);
        sess->child_fd = sockfds[1];
        handle_child(sess);
    } else {
        // nobody 进程
        close(sockfds[1]);
        sess->parent_fd = sockfds[0];
        handle_parent(sess);
    }
}