/*
 * WPA Supplicant - driver_wext exported functions
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef DRIVER_WEXT_H
#define DRIVER_WEXT_H


#include "AirPortLinux.hpp"
#include <linux/types.h>
#include <linux/_malloc.h>
#include <linux/device.h>
#include <linux/pci.h>
#include "iwl-trans.h"

#define os_memset memset
#define os_strlcpy strlcpy
#define os_memcpy memcpy



#define SSID_MAX_LEN 32

struct privsep_cmd_associate {
    u8 bssid[ETH_ALEN];
    u8 ssid[SSID_MAX_LEN];
    size_t ssid_len;
    int hwmode;
    int freq;
    int channel;
    int pairwise_suite;
    int group_suite;
    int key_mgmt_suite;
    int auth_alg;
    int mode;
    size_t wpa_ie_len;
    /* followed by wpa_ie_len bytes of wpa_ie */
};

#define strerror(x) x

enum wpa_alg {
    WPA_ALG_NONE,
    WPA_ALG_WEP,
    WPA_ALG_TKIP,
    WPA_ALG_CCMP,
    WPA_ALG_IGTK,
    WPA_ALG_PMK,
    WPA_ALG_GCMP,
    WPA_ALG_SMS4,
    WPA_ALG_KRK,
    WPA_ALG_GCMP_256,
    WPA_ALG_CCMP_256,
    WPA_ALG_BIP_GMAC_128,
    WPA_ALG_BIP_GMAC_256,
    WPA_ALG_BIP_CMAC_256
};

#define WPA_PROTO_WPA BIT(0)
#define WPA_PROTO_RSN BIT(1)
#define WPA_PROTO_WAPI BIT(2)
#define WPA_PROTO_OSEN BIT(3)

#define WPA_AUTH_ALG_OPEN BIT(0)
#define WPA_AUTH_ALG_SHARED BIT(1)
#define WPA_AUTH_ALG_LEAP BIT(2)
#define WPA_AUTH_ALG_FT BIT(3)
#define WPA_AUTH_ALG_SAE BIT(4)
#define WPA_AUTH_ALG_FILS BIT(5)
#define WPA_AUTH_ALG_FILS_SK_PFS BIT(6)

#define WPA_CIPHER_NONE BIT(0)
#define WPA_CIPHER_WEP40 BIT(1)
#define WPA_CIPHER_WEP104 BIT(2)
#define WPA_CIPHER_TKIP BIT(3)
#define WPA_CIPHER_CCMP BIT(4)
#define WPA_CIPHER_AES_128_CMAC BIT(5)
#define WPA_CIPHER_GCMP BIT(6)
#define WPA_CIPHER_SMS4 BIT(7)
#define WPA_CIPHER_GCMP_256 BIT(8)
#define WPA_CIPHER_CCMP_256 BIT(9)
#define WPA_CIPHER_BIP_GMAC_128 BIT(11)
#define WPA_CIPHER_BIP_GMAC_256 BIT(12)
#define WPA_CIPHER_BIP_CMAC_256 BIT(13)
#define WPA_CIPHER_GTK_NOT_USED BIT(14)

#define WPA_KEY_MGMT_IEEE8021X BIT(0)
#define WPA_KEY_MGMT_PSK BIT(1)
#define WPA_KEY_MGMT_NONE BIT(2)
#define WPA_KEY_MGMT_IEEE8021X_NO_WPA BIT(3)
#define WPA_KEY_MGMT_WPA_NONE BIT(4)
#define WPA_KEY_MGMT_FT_IEEE8021X BIT(5)
#define WPA_KEY_MGMT_FT_PSK BIT(6)
#define WPA_KEY_MGMT_IEEE8021X_SHA256 BIT(7)
#define WPA_KEY_MGMT_PSK_SHA256 BIT(8)
#define WPA_KEY_MGMT_WPS BIT(9)
#define WPA_KEY_MGMT_SAE BIT(10)
#define WPA_KEY_MGMT_FT_SAE BIT(11)
#define WPA_KEY_MGMT_WAPI_PSK BIT(12)
#define WPA_KEY_MGMT_WAPI_CERT BIT(13)
#define WPA_KEY_MGMT_CCKM BIT(14)
#define WPA_KEY_MGMT_OSEN BIT(15)
#define WPA_KEY_MGMT_IEEE8021X_SUITE_B BIT(16)
#define WPA_KEY_MGMT_IEEE8021X_SUITE_B_192 BIT(17)
#define WPA_KEY_MGMT_FILS_SHA256 BIT(18)
#define WPA_KEY_MGMT_FILS_SHA384 BIT(19)
#define WPA_KEY_MGMT_FT_FILS_SHA256 BIT(20)
#define WPA_KEY_MGMT_FT_FILS_SHA384 BIT(21)
#define WPA_KEY_MGMT_OWE BIT(22)
#define WPA_KEY_MGMT_DPP BIT(23)
#define WPA_KEY_MGMT_FT_IEEE8021X_SHA384 BIT(24)

