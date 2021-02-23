//
//  driver_wext.c
//  AirPortLinux
//
//  Created by Zhong-MAC on 2021/2/15.
//  Copyright © 2021 User-Mac. All rights reserved.
//

#include "driver_wext.h"
#include "help_ifconfig.h"

int wpa_driver_wext_set_auth_param(struct wpa_driver_wext_data *drv,
                   int idx, u32 value)
{
    struct iwreq iwr;
    int ret = 0;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    iwr.u.param.flags = idx & IW_AUTH_INDEX;
    iwr.u.param.value = value;

    if ((ret = ioctl(drv->ioctl_sock, SIOCSIWAUTH, &iwr)) < 0) {
        if (-ret != EOPNOTSUPP) {
            wpa_printf(MSG_DEBUG, "WEXT: SIOCSIWAUTH(param %d "
                   "value 0x%x) failed: %s)",
                   idx, value, strerror(ret));
        }
        ret = -ret == EOPNOTSUPP ? -2 : -1;
    }

    return ret;
}

/**
 * wpa_driver_wext_get_bssid - Get BSSID, SIOCGIWAP
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @bssid: Buffer for BSSID
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_get_bssid(void *priv, u8 *bssid)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    struct iwreq iwr;
    int ret = 0;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);

    if (ioctl(drv->ioctl_sock, SIOCGIWAP, &iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCGIWAP]: %d", strerror(ret));
        ret = -1;
    }
    os_memcpy(bssid, iwr.u.ap_addr.sa_data, ETH_ALEN);

    return ret;
}

/**
 * wpa_driver_wext_set_bssid - Set BSSID, SIOCSIWAP
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @bssid: BSSID
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_bssid(void *priv, const u8 *bssid)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    struct iwreq iwr;
    int ret = 0;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    iwr.u.ap_addr.sa_family = ARPHRD_ETHER;
    if (bssid)
        os_memcpy(iwr.u.ap_addr.sa_data, bssid, ETH_ALEN);
    else
        os_memset(iwr.u.ap_addr.sa_data, 0, ETH_ALEN);

    if (ioctl(drv->ioctl_sock, SIOCSIWAP, &iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWAP]: %s", strerror(ret));
        ret = -1;
    }

    return ret;
}

/**
 * wpa_driver_wext_get_ssid - Get SSID, SIOCGIWESSID
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @ssid: Buffer for the SSID; must be at least 32 bytes long
 * Returns: SSID length on success, -1 on failure
 */
int wpa_driver_wext_get_ssid(void *priv, u8 *ssid)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    struct iwreq iwr;
    int ret = 0;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    iwr.u.essid.pointer = (caddr_t) ssid;
    iwr.u.essid.length = SSID_MAX_LEN;

    if (ioctl(drv->ioctl_sock, SIOCGIWESSID, &iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCGIWESSID]: %s",
               strerror(ret));
        ret = -1;
    } else {
        ret = iwr.u.essid.length;
        if (ret > SSID_MAX_LEN)
            ret = SSID_MAX_LEN;
        /* Some drivers include nul termination in the SSID, so let's
         * remove it here before further processing. WE-21 changes this
         * to explicitly require the length _not_ to include nul
         * termination. */
        if (ret > 0 && ssid[ret - 1] == '\0' &&
            drv->we_version_compiled < 21)
            ret--;
    }

    return ret;
}

