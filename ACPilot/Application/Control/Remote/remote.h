//
// Created by zhaohe on 2024/4/28.
//

#ifndef REMOTE_H_
#define REMOTE_H_

#include "Type/type.h"
#include "Controller/expect_state.h"
#include "RemoteMap/remote_mapper.h"
#include "Mailbox/mailbox.h"

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

    static AC_RET analysis(Component::ExpectState &expect_state);
private:
    static Utils::Mailbox<RemoteData> *remote_mailbox;
    static Component::RemoteMapper t_z;
    static Component::RemoteMapper r_x;
    static Component::RemoteMapper p_y;
    static Component::RemoteMapper y_w;
    static Component::RemoteMapper l;
    static Component::RemoteMapper m;
};

#endif //REMOTE_H_
