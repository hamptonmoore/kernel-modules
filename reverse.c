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
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "reverse"
#define BUFFER_SIZE 1024

static int Major;
static int Device_Open = 0;
static char txt[BUFFER_SIZE] = {0};
static int readIndex = 0;

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

  printk(KERN_INFO "Reverse has been loaded, use 'mknod /dev/reverse c %d 0' to mount it.\n", Major);

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
  return 0;
}

// Called on read
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{

  int count = 0;
  while (txt[readIndex] != 0)
  {
    put_user(txt[readIndex], buffer);
    readIndex++;
    buffer++;
    count++;
  }
  return count;
}

// Called on write
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
  int index = length - 1;
  int count = 0;
  readIndex = 0;
  memset(txt, 0, BUFFER_SIZE);

  // while (length > 0)
  // {
  //   printk(KERN_INFO "Copying byte from %d to %d", index, count);
  //   txt[count++] = buffer[index--];
  //   printk(KERN_INFO "Copied Byte");
  //   length--;
  // }
  printk(KERN_INFO "All the info we have %zu %zu %zu", buffer, length, offset);
  printk(KERN_INFO "Copying byte %zu", buffer);

  return length;
}
