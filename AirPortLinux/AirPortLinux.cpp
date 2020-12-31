/* add your code here */

#include "AirPortLinux.hpp"

#include <linux/types.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <net/wireless/core.h>

OSDefineMetaClassAndStructors(AirPortLinux, IOController);
//OSDefineMetaClassAndStructors(IOKitTimeout, OSObject)
#define super IO80211Controller

int _stop(struct kmod_info*, void*) {
    IOLog("_stop(struct kmod_info*, void*) has been invoked\n");
    return 0;
};
int _start(struct kmod_info*, void*) {
    IOLog("_start(struct kmod_info*, void*) has been invoked\n");
    return 0;
};

//struct ifnet *_ifp;
struct pci_dev *_pdev;

bool AirPortLinux::init(OSDictionary* parameters) {
    IOLog("AirPortLinux: Init");
    
    if (!super::init(parameters)) {
        IOLog("AirPortLinux: Failed to call IO80211Controller::init!");
        return false;
    }
    return true;
}

IOService* AirPortLinux::probe(IOService* provider, SInt32 *score)
{
    super::probe(provider, score);
    IOPCIDevice* _fPciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!_fPciDevice) {
        return NULL;
    }
    
//    struct pci_attach_args *pa = IONew(struct pci_attach_args, 1);
//    this->pa.dev.dev = this;
//    this->pa.pa_tag = device;
//
//    this->pa.vendor = this->pa.pa_tag->extendedConfigRead16(kIOPCIConfigVendorID);
//    this->pa.device = this->pa.pa_tag->extendedConfigRead16(kIOPCIConfigDeviceID);
//    this->pa.pa_id = (this->pa.device << 16) + pa.vendor;
//    this->pa.subsystem_vendor = this->pa.pa_tag->extendedConfigRead16(kIOPCIConfigSubSystemVendorID);
//    this->pa.subsystem_device = this->pa.pa_tag->extendedConfigRead16(kIOPCIConfigSubSystemID);
//    this->pa.revision = this->pa.pa_tag->extendedConfigRead8(kIOPCIConfigRevisionID);
//
//    for (int i = 0; i < cdlist.size; i++) {
//        this->ca = &calist.ca[i];
//        if (this->ca->ca_match((struct device *)provider, this, &this->pa)) {
//            return this;
//        }
//    }
    
    
    struct pci_dev *__pdev = IONew(struct pci_dev, 1);
    __pdev->dev.dev = this;
    __pdev->dev.dev->fPciDevice = _fPciDevice;
    __pdev->vendor = _fPciDevice->extendedConfigRead16(kIOPCIConfigVendorID);
    __pdev->device = _fPciDevice->extendedConfigRead16(kIOPCIConfigDeviceID);
    __pdev->subsystem_vendor = _fPciDevice->extendedConfigRead16(kIOPCIConfigSubSystemVendorID);
    __pdev->subsystem_device = _fPciDevice->extendedConfigRead16(kIOPCIConfigSubSystemID);
    
    int err;
    err = iwl_drv_init(__pdev);
    if (err)
        goto fail;
    
    err = iwl_mvm_init();
    if (err)
        goto fail;
    
    this->pdev = __pdev;
    return this;
    
fail:
    IODelete(_pdev, struct pci_dev, 1);
    return NULL;
}

bool AirPortLinux::start(IOService* provider) {
    IOLog("AirPortLinux: Start");
    if (!super::start(provider)) {
        IOLog("AirPortLinux: Failed to call IO80211Controller::start!");
        return false;
    }
    
    fPciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!fPciDevice) {
        IOLog("AirPortLinux: Failed to cast provider to IOPCIDevice!");
        return false;
    }
    
    fPciDevice->retain();
    
    if (!fPciDevice->open(this)) {
        IOLog("AirPortLinux: Failed to open provider.\n");
        return false;
    }
    
    if (fPciDevice->requestPowerDomainState(kIOPMPowerOn,
                                            (IOPowerConnection *) getParentEntry(gIOPowerPlane),
                                            IOPMLowestState ) != IOPMNoErr) {
        IOLog("%s Power thingi failed\n", getName());
        return  false;
    }
    
    fWorkloop = OSDynamicCast(IO80211WorkLoop, getWorkLoop());
    if (!fWorkloop) {
        IOLog("AirPortLinux: Failed to get workloop!");
        return false;
    }
    
    fWorkloop->retain();
    
    fCommandGate = IOCommandGate::commandGate(this, (IOCommandGate::Action)tsleepHandler);
    if (!fCommandGate) {
        IOLog("AirPortLinux: Failed to create command gate!");
        return false;
    }
    
    if (fWorkloop->addEventSource(fCommandGate) != kIOReturnSuccess) {
        IOLog("AirPortLinux: Failed to register command gate event source!");
        return false;
    }
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    fCommandGate->enable();
    _pdev = this->pdev;
    this->pdev->dev.dev = this;
    memcpy(this->pdev->dev.name, "AirPortLinux", sizeof(this->pdev->dev.name));
    int err = iwl_pci_probe(this->pdev, this->pdev->dev.ent);
    
    if (err)
        return NULL;
    DebugLog("--%s: line = %d err = %d", __FUNCTION__, __LINE__, err);
    
    struct _ifreq ifr = {};
    int ret;

    strncpy(ifr.ifr_name, "wlan0", sizeof(ifr.ifr_name));
    ret = ioctl(&init_net, SIOCGIFHWADDR, &ifr, NULL);
    DebugLog("--%s: ifr.ifr_name = %s, address %s\n", __FUNCTION__, ifr.ifr_name, ether_sprintf((u_char *)ifr.ifr_hwaddr.sa_data));
    DebugLog("--%s: ifr.ifr_name = %s, address %s\n", __FUNCTION__, ifr.ifr_name, ether_sprintf((u_char *)ifr.ifr_hwaddr.sa_data));
