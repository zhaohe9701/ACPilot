//
// Created by zhaohe on 2024/4/28.
//

#include "remote.h"
#include "error_handing.h"
#include "default_debug.h"
#include "Notify/notify.h"

Mailbox<RemoteData> *Remote::remote_mailbox = nullptr;
RemoteMapper Remote::t_z{THROTTLE_OR_Z_AXIS_ID};
RemoteMapper Remote::r_x{ROLL_OR_X_AXIS_ID};
RemoteMapper Remote::p_y{PITCH_OR_Y_AXIS_ID};
RemoteMapper Remote::y_w{YAW_OR_W_AXIS_ID};
RemoteMapper Remote::l{LOCK_AXIS_ID};
RemoteMapper Remote::m{MODE_AXIS_ID};

#define POSE_LIMIT (40.f)
#define YAW_LIMIT (90.f)
#define HEIGHT_LIMIT (2.f)

AC_RET Remote::init()
{
    remote_mailbox = Mailbox<RemoteData>::find("remote");
    NULL_CHECK(remote_mailbox);
    RemoteMapper::init("/remote");

    return AC_OK;
    error:
    BASE_ERROR("remote init failed");
    return AC_ERROR;
}

AC_RET Remote::analysis(ExpectState &expect_state)
{
    RemoteData remote_data;
    uint8_t key = 0;
    remote_mailbox->copy(&remote_data);

    key = l.map(remote_data);
    if (l.isChanged())
    {
        switch (key)
        {
            case 0:
                Notify::notify(LOCK_COMMAND_EVENT);
                break;
            case 1:
                if  (t_z.map(remote_data) < 5 && m.map(remote_data) == 0)
                {
                    Notify::notify(UNLOCK_COMMAND_EVENT);
                }
                break;
            default:
                break;
        }
    }
    key = m.map(remote_data);
    if (m.isChanged())
    {
        switch (key)
        {
            case 0:
                Notify::notify(MANUAL_COMMAND_EVENT);
                break;
            case 1:
                Notify::notify(HEIGHT_COMMAND_EVENT);
                break;
            default:
                break;
        }
    }

    expect_state.euler.roll = ((float) r_x.map(remote_data) - (float) REMOTE_MAX / 2) * POSE_LIMIT / (float) REMOTE_MAX;
    expect_state.euler.pitch =
            ((float) p_y.map(remote_data) - (float) REMOTE_MAX / 2) * POSE_LIMIT / (float) REMOTE_MAX;
    expect_state.euler.yaw = -((float) y_w.map(remote_data) - (float) REMOTE_MAX / 2) * YAW_LIMIT / (float) REMOTE_MAX;

    expect_state.throttle = (float) t_z.map(remote_data) * 100.f / (float) REMOTE_MAX;
    expect_state.height = ((float) t_z.map(remote_data) - (float) REMOTE_MAX / 2) * HEIGHT_LIMIT / (float) REMOTE_MAX;
    return AC_OK;
}
