//
// Created by zhaohe on 2024/4/28.
//

#include "remote.h"
#include "error_handing.h"
#include "Debug/default_debug.h"
#include "Notify/notify.h"

Utils::Mailbox<RemoteData> *Remote::remote_mailbox = nullptr;
Component::RemoteMapper Remote::t_z{THROTTLE_OR_Z_AXIS_ID};
Component::RemoteMapper Remote::r_x{ROLL_OR_X_AXIS_ID};
Component::RemoteMapper Remote::p_y{PITCH_OR_Y_AXIS_ID};
Component::RemoteMapper Remote::y_w{YAW_OR_W_AXIS_ID};
Component::RemoteMapper Remote::l{LOCK_AXIS_ID};
Component::RemoteMapper Remote::m{MODE_AXIS_ID};

#define POSE_LIMIT (40.f)
#define YAW_LIMIT (90.f)
#define HEIGHT_RATE_LIMIT (2.f)

AC_RET Remote::init()
{
    remote_mailbox = Utils::Mailbox<RemoteData>::find("remote");
    NULL_CHECK(remote_mailbox);
    Component::RemoteMapper::init("/remote");

    return AC_OK;
    error:
    BASE_ERROR("remote init failed");
    return AC_ERROR;
}

AC_RET Remote::analysis(Component::ExpectState &expect_state)
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
                Utils::Notify::notify(LOCK_COMMAND_EVENT);
                break;
            case 1:
                if (t_z.map(remote_data) < 5)
                {
                    switch (m.map(remote_data))
                    {
                        case 0:
                            Utils::Notify::notify(MANUAL_COMMAND_EVENT);
                            break;
                        case 1:
                            Utils::Notify::notify(HEIGHT_COMMAND_EVENT);
                            break;
                        default:
                            break;
                    }
                }
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
    expect_state.height_rate = ((float) t_z.map(remote_data) - (float) REMOTE_MAX / 2) * HEIGHT_RATE_LIMIT / (float) REMOTE_MAX;
    return AC_OK;
}