//    return NULL;
    
    fWatchdogTimer = IOTimerEventSource::timerEventSource(this, OSMemberFunctionCast(IOTimerEventSource::Action, this, &AirPortLinux::if_watchdog));
    
    if (!fWatchdogTimer) {
        IOLog("AirPortLinux: Failed to create IOTimerEventSource.\n");
        return false;
    }
    fWorkloop->addEventSource(fWatchdogTimer);
    
    if (!attachInterface((IONetworkInterface**) &this->iface, true)) {
        panic("AirPortLinux: Failed to attach interface!");
    }
    
    
    this->mediumDict = OSDictionary::withCapacity(1);
    this->addMediumType(kIOMediumIEEE80211Auto, 128000,  MEDIUM_TYPE_AUTO);
    this->publishMediumDictionary(this->mediumDict);
    this->setCurrentMedium(this->mediumTable[MEDIUM_TYPE_AUTO]);
    this->setSelectedMedium(this->mediumTable[MEDIUM_TYPE_AUTO]);
    
    
    registerService();
    this->iface->registerService();
    
    return true;
}

void AirPortLinux::stop(IOService* provider) {
    if (fCommandGate) {
        fCommandGate->disable();
        if (fWorkloop) {
            fWorkloop->removeEventSource(fCommandGate);
        }
    }
    
    super::stop(provider);
}

void AirPortLinux::free() {
    IOLog("AirPortLinux: Free");
    
    RELEASE(fCommandGate);
    RELEASE(mediumDict);
    RELEASE(fPciDevice);

    super::free();
}

IOReturn AirPortLinux::getHardwareAddress(IOEthernetAddress* addr)
{
//    struct cfg80211_registered_device *rdev = (struct cfg80211_registered_device *)_pdev->dev.platform_data;
//
//    if (rdev->wiphy.n_addresses > 0) {
//        bcopy(rdev->wiphy.addresses[0].addr, addr->bytes, kIOEthernetAddressSize);
//    }
    
    struct _ifreq ifr = {};
    int ret;
    
    strncpy(ifr.ifr_name, "wlan0", sizeof(ifr.ifr_name));
    ret = ioctl(&init_net, SIOCGIFHWADDR, &ifr, NULL);
    if (ret) {
        return kIOReturnError;
    }
    
    DebugLog("--%s: ifr.ifr_name = %s, address %s\n", __FUNCTION__, ifr.ifr_name, ether_sprintf((u_char *)ifr.ifr_hwaddr.sa_data));
    
    bcopy(ifr.ifr_hwaddr.sa_data, addr->bytes, kIOEthernetAddressSize);
    
    return kIOReturnSuccess;
}

IOReturn AirPortLinux::outputStart(IONetworkInterface *interface, IOOptionBits options)
{
    mbuf_t m;
    while ((interface->dequeueOutputPackets(1, &m) == kIOReturnSuccess)) {
        IOReturn ret = this->outputPacket(m, NULL);
        if (ret != kIOReturnSuccess) {
//            _ifp->if_oerrors++;
            return ret;
        }
    }

    return kIOReturnNoResources;
}

