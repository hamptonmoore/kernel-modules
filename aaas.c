#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

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
#define BLOCK_SIZE 65536

static int Major;
static int Device_Open = 0;
static char AAA[BLOCK_SIZE] = {65};
static int RAND_MAX = (BLOCK_SIZE / 2) - 1;
static int i = 0;

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

  memset(AAA, 65, BLOCK_SIZE);
  for (i = 0; i < BLOCK_SIZE; i++)
  {
    if (get_random_int() & 1)
    {
      AAA[i] += 32;
    }
  }

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

  int start = (get_random_int() % RAND_MAX);
  length = (get_random_int() % (BLOCK_SIZE - start)) + 1;
  copy_to_user(buffer, AAA + start, length);
  return length;
}

// Called on write
static ssize_t
device_write(struct file *filp,
             const char *buf, size_t len, loff_t *off)
{
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;
}