/**
 * wpa_driver_wext_set_ssid - Set SSID, SIOCSIWESSID
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @ssid: SSID
 * @ssid_len: Length of SSID (0..32)
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_ssid(void *priv, const u8 *ssid, size_t ssid_len)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    struct iwreq iwr;
    int ret = 0;
    char buf[33];

    if (ssid_len > SSID_MAX_LEN)
        return -1;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    /* flags: 1 = ESSID is active, 0 = not (promiscuous) */
    iwr.u.essid.flags = (ssid_len != 0);
    os_memset(buf, 0, sizeof(buf));
    os_memcpy(buf, ssid, ssid_len);
    iwr.u.essid.pointer = (caddr_t) buf;
    if (drv->we_version_compiled < 21) {
        /* For historic reasons, set SSID length to include one extra
         * character, C string nul termination, even though SSID is
         * really an octet string that should not be presented as a C
         * string. Some Linux drivers decrement the length by one and
         * can thus end up missing the last octet of the SSID if the
         * length is not incremented here. WE-21 changes this to
         * explicitly require the length _not_ to include nul
         * termination. */
        if (ssid_len)
            ssid_len++;
    }
    iwr.u.essid.length = ssid_len;

    if (ioctl(drv->ioctl_sock, SIOCSIWESSID, &iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWESSID]: %s",
               strerror(ret));
        ret = -1;
    }

    return ret;
}

/**
 * wpa_driver_wext_set_freq - Set frequency/channel, SIOCSIWFREQ
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @freq: Frequency in MHz
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_freq(void *priv, int freq)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    struct iwreq iwr;
    int ret = 0;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    iwr.u.freq.m = freq * 100000;
    iwr.u.freq.e = 1;

    if (ioctl(drv->ioctl_sock, SIOCSIWFREQ, &iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWFREQ]: %s",
               strerror(ret));
        ret = -1;
    }

    return ret;
}

static int wpa_driver_wext_set_drop_unencrypted(void *priv,
                        int enabled)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
    drv->use_crypt = enabled;
    return wpa_driver_wext_set_auth_param(drv, IW_AUTH_DROP_UNENCRYPTED,
                          enabled);
}

static int wpa_driver_wext_set_auth_alg(void *priv, int auth_alg)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    int algs = 0, res;

    if (auth_alg & WPA_AUTH_ALG_OPEN)
        algs |= IW_AUTH_ALG_OPEN_SYSTEM;
    if (auth_alg & WPA_AUTH_ALG_SHARED)
        algs |= IW_AUTH_ALG_SHARED_KEY;
    if (auth_alg & WPA_AUTH_ALG_LEAP)
        algs |= IW_AUTH_ALG_LEAP;
    if (algs == 0) {
        /* at least one algorithm should be set */
        algs = IW_AUTH_ALG_OPEN_SYSTEM;
    }

    res = wpa_driver_wext_set_auth_param(drv, IW_AUTH_80211_AUTH_ALG,
                         algs);
    drv->auth_alg_fallback = res == -2;
    return res;
}

static int
wpa_driver_wext_auth_alg_fallback(struct wpa_driver_wext_data *drv,
                  struct wpa_driver_associate_params *params)
{
    struct iwreq iwr;
    int ret = 0;

    wpa_printf(MSG_DEBUG, "WEXT: Driver did not support "
           "SIOCSIWAUTH for AUTH_ALG, trying SIOCSIWENCODE");

    memset(&iwr, 0, sizeof(iwr));
    strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    /* Just changing mode, not actual keys */
    iwr.u.encoding.flags = 0;
    iwr.u.encoding.pointer = (caddr_t) NULL;
    iwr.u.encoding.length = 0;

    /*
     * Note: IW_ENCODE_{OPEN,RESTRICTED} can be interpreted to mean two
     * different things. Here they are used to indicate Open System vs.
     * Shared Key authentication algorithm. However, some drivers may use
     * them to select between open/restricted WEP encrypted (open = allow
     * both unencrypted and encrypted frames; restricted = only allow
     * encrypted frames).
     */

    if (!drv->use_crypt) {
        iwr.u.encoding.flags |= IW_ENCODE_DISABLED;
    } else {
        if (params->auth_alg & WPA_AUTH_ALG_OPEN)
            iwr.u.encoding.flags |= IW_ENCODE_OPEN;
        if (params->auth_alg & WPA_AUTH_ALG_SHARED)
            iwr.u.encoding.flags |= IW_ENCODE_RESTRICTED;
    }

    if (ioctl(drv->ioctl_sock, SIOCSIWENCODE, &iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWENCODE]: %d",
               strerror(ret));
        ret = -1;
    }

    return ret;
}

