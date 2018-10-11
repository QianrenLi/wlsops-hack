#ifndef __WLSOPS_H__
#define __WLSOPS_H__

#include <linux/string.h>
#include <linux/netdevice.h>
#include <net/cfg80211.h>
#include <net/mac80211.h>
#include "include/ieee80211_i.h"

extern struct ieee80211_local *wls_local;
extern struct ieee80211_vif *wls_vif;
extern struct ieee80211_hw *wls_hw;

struct __attribute__((__packed__)) tx_param
{
    uint16_t ac;
    uint16_t cw_min;
    uint16_t cw_max;
    uint16_t txop;
    uint8_t aifs;
};

int wls_hacker_init(void);
int wls_conf_tx(struct tx_param);

#endif