#ifndef _LINKCUT_H
#define _LINKCUT_H
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <algorithm>

class splay_t{
    public:
    struct node {
            int key = -1;
            node *left = NULL;
            node *right = NULL;
            node *parent = NULL;
            int size = 0;
            int data = 0;
            std::vector<node*> dups;

            node *path_parent_ptr;

            node(int d, int k, node *P){
                if(P != NULL)
                    path_parent_ptr = P;
                key = k;
                data = d;
                size = 1;
            }

            void print_node(){
                std::cout << "key: " << key << ", data = " << data << ", size = " << size;
                if(parent != NULL)
                    std::cout << ", parent key = " << parent->key;
                if(left != NULL)
                    std::cout << ", left child key = " << left->key;
                if(right != NULL)
                    std::cout << ", right child key = " << right->key;
                if(path_parent_ptr != NULL) {
                    std::cout << ", path parent key = " << path_parent_ptr->key;
                    std::cout << ", path parent dat = " << path_parent_ptr->data;
                }

                std::cout << std::endl;

            }
    }; 

    void up_rotate_right(node *e);
    void up_rotate_left(node *e);
    bool inline_left(node *e);
    bool inline_right(node *e);
    node* subtree_max(node *T);
    splay_t::node* bst_insert(node *T, node *e);

    node* splay(node *e); 
    node* split(node *e);
    node* join(node *T1, node *T2); 

    node* find(node *T, int k);
    node* select(node *T, int k); 
    int rank(node *e);

    // void delete_key(node *T, int key);
    void swap_edge(node *x, node *y);
    void delete_element(node *T, node *e); 
    node* insert(node *T, int data, int k, node *P);
    node* bst_insert(node* T, int k);

    void inorder(node *T);
    void delete_tree(node *T);

};


/* Structure: 
        The represented tree, T, is a tree of arbitrary, unordered nodes split into paths represented by auxillary trees, S, 
                    Nodes from left -> right in the S trees represent the path from the root to the last node on the path 
                    Connected nodes in the represented tree, T, that are not on the same preerred path (and therefore not in the same aux tree S), 
                    are connected via a path-parent pointer, which is stored in the root of the aux tree S, representing the path

        Solidifying: When an path to a node v is made in the represented tree, we find the path from the node v to the root of the represented tree 
*/
class link_cut : public splay_t{
    public:
    static const int MAX_KEY = 10;
    bool verbose = true;

    std::vector<splay_t::node*> paths;
    
    void make_tree(int n);
    splay_t::node* get_rand_element();

    splay_t::node* path(splay_t::node *v, int i); 
    splay_t::node* find_root(splay_t::node *v);
    splay_t::node* find_branch(splay_t::node *v);
    splay_t::node* cut(splay_t::node *v);
    splay_t::node* link(splay_t::node *v, splay_t::node *w);
    int counting_query(splay_t::node *i, splay_t::node *j);


    link_cut(bool dbg){
        if(dbg)
            verbose = true;
    }

    ~link_cut(){
        for(auto &p : paths) {
            //std::cout << "deleting tree " << &p - &paths[0] << std::endl;
            splay_t::delete_tree(p);
        }
    }
};


#endif /* ----- #ifndef _LINKCUT_H  ----- */