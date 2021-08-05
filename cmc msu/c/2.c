#include <stdio.h>
#include <stdlib.h>

typedef struct list
{
    int value;
    struct list *right;
} list;

list* list_init(int value)
{
    list *head = (list*)malloc(sizeof(list));
    head->right = NULL;
    head->value = value;
    return head;
}

list* list_add(list *el, int value)
{
    list *new_el = malloc(sizeof(list));
    new_el->right = el->right;
    new_el->value = value;
    el->right = new_el;
    return new_el;
}

void list_destroy(list *head)
{
    list *d = head;
    while(d != NULL)
    {
        list *right = d->right;
        free(d);
        d = right;
    }
}

list* merge(list *l1, list *l2)
{
    if(l1 == NULL)
    {
        return l2;
    }
    if(l2 == NULL)
    {
        return l1;
    }
    if(l1->value < l2->value)
    {
        l1->right = merge(l1->right, l2);
        return l1;
    }
    else
    {
        l2->right = merge(l1, l2->right);
        return l2;
    }
}

void to_halves(list *head, list **left, list **right)
{
    *left = head;
    list *mid = head, *end = head->right;
    while(end != NULL)
    {
        end = end->right;
        if(end != NULL)
        {
            end = end->right;
            mid = mid->right;
        }
    }
    *right = mid->right;
    mid->right = NULL;
}

void sort(list **head)
{
    if(*head == NULL || (*head)->right == NULL)
    {
        return;
    }
    list *left, *right;
    to_halves(*head, &left, &right);
    sort(&left);
    sort(&right);
    *head = merge(left, right);
}

void print(FILE *output, list *head)
{
    while(head != NULL)
    {
        fprintf(output, "%d ", head->value);
        head = head->right;
    }
    printf("\n");
}

int main(void)
{
	FILE *input, *output;
	input = fopen("input.txt", "r");
	output = fopen("output.txt", "w");
    int num;
    fscanf(input, "%d", &num);
    list *head = list_init(num), *last = head;
    while(fscanf(input, "%d", &num) != EOF)
    {
        last = list_add(last, num);
    }
    sort(&head);
    print(output, head);
    list_destroy(head);
	fclose(input);
	fclose(output);
	return 0;
}