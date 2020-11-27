/* add your code here*/

typedef unsigned int ifnet_ctl_cmd_t;

#include "IONetworkInterface.h"
#include "IONetworkController.h"


#include "AirPortLinux.hpp"

#include <linux/types.h>
#include <linux/device.h>
#include <linux/pci.h>

OSDefineMetaClassAndStructors(AirPortLinux, IO80211Controller);
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

struct ifnet *_ifp;
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
    IOPCIDevice* fPciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!fPciDevice) {
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
    
    
    struct pci_dev *pdev = IONew(struct pci_dev, 1);
    pdev->dev.dev = this;
    pdev->device = fPciDevice->extendedConfigRead16(kIOPCIConfigDeviceID);
    pdev->subsystem_device = fPciDevice->extendedConfigRead16(kIOPCIConfigSubSystemID);
    
    iwl_drv_init();
    iwl_mvm_init();
    iwl_pci_probe(pdev, pdev->dev.ent);
    return NULL;
    
//    this->ca = &calist.ca[0];
//    if (!this->ca->ca_match((struct device *)provider, this, &pa)) {
//        return NULL;
//    }
//    return this;
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
    
    fCommandGate->enable();
    
//    if_softc = IOMallocZero(this->ca->ca_devsize);
//    struct device *dev = (struct device *)if_softc;
//    dev->dev = this;
//
//    struct ieee80211com *ic = (struct ieee80211com *)((char *)if_softc + sizeof(struct device));
//
//    _ifp = &ic->ic_if;
//    _ifp->fWorkloop = fWorkloop;
//    _ifp->fCommandGate = fCommandGate;
//    _ifp->if_link_state = LINK_STATE_DOWN;
//
//    _scanResults = OSArray::withCapacity(512); // by default, but it autoexpands
//    _resultsPending = _scanResults->getCount();
//
//    fwLoadLock = IOLockAlloc();
//
//    this->pa.dev.dev = this;
////    bcopy(this->cd->cd_name, this->pa.dev.dv_xname, sizeof(this->cd->cd_name));
//    this->pa.workloop = fWorkloop;
//    this->pa.pa_tag = fPciDevice;
    
    fPciDevice->setBusMasterEnable(true);
    fPciDevice->setIOEnable(true);
    fPciDevice->setMemoryEnable(true);
//
//    this->ca->ca_attach((struct device *)provider, (struct device *)if_softc, &this->pa);
//    if (_ifp->err)
//    {
//        return false;
//    }
    
    fWatchdogTimer = IOTimerEventSource::timerEventSource(this, OSMemberFunctionCast(IOTimerEventSource::Action, this, &AirPortLinux::if_watchdog));
    
    if (!fWatchdogTimer) {
        IOLog("AirPortLinux: Failed to create IOTimerEventSource.\n");
        return false;
    }
    fWorkloop->addEventSource(fWatchdogTimer);
    
    
//    IFConfig = OSDynamicCast(OSString, getProperty(kIFConfigName));
    
//    OSDictionary *params = OSDynamicCast(OSDictionary, getProperty(kParamName));
//    BSSID = OSString::withCString("");
//    PWD = OSString::withCString("");
//    if (params) {
//        BSSID = OSDynamicCast(OSString, params->getObject(kBSSIDName));
//        PWD = OSDynamicCast(OSString, params->getObject(kPWDName));
//    }
    

//    this->mediumDict = OSDictionary::withCapacity(1);
//    this->addMediumType(kIOMediumIEEE80211Auto,  _ifp->if_baudrate,  MEDIUM_TYPE_AUTO);
//    this->publishMediumDictionary(this->mediumDict);
//    this->setCurrentMedium(this->mediumTable[MEDIUM_TYPE_AUTO]);
//    this->setSelectedMedium(this->mediumTable[MEDIUM_TYPE_AUTO]);
    
    registerService();
    
    return true;
}

void AirPortLinux::stop(IOService* provider) {
    if (fCommandGate) {
        fCommandGate->disable();
        if (fWorkloop) {
            fWorkloop->removeEventSource(fCommandGate);
        }
    }
    
//    if (_ifp->iface){
//        struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//        ieee80211_ifdetach(&ic->ic_ac.ac_if);
//        _ifp->iface = NULL;
//    }
    
    super::stop(provider);
}

void AirPortLinux::free() {
    IOLog("AirPortLinux: Free");
    
//    ((pci_intr_handle_t)if_softc->sc_ih)->intr->disable();
//
//    if (fWorkloop) {
//        if (((pci_intr_handle_t)if_softc->sc_ih)->intr) {
//            fWorkloop->removeEventSource(((pci_intr_handle_t)if_softc->sc_ih)->intr);
//            RELEASE(((pci_intr_handle_t)if_softc->sc_ih)->intr);
//        }
//        if (fCommandGate) {
//            fWorkloop->removeEventSource(fCommandGate);
//            RELEASE(fCommandGate);
//        }
//        RELEASE(fWorkloop);
//    }
    
    RELEASE(fCommandGate);
    RELEASE(mediumDict);
    RELEASE(fPciDevice);

    super::free();
}

IOReturn AirPortLinux::getHardwareAddress(IOEthernetAddress* addr) {
    //    addr->bytes[0] = 0xAA;
    //    addr->bytes[1] = 0x99;
    //    addr->bytes[2] = 0x88;
    //    addr->bytes[3] = 0x77;
    //    addr->bytes[4] = 0x66;
    //    addr->bytes[5] = 0x55;
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    bcopy(ic->ic_myaddr, addr->bytes, kIOEthernetAddressSize);
    return kIOReturnSuccess;
}

UInt32 AirPortLinux::outputPacket(mbuf_t m, void* param) {
    
    int error;

//    IFQ_ENQUEUE(&_ifp->if_snd, m, error);
    if (error) {
        return kIOReturnOutputDropped;
    }

//    kprintf("%s:start line = %d", __FUNCTION__, __LINE__);
//    _ifp->if_start(_ifp);
//    fCommandGate->runAction(&AirPortLinux::if_start_task, _ifp);

    return kIOReturnSuccess;
}

IOReturn AirPortLinux::outputRaw80211Packet(IO80211Interface*, mbuf_t)
{
    kprintf("--%s:start line = %d", __FUNCTION__, __LINE__);
    
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
    
//    setLinkStatus((kIONetworkLinkValid | kIONetworkLinkActive), mediumTable[MEDIUM_TYPE_AUTO], _ifp->if_baudrate, NULL);
//
////    const char *ifconfig = "nwid FAST_88FED0 wpakey 126abc!@ABC wpaprotos wpa1,wpa2";
//    const char *configStr = this->IFConfig->getCStringNoCopy();
//    ifconfig(configStr);
    
//    ifnet *ifp = &if_softc.sc_ic.ic_if;
//    if (ifp->iface) ifp->iface->postMessage(APPLE80211_M_POWER_CHANGED);
//    if (fOutputQueue) fOutputQueue->setCapacity(200); // FIXME !!!!
    
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
    *maxSize = 1500;
    return kIOReturnSuccess;
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
    return OSString::withCString("Intel");
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
    
    if (arg1 == 0) {
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

