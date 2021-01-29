//
//  seq_file.h
//  AirPortLinux
//
//  Created by Zhong-Mac on 2020/12/6.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef seq_file_h
#define seq_file_h

#include <linux/mutex.h>

struct seq_operations;

struct seq_file {
    char *buf;
    size_t size;
    size_t from;
    size_t count;
    size_t pad_until;
    loff_t index;
    loff_t read_pos;
    struct mutex lock;
    const struct seq_operations *op;
    int poll_event;
    const struct file *file;
    void *_private;
};

struct seq_operations {
    void * (*start) (struct seq_file *m, loff_t *pos);
    void (*stop) (struct seq_file *m, void *v);
    void * (*next) (struct seq_file *m, void *v, loff_t *pos);
    int (*show) (struct seq_file *m, void *v);
};

#endif /* seq_file_h */
