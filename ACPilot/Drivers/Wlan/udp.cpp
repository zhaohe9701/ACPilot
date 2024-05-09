//
// Created by zhaohe on 2024/4/11.
//

#include "udp.h"
#include "default_debug.h"



Udp::Udp(UdpHandle *handle, uint8_t port_num) : Com(port_num)
{
    _handle = handle;
}

AC_RET Udp::init()
{
    _wifi_connect_token = Notify::sub(WIFI_CONNECT_SUCCESS_EVENT);
    if (nullptr == _wifi_connect_token)
    {
        BASE_ERROR("sub wifi connect success event fail");
        return AC_ERROR;
    }
    _udp_task = new AcThread("udp", UDP_RECEIVE_TASK_STACK, UDP_RECEIVE_TASK_PRIO, UDP_RECEIVE_TASK_CORE);
    _udp_task->run(Udp::_receive_task, this);
    return AC_OK;
}

void Udp::_receive_task(void *param)
{
    Udp *udp = static_cast<Udp *>(param);
    sockaddr_in dest_addr{};

    /* 等待wifi连接 */
    udp->_wifi_connect_token->wait();

    for (;;)
    {
        /* 初始化本地套接字配置 */
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(udp->_handle->port);

        /* 创建套接字 */
        udp->_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (udp->_sock < 0)
        {
            BASE_ERROR("Unable to create socket: errno %d", errno);
            break;
        }
        BASE_INFO("Socket created");

        /* 绑定套接字与配置 */
        int err = bind(udp->_sock, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
        if (err < 0)
        {
            BASE_ERROR("Socket unable to bind: errno %d", errno);
            break;
        }
        BASE_INFO("Socket bound: port %d", udp->_handle->port);

        socklen_t socklen = sizeof(udp->_source_addr);

        for (;;)
        {
            int len = recvfrom(udp->_sock, udp->_buf, MAX_UDP_BUF_LEN - 1, 0, (struct sockaddr *) &udp->_source_addr,
                               &socklen);
            // Error occurred during receiving
            if (len < 0)
            {
                BASE_ERROR("recvfrom failed: errno %d", errno);
                break;
            } else
            {
                udp->_buf[len] = '\0';
                udp->_is_connected = true;
                udp->_recv_pool = MemoryPool::getGeneral(len);
                if (nullptr == udp->_recv_pool)
                {
                    continue;
                }
                udp->_recv_buffer = udp->_recv_pool->alloc();
                memcpy(udp->_recv_buffer, udp->_buf, len);
                udp->receive(len);
            }
        }

        if (udp->_sock != -1)
        {
            BASE_INFO("Shutting down socket and restarting...");
            udp->_is_connected = false;
            shutdown(udp->_sock, 0);
            close(udp->_sock);
        }
    }
    AcThread::killSelf();
}

AC_RET Udp::send(uint8_t *buf, uint16_t length, uint32_t timeout)
{
    if (!_is_connected)
    {
        return AC_ERROR;
    }
    int err = sendto(_sock, buf, length, 0, (struct sockaddr *) &_source_addr, sizeof(_source_addr));
    if (err < 0)
    {
        return AC_ERROR;
    }
    return AC_OK;
}
