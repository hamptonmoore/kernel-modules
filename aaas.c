#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *,
                            const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "Andrew-As-A-Service"

static int Major;
static int Device_Open = 0;
static unsigned int seed = 5323;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release};

// Module loaded
int init_module(void)
{
  Major = register_chrdev(0, DEVICE_NAME, &fops);

  if (Major < 0)
  {
    printk(KERN_ALERT "Registering char device failed with %d\n", Major);
    return Major;
  }

  printk(KERN_INFO "Andrew-As-A-Service has been loaded, use 'mknod /dev/aaas c %d 0' to mount it.\n", Major);

  return SUCCESS;
}

// Module unloaded
void cleanup_module(void)
{
  // Unregister
  unregister_chrdev(Major, DEVICE_NAME);
}

// Called on open
static int device_open(struct inode *inode, struct file *filp)
{
  if (Device_Open)
    return -EBUSY;

  Device_Open++;
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

// Called on close
static int device_release(struct inode *inode, struct file *filp)
{
  Device_Open--;
  module_put(THIS_MODULE);
  return SUCCESS;
}

// Called on read
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{

  int bytes_read = 0;
  seed = (8253729 * seed + 2396403);
  put_user(65 + (((seed % 32767) % 2) * 32), buffer++);
  length--;
  bytes_read++;
  return bytes_read;
}

// Called on write
static ssize_t
device_write(struct file *filp,
             const char *buf, size_t len, loff_t *off)
{
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;
}