UInt32 AirPortLinux::outputPacket(mbuf_t m, void* param) {
    
    int error;
    
    if (m == NULL) {
        return kIOReturnOutputDropped;
    }
    if (!(mbuf_flags(m) & MBUF_PKTHDR) ){
        mbuf_freem(m);
        return kIOReturnOutputDropped;
    }
    if (mbuf_type(m) == MBUF_TYPE_FREE) {
        return kIOReturnOutputDropped;
    }
    
//    IFQ_ENQUEUE(&_ifp->if_snd, m, error);
//    if (error) {
//        return kIOReturnOutputDropped;
//    }
    
//    if_start(_ifp);
    
    return kIOReturnSuccess;
}

// Power Management
IOReturn
AirPortLinux::registerWithPolicyMaker(IOService* provider)
{
    static IOPMPowerState powerStateArray[ 2 ] = {
        { 1,0,0,0,0,0,0,0,0,0,0,0 },
        { 1,kIOPMDeviceUsable,kIOPMPowerOn,kIOPMPowerOn,0,0,0,0,0,0,0,0 }
    };
    return provider->registerPowerDriver( this, powerStateArray, 2 );
}

IOReturn AirPortLinux::setPowerState(unsigned long powerStateOrdinal, IOService *policyMaker)
{
    IOReturn result = IOPMAckImplied;
    
    if (powerStateOrdinal == powerState) {
        goto done;
    }

    fCommandGate->runAction(setPowerStateAction, &powerStateOrdinal);

done:
    return result;
}

bool AirPortLinux::configureInterface(IONetworkInterface * netif)
{
    IONetworkData *data;
    if (super::configureInterface(netif) == false)
        return false;
    
    // Get the generic network statistics structure.
    
    data = netif->getParameter(kIONetworkStatsKey);
    if (!data || !(netStats = (IONetworkStats *)data->getBuffer())) {
        return false;
    }
    
    // Get the Ethernet statistics structure.
    
    data = netif->getParameter(kIOEthernetStatsKey);
    if (!data || !(etherStats = (IOEthernetStats *)data->getBuffer())) {
        return false;
    }
    
    return true;
}

IOReturn AirPortLinux::enable(IONetworkInterface *netif) {
    IOReturn result = kIOReturnError;
    
    kprintf("enable() ===>\n");
    
    setLinkStatus((kIONetworkLinkValid | kIONetworkLinkActive), mediumTable[MEDIUM_TYPE_AUTO], 128000, NULL);
    
    result = kIOReturnSuccess;
    
    kprintf("enable() <===\n");
    
done:
    return result;
}

IOReturn AirPortLinux::disable(IONetworkInterface *netif) {
    IOReturn result = kIOReturnSuccess;
    
    kprintf("disable() ===>\n");
    
//    if (_ifp->iface) _ifp->iface->postMessage(APPLE80211_M_POWER_CHANGED);
    
    kprintf("disable() <===\n");
    
    return kIOReturnSuccess;
}

IOReturn AirPortLinux::getMaxPacketSize( UInt32* maxSize ) const {
    return super::getMaxPacketSize(maxSize);
}

IOReturn AirPortLinux::setPromiscuousMode(IOEnetPromiscuousMode mode) {
    return kIOReturnSuccess;
}

IOReturn AirPortLinux::setMulticastMode(IOEnetMulticastMode mode) {
    return kIOReturnSuccess;
}

IOReturn AirPortLinux::setMulticastList(IOEthernetAddress* addr, UInt32 len) {
    return kIOReturnSuccess;
}

const OSString* AirPortLinux::newVendorString() const {
    return OSString::withCString("Apple");
}

const OSString* AirPortLinux::newModelString() const {
    #define kNameLenght 64
    char modelName[kNameLenght];
//    snprintf(modelName, kNameLenght, "Intel %s PCI Express Wifi", if_softc.sc_fwname);
    return OSString::withCString(modelName);
}

//const OSString* AirPortLinux::newRevisionString() const {
//    return OSString::withCString("");
////    return OSString::withCString(((struct ifp_softc *)_ifp->if_softc).sc_fwver);
//}






IOReturn AirPortLinux::setPowerStateAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4)
{
    AirPortLinux* dev = OSDynamicCast(AirPortLinux, owner);
    if (dev == NULL)
        return kIOReturnError;
    unsigned long *powerStateOrdinal = (unsigned long *)arg1;
    
    dev->changePowerState(dev->iface, *powerStateOrdinal);
    return kIOReturnSuccess;
}

