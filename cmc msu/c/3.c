#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 101
#define INDEX 10000U

typedef struct pair
{
    char key[MAX_SIZE];
    long long value;
} pair;

void pair_init(pair *p, char key[MAX_SIZE], long long value)
{
    for(int i = 0; i < MAX_SIZE; ++i)
    {
        p->key[i] = key[i];
    }
    p->value = value;
}

typedef struct list
{
    pair value;
    struct list *right;
} list;
list *hash_table[INDEX] = {NULL};

list* list_init(char key[MAX_SIZE], long long value)
{
    list *head = (list*)malloc(sizeof(list));
    head->right = NULL;
    pair_init(&head->value, key, value);
    return head;
}

list* list_add(list **el, char key[MAX_SIZE], long long value)
{
    if(*el == NULL)
    {
        *el = list_init(key, value);
        return *el;
    }
    list *new_el = (list*)malloc(sizeof(list));
    new_el->right = (*el)->right;
    pair_init(&new_el->value, key, value);
    (*el)->right = new_el;
    return new_el;
}

long long list_find(list* head, char key[MAX_SIZE])
{
    while(head != NULL)
    {
        if(strncmp(key, head->value.key, MAX_SIZE) == 0)
        {
            return head->value.value;
        }
        head = head->right;
    }
    return -1;
}

void list_destroy(list *head)
{
    while(head != NULL)
    {
        list *right = head->right;
        free(head);
        head = right;
    }
}

unsigned MurmurHash2(char key[MAX_SIZE])
{
    const unsigned m = 0x5bd1e995;
    const unsigned seed = 0;
    const int r = 24;
    unsigned len = strlen(key);

    unsigned h = seed ^ len;

    const unsigned char *data = (const unsigned char *)key;
    unsigned k;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
        default:
            break;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h % INDEX;
}

void hash_table_add(char key[MAX_SIZE], long long value)
{
    list_add(&hash_table[MurmurHash2(key)], key, value);
}

long long hash_table_find(char key[MAX_SIZE])
{
    return list_find(hash_table[MurmurHash2(key)], key);
}

int main(void)
{
	FILE *input, *output;
	input = fopen("input.txt", "r");
	output = fopen("output.txt", "w");

    int names_num;
    fscanf(input, "%d\n", &names_num);
    for(int i = 0; i < names_num; ++i)
    {
        char comp[MAX_SIZE];
        long long address;
        fscanf(input, "%s %lld\n", comp, &address);
        hash_table_add(comp, address);
    }

    int requests_num;
    fscanf(input, "%d\n", &requests_num);
    for(int i = 0; i < requests_num; ++i)
    {
        char comp[MAX_SIZE];
        fscanf(input, "%s\n", comp);
        fprintf(output, "%lld\n", hash_table_find(comp));
    }

    for(int i = 0; i < INDEX; ++i)
    {
        if(hash_table[i] != NULL)
        {
            list_destroy(hash_table[i]);
        }
    }
	fclose(input);
	fclose(output);
	return 0;
}