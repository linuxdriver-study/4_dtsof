#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/ide.h>
#include <linux/device.h>
#include <linux/of.h>

static int __init dtsof_init(void)
{
        int ret = 0;
        struct device_node *bl_node = NULL;
        struct property *bl_prop = NULL;
        const char *str = NULL;
        u32 bl_def_level = 0;
        int  elements = 0;
        u32 *value_buf = NULL;
        int i = 0;
        
        bl_node = of_find_node_by_path("/backlight");
        if (bl_node == NULL) {
                ret = -EINVAL;
                goto find_node_err;
        } else {
                printk("backlight name: %s\n", bl_node->name);
        }

        bl_prop = of_find_property(bl_node, "compatible", NULL);
        if (bl_prop == NULL) {
                ret = -EINVAL;
                goto find_prop_err;
        } else {
                printk("compatible: %s, len=%d\n", (char*)bl_prop->value, bl_prop->length);
        }

        ret = of_property_read_string(bl_node, "status", &str);
        if (ret) {
                printk("read string error!\n");
                goto read_str_err;
        } else {
                printk("status:%s\n", str);
        }

        ret = of_property_read_u32(bl_node, "default-brightness-level", &bl_def_level);
        if (ret < 0) {
                printk("read u32 error!\n");
                goto read_u32_err;
        } else {
                printk("default-brightness-level:%d\n", bl_def_level);
        }

        elements = of_property_count_u32_elems(bl_node, "brightness-levels");
        if (elements < 0) {
                printk("count_u32_elems error!\n");
                goto count_elems_err;
        } else {
                printk("brightness-levels elements: %d\n", elements);
        }

        value_buf = kmalloc(elements * sizeof(u32), GFP_KERNEL);
        if (value_buf == NULL) {
                printk("kmalloc error!");
                goto fail_malloc;
        }

        ret = of_property_read_u32_array(bl_node, "brightness-levels", value_buf, elements);
        if (ret < 0) {
                printk("read u32 array error!\n");
                goto read_array_err;
        } else {
                for(i = 0; i < elements; i++) {
                        printk("brightness_levels %d is %d\n", i, *(value_buf+i));
                }
        }
        
read_array_err:
fail_malloc:
        kfree(value_buf);
count_elems_err:
read_u32_err:
read_str_err:
find_prop_err:
find_node_err:
        return ret;
}

static void __exit dtsof_exit(void)
{
        printk("dtsof exit\n");
}

module_init(dtsof_init);
module_exit(dtsof_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("wanglei");
