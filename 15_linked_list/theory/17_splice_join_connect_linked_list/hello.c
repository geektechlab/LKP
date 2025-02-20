#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

LIST_HEAD(listhead1);
LIST_HEAD(listhead2);

struct mylist
{
	int data;
	struct list_head list;
};

void add_data(int data, struct list_head *listhead)
{
	struct mylist *node = kmalloc(sizeof(struct mylist), GFP_KERNEL);
	pr_info("%s: Node:%px\t node.list:%px\n", __func__, node, &(node->list));
	if (node)
	{
		node->data = data;
		list_add(&node->list, listhead);
	}
}

void add_data_end(int data, struct list_head *listhead)
{
	struct mylist *node = kmalloc(sizeof(struct mylist), GFP_KERNEL);
	pr_info("%s: Node:%px\t node.list:%px\n", __func__, node, &(node->list));
	if (node)
	{
		node->data = data;
		list_add_tail(&node->list, listhead);
	}
}

void traversal(struct list_head *listhead)
{
	struct list_head *ptr;
	struct mylist *node;
	for(ptr = listhead->next; ptr != listhead; ptr = ptr->next)
	{
		node = list_entry(ptr, struct mylist, list);
		pr_info("%s: Data:%d\n", __func__, node->data);
	}
}

void traversal_ll(struct list_head *listhead)
{
	struct list_head *ptr;
	struct mylist *node;
	list_for_each(ptr, listhead)
	{	
		node = list_entry(ptr, struct mylist, list);
		pr_info("%s: Data:%d\n", __func__, node->data);
	}

}
void traversal_ll2(struct list_head *listhead)
{
	struct mylist *node;
	list_for_each_entry(node, listhead, list)
	{	
		pr_info("%s: Data:%d\n", __func__, node->data);
	}
}

void traversal_ll3(struct list_head *listhead)
{
	struct mylist *node;
	list_for_each_entry_reverse(node, listhead, list)
	{	
		pr_info("%s: Data:%d\n", __func__, node->data);
	}
}

void delete(int data, struct list_head *listhead)
{
	struct mylist *node;
	list_for_each_entry(node, listhead, list)
	{	
		pr_info("%s: Data:%d\n", __func__, node->data);
		if (node->data == data)
		{
			list_del(&node->list);
			break;
		}
	}
	kfree(node);
	
}

void delete_safe(int data, struct list_head *listhead)
{
	struct mylist *node, *next;
	list_for_each_entry_safe(node, next, listhead, list)
	{	
		pr_info("%s: Data:%d\n", __func__, node->data);
		if (node->data == data)
		{
			list_del(&node->list);
			kfree(node);
		}
	}
}

struct mylist *delete_init(int data, struct list_head *listhead)
{
	struct mylist *node;
	list_for_each_entry(node, listhead, list)
	{	
		pr_info("%s: Data:%d\n", __func__, node->data);
		if (node->data == data)
		{
			list_del_init(&node->list);
			break;
		}
	}
	return node;
}

static int __init test_hello_init(void)
{
    add_data_end(1, &listhead1);
    add_data_end(2, &listhead1);
    add_data_end(3, &listhead1);

    pr_info("printing list1\n");
    traversal_ll2(&listhead1);
    
    add_data_end(6, &listhead2);
    add_data_end(7, &listhead2);
    add_data_end(8, &listhead2);
    pr_info("printing list2\n");
    traversal_ll2(&listhead2);

    pr_info("listhead1:%px\t %px\n", listhead1.next, listhead1.prev);
    pr_info("listhead2:%px\t %px\n", listhead2.next, listhead2.prev);
    list_splice_init(&listhead1, &listhead2);
    pr_info("listhead1:%px\t %px\n", listhead1.next, listhead1.prev);
    pr_info("listhead2:%px\t %px\n", listhead2.next, listhead2.prev);
    pr_info("printing list1 after splicing\n");
    traversal_ll2(&listhead1);
    pr_info("printing list2 after splicing\n");
    traversal_ll2(&listhead2);
    return -1;
}

static void __exit test_hello_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
