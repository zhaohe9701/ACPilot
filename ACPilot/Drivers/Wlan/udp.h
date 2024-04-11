//
// Created by zhaohe on 2024/4/11.
//

#ifndef UDP_H_
#define UDP_H_

#include "type.h"
#include "Com/com_interface.h"
#include "Thread/ac_thread.h"
#include "Notify/notify.h"
#include <lwip/sockets.h>

class UdpHandle
{
public:
    int port;
};

class Udp : public ComInterface
{
public:
    Udp(UdpHandle *handle, uint8_t port_num);

    AC_RET init() override;

    AC_RET send(uint8_t *buf, uint16_t length, uint32_t timeout) override;
private:
    UdpHandle *_handle;

    AcThread *_udp_task = nullptr;

    NotifyToken *_wifi_connect_token = nullptr;

    sockaddr_storage _source_addr{};

    bool _is_connected = false;

    int _sock = -1;

    uint8_t _buf[MAX_UDP_BUF_LEN] = {0};

    static void _receive_task(void *param);
};


#endif //UDP_H_