#define WPA_KEY_MGMT_FT (WPA_KEY_MGMT_FT_PSK | \
             WPA_KEY_MGMT_FT_IEEE8021X | \
             WPA_KEY_MGMT_FT_IEEE8021X_SHA384 | \
             WPA_KEY_MGMT_FT_SAE | \
             WPA_KEY_MGMT_FT_FILS_SHA256 | \
             WPA_KEY_MGMT_FT_FILS_SHA384)

/* IEEE 802.11i */
#define PMKID_LEN 16
#define PMK_LEN 32
#define PMK_LEN_SUITE_B_192 48
#define PMK_LEN_MAX 64
#define WPA_REPLAY_COUNTER_LEN 8
#define WPA_NONCE_LEN 32
#define WPA_KEY_RSC_LEN 8
#define WPA_GMK_LEN 32
#define WPA_GTK_MAX_LEN 32


/**
 * enum mfp_options - Management frame protection (IEEE 802.11w) options
 */
enum mfp_options {
    NO_MGMT_FRAME_PROTECTION = 0,
    MGMT_FRAME_PROTECTION_OPTIONAL = 1,
    MGMT_FRAME_PROTECTION_REQUIRED = 2,
};

/**
 * enum hostapd_hw_mode - Hardware mode
 */
enum hostapd_hw_mode {
    HOSTAPD_MODE_IEEE80211B,
    HOSTAPD_MODE_IEEE80211G,
    HOSTAPD_MODE_IEEE80211A,
    HOSTAPD_MODE_IEEE80211AD,
    HOSTAPD_MODE_IEEE80211ANY,
    NUM_HOSTAPD_MODES
};

enum ieee80211_op_mode {
    IEEE80211_MODE_INFRA = 0,
    IEEE80211_MODE_IBSS = 1,
    IEEE80211_MODE_AP = 2,
    IEEE80211_MODE_MESH = 5,

    /* only add new entries before IEEE80211_MODE_NUM */
    IEEE80211_MODE_NUM
};

#define HE_MAX_MAC_CAPAB_SIZE    6
#define HE_MAX_PHY_CAPAB_SIZE    11
#define HE_MAX_MCS_CAPAB_SIZE    12
#define HE_MAX_PPET_CAPAB_SIZE    25

/**
 * struct he_capabilities - IEEE 802.11ax HE capabilities
 */
struct he_capabilities {
    u8 he_supported;
    u8 phy_cap[HE_MAX_PHY_CAPAB_SIZE];
    u8 mac_cap[HE_MAX_MAC_CAPAB_SIZE];
    u8 mcs[HE_MAX_MCS_CAPAB_SIZE];
    u8 ppet[HE_MAX_PPET_CAPAB_SIZE];
};


/**
 * struct hostapd_hw_modes - Supported hardware mode information
 */
struct hostapd_hw_modes {
    /**
     * mode - Hardware mode
     */
    enum hostapd_hw_mode mode;

    /**
     * num_channels - Number of entries in the channels array
     */
    int num_channels;

    /**
     * channels - Array of supported channels
     */
    struct hostapd_channel_data *channels;

    /**
     * num_rates - Number of entries in the rates array
     */
    int num_rates;

    /**
     * rates - Array of supported rates in 100 kbps units
     */
    int *rates;

    /**
     * ht_capab - HT (IEEE 802.11n) capabilities
     */
    u16 ht_capab;

    /**
     * mcs_set - MCS (IEEE 802.11n) rate parameters
     */
    u8 mcs_set[16];

    /**
     * a_mpdu_params - A-MPDU (IEEE 802.11n) parameters
     */
    u8 a_mpdu_params;

    /**
     * vht_capab - VHT (IEEE 802.11ac) capabilities
     */
    u32 vht_capab;

    /**
     * vht_mcs_set - VHT MCS (IEEE 802.11ac) rate parameters
     */
    u8 vht_mcs_set[8];

    unsigned int flags; /* HOSTAPD_MODE_FLAG_* */

    /**
     * he_capab - HE (IEEE 802.11ax) capabilities
     */
    struct he_capabilities he_capab[IEEE80211_MODE_NUM];
};

/**
 * enum wps_mode - WPS mode
 */
enum wps_mode {
    /**
     * WPS_MODE_NONE - No WPS provisioning being used
     */
    WPS_MODE_NONE,

    /**
     * WPS_MODE_OPEN - WPS provisioning with AP that is in open mode
     */
    WPS_MODE_OPEN,

    /**
     * WPS_MODE_PRIVACY - WPS provisioning with AP that is using protection
     */
    WPS_MODE_PRIVACY
};

/**
 * struct hostapd_freq_params - Channel parameters
 */
struct hostapd_freq_params {
    /**
     * mode - Mode/band (HOSTAPD_MODE_IEEE80211A, ..)
     */
    enum hostapd_hw_mode mode;

    /**
     * freq - Primary channel center frequency in MHz
     */
    int freq;

    /**
     * channel - Channel number
     */
    int channel;

    /**
     * ht_enabled - Whether HT is enabled
     */
    int ht_enabled;

    /**
     * sec_channel_offset - Secondary channel offset for HT40
     *
     * 0 = HT40 disabled,
     * -1 = HT40 enabled, secondary channel below primary,
     * 1 = HT40 enabled, secondary channel above primary
     */
    int sec_channel_offset;