static int wpa_driver_wext_set_gen_ie(void *priv, const u8 *ie,
                      size_t ie_len)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    struct iwreq iwr;
    int ret = 0;

    memset(&iwr, 0, sizeof(iwr));
    strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) ie;
    iwr.u.data.length = ie_len;

    if (ioctl(drv->ioctl_sock, SIOCSIWGENIE, &iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWGENIE]: %d",
               strerror(ret));
        ret = -1;
    }

    return ret;
}

static int wpa_driver_wext_set_psk(struct wpa_driver_wext_data *drv,
                   const u8 *psk)
{
    struct iw_encode_ext *ext;
    struct iwreq iwr;
    int ret;

    wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);

    if (!(drv->capa.flags & WPA_DRIVER_FLAGS_4WAY_HANDSHAKE_8021X))
        return 0;

    if (!psk)
        return 0;

    memset(&iwr, 0, sizeof(iwr));
    strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);

    ext = (typeof ext)zalloc(sizeof(*ext) + PMK_LEN);
    if (ext == NULL)
        return -1;

    iwr.u.encoding.pointer = (caddr_t) ext;
    iwr.u.encoding.length = sizeof(*ext) + PMK_LEN;
    ext->key_len = PMK_LEN;
    memcpy(&ext->key, psk, ext->key_len);
    ext->alg = IW_ENCODE_ALG_PMK;

    ret = ioctl(drv->ioctl_sock, SIOCSIWENCODEEXT, &iwr);
    if (ret < 0)
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWENCODEEXT] PMK: %d",
               strerror(ret));
    free(ext);

    return ret;
}

/**
 * wpa_driver_wext_set_mode - Set wireless mode (infra/adhoc), SIOCSIWMODE
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @mode: 0 = infra/BSS (associate with an AP), 1 = adhoc/IBSS
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_mode(void *priv, int mode)
{
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    struct iwreq iwr;
    int ret = -1;
    unsigned int new_mode = mode ? IW_MODE_ADHOC : IW_MODE_INFRA;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->ifname, IFNAMSIZ);
    iwr.u.mode = new_mode;
    ret = ioctl(drv->ioctl_sock, SIOCSIWMODE, &iwr);
    if (ret == 0) {
        ret = 0;
        goto done;
    }

    if (-ret != EBUSY) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWMODE]: %d",
               strerror(ret));
        goto done;
    }

    /* mac80211 doesn't allow mode changes while the device is up, so if
     * the device isn't in the mode we're about to change to, take device
     * down, try to set the mode again, and bring it back up.
     */
    ret = ioctl(drv->ioctl_sock, SIOCGIWMODE, &iwr);
    if (ret < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCGIWMODE]: %d",
               strerror(ret));
        goto done;
    }

    if (iwr.u.mode == new_mode) {
        ret = 0;
        goto done;
    }

//    if (linux_set_iface_flags(1, drv->ifname, 0) == 0) {
//        /* Try to set the mode again while the interface is down */
//        iwr.u.mode = new_mode;
//        if (ioctl(drv->ioctl_sock, SIOCSIWMODE, &iwr) < 0)
//            wpa_printf(MSG_ERROR, "ioctl[SIOCSIWMODE]: %s",
//                   strerror(errno));
//        else
//            ret = 0;
//
//        (void) linux_set_iface_flags(1, drv->ifname, 1);
//    }

done:
    return ret;
}

int wpa_driver_wext_cipher2wext(int cipher)
{
    switch (cipher) {
    case WPA_CIPHER_NONE:
        return IW_AUTH_CIPHER_NONE;
    case WPA_CIPHER_WEP40:
        return IW_AUTH_CIPHER_WEP40;
    case WPA_CIPHER_TKIP:
        return IW_AUTH_CIPHER_TKIP;
    case WPA_CIPHER_CCMP:
        return IW_AUTH_CIPHER_CCMP;
    case WPA_CIPHER_WEP104:
        return IW_AUTH_CIPHER_WEP104;
    default:
        return 0;
    }
}

