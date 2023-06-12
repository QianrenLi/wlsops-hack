#include "wlsops.h"
#include "WLSINC.h"
#include <linux/string.h>

_adapter *padapter = NULL;
struct ieee80211_vif *wls_vif = NULL;
// struct ieee80211_vif *wls_vif = NULL;
// struct ieee80211_hw *wls_hw = NULL;

int wls_hack_init(char * ifname)
{
    struct net_device *dev;
    struct rtw_netdev_priv_indicator *ptr;

    for_each_netdev(&init_net, dev)
    {
        if (strcmp(dev->name, ifname) == 0)
        {
            ptr = ((struct rtw_netdev_priv_indicator *)netdev_priv(dev));
            printh( "private address: %p, %d\n", ptr->priv, ptr->sizeof_priv );
            padapter = (_adapter *)rtw_netdev_priv(dev);
            if (!padapter) {
                printh("adapter: %p\n", padapter);
                break;
            }
            printh("find wireless device: %s\n", dev->name);
            break;
            // wls_vif = wdev_to_ieee80211_vif(dev->ieee80211_ptr);
            // if (wls_vif) {
            //     // adapter = (struct _ADAPTER *) rtw_netdev_priv(dev);
            //     printh("find wireless device: %s\n", dev->name);
            //     break;
            // }
        }
    }

    if (!padapter)
    {
        printh("No 802.11 device found.\n");
        return -1;
    }
    
    return 0;
}

int wls_conf_tx(struct tx_param param)
{
    uint8_t CWMIN, CWMAX, AIFS, aSifsTime;
    uint16_t TXOP;
    uint32_t acParm;

	struct mlme_ext_priv	*pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info	*pmlmeinfo = &(pmlmeext->mlmext_info);

	// if (IsSupported5G(pmlmeext->cur_wireless_mode) || 
	// 	(pmlmeext->cur_wireless_mode & WIRELESS_11_24N) )
	// 	aSifsTime = 16;
	// else
    aSifsTime = 10;
    printh("CW-min %d, CW-max %d,AIFS (ms) %d,  TXOP %d\n", param.cw_min,param.cw_max,param.aifs,param.txop);
    CWMIN = (uint8_t)param.cw_min ;
    CWMAX = (uint8_t)param.cw_max ;
    AIFS = param.aifs * pmlmeinfo->slotTime + aSifsTime;
    TXOP = param.txop;
    printh("CW-min %d, CW-max %d,AIFS (ms) %d,  TXOP %d\n", CWMIN,CWMAX,AIFS,TXOP);
    acParm = AIFS | (CWMAX << 4) | (CWMIN << 10) | (TXOP << 16);
    printh("AC_param_value %d\n", acParm);
    switch (param.ac)
    {
    case 0:
        padapter->hal_func.set_hw_reg_handler(padapter,HW_VAR_AC_PARAM_VO, (uint8_t*)(&acParm));
        break;
    case 1:
        padapter->hal_func.set_hw_reg_handler(padapter,HW_VAR_AC_PARAM_VI, (uint8_t*)(&acParm));
        break;
    case 2:
        padapter->hal_func.set_hw_reg_handler(padapter,HW_VAR_AC_PARAM_BE, (uint8_t*)(&acParm));
        
        break;
    case 3: 
        padapter->hal_func.set_hw_reg_handler(padapter,HW_VAR_AC_PARAM_BK, (uint8_t*)(&acParm));
        break;
    default:
        padapter->hal_func.set_hw_reg_handler(padapter,HW_VAR_AC_PARAM_BE, (uint8_t*)(&acParm));
        break;
    }

    return 0;
}