    /**
     * vht_enabled - Whether VHT is enabled
     */
    int vht_enabled;

    /**
     * he_enabled - Whether HE is enabled
     */
    int he_enabled;

    /**
     * center_freq1 - Segment 0 center frequency in MHz
     *
     * Valid for both HT and VHT.
     */
    int center_freq1;

    /**
     * center_freq2 - Segment 1 center frequency in MHz
     *
     * Non-zero only for bandwidth 80 and an 80+80 channel
     */
    int center_freq2;

    /**
     * bandwidth - Channel bandwidth in MHz (20, 40, 80, 160)
     */
    int bandwidth;
};

/**
 * struct wpa_driver_associate_params - Association parameters
 * Data for struct wpa_driver_ops::associate().
 */
struct wpa_driver_associate_params {
    /**
     * bssid - BSSID of the selected AP
     * This can be %NULL, if ap_scan=2 mode is used and the driver is
     * responsible for selecting with which BSS to associate. */
    const u8 *bssid;

    /**
     * bssid_hint - BSSID of a proposed AP
     *
     * This indicates which BSS has been found a suitable candidate for
     * initial association for drivers that use driver/firmwate-based BSS
     * selection. Unlike the @bssid parameter, @bssid_hint does not limit
     * the driver from selecting other BSSes in the ESS.
     */
    const u8 *bssid_hint;

    /**
     * ssid - The selected SSID
     */
    const u8 *ssid;

    /**
     * ssid_len - Length of the SSID (1..32)
     */
    size_t ssid_len;

    /**
     * freq - channel parameters
     */
    struct hostapd_freq_params freq;

    /**
     * freq_hint - Frequency of the channel the proposed AP is using
     *
     * This provides a channel on which a suitable BSS has been found as a
     * hint for the driver. Unlike the @freq parameter, @freq_hint does not
     * limit the driver from selecting other channels for
     * driver/firmware-based BSS selection.
     */
    int freq_hint;

    /**
     * bg_scan_period - Background scan period in seconds, 0 to disable
     * background scan, or -1 to indicate no change to default driver
     * configuration
     */
    int bg_scan_period;

    /**
     * beacon_int - Beacon interval for IBSS or 0 to use driver default
     */
    int beacon_int;

    /**
     * wpa_ie - WPA information element for (Re)Association Request
     * WPA information element to be included in (Re)Association
     * Request (including information element id and length). Use
     * of this WPA IE is optional. If the driver generates the WPA
     * IE, it can use pairwise_suite, group_suite, group_mgmt_suite, and
     * key_mgmt_suite to select proper algorithms. In this case,
     * the driver has to notify wpa_supplicant about the used WPA
     * IE by generating an event that the interface code will
     * convert into EVENT_ASSOCINFO data (see below).
     *
     * When using WPA2/IEEE 802.11i, wpa_ie is used for RSN IE
     * instead. The driver can determine which version is used by
     * looking at the first byte of the IE (0xdd for WPA, 0x30 for
     * WPA2/RSN).
     *
     * When using WPS, wpa_ie is used for WPS IE instead of WPA/RSN IE.
     */
    const u8 *wpa_ie;

    /**
     * wpa_ie_len - length of the wpa_ie
     */
    size_t wpa_ie_len;

    /**
     * wpa_proto - Bitfield of WPA_PROTO_* values to indicate WPA/WPA2
     */
    unsigned int wpa_proto;

    /**
     * pairwise_suite - Selected pairwise cipher suite (WPA_CIPHER_*)
     *
     * This is usually ignored if @wpa_ie is used.
     */
    unsigned int pairwise_suite;

    /**
     * group_suite - Selected group cipher suite (WPA_CIPHER_*)
     *
     * This is usually ignored if @wpa_ie is used.
     */
    unsigned int group_suite;

    /**
     * mgmt_group_suite - Selected group management cipher suite (WPA_CIPHER_*)
     *
     * This is usually ignored if @wpa_ie is used.
     */
    unsigned int mgmt_group_suite;

    /**
     * key_mgmt_suite - Selected key management suite (WPA_KEY_MGMT_*)
     *
     * This is usually ignored if @wpa_ie is used.
     */
    unsigned int key_mgmt_suite;

    /**
     * auth_alg - Allowed authentication algorithms
     * Bit field of WPA_AUTH_ALG_*
     */
    int auth_alg;

    /**
     * mode - Operation mode (infra/ibss) IEEE80211_MODE_*
     */
    int mode;

    /**
     * wep_key - WEP keys for static WEP configuration
     */
    const u8 *wep_key[4];

    /**
     * wep_key_len - WEP key length for static WEP configuration
     */
    size_t wep_key_len[4];

    /**
     * wep_tx_keyidx - WEP TX key index for static WEP configuration
     */
    int wep_tx_keyidx;

    /**
     * mgmt_frame_protection - IEEE 802.11w management frame protection
     */
    enum mfp_options mgmt_frame_protection;

