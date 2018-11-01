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
#include <linux/delay.h>



/* Counter Struct */MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("John Rufino Macasaet");
MODULE_DESCRIPTION("Prints nth triangular number to /proc/macasaet_procfs");
typedef struct __counter_t {
  volatile int value;
} counter_t;

counter_t count;
//count.value=0;

static struct task_struct *thread1, *thread2, *thread3, *thread4, *thread5;
static spinlock_t counter_lock = __SPIN_LOCK_UNLOCKED(); //set spinlock to unlocked state

/*counter struct*/

void increment (counter_t *c) {
  c->value++;
}
void decrement (counter_t *c) {
  c->value--;

}
int get (counter_t *c) {
  return c->value;
}

/* Threads */

int thread_fn1(void *myvar){
    int i, trylock_thread1;
    msleep(9000);
    for(i=0; i<500000;){
        trylock_thread1 = spin_trylock(&counter_lock);
        if(!trylock_thread1){
            //printk(KERN_INFO "Lock failed\n");
        }
        else{
            increment(&count);
            spin_unlock(&counter_lock);
            i++;
        }
    }
    printk(KERN_INFO "Count: %d\n",count.value);
    return 0;
}
int thread_fn2(void *myvar){
    int i, trylock_thread2;
    msleep(9000);
    for(i=0; i<500000;){
        trylock_thread2 = spin_trylock(&counter_lock);
        if(!trylock_thread2){
            //printk(KERN_INFO "Lock failed\n");
        }
        else{
            increment(&count);
            spin_unlock(&counter_lock);
            i++;
        }
    }
    printk(KERN_INFO "Count: %d\n",count.value);
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

int init_module (void) {
    /* thread var */
    char first_thread[8] = "thread1";
    char second_thread[8] = "thread2";
    char third_thread[8] = "thread3";
    char fourth_thread[8] = "thread4";
    char fifth_thread[8] = "thread5";
    /* counter var */
    printk(KERN_INFO "%s: Module init\n",__FUNCTION__);
    /* counter init*/
    thread1 = kthread_create(thread_fn2,NULL,first_thread);
    if((thread1)){
        printk(KERN_INFO "In thread1.");
        wake_up_process(thread1);
    }
    msleep(2000);
    thread2 = kthread_create(thread_fn1,NULL,second_thread);
    if((thread2)){
        printk(KERN_INFO "In thread2.");
        wake_up_process(thread2);
    }
    //msleep(2000);
    thread3 = kthread_create(thread_fn2,NULL,third_thread);
    if((thread3)){
        printk(KERN_INFO "In thread3.");
        wake_up_process(thread3);
    }
    //msleep(2000);
    thread4 = kthread_create(thread_fn2,NULL,fourth_thread);
    if((thread4)){
        printk(KERN_INFO "In thread4.");
        wake_up_process(thread4);
    }
    //msleep(1000);
    thread5 = kthread_create(thread_fn1,NULL,fifth_thread);
    if((thread5)){
        printk(KERN_INFO "In thread5.");
        wake_up_process(thread5);
    }
    return 0;
}

void cleanup_module(void){
    printk(KERN_INFO "%s: LKM removed.\n",__FUNCTION__);
}
