//
//  fs.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef fs_h
#define fs_h

#include <linux/types.h>
#include <linux/mutex.h>


/*
 * Keep mostly read-only and often accessed (especially for
 * the RCU path lookup and 'stat' data) fields at the beginning
 * of the 'struct inode'
 */
struct inode {
//    umode_t            i_mode;
    unsigned short        i_opflags;
//    kuid_t            i_uid;
//    kgid_t            i_gid;
    unsigned int        i_flags;

#ifdef CONFIG_FS_POSIX_ACL
    struct posix_acl    *i_acl;
    struct posix_acl    *i_default_acl;
#endif

    const struct inode_operations    *i_op;
    struct super_block    *i_sb;
    struct address_space    *i_mapping;

#ifdef CONFIG_SECURITY
    void            *i_security;
#endif

    /* Stat data, not accessed from path walking */
    unsigned long        i_ino;
    /*
     * Filesystems may only read i_nlink directly.  They shall use the
     * following functions for modification:
     *
     *    (set|clear|inc|drop)_nlink
     *    inode_(inc|dec)_link_count
     */
    union {
        const unsigned int i_nlink;
        unsigned int __i_nlink;
    };
    dev_t            i_rdev;
    loff_t            i_size;
//    struct timespec64    i_atime;
//    struct timespec64    i_mtime;
//    struct timespec64    i_ctime;
    spinlock_t        i_lock;    /* i_blocks, i_bytes, maybe i_size */
    unsigned short          i_bytes;
    u8            i_blkbits;
    u8            i_write_hint;
    blkcnt_t        i_blocks;

#ifdef __NEED_I_SIZE_ORDERED
    seqcount_t        i_size_seqcount;
#endif

    /* Misc */
    unsigned long        i_state;
//    struct rw_semaphore    i_rwsem;

    unsigned long        dirtied_when;    /* jiffies of first dirtying */
    unsigned long        dirtied_time_when;

    struct hlist_node    i_hash;
    struct list_head    i_io_list;    /* backing dev IO list */
#ifdef CONFIG_CGROUP_WRITEBACK
    struct bdi_writeback    *i_wb;        /* the associated cgroup wb */

    /* foreign inode detection, see wbc_detach_inode() */
    int            i_wb_frn_winner;
    u16            i_wb_frn_avg_time;
    u16            i_wb_frn_history;
#endif
    struct list_head    i_lru;        /* inode LRU list */
    struct list_head    i_sb_list;
    struct list_head    i_wb_list;    /* backing dev writeback list */
    union {
        struct hlist_head    i_dentry;
        struct rcu_head        i_rcu;
    };
    atomic64_t        i_version;
    atomic64_t        i_sequence; /* see futex */
    atomic_t        i_count;
    atomic_t        i_dio_count;
    atomic_t        i_writecount;
#if defined(CONFIG_IMA) || defined(CONFIG_FILE_LOCKING)
    atomic_t        i_readcount; /* struct files open RO */
#endif
    union {
        const struct file_operations    *i_fop;    /* former ->i_op->default_file_ops */
        void (*free_inode)(struct inode *);
    };
    struct file_lock_context    *i_flctx;
//    struct address_space    i_data;
    struct list_head    i_devices;
    union {
        struct pipe_inode_info    *i_pipe;
        struct block_device    *i_bdev;
        struct cdev        *i_cdev;
        char            *i_link;
        unsigned        i_dir_seq;
    };

    __u32            i_generation;

#ifdef CONFIG_FSNOTIFY
    __u32            i_fsnotify_mask; /* all events this inode cares about */
    struct fsnotify_mark_connector __rcu    *i_fsnotify_marks;
#endif

#ifdef CONFIG_FS_ENCRYPTION
    struct fscrypt_info    *i_crypt_info;
#endif

#ifdef CONFIG_FS_VERITY
    struct fsverity_info    *i_verity_info;
#endif

    void            *i_private; /* fs or device private pointer */
} __randomize_layout;