    /**
     * passphrase - RSN passphrase for PSK
     *
     * This value is made available only for WPA/WPA2-Personal (PSK) and
     * only for drivers that set WPA_DRIVER_FLAGS_4WAY_HANDSHAKE_PSK. This
     * is the 8..63 character ASCII passphrase, if available. Please note
     * that this can be %NULL if passphrase was not used to generate the
     * PSK. In that case, the psk field must be used to fetch the PSK.
     */
    const char *passphrase;

    /**
     * psk - RSN PSK (alternative for passphrase for PSK)
     *
     * This value is made available only for WPA/WPA2-Personal (PSK) and
     * only for drivers that set WPA_DRIVER_FLAGS_4WAY_HANDSHAKE_PSK. This
     * is the 32-octet (256-bit) PSK, if available. The driver wrapper
     * should be prepared to handle %NULL value as an error.
     */
    const u8 *psk;

    /**
     * drop_unencrypted - Enable/disable unencrypted frame filtering
     *
     * Configure the driver to drop all non-EAPOL frames (both receive and
     * transmit paths). Unencrypted EAPOL frames (ethertype 0x888e) must
     * still be allowed for key negotiation.
     */
    int drop_unencrypted;

    /**
     * prev_bssid - Previously used BSSID in this ESS
     *
     * When not %NULL, this is a request to use reassociation instead of
     * association.
     */
    const u8 *prev_bssid;

    /**
     * wps - WPS mode
     *
     * If the driver needs to do special configuration for WPS association,
     * this variable provides more information on what type of association
     * is being requested. Most drivers should not need ot use this.
     */
    enum wps_mode wps;

    /**
     * p2p - Whether this connection is a P2P group
     */
    int p2p;

    /**
     * uapsd - UAPSD parameters for the network
     * -1 = do not change defaults
     * AP mode: 1 = enabled, 0 = disabled
     * STA mode: bits 0..3 UAPSD enabled for VO,VI,BK,BE
     */
    int uapsd;

    /**
     * fixed_bssid - Whether to force this BSSID in IBSS mode
     * 1 = Fix this BSSID and prevent merges.
     * 0 = Do not fix BSSID.
     */
    int fixed_bssid;

    /**
     * fixed_freq - Fix control channel in IBSS mode
     * 0 = don't fix control channel (default)
     * 1 = fix control channel; this prevents IBSS merging with another
     *    channel
     */
    int fixed_freq;

    /**
     * disable_ht - Disable HT (IEEE 802.11n) for this connection
     */
    int disable_ht;

    /**
     * htcaps - HT Capabilities over-rides
     *
     * Only bits set in the mask will be used, and not all values are used
     * by the kernel anyway. Currently, MCS, MPDU and MSDU fields are used.
     *
     * Pointer to struct ieee80211_ht_capabilities.
     */
    const u8 *htcaps;

    /**
     * htcaps_mask - HT Capabilities over-rides mask
     *
     * Pointer to struct ieee80211_ht_capabilities.
     */
    const u8 *htcaps_mask;

#ifdef CONFIG_VHT_OVERRIDES
    /**
     * disable_vht - Disable VHT for this connection
     */
    int disable_vht;

    /**
     * VHT capability overrides.
     */
    const struct ieee80211_vht_capabilities *vhtcaps;
    const struct ieee80211_vht_capabilities *vhtcaps_mask;
#endif /* CONFIG_VHT_OVERRIDES */

    /**
     * req_key_mgmt_offload - Request key management offload for connection
     *
     * Request key management offload for this connection if the device
     * supports it.
     */
    int req_key_mgmt_offload;

    /**
     * req_handshake_offload - Request EAPOL handshake offload
     *
     * Request EAPOL handshake offload for this connection if the device
     * supports it.
     */
    int req_handshake_offload;

    /**
     * Flag for indicating whether this association includes support for
     * RRM (Radio Resource Measurements)
     */
    int rrm_used;

    /**
     * pbss - If set, connect to a PCP in a PBSS. Otherwise, connect to an
     * AP as usual. Valid for DMG network only.
     */
    int pbss;

    /**
     * fils_kek - KEK for FILS association frame protection (AES-SIV)
     */
    const u8 *fils_kek;

    /**
     * fils_kek_len: Length of fils_kek in bytes
     */
    size_t fils_kek_len;

    /**
     * fils_nonces - Nonces for FILS association frame protection
     * (AES-SIV AAD)
     */
    const u8 *fils_nonces;

    /**
     * fils_nonces_len: Length of fils_nonce in bytes
     */
    size_t fils_nonces_len;

    /**
     * fils_erp_username - Username part of keyName-NAI
     */
    const u8 *fils_erp_username;

    /**
     * fils_erp_username_len - Length of fils_erp_username in bytes
     */
    size_t fils_erp_username_len;

    /**
     * fils_erp_realm - Realm/domain name to use in FILS ERP
     */
    const u8 *fils_erp_realm;

    /**
     * fils_erp_realm_len - Length of fils_erp_realm in bytes
     */
    size_t fils_erp_realm_len;

    /**
     * fils_erp_next_seq_num - The next sequence number to use in FILS ERP
     * messages
     */
    u16 fils_erp_next_seq_num;

