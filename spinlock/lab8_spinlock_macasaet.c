/*
 * John Rufino Macasaet
 * 2013-18722
 * CoE 135 TLM FVW
 */

#include <linux/init.h>
#include <linux/kernel.h>  
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/spinlock_types.h>

//static struct task_struct *thread1, *thread2;
static spinlock_t counter_lock = __SPIN_LOCK_UNLOCKED; //set spinlock to unlocked state

/* Threads */


/* Counter Struct */
typedef struct __counter_t {
  int value;
} counter_t;

void init (counter_t *c) {
  int try_init;
  try_init = spin_trylock(&counter_lock);
  if(!try_init){
    printk(KERN_INFO "Unable to acquire lock\n");
  }
  else{
    printk(KERN_INFO "Lock acquired\n");
    c->value = 0;
    spin_unlock(&counter_lock);
  }
  
}
void increment (counter_t *c) {
  int try_increment;
  try_increment = spin_trylock(&counter_lock);
  if (!try_increment){
    printk(KERN_INFO "Unable to acquire lock\n");
  }
  else{
    printk(KERN_INFO "Lock acquired\n");
    c->value++;
    spin_unlock(&counter_lock);
  }
  
}
void decrement (counter_t *c) {
  int try_decrement;
  try_decrement = spin_trylock(&counter_lock);
  if(!try_decrement){
    printk(KERN_INFO "Unable to acquire lock\n");
  }
  else{
    printk(KERN_INFO "Lock acquired\n");
    c->value--;
    spin_unlock(&counter_lock);
  }
}
int get (counter_t *c) {
  return c->value;
}

void cleanup(void){
  printk(KERN_INFO "Module cleanup");
}