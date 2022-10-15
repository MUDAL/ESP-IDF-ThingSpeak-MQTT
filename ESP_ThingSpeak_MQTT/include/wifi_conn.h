#ifndef WIFI_CONNECTIVITY_H
#define WIFI_CONNECTIVITY_H

#include "credentials.h"
#define MAXIMUM_RETRIES               5

/**
 * @brief Configure Wi-Fi.
 * 
 */
void wifi_config(void);
/**
 * @brief Return Wi-Fi connection status.
 * 
 * @return true if module connects to an access point.
 * @return false if module fails to connect to an access point.
 */
bool wifi_connected(void);
/**
 * @brief Return IP status. IP is obtained after a  
 * successful Wi-Fi connection.
 * 
 * @return true if IP was obtained.
 * @return false if IP was not obtained (due to no connection).
 */
bool wifi_got_ip(void);
/**
 * @brief Return Wi-Fi disconnection status after several attempts  
 * to re-establish connection to an access point.
 * 
 * @return true if module failed to re-establish connection with access  
 * point after 'MAX_RETRIES'.
 * @return false if module is connected to an access point.
 */
bool wifi_disconnected_after_max_retries(void);
/**
 * @brief Reconnect to an access point after a time period specified  
 * in milliseconds. Call this function if wifi_disconnected_after_max_retries()  
 * returns true.
 * This function blocks for the number of milliseconds specified.
 * 
 * @param time_milli_secs time to wait before reconnecting to an access point.
 */
void wifi_reconnect_after_specified_time(uint32_t time_milli_secs);

#endif