    /**
     * fils_erp_rrk - Re-authentication root key (rRK) for the keyName-NAI
     * specified by fils_erp_username@fils_erp_realm.
     */
    const u8 *fils_erp_rrk;

    /**
     * fils_erp_rrk_len - Length of fils_erp_rrk in bytes
     */
    size_t fils_erp_rrk_len;
};

//#include <net/if.h>
#define wpa_printf(msg, arg...) kprintf(arg)


enum hide_ssid {
    NO_SSID_HIDING,
    HIDDEN_SSID_ZERO_LEN,
    HIDDEN_SSID_ZERO_CONTENTS
};

enum ch_switch_state {
    CH_SW_STARTED,
    CH_SW_FINISHED
};

struct wowlan_triggers {
    u8 any;
    u8 disconnect;
    u8 magic_pkt;
    u8 gtk_rekey_failure;
    u8 eap_identity_req;
    u8 four_way_handshake;
    u8 rfkill_release;
};

/**
 * struct wpa_driver_capa - Driver capability information
 */
struct wpa_driver_capa {
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA        0x00000001
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA2        0x00000002
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA_PSK    0x00000004
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA2_PSK    0x00000008
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA_NONE    0x00000010
#define WPA_DRIVER_CAPA_KEY_MGMT_FT        0x00000020
#define WPA_DRIVER_CAPA_KEY_MGMT_FT_PSK        0x00000040
#define WPA_DRIVER_CAPA_KEY_MGMT_WAPI_PSK    0x00000080
#define WPA_DRIVER_CAPA_KEY_MGMT_SUITE_B    0x00000100
#define WPA_DRIVER_CAPA_KEY_MGMT_SUITE_B_192    0x00000200
#define WPA_DRIVER_CAPA_KEY_MGMT_OWE        0x00000400
#define WPA_DRIVER_CAPA_KEY_MGMT_DPP        0x00000800
#define WPA_DRIVER_CAPA_KEY_MGMT_FILS_SHA256    0x00001000
#define WPA_DRIVER_CAPA_KEY_MGMT_FILS_SHA384    0x00002000
#define WPA_DRIVER_CAPA_KEY_MGMT_FT_FILS_SHA256 0x00004000
#define WPA_DRIVER_CAPA_KEY_MGMT_FT_FILS_SHA384 0x00008000
#define WPA_DRIVER_CAPA_KEY_MGMT_SAE         0x00010000
    /** Bitfield of supported key management suites */
    unsigned int key_mgmt;

#define WPA_DRIVER_CAPA_ENC_WEP40    0x00000001
#define WPA_DRIVER_CAPA_ENC_WEP104    0x00000002
#define WPA_DRIVER_CAPA_ENC_TKIP    0x00000004
#define WPA_DRIVER_CAPA_ENC_CCMP    0x00000008
#define WPA_DRIVER_CAPA_ENC_WEP128    0x00000010
#define WPA_DRIVER_CAPA_ENC_GCMP    0x00000020
#define WPA_DRIVER_CAPA_ENC_GCMP_256    0x00000040
#define WPA_DRIVER_CAPA_ENC_CCMP_256    0x00000080
#define WPA_DRIVER_CAPA_ENC_BIP        0x00000100
#define WPA_DRIVER_CAPA_ENC_BIP_GMAC_128    0x00000200
#define WPA_DRIVER_CAPA_ENC_BIP_GMAC_256    0x00000400
#define WPA_DRIVER_CAPA_ENC_BIP_CMAC_256    0x00000800
#define WPA_DRIVER_CAPA_ENC_GTK_NOT_USED    0x00001000
    /** Bitfield of supported cipher suites */
    unsigned int enc;

#define WPA_DRIVER_AUTH_OPEN        0x00000001
#define WPA_DRIVER_AUTH_SHARED        0x00000002
#define WPA_DRIVER_AUTH_LEAP        0x00000004
    /** Bitfield of supported IEEE 802.11 authentication algorithms */
    unsigned int auth;

/** Driver generated WPA/RSN IE */
#define WPA_DRIVER_FLAGS_DRIVER_IE    0x00000001
/** Driver needs static WEP key setup after association command */
#define WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC 0x00000002
/** Driver takes care of all DFS operations */
#define WPA_DRIVER_FLAGS_DFS_OFFLOAD            0x00000004
/** Driver takes care of RSN 4-way handshake internally; PMK is configured with
 * struct wpa_driver_ops::set_key using alg = WPA_ALG_PMK */
#define WPA_DRIVER_FLAGS_4WAY_HANDSHAKE_8021X        0x00000008
/** Driver is for a wired Ethernet interface */
#define WPA_DRIVER_FLAGS_WIRED        0x00000010
/** Driver provides separate commands for authentication and association (SME in
 * wpa_supplicant). */
#define WPA_DRIVER_FLAGS_SME        0x00000020
/** Driver supports AP mode */
#define WPA_DRIVER_FLAGS_AP        0x00000040
/** Driver needs static WEP key setup after association has been completed */
#define WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC_DONE    0x00000080
/** Driver supports dynamic HT 20/40 MHz channel changes during BSS lifetime */
#define WPA_DRIVER_FLAGS_HT_2040_COEX            0x00000100
/** Driver supports concurrent P2P operations */
#define WPA_DRIVER_FLAGS_P2P_CONCURRENT    0x00000200
/**
 * Driver uses the initial interface as a dedicated management interface, i.e.,
 * it cannot be used for P2P group operations or non-P2P purposes.
 */
#define WPA_DRIVER_FLAGS_P2P_DEDICATED_INTERFACE    0x00000400
/** This interface is P2P capable (P2P GO or P2P Client) */
#define WPA_DRIVER_FLAGS_P2P_CAPABLE    0x00000800
/** Driver supports station and key removal when stopping an AP */
#define WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT        0x00001000
/**
 * Driver uses the initial interface for P2P management interface and non-P2P
 * purposes (e.g., connect to infra AP), but this interface cannot be used for
 * P2P group operations.
 */
#define WPA_DRIVER_FLAGS_P2P_MGMT_AND_NON_P2P        0x00002000
/**
 * Driver is known to use sane error codes, i.e., when it indicates that
 * something (e.g., association) fails, there was indeed a failure and the
 * operation does not end up getting completed successfully later.
 */
#define WPA_DRIVER_FLAGS_SANE_ERROR_CODES        0x00004000
/** Driver supports off-channel TX */
#define WPA_DRIVER_FLAGS_OFFCHANNEL_TX            0x00008000
/** Driver indicates TX status events for EAPOL Data frames */
#define WPA_DRIVER_FLAGS_EAPOL_TX_STATUS        0x00010000
/** Driver indicates TX status events for Deauth/Disassoc frames */
#define WPA_DRIVER_FLAGS_DEAUTH_TX_STATUS        0x00020000
/** Driver supports roaming (BSS selection) in firmware */
#define WPA_DRIVER_FLAGS_BSS_SELECTION            0x00040000
/** Driver supports operating as a TDLS peer */
#define WPA_DRIVER_FLAGS_TDLS_SUPPORT            0x00080000
/** Driver requires external TDLS setup/teardown/discovery */
#define WPA_DRIVER_FLAGS_TDLS_EXTERNAL_SETUP        0x00100000
/** Driver indicates support for Probe Response offloading in AP mode */
#define WPA_DRIVER_FLAGS_PROBE_RESP_OFFLOAD        0x00200000
/** Driver supports U-APSD in AP mode */
#define WPA_DRIVER_FLAGS_AP_UAPSD            0x00400000
/** Driver supports inactivity timer in AP mode */
#define WPA_DRIVER_FLAGS_INACTIVITY_TIMER        0x00800000
/** Driver expects user space implementation of MLME in AP mode */
#define WPA_DRIVER_FLAGS_AP_MLME            0x01000000
/** Driver supports SAE with user space SME */
#define WPA_DRIVER_FLAGS_SAE                0x02000000
/** Driver makes use of OBSS scan mechanism in wpa_supplicant */
#define WPA_DRIVER_FLAGS_OBSS_SCAN            0x04000000
/** Driver supports IBSS (Ad-hoc) mode */
#define WPA_DRIVER_FLAGS_IBSS                0x08000000
/** Driver supports radar detection */
#define WPA_DRIVER_FLAGS_RADAR                0x10000000
/** Driver supports a dedicated interface for P2P Device */
#define WPA_DRIVER_FLAGS_DEDICATED_P2P_DEVICE        0x20000000
/** Driver supports QoS Mapping */
#define WPA_DRIVER_FLAGS_QOS_MAPPING            0x40000000
/** Driver supports CSA in AP mode */
#define WPA_DRIVER_FLAGS_AP_CSA                0x80000000
/** Driver supports mesh */
#define WPA_DRIVER_FLAGS_MESH            0x0000000100000000ULL
/** Driver support ACS offload */
#define WPA_DRIVER_FLAGS_ACS_OFFLOAD        0x0000000200000000ULL
/** Driver supports key management offload */
#define WPA_DRIVER_FLAGS_KEY_MGMT_OFFLOAD    0x0000000400000000ULL
/** Driver supports TDLS channel switching */
#define WPA_DRIVER_FLAGS_TDLS_CHANNEL_SWITCH    0x0000000800000000ULL
/** Driver supports IBSS with HT datarates */
#define WPA_DRIVER_FLAGS_HT_IBSS        0x0000001000000000ULL
/** Driver supports IBSS with VHT datarates */
#define WPA_DRIVER_FLAGS_VHT_IBSS        0x0000002000000000ULL
/** Driver supports automatic band selection */
#define WPA_DRIVER_FLAGS_SUPPORT_HW_MODE_ANY    0x0000004000000000ULL
/** Driver supports simultaneous off-channel operations */
#define WPA_DRIVER_FLAGS_OFFCHANNEL_SIMULTANEOUS    0x0000008000000000ULL
/** Driver supports full AP client state */
#define WPA_DRIVER_FLAGS_FULL_AP_CLIENT_STATE    0x0000010000000000ULL
/** Driver supports P2P Listen offload */
#define WPA_DRIVER_FLAGS_P2P_LISTEN_OFFLOAD     0x0000020000000000ULL
/** Driver supports FILS */
#define WPA_DRIVER_FLAGS_SUPPORT_FILS        0x0000040000000000ULL
/** Driver supports Beacon frame TX rate configuration (legacy rates) */
#define WPA_DRIVER_FLAGS_BEACON_RATE_LEGACY    0x0000080000000000ULL
/** Driver supports Beacon frame TX rate configuration (HT rates) */
#define WPA_DRIVER_FLAGS_BEACON_RATE_HT        0x0000100000000000ULL
/** Driver supports Beacon frame TX rate configuration (VHT rates) */
#define WPA_DRIVER_FLAGS_BEACON_RATE_VHT    0x0000200000000000ULL
/** Driver supports mgmt_tx with random TX address in non-connected state */
#define WPA_DRIVER_FLAGS_MGMT_TX_RANDOM_TA    0x0000400000000000ULL
/** Driver supports mgmt_tx with random TX addr in connected state */
#define WPA_DRIVER_FLAGS_MGMT_TX_RANDOM_TA_CONNECTED    0x0000800000000000ULL
/** Driver supports better BSS reporting with sched_scan in connected mode */
#define WPA_DRIVER_FLAGS_SCHED_SCAN_RELATIVE_RSSI    0x0001000000000000ULL
/** Driver supports HE capabilities */
#define WPA_DRIVER_FLAGS_HE_CAPABILITIES    0x0002000000000000ULL
/** Driver supports FILS shared key offload */
#define WPA_DRIVER_FLAGS_FILS_SK_OFFLOAD    0x0004000000000000ULL
/** Driver supports all OCE STA specific mandatory features */
#define WPA_DRIVER_FLAGS_OCE_STA        0x0008000000000000ULL
/** Driver supports all OCE AP specific mandatory features */
#define WPA_DRIVER_FLAGS_OCE_AP            0x0010000000000000ULL
/**
 * Driver supports all OCE STA-CFON specific mandatory features only.
 * If a driver sets this bit but not the %WPA_DRIVER_FLAGS_OCE_AP, the
 * userspace shall assume that this driver may not support all OCE AP
 * functionality but can support only OCE STA-CFON functionality.
 */
#define WPA_DRIVER_FLAGS_OCE_STA_CFON        0x0020000000000000ULL
/** Driver supports MFP-optional in the connect command */
#define WPA_DRIVER_FLAGS_MFP_OPTIONAL        0x0040000000000000ULL
/** Driver is a self-managed regulatory device */
#define WPA_DRIVER_FLAGS_SELF_MANAGED_REGULATORY       0x0080000000000000ULL
/** Driver supports FTM responder functionality */
#define WPA_DRIVER_FLAGS_FTM_RESPONDER        0x0100000000000000ULL
/** Driver support 4-way handshake offload for WPA-Personal */
#define WPA_DRIVER_FLAGS_4WAY_HANDSHAKE_PSK    0x0200000000000000ULL
    u64 flags;

#define FULL_AP_CLIENT_STATE_SUPP(drv_flags) \
    (drv_flags & WPA_DRIVER_FLAGS_FULL_AP_CLIENT_STATE)

#define WPA_DRIVER_SMPS_MODE_STATIC            0x00000001
#define WPA_DRIVER_SMPS_MODE_DYNAMIC            0x00000002
    unsigned int smps_modes;

