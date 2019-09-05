#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/fcntl.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/termios.h>
#include <linux/fs.h>
#include <linux/uaccess.h> 

/* make a structure for birthday */
struct birthday{
    int day;
    int month; 
    int year;
    struct list_head list;
};

/* make a static list head */
static LIST_HEAD(birthday_list);

/* return a new person's birthday node */
struct birthday *newBirthday(int day, int month, int year) {

    // init a new person's birthday
    struct birthday *new_person;

    // assign memory through kernel mode
    new_person = kmalloc(sizeof(*new_person), GFP_KERNEL);

    // initialize all data members of the object
    new_person->day = day;
    new_person->month = month;
    new_person->year = year;

    // return the node of the new person
    return new_person;
}

/* Make random birthdays (i.e add to linked list) */
struct birthday *makeRandomBirthdays(int n) {

    struct birthday *new_person;
    
    // defining starting date, month and year
    int day = 1;
    int month = 1;
    int year = 2000;
	int i;
    for(i = 0; i<n; i++) {
        new_person = newBirthday(day, month, year);

        day = (day%30) + 1;
        month = (month%12) + 1;
        year += 1;

        /* Initialize the list head */
        INIT_LIST_HEAD(&new_person->list);
        list_add_tail(&new_person->list, &birthday_list);
    }

    return new_person;
}

/* This function is called when the module is loaded. */
int simple_init(void){

    /* Display success message */
    printk(KERN_INFO "Module is loaded successfully\n");

    /* Add some random birthdays for visualization and making the linked list */
    int number = 10;
    struct birthday *root = makeRandomBirthdays(number);

    /* Traverse the list and print the birthdays */
    printk(KERN_INFO "# **** Traversing list and printing the birthdays **** #\n");

    list_for_each_entry(root, &birthday_list, list){
        printk(KERN_INFO "Date->%d Month->%d Year->%d\n", root->day, root->month, root->year);
    }

    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void){
    printk(KERN_INFO "List is deleted\n");
    struct birthday *ptr;
    struct birthday *next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list){
        list_del(&ptr->list);
        kfree(ptr);
    }

    printk(KERN_INFO "Module has been removed\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("OPEN SOURCE");
MODULE_DESCRIPTION("OS ASSIGNMENT 2");
MODULE_AUTHOR("PARTH GOYAL");
