#ifndef _LINKCUT_H
#define _LINKCUT_H
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <iostream>


class splay_t {
    public:
    struct node {
            int key = -1;
            node *left = NULL;
            node *right = NULL;
            node *parent = NULL;
            int size = 0;

            node(int k){
                key = k;
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

    void splay(node *e); 
    node* split(node *e);
    node* join(node *T1, node *T2); 

    node *find(node *T, int k);
    node* select(node *T, int k); //find == select
    int rank(node *e);

    void del(node *e);
    node* insert(node *T, int key);
    node *bst_insert(node* T, int k);

    void inorder(node *T);

}; 

// class link_cut : splay_t {

//     access(node *v){
//         // nove v will have no preferred children, and is placed at end of path

//         //nodes in aux tree are keyed at depth, meaning any nodes to the right of v in aux tree are disconnected.

//         //steps:
//             //1. splay tree at v, bringing v to root of aux tree
//             //2. disconnect right subtree of v
//             //3. *path-parent for root of disconnected tree -> v

//             //walk up the rep tree to the root R, and resettle preferred path where necessary:
//                 //follow *parent-path from v:
//                     //4. if path v is contains R (e.g. left-most node in the aux tree), then *parent = NULL; return
//                     //5. else follow *parent-path along some other path w, where we break old preferred path off w and reconnect it to path v is on
//                     //5.a splay at w and disconnect right subtree


//         splay(v);
//         switch_preffered_child(v, null);
//         if(path-parent(v) != NULL){
//             w = path_parent(v);
//             splay(w);
//             switch_preferred_child(w,v);
//             access(v);
//         }
//     }





//     public: 
//     find_root(node *v);
//     cut(node *v);
//     link(node *v1, node *v2);
//     path(node *v1);

// }


#endif /* ----- #ifndef _LINKCUT_H  ----- */