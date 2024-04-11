#ifndef CRSF_H_
#define CRSF_H_

#include "Receive/receive_parser.h"
#include "Remote/remote_data.h"
#include "Mail/mailbox.h"
#include "type.h"

#define CRSF_HEAD           0xC8
#define CRSF_PAYLOAD        0x16
#define CRSF_CHANNEL_NUM    16
#define CRSF_BIT_LEN        11
#define CRSF_MIN            172
#define CRSF_MAX            1810
class CrsfParser : virtual public MessageReceiveParser
{
public:
    CrsfParser();
    bool match(ComMessage &message) override;
    AC_RET parse(ComMessage &message, bool &free_message) override;
private:
    uint16_t _channel_data[CRSF_CHANNEL_NUM] = {0};
    Mailbox<RemoteData> *_manager = nullptr;
    AC_RET _decode(const uint8_t *bin, uint16_t *axis, int bit_num, int len);
};

#endif