int wpa_driver_wext_keymgmt2wext(int keymgmt)
{
    switch (keymgmt) {
    case WPA_KEY_MGMT_IEEE8021X:
    case WPA_KEY_MGMT_IEEE8021X_NO_WPA:
        return IW_AUTH_KEY_MGMT_802_1X;
    case WPA_KEY_MGMT_PSK:
        return IW_AUTH_KEY_MGMT_PSK;
    default:
        return 0;
    }
}

int wpa_driver_wext_associate(void *priv,
                  struct wpa_driver_associate_params *params)
{
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    struct wpa_driver_wext_data *drv = (typeof drv)priv;
    int ret = 0;
    int allow_unencrypted_eapol;
    int value;

    wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);

    if (drv->cfg80211) {
        /*
         * Stop cfg80211 from trying to associate before we are done
         * with all parameters.
         */
        if (wpa_driver_wext_set_ssid(drv, (u8 *) "", 0) < 0) {
            wpa_printf(MSG_DEBUG,
                   "WEXT: Failed to clear SSID to stop pending cfg80211 association attempts (if any)");
            /* continue anyway */
        }
    }

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    if (wpa_driver_wext_set_drop_unencrypted(drv, params->drop_unencrypted)
        < 0)
        ret = -1;
    if (wpa_driver_wext_set_auth_alg(drv, params->auth_alg) < 0)
        ret = -1;
    if (wpa_driver_wext_set_mode(drv, params->mode) < 0)
        ret = -1;

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    /*
     * If the driver did not support SIOCSIWAUTH, fallback to
     * SIOCSIWENCODE here.
     */
    if (drv->auth_alg_fallback &&
        wpa_driver_wext_auth_alg_fallback(drv, params) < 0)
        ret = -1;

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    if (!params->bssid &&
        wpa_driver_wext_set_bssid(drv, NULL) < 0)
        ret = -1;

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    /* TODO: should consider getting wpa version and cipher/key_mgmt suites
     * from configuration, not from here, where only the selected suite is
     * available */
    if (wpa_driver_wext_set_gen_ie(drv, params->wpa_ie, params->wpa_ie_len)
        < 0)
        ret = -1;
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    if (params->wpa_proto & WPA_PROTO_RSN)
        value = IW_AUTH_WPA_VERSION_WPA2;
    else if (params->wpa_proto & WPA_PROTO_WPA)
        value = IW_AUTH_WPA_VERSION_WPA;
    else
        value = IW_AUTH_WPA_VERSION_DISABLED;
    if (wpa_driver_wext_set_auth_param(drv,
                       IW_AUTH_WPA_VERSION, value) < 0)
        ret = -1;
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    value = wpa_driver_wext_cipher2wext(params->pairwise_suite);
    if (wpa_driver_wext_set_auth_param(drv,
                       IW_AUTH_CIPHER_PAIRWISE, value) < 0)
        ret = -1;
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    value = wpa_driver_wext_cipher2wext(params->group_suite);
    if (wpa_driver_wext_set_auth_param(drv,
                       IW_AUTH_CIPHER_GROUP, value) < 0)
        ret = -1;
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    value = wpa_driver_wext_keymgmt2wext(params->key_mgmt_suite);
    if (wpa_driver_wext_set_auth_param(drv,
                       IW_AUTH_KEY_MGMT, value) < 0)
        ret = -1;
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    value = params->key_mgmt_suite != WPA_KEY_MGMT_NONE ||
        params->pairwise_suite != WPA_CIPHER_NONE ||
        params->group_suite != WPA_CIPHER_NONE ||
        (params->wpa_proto & (WPA_PROTO_RSN | WPA_PROTO_WPA));
    if (wpa_driver_wext_set_auth_param(drv,
                       IW_AUTH_PRIVACY_INVOKED, value) < 0)
        ret = -1;

    /* Allow unencrypted EAPOL messages even if pairwise keys are set when
     * not using WPA. IEEE 802.1X specifies that these frames are not
     * encrypted, but WPA encrypts them when pairwise keys are in use. */
    if (params->key_mgmt_suite == WPA_KEY_MGMT_IEEE8021X ||
        params->key_mgmt_suite == WPA_KEY_MGMT_PSK)
        allow_unencrypted_eapol = 0;
    else
        allow_unencrypted_eapol = 1;

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    if (wpa_driver_wext_set_psk(drv, params->psk) < 0)
        ret = -1;
    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    if (wpa_driver_wext_set_auth_param(drv,
                       IW_AUTH_RX_UNENCRYPTED_EAPOL,
                       allow_unencrypted_eapol) < 0)
        ret = -1;
