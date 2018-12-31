/* -------- Relationship between LED and BCM --------
*
*		GND------GND
*		LED0-----BCM4
*		LED1-----BCM18
*		LED2-----BCM17
*		LED3-----BCM27
*		LED4-----BCM22
*		LED5-----BCM23
*		LED6-----BCM24
*		LED7-----BCM25
*
 -------- Relationship between LED and BCM --------*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define		N		8

MODULE_AUTHOR("Atsuya Koike");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;
static int port[] = { 4, 18, 17, 27, 22, 23, 24, 25 };
static int c_cnt = 0;

void mychange(char *);
void gpio_output(unsigned char);

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	static char c[256];
	if ( copy_from_user( &c[c_cnt], buf, sizeof(char)) ) {
        return -EFAULT;
	} else {
		if( c_cnt == 1 ) {
			c_cnt = 0;
			mychange(c);
		} else {
			c_cnt = 1;
		}
	}
	printk(KERN_INFO "receive %c\n",c[c_cnt]);
	return 1;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write
};

static int __init init_mod(void) {
	int retval;
	int i = 0;
	gpio_base = ioremap_nocache(0x3f200000, 0xA0);
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	
	if (retval < 0) {
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
	
	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	
	if ( retval < 0 ) {
		printk( KERN_ERR "cdev_add failed. major:%d, minor:%d", MAJOR(dev), MINOR(dev) );
		return retval;
	}

	cls = class_create( THIS_MODULE, "myled" );
	
	if ( IS_ERR(cls) ) {
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create( cls, NULL, dev, NULL, "myled%d", MINOR(dev) );
	
	for ( i = 0; i < N; i++ ) {
		const u32 led = port[i];
		const u32 index = led / 10;
		const u32 shift  =( led % 10 ) * 3;
		const u32 mask = ~( 0x7 << shift );
		gpio_base[index] = (gpio_base[index]  & mask ) | ( 0x1 << shift );
	}
	return 0;
}


static void __exit cleanup_mod(void) {
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
}


void mychange(char mozi[]) {
	unsigned char light[2], data;
	unsigned char i;
		
	for ( i = 0; i < 2; i++ ) {
		if ( mozi[i] == '0' ) light[i] = 0x00;
		else if ( mozi[i] == '1' ) light[i] = 0x01;
		else if ( mozi[i] == '2' ) light[i] = 0x02;
		else if ( mozi[i] == '3' ) light[i] = 0x03;
		else if ( mozi[i] == '4' ) light[i] = 0x04;
		else if ( mozi[i] == '5' ) light[i] = 0x05;
		else if ( mozi[i] == '6' ) light[i] = 0x06;
		else if ( mozi[i] == '7' ) light[i] = 0x07;
		else if ( mozi[i] == '8' ) light[i] = 0x08;
		else if ( mozi[i] == '9' ) light[i] = 0x09;
		else if ( mozi[i] == 'A' || mozi[i] == 'a' ) light[i] = 0x0A;
		else if ( mozi[i] == 'B' || mozi[i] == 'b' ) light[i] = 0x0B;
		else if ( mozi[i] == 'C' || mozi[i] == 'c' ) light[i] = 0x0C;
		else if ( mozi[i] == 'D' || mozi[i] == 'd' ) light[i] = 0x0D;
		else if ( mozi[i] == 'E' || mozi[i] == 'e' ) light[i] = 0x0E;
		else if ( mozi[i] == 'F' || mozi[i] == 'f' ) light[i] = 0x0F;
		else light[i] = 0x00;
	}
	light[0] = light[0] << 4;
	data = light[0] | light[1];
	gpio_output(data);
}
void gpio_output( unsigned char light ) {
	unsigned char i;
	unsigned char data[N];
	for ( i = 0; i < N; i++ ) {
		data[i] = ( light << i ) & 0x80;
	}
	for ( i = 0; i < N; i++ ) {
		if ( data[i] == 0 ) {
			gpio_base[10] = 1 << port[i];
		}
		else {
			gpio_base[7] = 1 << port[i];
		}
	}
}
module_init(init_mod);
module_exit(cleanup_mod);