    unsigned int wmm_ac_supported:1;

    unsigned int mac_addr_rand_scan_supported:1;
    unsigned int mac_addr_rand_sched_scan_supported:1;

    /** Maximum number of supported active probe SSIDs */
    int max_scan_ssids;

    /** Maximum number of supported active probe SSIDs for sched_scan */
    int max_sched_scan_ssids;

    /** Maximum number of supported scan plans for scheduled scan */
    unsigned int max_sched_scan_plans;

    /** Maximum interval in a scan plan. In seconds */
    u32 max_sched_scan_plan_interval;

    /** Maximum number of iterations in a single scan plan */
    u32 max_sched_scan_plan_iterations;

    /** Whether sched_scan (offloaded scanning) is supported */
    int sched_scan_supported;

    /** Maximum number of supported match sets for sched_scan */
    int max_match_sets;

    /**
     * max_remain_on_chan - Maximum remain-on-channel duration in msec
     */
    unsigned int max_remain_on_chan;

    /**
     * max_stations - Maximum number of associated stations the driver
     * supports in AP mode
     */
    unsigned int max_stations;

    /**
     * probe_resp_offloads - Bitmap of supported protocols by the driver
     * for Probe Response offloading.
     */
/** Driver Probe Response offloading support for WPS ver. 1 */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_WPS        0x00000001
/** Driver Probe Response offloading support for WPS ver. 2 */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_WPS2        0x00000002
/** Driver Probe Response offloading support for P2P */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_P2P        0x00000004
/** Driver Probe Response offloading support for IEEE 802.11u (Interworking) */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_INTERWORKING    0x00000008
    unsigned int probe_resp_offloads;