#ifdef CONFIG_IEEE80211W
    switch (params->mgmt_frame_protection) {
    case NO_MGMT_FRAME_PROTECTION:
        value = IW_AUTH_MFP_DISABLED;
        break;
    case MGMT_FRAME_PROTECTION_OPTIONAL:
        value = IW_AUTH_MFP_OPTIONAL;
        break;
    case MGMT_FRAME_PROTECTION_REQUIRED:
        value = IW_AUTH_MFP_REQUIRED;
        break;
    };
    if (wpa_driver_wext_set_auth_param(drv, IW_AUTH_MFP, value) < 0)
        ret = -1;
#endif /* CONFIG_IEEE80211W */
    if (params->freq.freq &&
        wpa_driver_wext_set_freq(drv, params->freq.freq) < 0)
        ret = -1;
    if (!drv->cfg80211 &&
        wpa_driver_wext_set_ssid(drv, params->ssid, params->ssid_len) < 0)
        ret = -1;
    if (params->bssid &&
        wpa_driver_wext_set_bssid(drv, params->bssid) < 0)
        ret = -1;
    if (drv->cfg80211 &&
        wpa_driver_wext_set_ssid(drv, params->ssid, params->ssid_len) < 0)
        ret = -1;

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    return ret;
}

void wpa_priv_cmd_associate(struct wpa_driver_wext_data *drv,
                   void *buf, size_t len)
{
    struct wpa_driver_associate_params params;
    struct privsep_cmd_associate *assoc;
    u8 *bssid;
    int res;

//    if (iface->drv_priv == NULL || iface->driver->associate == NULL)
//        return;

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    
    if (len < sizeof(*assoc)) {
        wpa_printf(MSG_DEBUG, "Invalid association request");
        return;
    }

    assoc = (typeof assoc)buf;
    if (sizeof(*assoc) + assoc->wpa_ie_len > len) {
        wpa_printf(MSG_DEBUG, "Association request overflow");
        return;
    }

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    
    os_memset(&params, 0, sizeof(params));
    bssid = assoc->bssid;
    if (bssid[0] | bssid[1] | bssid[2] | bssid[3] | bssid[4] | bssid[5])
        params.bssid = bssid;
    params.ssid = assoc->ssid;
    if (assoc->ssid_len > SSID_MAX_LEN)
        return;
    params.ssid_len = assoc->ssid_len;
    params.freq.mode = (enum hostapd_hw_mode)assoc->hwmode;
    params.freq.freq = assoc->freq;
    params.freq.channel = assoc->channel;
    if (assoc->wpa_ie_len) {
        params.wpa_ie = (u8 *) (assoc + 1);
        params.wpa_ie_len = assoc->wpa_ie_len;
    }
    params.pairwise_suite = assoc->pairwise_suite;
    params.group_suite = assoc->group_suite;
    params.key_mgmt_suite = assoc->key_mgmt_suite;
    params.auth_alg = assoc->auth_alg;
    params.mode = assoc->mode;

    DebugLogSleep("--%s: line = %d", __FUNCTION__, __LINE__);
    
    res = wpa_driver_wext_associate(drv, &params);
    wpa_printf(MSG_DEBUG, "drv->associate: res=%d", res);
}
