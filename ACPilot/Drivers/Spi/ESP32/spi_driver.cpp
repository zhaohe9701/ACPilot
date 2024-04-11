//
// Created by zhaohe on 2023/11/28.
//
#include "Spi/spi_driver.h"

SpiBusHandle::SpiBusHandle()
{
    memset(&config, 0, sizeof(spi_bus_config_t));
}

SpiHandle::SpiHandle()
{
    memset(&config, 0, sizeof(spi_device_interface_config_t));
    memset(&handle, 0, sizeof(spi_device_handle_t));
}
SpiBus::SpiBus(SpiBusHandle* handle)
{
    this->handle = handle;
}

AC_RET SpiBus::init() const
{
    if (ESP_OK != spi_bus_initialize(handle->host, &handle->config, handle->dma))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

bool SpiBus::matchHandle(SpiBusHandle *handle) const
{
    if (handle == this->handle)
    {
        return true;
    } else
    {
        return false;
    }
}

AC_RET SpiBus::lock(uint32_t timeout)
{
    return _mutex.lock(timeout);
}

void SpiBus::unlock()
{
    _mutex.unlock();
}

Spi::Spi(SpiBus *bus, SpiHandle *handle)
{
    _bus = bus;
    _handle = handle;
}

AC_RET Spi::init()
{
    if (ESP_OK != spi_bus_add_device(_bus->handle->host, &_handle->config, &_handle->handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Spi::readReg(uint8_t address, uint8_t &value, uint32_t timeout)
{
    spi_transaction_t data;
    memset(&data, 0, sizeof(spi_transaction_t));
    data.addr = address;  // 设备ID
    data.length = 8;
    data.rx_buffer = &value;
    data.flags = SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_USE_RXDATA;
    _bus->lock(timeout);
    spi_device_polling_transmit(_handle->handle, &data);
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    spi_transaction_t data;
    memset(&data, 0, sizeof(spi_transaction_t));
    data.addr = address;  // 设备ID
    data.length = 8 * len;
    data.rx_buffer = dataBuf;
    data.flags = SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_USE_RXDATA;
    _bus->lock(timeout);
    spi_device_polling_transmit(_handle->handle, &data);
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    spi_transaction_t data;
    memset(&data, 0, sizeof(spi_transaction_t));
    data.addr = address;  // 设备ID
    data.length = 8 * len;
    data.rx_buffer = dataBuf;
    data.flags = SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_USE_RXDATA;
    _bus->lock(timeout);
    spi_device_polling_transmit(_handle->handle, &data);
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::writeReg(uint8_t address, uint8_t value, uint32_t timeout)
{
    spi_transaction_t data;
    memset(&data, 0, sizeof(spi_transaction_t));
    data.addr = address;  // 设备ID
    data.length = 8;
    data.tx_buffer = &value;
    data.flags = SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_USE_TXDATA;
    _bus->lock(timeout);
    spi_device_polling_transmit(_handle->handle, &data);
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::writeBytes(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    spi_transaction_t data;
    memset(&data, 0, sizeof(spi_transaction_t));
    data.addr = address;  // 设备ID
    data.length = 8 * len;
    data.tx_buffer = value;
    data.flags = SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_USE_TXDATA;
    _bus->lock(timeout);
    spi_device_polling_transmit(_handle->handle, &data);
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::writeBytesDMA(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    spi_transaction_t data;
    memset(&data, 0, sizeof(spi_transaction_t));
    data.addr = address;  // 设备ID
    data.length = 8 * len;
    data.tx_buffer = value;
    data.flags = SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_USE_TXDATA;
    _bus->lock(timeout);
    spi_device_polling_transmit(_handle->handle, &data);
    _bus->unlock();
    return AC_OK;
}