IOReturn AirPortLinux::changePowerState(IOInterface *interface, int powerStateOrdinal)
{
    IOReturn ret = kIOReturnSuccess;
    
    if (powerState == powerStateOrdinal) {
        return ret;
    }
    
    switch (powerStateOrdinal) {
        case APPLE_POWER_ON:
//            DPRINTF(("Setting power on\n"));
            
            if (firstUp) {
                firstUp = false;
                
                const char *configArr[] = {"up", "debug"};
//                ifconfig(configArr, nitems(configArr));
                ifup(__FUNCTION__);
            }else {
//                this->ca->ca_activate((struct device *)if_softc, DVACT_WAKEUP);
            }
            
            this->fWatchdogTimer->setTimeoutMS(kTimeoutMS);

#ifdef Ethernet
            this->fTimerEventSource->setAction(&AirPortOpenBSD::autoASSOC);
            this->fTimerEventSource->setTimeoutUS(1);
#endif
            
            ret =  kIOReturnSuccess;
            break;
        case APPLE_POWER_OFF:
//            DPRINTF(("Setting power off\n"));
            this->fWatchdogTimer->cancelTimeout();
            
//            this->ca->ca_activate((struct device *)if_softc, DVACT_QUIESCE);
            ret = kIOReturnSuccess;
            break;
        default:
            ret = kIOReturnError;
            break;
    };

    powerState = powerStateOrdinal;
    
    return ret;
}

void
AirPortLinux::ether_ifattach()
{
//    if (!attachInterface((IONetworkInterface**) &_ifp->iface, true)) {
//        panic("AirPortLinux: Failed to attach interface!");
//    }
}

void
AirPortLinux::ether_ifdetach()
{
//    detachInterface((IONetworkInterface*) _ifp->iface, true);
}

int AirPortLinux::enqueueInputPacket2(mbuf_t m)
{
//    DebugLog("---%s: line = %d RTX TX: ", __FUNCTION__, __LINE__);
//    _ifp->iface->enqueueInputPacket(m);
    return 0;
}

void AirPortLinux::flushInputQueue2()
{
//    _ifp->iface->flushInputQueue();
}

//IOReturn AirPortLinux::if_start_task(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3)
//{
////    struct ifnet *ifp = (struct ifnet *)arg0;
////    ifp->if_start(ifp);
//
//    return kIOReturnSuccess;
//}

IOReturn AirPortLinux::tsleepHandler(OSObject* owner, void* arg0 = 0, void* arg1 = 0, void* arg2 = 0, void* arg3 = 0) {
    AirPortLinux* dev = OSDynamicCast(AirPortLinux, owner);
    if (dev == 0)
        return kIOReturnError;
    
    if ((*(int*)arg1) == 0) {
        // no deadline
        if (dev->fCommandGate->commandSleep(arg0, THREAD_INTERRUPTIBLE) == THREAD_AWAKENED)
            return kIOReturnSuccess;
        else
            return kIOReturnTimeout;
    } else {
        AbsoluteTime deadline;
        clock_interval_to_deadline((*(int*)arg1), kNanosecondScale, reinterpret_cast<uint64_t*> (&deadline));
        if (dev->fCommandGate->commandSleep(arg0, deadline, THREAD_INTERRUPTIBLE) == THREAD_AWAKENED)
            return kIOReturnSuccess;
        else
            return kIOReturnTimeout;
    }
}

void AirPortLinux::firmwareLoadComplete( OSKextRequestTag requestTag, OSReturn result, const void *resourceData, uint32_t resourceDataLength, void *context) {
    AirPortLinux *dev = (AirPortLinux *)context;
    if(result == kOSReturnSuccess) {
        dev->firmwareData = OSData::withBytes(resourceData, resourceDataLength);
    } else
        printf("firmwareLoadComplete FAILURE: %08x.\n", result);
    IOLockWakeup(dev->fwLoadLock, dev, true);
    
}

int AirPortLinux::loadfirmware(const char *name, u_char **bufp, size_t *buflen)
{
    const char *firmware_name = name;
    IOLockLock(this->fwLoadLock);
    
    OSReturn ret = OSKextRequestResource(OSKextGetCurrentIdentifier(),
                                         firmware_name,
                                         firmwareLoadComplete,
                                         this,
                                         NULL);
    if(ret != kOSReturnSuccess) {
        IOLog("%s Unable to load firmware file %08x\n", __FUNCTION__, ret);
        IOLockUnlock(this->fwLoadLock);
        return 1;
    }
    IOLockSleep(this->fwLoadLock, this, THREAD_INTERRUPTIBLE);
    IOLockUnlock(this->fwLoadLock);
    
    *buflen = this->firmwareData->getLength();
    *bufp = (u_char *)this->firmwareData->getBytesNoCopy();
    
    return 0;
}

void AirPortLinux::if_watchdog(IOTimerEventSource *timer)
{
//    _ifp->if_watchdog(_ifp);
    fWatchdogTimer->setTimeoutMS(kTimeoutMS);
}

