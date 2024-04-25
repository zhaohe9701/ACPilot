//
// Created by zhaohe on 2024/4/22.
//
#include "Nvs/nvs_driver.h"
#include "default_debug.h"
#include "error_handing.h"

AC_RET Nvs::read(const char *name, void *data, uint32_t len) const
{
    size_t length = 0;
    if (ESP_OK != nvs_get_blob(_handle, name, nullptr, &length))
    {
        BASE_ERROR("nvs not found: %s", name);
        return AC_ERROR;
    }
    if (len != length)
    {
        BASE_ERROR("nvs length not match");
        return AC_ERROR;
    }
    if (ESP_OK != nvs_get_blob(_handle, name, data, &length))
    {
        BASE_ERROR("nvs read failed");
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Nvs::write(const char *name, void *data, uint32_t len) const
{
    size_t length = 0;
    esp_err_t err;
    err = nvs_get_blob(_handle, name, nullptr, &length);

    if (err == ESP_OK)
    {
        if (len != length)
        {
            BASE_ERROR("nvs length not match");
            return AC_ERROR;
        }
        if (ESP_OK != (err = nvs_set_blob(_handle, name, data, len)))
        {
            BASE_ERROR("nvs write failed:%d", err);
            return AC_ERROR;
        }
        return AC_OK;
    }
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        if (ESP_OK != (err = nvs_set_blob(_handle, name, data, len)))
        {
            BASE_ERROR("nvs write failed:%d", err);
            return AC_ERROR;
        }
        return AC_OK;
    }
    return AC_ERROR;
}

AC_RET Nvs::save() const
{
    if (ESP_OK != nvs_commit(_handle))
    {
        BASE_ERROR("nvs commit failed");
        return AC_ERROR;
    }
    return AC_OK;
}

Nvs *Nvs::open(const char *name)
{
    Nvs *nvs = new Nvs();

    if (ESP_OK != nvs_open(name, NVS_READWRITE, &nvs->_handle))
    {
        return nullptr;
    }
    return nvs;
}

void Nvs::close(Nvs *nvs)
{
    if (nvs == nullptr)
    {
        return;
    }
    nvs_close(nvs->_handle);
    delete nvs;
}

AC_RET Nvs::erase(const char *name) const
{
    if (ESP_OK != nvs_erase_key(_handle, name))
    {
        BASE_ERROR("nvs erase failed");
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Nvs::eraseAll(Nvs *nvs)
{
    if (ESP_OK != nvs_erase_all(nvs->_handle))
    {
        BASE_ERROR("nvs erase failed");
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Nvs::eraseAll(const char *name)
{
    Nvs *fd = Nvs::open(name);
    NULL_CHECK(fd);

    RETURN_CHECK(Nvs::eraseAll(fd));

    Nvs::close(fd);

    return AC_OK;
    error:
    if (fd != nullptr)
    {
        Nvs::close(fd);
    }
    return AC_ERROR;
}

AC_RET Nvs::info(char *buf, uint32_t len)
{
    nvs_stats_t nvs_stats;
    nvs_get_stats(nullptr, &nvs_stats);
    snprintf(buf, len, "NVS INFO:\n"
                       "\tused    free    all\n\t%-8zu%-8zu%-8zu\n",
                       nvs_stats.used_entries,
                       nvs_stats.free_entries,
                       nvs_stats.total_entries);

    return AC_OK;
}



