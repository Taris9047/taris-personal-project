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
static int Destroy(rTrieNode node);
static int AttachNode(rTrieNode me, rTrieNode addon);
static LNode ChildSearch(LNode children, const char key);
static int ChildDestroy(LNode children);
static rTrieNode Traverse(rTrieRoot root, char* key);


/* Constructors and Destructors */
rTrieRoot rTrieInit()
{
    rTrieRoot root = (rTrieRoot)malloc(sizeof(rwe_trie_root));
    assert(root);

    root->name = NULL;
    root->head = NULL;
    return root;
}

rTrieRoot rTrieInitN(char* rtrie_name)
{
    rTrieRoot root = rTrieInit();
    root->name = rtrie_name;
    return root;
}

int rTrieDestroy(rTrieRoot root)
{
    assert(root);
    LNode tmp;

    if (root->head) {
        tmp = root->head;
        while (tmp) {
            Destroy((rTrieNode)tmp->value);
            tmp = tmp->next;
        }
    }

    return 0;
}

/* Utils */
int rTrieInsert(rTrieRoot root, char* key, rtrie_value_t value)
{
    assert(root);
    assert(strlen(key)>0);
    assert(value);

    int keylen = strlen(key);
    unsigned long i;
    rTrieNode rtp;
    rTrieNode rtptmp;
    LNode cc;

    /* if root is empty, make first node */
    if (!root->head) {
        root->head = ListInit();
        assert(root->head);
        rtp = New((rTrieNode)root, key[0], NULL);
        ListPush(&root->head, rtp);
    }
    /* if we already have first node, initialize rtp and cc */
    else {
        cc = ChildSearch(root->head, key[0]);
        if (cc) rtp = cc->value;
        else {
            rtp = New((rTrieNode)root, key[0], NULL);
            assert(rtp);
            ListPush(&root->head, rtp);
        }
    }
    //cc = rtp->children;
    i = 0;

    /* Ok, running the insertion from the 1st node */
    while (i < keylen-1) {
        cc = ChildSearch(rtp->children, key[i+1]);
        if (cc) rtp = (rTrieNode)cc->value;
        else {
            rtptmp = New(rtp, key[i+1], NULL);
            assert(rtptmp);
            AttachNode(rtp, rtptmp);
            rtp = rtptmp;
        }
        ++i;
    }

    /* lastly, attach the termination character */
    rtptmp = New(rtp, key[i+1], value);
    AttachNode(rtp, rtptmp);

    return 0;
}

/* Checking if given key string is a member of
   current R-Way Trie */
int rTrieIsMember(rTrieRoot root, char* key)
{
    assert(root);
    assert(strlen(key)>0);

    if (Traverse(root, key)) return 1;
    else return 0;
}

/* Remove given keystring and entry */
int rTrieRemove(rTrieRoot root, char* key)
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
rtrie_value_t rTrieGet(rTrieRoot root, char* key)
{
    assert(root);
    assert(strlen(key)>0);

    rTrieNode rtp = Traverse(root, key);

    if (rtp) return rtp->value;
    else return NULL;
}
int rTrieSet(rTrieRoot root, char* key, rtrie_value_t value)
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
    rt->value = value;

    LNode ch = ListInit();
    assert(ch);
    rt->children = ch;

    return rt;
}

static int Destroy(rTrieNode node)
{
    if (!node) return 0;

    LNode cld;
    if (node->children) {
        cld = node->children;
        ChildDestroy(cld);

    }

    if (node->value) free(node->value);
    free(node);

    return 0;
}

static int AttachNode(rTrieNode me, rTrieNode addon)
{
    assert(me);
    assert(addon);

    if(ListPush(&me->children, addon)) return -1;

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

static int ChildDestroy(LNode children)
{
    if (!children) return 0;

    LNode tmp;
    while (children) {
        if (children->value) Destroy((rTrieNode)children->value);
        tmp = children;
        children = children->next;
        free(tmp);
    }

    return 0;
}

/* Traverse into the R-Way Existence Trie with given
   keystring... */
static rTrieNode Traverse(rTrieRoot root, char* key)
{
    assert(root);
    assert(strlen(key)>0);

    unsigned long i = 0;
    int keylen = strlen(key);
    rTrieNode rtp;
    LNode lntmp;

    /* Root is empty? */
    if (!root->head) return 0;

    /* Assign initial point to rtp */
    lntmp = ChildSearch(root->head, key[0]);
    if (!lntmp) return NULL;
    else rtp = (rTrieNode)lntmp->value;

    i = 0;
    while (1) {
        if (i < keylen-1) {
            lntmp = ChildSearch(rtp->children, key[i+1]);
            if (!lntmp) return NULL;
            else rtp = (rTrieNode)lntmp->value;
        }
        else {
            /* Get Last Node.. must be same('\0') for all keys */
            lntmp = ChildSearch(rtp->children, key[i+1]);
            if (!lntmp) return NULL;
            else return (rTrieNode)lntmp->value;
        }
        ++i;
    }
}
