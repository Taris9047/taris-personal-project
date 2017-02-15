/***************************************

  Practice coding for R-Way Existence Trie

  Implementation file

  Taylor Shin, Feb. 14 2017

  Reference:
  http://www.cs.princeton.edu/courses/archive/spr04/cos226/lectures/trie.4up.pdf

 ***************************************/

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "rwe_trie.h"

/* Some static functions */
static rTrieNode New(rTrieNode parent, const char key, rtrie_value_t value);
static int Delete(rTrieNode node);
static int AttachNode(rTrieNode me, const rTrieNode addon);
static LNode ChildSearch(LNode children, const char key);
static rTrieNode Traverse(rTrieRoot root, const char* key);


/* Constructors and Destructors */
rTrieRoot rTrieInit()
{
    rTrieRoot root = (rTrieRoot)malloc(sizeof(rwe_trie_root));
    assert(root);

    root->name = NULL;
    root->head = NULL;
    return root;
}

rTrieRoot rTrieInitN(const char* rtrie_name)
{
    rTrieRoot root = rTrieInit();
    root->name = rtrie_name;
    return root;
}

int rTrieDelete(rTrieRoot root)
{
    assert(root);

    if (root->head) Delete(root->head);

    return 0;
}

/* Utils */
int rTrieInsert(rTrieRoot root, const char* key, rtrie_value_t value)
{
    assert(root);
    assert(strlen(key)>0);
    assert(value);

    int keylen = strlen(key);
    unsigned long i = 0;
    rTrieNode rtp;
    rTrieNode tmp_rtp;
    LNode tmp;

    /* if root is empty, make first node */
    if (!root->head) root->head = New(root, '\0', NULL);
    rtp = root->head;

    while (1) {
        /* last key always finishes with '\0' */
        // if (key[i] == '\0') {
        //     rtp->value = value;
        //     break;
        // }

        /* If current key letter matches ... */
        if (rtp->key == key[i]) {
            if (i < keylen-2) {
                tmp = ChildSearch(rtp->children, key[i+1]);
                if (tmp) rtp = (rTrieNode)tmp->value;
                else {
                    tmp_rtp = New(rtp, key[i+1], NULL);
                    AttachNode(rtp, tmp_rtp);
                    rtp = tmp_rtp;
                }
            }
            else {
                /* if i >= keylen-1 --> last key letter which is always '\0' */
                rtp->value = value;
                break;
            }
        }
        /* If current key letter doesn't match... */
        /* Not sure if we reach this point, though */
        else {
            /* backstep */
            --i;
            rtp = rtp->parent;
        }
        ++i;
    }

    return 0;
}

/* Checking if given key string is a member of
   current R-Way Trie */
int rTrieIsMember(rTrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key)>0);

    if (Traverse(root, key)) return 1;
    else return 0;
}

/* Remove given keystring and entry */
int rTrieRemove(rTrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key)>0);

    int keylen = strlen(key);
    rTrieNode rtp = Traverse(root, key);
    rTrieNode rtptmp;
    if (rtp) {
        /* Ok, track up and delete all */
        while (1) {
            rtptmp = rtp->parent;
            if (!ListDelete(rtptmp->children, rtp)) rtp = rtptmp;
            if (ListLen(rtp->children) > 1) break;
        }
        return 0;
    }
    else return 1;
}

/* Get and Set */
rtrie_value_t rTrieGet(rTrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key)>0);

    rTrieNode rtp = Traverse(root, key);

    if (rtp) return rtp->value;
    else return NULL;
}
int rTrieSet(rTrieRoot root, const char* key, rtrie_value_t value)
{
    assert(root);
    assert(strlen(key)>0);
    //assert(value);

    rTrieNode rtp = Traverse(root, key);

    if (rtp) rtp->value = value;
    else return -1;

    return 0;
}


/* Static function implementations */
static rTrieNode New(rTrieNode parent, const char key, rtrie_value_t value)
{
    rTrieNode rt = (rTrieNode)malloc(sizeof(rwe_trie_node));
    assert(rt);
    rt->parent = parent;
    rt->key = key;
    LNode ch = ListInit();
    rt->children = ch;
}

static int Delete(rTrieNode node)
{
    assert(node);
    LNode cld = node->children;

    while (cld->next) {
        Delete(cld->next);
        cld = cld->next;
    }

    ListDestroy(node->children);
    free(node->value);
    free(node);

    return 0;
}

static int AttachNode(rTrieNode me, const rTrieNode addon)
{
    assert(me);
    assert(addon);

    if(ListPush(me->children, addon)) return -1;

    return 0;
}

static LNode ChildSearch(LNode children, const char key)
{
    assert(children);

    LNode tmp = children;
    while (tmp->next) {
        if ( ((rTrieNode)(tmp->value))->key == key ) return tmp;
        tmp = tmp->next;
    }

    return NULL;
}

/* Traverse into the R-Way Existence Trie with given
   keystring... */
static rTrieNode Traverse(rTrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key)>0);

    unsigned long i = 0;
    int keylen = strlen(key);
    rTrieNode rtp = root->head;
    LNode lntmp;

    /* Root is empty? */
    if (!root->head) return 0;

    while (1) {
        if (i < keylen-2) {
            if (rtp->key == key[i]) {
                lntmp = ChildSearch(rtp->children, key[i+1]);
                if (lntmp) rtp = (rTrieNode)lntmp->value;
                else return NULL;
            }
            else return NULL;
        }
        else {
            /* Last Node.. must be same('\0') for all keys */
            return rtp;
        }
        ++i;
    }
}
