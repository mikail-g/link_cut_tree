#ifndef _LINKCUT_H
#define _LINKCUT_H
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <iostream>
#include <unordered_map>

template <typename T>
class splay_t<T> {
    public:
    struct node {
            int key = -1;
            node *left = NULL;
            node *right = NULL;
            node *parent = NULL;
            int size = 0;
            T *data = NULL;

            node(T &dat, int k){
                key = k;
                data = dat;
                size = 1;
            }

            void print_node(){
                std::cout << "key: " << key << ", size = " << size;
                if(parent != NULL)
                    std::cout << ", parent key = " << parent->key;
                if(left != NULL)
                    std::cout << ", left child key = " << left->key;
                if(right != NULL)
                    std::cout << ", right child key = " << right->key;
                std::cout << std::endl;

            }
    }; 

    void up_rotate_right(node *e);
    void up_rotate_left(node *e);
    bool inline_left(node *e);
    bool inline_right(node *e);
    node* subtree_max(node *T);

    node* splay(node *e); 
    node* split(node *e);
    node* join(node *T1, node *T2); 

    node* find(node *T, int k);
    node* select(node *T, int k); //find == select
    int rank(node *e);

    void del(node *T, int key);
    node* insert(node *T, int key);
    node* insert(node *T, T dat, int k);
    node* bst_insert(node* T, int k);

    void inorder(node *T);

};


/* Structure:   Idea is to reprsent the tree as a subset of link-cuts

The represented tree, T, is a tree of arbitrary, unordered nodes split into paths represented by auxillary trees, S, 
                    Nodes from left -> right in the S trees represent the path from the root to the last node on the path (e.g. an inorder traversal)
                    Connected nodes in the represented tree, T, that are not on the same preerred path (and therefore not in the same aux tree S), 
                    are connected via a path-parent pointer, which is stored in the root of the aux tree S, presenting the path

        Preferred paths:    When an access to a node, v, is made in the represented tree, T, that path that is taken becomes the preffered path. 
                            The preferred child of a node is the last child that was on the preferred path, or NULL if the last access was to itself 
                            or has not been accessed
                            A preferred edge connects to the preferred child 
    
*/
class link_cut : public splay_t{

    struct aux_t { //represented tree node (e.g. a preferred path and it's parent pointer in the original T)
        splay_t::node *path;
        splay_t::node *path_parent_ptr; 
        aux_t *pref_child = NULL;
    };
    

    lc_node *make_tree(int n);

    splay_t::node *access(splay_t::node *v); 
    void switch_preferred_child(node *x, node *y);

    public: 
    node *find_root(node *v);
    void cut(node *v);
    void link(node *v1, node *v2);
};


#endif /* ----- #ifndef _LINKCUT_H  ----- */