    unsigned int max_acl_mac_addrs;

    /**
     * Number of supported concurrent channels
     */
    unsigned int num_multichan_concurrent;

    /**
     * extended_capa - extended capabilities in driver/device
     *
     * Must be allocated and freed by driver and the pointers must be
     * valid for the lifetime of the driver, i.e., freed in deinit()
     */
    const u8 *extended_capa, *extended_capa_mask;
    unsigned int extended_capa_len;

    struct wowlan_triggers wowlan_triggers;

/** Driver adds the DS Params Set IE in Probe Request frames */
#define WPA_DRIVER_FLAGS_DS_PARAM_SET_IE_IN_PROBES    0x00000001
/** Driver adds the WFA TPC IE in Probe Request frames */
#define WPA_DRIVER_FLAGS_WFA_TPC_IE_IN_PROBES        0x00000002
/** Driver handles quiet period requests */
#define WPA_DRIVER_FLAGS_QUIET                0x00000004
/**
 * Driver is capable of inserting the current TX power value into the body of
 * transmitted frames.
 * Background: Some Action frames include a TPC Report IE. This IE contains a
 * TX power field, which has to be updated by lower layers. One such Action
 * frame is Link Measurement Report (part of RRM). Another is TPC Report (part
 * of spectrum management). Note that this insertion takes place at a fixed
 * offset, namely the 6th byte in the Action frame body.
 */
#define WPA_DRIVER_FLAGS_TX_POWER_INSERTION        0x00000008
/**
 * Driver supports RRM. With this support, the driver will accept to use RRM in
 * (Re)Association Request frames, without supporting quiet period.
 */
#define WPA_DRIVER_FLAGS_SUPPORT_RRM            0x00000010

/** Driver supports setting the scan dwell time */
#define WPA_DRIVER_FLAGS_SUPPORT_SET_SCAN_DWELL        0x00000020
/** Driver supports Beacon Report Measurement */
#define WPA_DRIVER_FLAGS_SUPPORT_BEACON_REPORT        0x00000040