struct module {
    struct module *next;
    const char *name;
    int gpl_compatible;
//    struct symbol *unres;
    int seen;
    int skip;
    int has_init;
    int has_cleanup;
//    struct buffer dev_table_buf;
    char         srcversion[25];
    int is_dot_o;
    // Missing namespace dependencies
//    struct namespace_list *missing_namespaces;
//    // Actual imported namespaces
//    struct namespace_list *imported_namespaces;
};

struct file_operations {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
    ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
    int (*iopoll)(struct kiocb *kiocb, bool spin);
    int (*iterate) (struct file *, struct dir_context *);
    int (*iterate_shared) (struct file *, struct dir_context *);
//    __poll_t (*poll) (struct file *, struct poll_table_struct *);
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    int (*mmap) (struct file *, struct vm_area_struct *);
    unsigned long mmap_supported_flags;
    int (*open) (struct inode *, struct file *);
//    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *);
    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    int (*fasync) (int, struct file *, int);
    int (*lock) (struct file *, int, struct file_lock *);
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
    int (*check_flags)(int);
    int (*flock) (struct file *, int, struct file_lock *);
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **, void **);
    long (*fallocate)(struct file *file, int mode, loff_t offset,
              loff_t len);
    void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
    unsigned (*mmap_capabilities)(struct file *);
#endif
    ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
            loff_t, size_t, unsigned int);
    loff_t (*remap_file_range)(struct file *file_in, loff_t pos_in,
                   struct file *file_out, loff_t pos_out,
                   loff_t len, unsigned int remap_flags);
    int (*fadvise)(struct file *, loff_t, loff_t, int);
} __randomize_layout;


struct file {
    union {
        struct llist_node    fu_llist;
        struct rcu_head     fu_rcuhead;
    } f_u;
//    struct path        f_path;
    struct inode        *f_inode;    /* cached value */
    const struct file_operations    *f_op;

    /*
     * Protects f_ep_links, f_flags.
     * Must not be taken from IRQ context.
     */
    spinlock_t        f_lock;
//    enum rw_hint        f_write_hint;
    atomic_long_t        f_count;
    unsigned int         f_flags;
//    fmode_t            f_mode;
    struct mutex        f_pos_lock;
//    loff_t            f_pos;
//    struct fown_struct    f_owner;
//    const struct cred    *f_cred;
//    struct file_ra_state    f_ra;

    u64            f_version;
#ifdef CONFIG_SECURITY
    void            *f_security;
#endif
    /* needed for tty driver, and maybe others */
    void            *private_data;

#ifdef CONFIG_EPOLL
    /* Used by fs/eventpoll.c to link all the hooks to this file */
    struct list_head    f_ep_links;
    struct list_head    f_tfile_llink;
#endif /* #ifdef CONFIG_EPOLL */
    struct address_space    *f_mapping;
//    errseq_t        f_wb_err;
};    /* lest something weird decides that 2 is OK */


static ssize_t simple_read_from_buffer(void __user *to, size_t count, loff_t *ppos,
                const void *from, size_t available)
{
    loff_t pos = *ppos;
    size_t ret;

    if (pos < 0)
        return -EINVAL;
    if (pos >= available || !count)
        return 0;
    if (count > available - pos)
        count = available - pos;
    ret = copy_to_user(to, (char *)from + pos, count);
    if (ret == count)
        return -EFAULT;
    count -= ret;
    *ppos = pos + count;
    return count;
}

static int simple_open(struct inode *inode, struct file *file)
{
    if (inode->i_private)
        file->private_data = inode->i_private;
    return 0;
}


/*
 * "descriptor" for what we're up to with a read.
 * This allows us to use the same read code yet
 * have multiple different users of the data that
 * we read from a file.
 *
 * The simplest case just copies the data to user
 * mode.
 */
typedef struct {
    size_t written;
    size_t count;
    union {
        char __user *buf;
        void *data;
    } arg;
    int error;
} read_descriptor_t;

#endif /* fs_h */
