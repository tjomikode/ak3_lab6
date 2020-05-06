/*
*  Copyright (c) 2017, GlobalLogic Ukraine LLC
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  1. Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the distribution.
*  3. All advertising materials mentioning features or use of this software
*  must display the following acknowledgement:
*  This product includes software developed by the GlobalLogic.
*  4. Neither the name of the GlobalLogic nor the
*  names of its contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC `AS IS` AND ANY
*  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
*  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS

*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
 
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/errno.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint how_many = 1;
module_param(how_many, uint, S_IRUGO);
MODULE_PARM_DESC(how_many, "Parameter defines how many times current string should be printed.");

struct linked_list {
	struct list_head list_node;
	ktime_t start;
	ktime_t finish;
};

static LIST_HEAD(linked_head);

static int __init hello_init(void)
{
	uint i;
	struct linked_list *data;
	ktime_t start, finish;

	WARN_ON((how_many == 0) || (how_many >= 5 && how_many <= 10));

	BUG_ON(how_many > 10);

	BUG_ON(!list_empty(&linked_head));

	for (i = 0; i < how_many; i++)
	{
		start = ktime_get();
		printk(KERN_EMERG "Hello, world!\n");
		finish = ktime_get();

		data = (struct linked_list*) kmalloc(sizeof(struct linked_list), GFP_KERNEL);

		if (i == 3)
		{
			data = NULL;
		}

		*data = (struct linked_list) {
		(struct list_head) {NULL, NULL},
		start,
		finish
		};

		list_add_tail(&data->list_node, &linked_head);
	}

	return 0;

}

EXPORT_SYMBOL(hello_init);

static void __exit hello_exit(void)
{
	struct linked_list *data, *tmp;
	list_for_each_entry_safe(data, tmp, &linked_head, list_node)
	{
 
		printk(KERN_EMERG "time=%lld\n", data->finish - data->start);
		list_del(&data->list_node);
		kfree(data);
	}
}

module_init(hello_init);
module_exit(hello_exit);
