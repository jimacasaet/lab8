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

/* Counter Struct */
typedef struct __counter_t {
  int value;
} counter_t;


counter_t count;
static struct task_struct *thread1, *thread2;
static spinlock_t counter_lock = __SPIN_LOCK_UNLOCK(); //set spinlock to unlocked state

/*counter struct*/

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

/* Threads */

int thread_fn1(void){
    int i;
    for(i=0; i<500000; i++){
        increment(count);
    }
    return 0;
}
/*
int thread_fn2(void){
    int j=0;
    for(j=0; j<500000; j++){
        increment(count);
    }
    return 0;
}*/

void thread_init (counter_t *c) {
    /* thread var */
    char first_thread[8] = "thread1";
    char second_thread[8] = "thread2";
    /* counter var */
    int try_init;
    printk(KERN_INFO "Module init\n");
    /* counter init*/
    try_init = spin_trylock(&counter_lock);
    if(!try_init){
        printk(KERN_INFO "Unable to acquire lock\n");
    }
    else{
        printk(KERN_INFO "Lock acquired\n");
        c->value = 0;
        spin_unlock(&counter_lock);
    }
    /* Thread init */
    thread1 = kthread_create(thread_fn1,NULL,first_thread);
    thread2 = kthread_create(thread_fn1,NULL,second_thread);
    if((thread1)){
        printk(KERN_INFO "In thread1.");
        wake_up_process(thread1);
    }
    if((thread2)){
        printk(KERN_INFO "In thread2.");
        wake_up_process(thread2);
    }

}




