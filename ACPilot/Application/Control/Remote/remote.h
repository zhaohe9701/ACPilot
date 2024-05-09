//
// Created by zhaohe on 2024/4/28.
//

#ifndef REMOTE_H_
#define REMOTE_H_

#include "type.h"
#include "Controller/expect_state.h"
#include "RemoteMap/remote_mapper.h"
#include "Mail/mailbox.h"

#define THROTTLE_OR_Z_AXIS_ID       0
#define ROLL_OR_X_AXIS_ID           1
#define PITCH_OR_Y_AXIS_ID          2
#define YAW_OR_W_AXIS_ID            3
#define LOCK_AXIS_ID                4
#define MODE_AXIS_ID                5

class Remote
{
public:
    static AC_RET init();

    static AC_RET analysis(ExpectState &expect_state);
private:
    static Mailbox<RemoteData> *remote_mailbox;
    static RemoteMapper t_z;
    static RemoteMapper r_x;
    static RemoteMapper p_y;
    static RemoteMapper y_w;
    static RemoteMapper l;
    static RemoteMapper m;
};

#endif //REMOTE_H_
