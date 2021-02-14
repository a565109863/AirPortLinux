//
//  errno.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/7/12.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef errno_h
#define errno_h

#define NOTIFY_DONE        0x0000        /* Don't care */
#define NOTIFY_OK        0x0001        /* Suits me */
#define NOTIFY_STOP_MASK    0x8000        /* Don't call further */
#define NOTIFY_BAD        (NOTIFY_STOP_MASK|0x0002)
                        /* Bad/Veto action */
/*
 * Clean way to return from the notifier and stop further calls.
 */
#define NOTIFY_STOP        (NOTIFY_OK|NOTIFY_STOP_MASK)


/*
 * These should never be seen by user programs.  To return one of ERESTART*
 * codes, signal_pending() MUST be set.  Note that ptrace can observe these
 * at syscall exit tracing, but they will never be left for the debugged user
 * process to see.
 */
#define ERESTARTSYS    512
#define ERESTARTNOINTR    513
#define ERESTARTNOHAND    514    /* restart if no handler.. */
#define ENOIOCTLCMD    515    /* No ioctl command */
#define ERESTART_RESTARTBLOCK 516 /* restart by calling sys_restart_syscall */
#define EPROBE_DEFER    517    /* Driver requests probe retry */
#define EOPENSTALE    518    /* open found a stale dentry */
#define ENOPARAM    519    /* Parameter not supported */

/* Defined for the NFSv3 protocol */
#define EBADHANDLE    521    /* Illegal NFS file handle */
#define ENOTSYNC    522    /* Update synchronization mismatch */
#define EBADCOOKIE    523    /* Cookie is stale */
#define ENOTSUPP    524    /* Operation is not supported */
#define ETOOSMALL    525    /* Buffer or request is too small */
#define ESERVERFAULT    526    /* An untranslatable error occurred */
#define EBADTYPE    527    /* Type not supported by server */
#define EJUKEBOX    528    /* Request initiated, but will not complete before timeout */
#define EIOCBQUEUED    529    /* iocb queued, will get completion event */
#define ERECALLCONFLICT    530    /* conflict with recalled state */


#endif /* errno_h */
