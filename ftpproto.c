#include "ftpproto.h"
#include "sysutil.h"
#include "str.h"

void handle_child(session_t *sess) {
    char welcome_msg[] = "220 (miniftpd 0.1)\r\n";
    writen(sess->ctrl_fd, welcome_msg, sizeof(welcome_msg));
    int ret;
    while (1) {
        memset(sess->cmdline, 0, sizeof(sess->cmdline));
        memset(sess->cmd, 0, sizeof(sess->cmd));
        memset(sess->arg, 0, sizeof(sess->arg));
        ret = readline(sess->ctrl_fd, sess->cmdline, MAX_COMMAND_LINE);
        if (ret == -1) {
            ERR_EXIT("readline");
        } else if (ret == 0) {
            exit(EXIT_SUCCESS);
        }
        str_trim_crlf(sess->cmdline);
        // 解析 FTP 命令与参数
        str_split(sess->cmdline, sess->cmd, sess->arg, ' ');
        str_upper(sess->cmd);
        // 处理 FTP 命令
    }
}