    u32 rrm_flags;

    /* Driver concurrency capabilities */
    unsigned int conc_capab;
    /* Maximum number of concurrent channels on 2.4 GHz */
    unsigned int max_conc_chan_2_4;
    /* Maximum number of concurrent channels on 5 GHz */
    unsigned int max_conc_chan_5_0;

    /* Maximum number of supported CSA counters */
    u16 max_csa_counters;
};

struct wpa_driver_wext_data {
    void *ctx;
//    struct netlink_data *netlink;
    int ioctl_sock;
    int mlme_sock;
    char ifname[IFNAMSIZ + 1];
    char phyname[32];
    int ifindex;
    int ifindex2;
    int if_removed;
    int if_disabled;
//    struct rfkill_data *rfkill;
    u8 *assoc_req_ies;
    size_t assoc_req_ies_len;
    u8 *assoc_resp_ies;
    size_t assoc_resp_ies_len;
    struct wpa_driver_capa capa;
    int has_capability;
    int we_version_compiled;

    /* for set_auth_alg fallback */
    int use_crypt;
    int auth_alg_fallback;

    int operstate;

    char mlmedev[IFNAMSIZ + 1];

    int scan_complete_events;

    int cfg80211; /* whether driver is using cfg80211 */

    u8 max_level;
};

//int wpa_driver_wext_get_bssid(void *priv, u8 *bssid);
//int wpa_driver_wext_set_bssid(void *priv, const u8 *bssid);
//int wpa_driver_wext_get_ssid(void *priv, u8 *ssid);
//int wpa_driver_wext_set_ssid(void *priv, const u8 *ssid, size_t ssid_len);
//int wpa_driver_wext_set_freq(void *priv, int freq);
//int wpa_driver_wext_set_mode(void *priv, int mode);
//int wpa_driver_wext_set_key(const char *ifname, void *priv, enum wpa_alg alg,
//                const u8 *addr, int key_idx,
//                int set_tx, const u8 *seq, size_t seq_len,
//                const u8 *key, size_t key_len);
//int wpa_driver_wext_scan(void *priv, struct wpa_driver_scan_params *params);
//struct wpa_scan_results * wpa_driver_wext_get_scan_results(void *priv);
//
//void wpa_driver_wext_scan_timeout(void *eloop_ctx, void *timeout_ctx);
//
//int wpa_driver_wext_alternative_ifindex(struct wpa_driver_wext_data *drv,
//                    const char *ifname);
//
//void * wpa_driver_wext_init(void *ctx, const char *ifname);
//void wpa_driver_wext_deinit(void *priv);
//
//int wpa_driver_wext_set_operstate(void *priv, int state);
//int wpa_driver_wext_get_version(struct wpa_driver_wext_data *drv);
//
//int wpa_driver_wext_associate(void *priv,
//                  struct wpa_driver_associate_params *params);
//int wpa_driver_wext_get_capa(void *priv, struct wpa_driver_capa *capa);
//int wpa_driver_wext_set_auth_param(struct wpa_driver_wext_data *drv,
//                   int idx, u32 value);
//int wpa_driver_wext_cipher2wext(int cipher);
//int wpa_driver_wext_keymgmt2wext(int keymgmt);


void wpa_priv_cmd_associate(struct wpa_driver_wext_data *drv, void *buf, size_t len);

#endif /* DRIVER_WEXT_H */
