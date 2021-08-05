#include <stdio.h>
#include <stdlib.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

typedef struct node
{
    int key, height, value;
    struct node *left, *right;
} node;

int height(node *n) 
{
    if(n == NULL)
    {
        return 0; 
    }
    return n->height;
}

void node_update_height(node *n)
{
    n->height = max(height(n->left), height(n->right)) + 1;
}

int node_balance_factor(node *n)
{
    if(n == NULL)
    {
        return 0;
    }
    return height(n->right) - height(n->left);
}

node* node_init(int key, int value)
{
    node *n = (node*)malloc(sizeof(node));
    n->key = key;
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    n->height = 1;
    return n;
}

void node_destroy(node *n)
{
    free(n);
    n = NULL;
}

node* node_rotate_right(node *n)
{
    node *nl = n->left;
    n->left = nl->right;
    nl->right = n;
    node_update_height(n);
    node_update_height(nl);
    return nl;
}

node* node_rotate_left(node *n)
{
    node *nr = n->right;
    n->right = nr->left;
    nr->left = n;
    node_update_height(n);
    node_update_height(nr);
    return nr;
}

node* node_balance(node *n)
{
    node_update_height(n);
    if(node_balance_factor(n) == 2)
    {
        if(node_balance_factor(n->right) < 0)
        {
            n->right = node_rotate_right(n->right);
        }
        return node_rotate_left(n);
    }
    if(node_balance_factor(n) == -2)
    {
        if(node_balance_factor(n->left) > 0)
        {
            n->left = node_rotate_left(n->left);
        }
        return node_rotate_right(n);
    }
    return n;
}

node* node_insert(node *n, int key, int value)
{
    if(n == NULL)
    {
        return node_init(key, value);
    }
    if(key < n->key)
    {
        n->left  = node_insert(n->left, key, value);
    }
    else if(key > n->key)
    {
        n->right = node_insert(n->right, key, value);
    }
    else
    {
        n->value = value;
        return n;
    }
    return node_balance(n);
}

node* node_min(node *n)
{
    node *cur = n;
    while (cur->left != NULL) 
    {
        cur = cur->left;
    }
    return cur;
}

node *node_remove_min(node *n)
{
    if(n->left == NULL)
    {
        return n->right;
    }
    n->left = node_remove_min(n->left);
    return node_balance(n);
}

node* node_remove(node *root, int key)
{
    if(root == NULL)
    {
        return NULL;
    }
    if(key < root->key)
    {
        root->left = node_remove(root->left, key);
    }
    else if(key > root->key)
    {
        root->right = node_remove(root->right, key);
    }
    else
    {
        node *rl = root->left, *rr = root->right;
        node_destroy(root);
        if(rr == NULL)
        {
            return rl;
        }
        node *min = node_min(rr);
        min->right = node_remove_min(rr);
        min->left = rl;
        return node_balance(min);
    }
    return node_balance(root);
}

node* node_search(node *root, int key)
{
    if(root == NULL || root->key == key)
    {
        return root;
    }
    else
    {
        return root->key > key ? node_search(root->left, key) : node_search(root->right, key);
    }
}

int main(void)
{
    node *root = NULL;
    int leave = 0, key, value;
    char c;
    while(!leave)
    {
        scanf("%c", &c);
        switch(c)
        {
            case 'A':
                scanf("%d%d\n", &key, &value);
                root = node_insert(root, key, value);
                break;
            case 'D':
                scanf("%d\n", &key);
                root = node_remove(root, key);
                break;
            case 'S':
                scanf("%d\n", &key);
                node *n = node_search(root, key);
                if(n != NULL)
                {
                    printf("%d %d\n", key, n->value);
                }
                break;
            default:
                leave = 1;
                break;
        }
    }
    return 0;
}