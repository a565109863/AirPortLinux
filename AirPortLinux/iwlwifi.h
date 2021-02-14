//
//  iwlwifi.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/16.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef iwlwifi_h
#define iwlwifi_h

extern struct pci_driver iwl_pci_driver;

extern int iwl_drv_init(struct pci_dev *pdev);
int iwl_mvm_init(void);
int iwl_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent);


int __init cfg80211_init(void);
int __init netlink_proto_init(void);
extern struct pernet_operations netdev_net_ops;

//#include "if_iwm.h"
//#include "if_wpi.h"
//#include "if_iwn.h"
//
//extern struct cfdriver iwm_cd;
//extern struct cfdriver iwn_cd;
//extern struct cfdriver wpi_cd;
//
//
//static struct cfdriverlist cdlist = {
//    {iwm_cd, iwn_cd, wpi_cd},
//    3
//};
//
//
//extern struct cfattach iwm_ca;
//extern struct cfattach iwn_ca;
//extern struct cfattach wpi_ca;
//
//static struct cfattachlist calist = {
//    {wpi_ca,iwm_ca,iwn_ca},
//    3
//};

#endif /* iwlwifi_h */
