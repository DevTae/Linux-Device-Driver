#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xb8e7ce2c, "__put_user_8" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x24d273d1, "add_timer" },
	{ 0x720a27a7, "__register_blkdev" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x6ee7b12b, "blk_mq_alloc_tag_set" },
	{ 0xf242dd6f, "blk_mq_init_queue" },
	{ 0x90c98b36, "blk_queue_logical_block_size" },
	{ 0x1090b3b7, "__blk_alloc_disk" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x61b4b59, "set_capacity" },
	{ 0xdd29d497, "device_add_disk" },
	{ 0xbbd0d52c, "blk_mq_free_tag_set" },
	{ 0x999e8297, "vfree" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0x4e67820f, "del_gendisk" },
	{ 0x331b69f4, "put_disk" },
	{ 0xa82662df, "blk_put_queue" },
	{ 0x37a0cba, "kfree" },
	{ 0x69acdf38, "memcpy" },
	{ 0x71de7b6e, "blk_mq_start_request" },
	{ 0xbc9f9ad5, "blk_mq_end_request" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xd254e690, "current_task" },
	{ 0xf229424a, "preempt_count_add" },
	{ 0x706c5a65, "preempt_count_sub" },
	{ 0x4629334c, "__preempt_count" },
	{ 0x48d88a2c, "__SCT__preempt_schedule" },
	{ 0x67088107, "bio_endio" },
	{ 0xfb578fc5, "memset" },
	{ 0x597faa56, "bdev_check_media_change" },
	{ 0x58c9811c, "param_ops_int" },
	{ 0x1629506c, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4BBD09DDD6A89BE852265C4");
