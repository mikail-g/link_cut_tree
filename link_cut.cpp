#include "link_cut.hpp"

/* HELPER FUNC: Print in-order traversal of BST */
void splay_t::inorder(node *T)
{
    if (T == NULL)
        return;
    inorder(T->left);
    std::cout << T->key << " ";
    inorder(T->right);
}


/* up_rotate_right(x): rotates an element, x, up towards the root on the right */
void splay_t::up_rotate_right(node *x){
    if(x->parent == NULL) //nothing to rotate with
        return;

    node *y = x->parent;
    y->left = x->right;
    if(y->left != NULL)
        y->left->parent = y;
    
    x->right = y;
    x->parent = y->parent;
    if(x->parent != NULL){
        if(x->parent->left == y)
            x->parent->left = x;
        else
            x->parent->right = x;
    }
    
    y->parent = x;
    
    y->size -= x->size; //y loses size(x)
    x->size += y->size; //x gains size(updated y)
    if(y->left != NULL)
        y->size += y->left->size; //y regains size of x's previous right sub-tree


}

/* up_rotate_left(x): rotates an element, x, up towards the root on th left */
void splay_t::up_rotate_left(node* x) {
    if(x->parent == NULL)
        return;

    node *y = x->parent;
    y->right = x->left;
    if(y->right != NULL)
        y->right->parent = y;

    x->left = y;
    x->parent = y->parent;
    if(x->parent != NULL){
        if(x->parent->left == y)
            x->parent->left = x;
        else
            x->parent->right = x;
    }

    y->parent = x;

    y->size -= x->size;
    x->size += y->size; 
    if(y->right != NULL)
        y->size += y->right->size; 
}

/* check if an element, e, is inline with it's parent and grand-parent on the left */
bool splay_t::inline_left(node *e){
    if(e->parent->left != e || e->parent->parent->left != e->parent)
        return false;
    return true;
}

/* check if an element, e, is inline with it's parent and grand-parent on the right */
bool splay_t::inline_right(node *e){
    if(e->parent->right != e || e->parent->parent->right != e->parent)
        return false;
    return true;
}

/* split the splay tree on a desired element:
    1. splay the element to the root such that:
        1.1 e->left are all e's with key <= e->key
        1.2 e->right are all e's with key > e->key 
    2. return e->right
    
*/
splay_t::node* splay_t::split(node *e){
    e = splay(e);
    node *ret = e->right;
    e->size -= e->right->size;
    e->right = NULL;
    return ret;
};

/*  Join two splay trees T1 and T2:
    1. splay the maximum element in T1 to the root
    2. attach T2 as its right subtree
    3. update sizes and parent pointers
*/
splay_t::node* splay_t::join(node *T1, node *T2) {
    splay(subtree_max(T1));
    T1->right = T2;
    T1->size += T2->size;
    T2->parent = T1;
    return T1;
}

/* Find the maximum key of a subtree (e.g. bottom point on the right spine) */
splay_t::node* splay_t::subtree_max(node *T){
    if(T == NULL || T->right == NULL)
        return T;
    return subtree_max(T->right);
}

/* given a key, k, find if the element exists in the tree -- if it doesn't return NULL */
splay_t::node* splay_t::find(node *T, int k) {
    if(T == NULL) return NULL;

    if(k == T->key)
        return T;
    if(k < T->key)
        return find(T->left, k);
    else
        return find(T->right, k);
}

/* Given an index in the sequence, return the element (e.g. order-index element) */
splay_t::node* splay_t::select(node *T, int k) {
    if(T == NULL || T->left == NULL || T->right == NULL)
        return T;

    int r = T->left->size;
    if(k == r) 
        return T;
    if(k < r){
        if(T->left == NULL)
            return T;
        return select(T->left, k);
    }
    else {
        if(T->right == NULL)
            return T;
        return select(T->right, k-r-1);
    }
}

/* get the order index of an element */
int splay_t::rank(node *e){
    return e->left->size;
} 


/* Splay(e) : move element to root of the tree
    Steps: 
        1. if e is 1 step below root: rotate e up to root
        2. else if e is inline with parent and grandparent: rotate parent first then e
        3. else rototate e up 2 steps, using alternating single rotation
*/
splay_t::node *splay_t::splay(node *e) {
    while(e->parent != NULL) { 
        if(e->parent->parent == NULL) { //if e is one step below root --> just rotate up to root
            if(e == e->parent->left) 
                up_rotate_right(e);
            else
                up_rotate_left(e);
        } else if(inline_left(e)) { //e is in-line on left 
                up_rotate_right(e->parent);
                up_rotate_right(e);
        } else if(inline_right(e)) { //e is in-line on right
                up_rotate_left(e->parent);
                up_rotate_left(e);            
        } else if(e == e->parent->left) { //otherwise, rotate up 2 steps 
            up_rotate_right(e);
            up_rotate_left(e);
        } else {
            up_rotate_left(e);
            up_rotate_right(e);
        }
    }
    return e;
}

splay_t::node* splay_t::insert(node *T, int k) {
    if(T == NULL)
        return new node(k);

    //find parent node    
    node *rov = T;
    node *parent = NULL;
    while(rov != NULL){
        parent = rov;
        if(k < rov->key)
            rov = rov->left;
        else
            rov = rov->right;
    }

    //insert e into the BST
    node *e = new node(k);
    e->parent = parent;
    if(k < parent->key) 
        parent->left = e;
    else 
        parent->right = e;
    //update subtree sizes along the path
    while(parent != NULL) {
        parent->size++;
        parent = parent->parent;
    }
    //spay e to the root
    return splay(e);
}

template <typename T>
splay_t::node* splay_t::insert(node *T, T dat, int k) {
    if(T == NULL)
        return new node(dat, k);

    //find parent node    
    node *rov = T;
    node *parent = NULL;
    while(rov != NULL){
        parent = rov;
        if(k < rov->key)
            rov = rov->left;
        else
            rov = rov->right;
    }

    //insert e into the BST
    node *e = new node(k);
    e->parent = parent;
    if(k < parent->key) 
        parent->left = e;
    else 
        parent->right = e;
    //update subtree sizes along the path
    while(parent != NULL) {
        parent->size++;
        parent = parent->parent;
    }
    //spay e to the root
    return splay(e);
}

void splay_t::del(node *T, int key) {
    auto *e = find(T, 9);
    node *e_parent = e->parent;

    e->parent = NULL; //disconnect e from the tree, making a new tree with e as the root
    auto ret = join(e->left, e->right);
    ret->parent = e_parent;

    //reconnect e's children back to the original tree
    if(e == e_parent->left)
        e_parent->left = ret;
    else if(e == e_parent->right)
        e_parent->right = ret;

    //update sizes along the upwards path
    while(e_parent != NULL){
        e_parent->size--;
        e_parent = e_parent->parent;
    }
}


std::vector<splay_t::node *> get_path(splay_t::node *P, int lvl) {

    while(P != NULL) { 
        splay_t::node *path = splay_tree.insert(NULL, P->data, lvl);
        if(rov->left != NULL) {
            if(rov->right != NULL) {
                if(rov->right->size >= rov->left->size)
                    path = splay_tree.insert(path, P->data, ++lvl);
                    get_path(P->left, lvl)
                else

            }

                if(rov->right != NULL && rov->right->size >= rov->left->size) {
                    path = splay_tree.insert(path, l);
                }
                path = splay_tree.insert(path, rov->left->key);
        } else if(rover->right != NULL)
                path = splay_tree.insert(path, rov->right->key); 
    }


        while(there is a next node){
            if(rov->left != NULL) {
                if(rov->right != NULL && rov->right->size >= rov->left->size) {
                    path = splay_tree.insert(path, l);
                }
                path = splay_tree.insert(path, rov->left->key);
            } else if(rover->right != NULL)
                    path = splay_tree.insert(path, rov->right->key); 
            else {
                paths.push_back(path);
                
            }
        }
        paths.push_back(path);


    }
}



link_cut::lc_node *make_tree(int n, int max_key){
    /*  For any arbitrary tree, T, decompose the tree into a set of preferred paths
        Each path is an ancestor-descendant path represented by a splay tree and keyed by depth in T
        Each node has 0 or 1 preferred children, and a preferred edge -- we will use partition by size
        ------------------------------------------------------------------------------------------------ //
        For testing purposes, lets just create n randomized splay trees that represent some set
        of preferred paths along some tree
    */

    paths = get_path(p)









    while(!done){ // get all root -> leaf paths and add them to the vector
        splay_t::node *path = splay_tree.insert(NULL, 0);
        int l = 0;
        if(rov->left != NULL) {
                if(rov->right != NULL && rov->right->size >= rov->left->size) {
                    path = splay_tree.insert(path, l);
                }
                path = splay_tree.insert(path, rov->left->key);
            } else if(rover->right != NULL)
                    path = splay_tree.insert(path, rov->right->key); 
            else {
                paths.push_back(path);
            }
        }


        while(there is a next node){
            if(rov->left != NULL) {
                if(rov->right != NULL && rov->right->size >= rov->left->size) {
                    path = splay_tree.insert(path, l);
                }
                path = splay_tree.insert(path, rov->left->key);
            } else if(rover->right != NULL)
                    path = splay_tree.insert(path, rov->right->key); 
            else {
                paths.push_back(path);
                
            }
        }
        paths.push_back(path);


    }










    srand(time(513545546543)); //seed rand so we get the same numbers for testing 

    std::vector<aux_t> paths; //make auxillary trees

    for(int i = 0; i < n; i++) { 
        int root = rand % max_key; //choose a random node in the "tree" to represent a root
        int max_k = rand % ((max_key - (root+1) + 1)) + (root+1); //generate end of splay
        splay_t::node *T; 
        for(int j = root; j < max_k; j++){ //build the path of nodes
            T = splay_tree.insert(T, j);
        }
        paths.push_back({T, NULL, NULL});
    }
    
    //set the parent pointer paths of each 
    int n_paths = paths.size();
    for(int i = 0; i < n; i++){
        if(paths[i]->key == 0)
            
        int rand_root = -1;
        while(rand_root == -1 && ){
            rand_root = rand % n_paths; //pick a random other splay tree
            int rand_node = rand % paths[rand_root]->size;
            if(select(paths[rand_root], rand_node)->key < paths[i]->key) //get the kth elements key
                rand_root = 
        }
    }





}


splay_t::node *access(splay_t::node *v){
        // node v will have no preferred children, and is placed at end of path
        //nodes in aux tree are keyed at depth, meaning any nodes to the right of v in aux tree are disconnected.
        //steps:
            //1. splay tree at v, bringing v to root of aux tree
            //2. disconnect right subtree of v
            //3. *path-parent for root of disconnected tree -> v

            //walk up the rep tree to the root R, and resettle preferred path where necessary:
                //follow *parent-path from v:
                    //4. if path v is contains R (e.g. left-most node in the aux tree), then *parent = NULL; return
                    //5. else follow *parent-path along some other path w, where we break old preferred path off w and reconnect it to path v is on
                    //5.a splay at w and disconnect right subtree


        splay_t::node *aux = splay_t::splay(v); //
        switch_preffered_child(aux, v, null); //set disconnect v right subtree in S
        if(v->path_parent_ptr != NULL){
            node *w = v->path_parent_ptr;
            splay(w);
            switch_preferred_child(w,v);
            access(v);
        }
        return aux;
}

void link_cut::link(node *v, node *w){
    access(S, v);
    access(S, w);
    v->left = w;
    w->parent = v;
}

void link_cut::cut(node *v){
    access(S, v);
    if(v->left != NULL){
        v->left->path_parent_ptr = v->path_parent_ptr;
        v->left = NULL;
    }
    v->path_parent_ptr = NULL;
}

void link_cut::switch_preferred_child(node *x, node *y){
    if(x->right != NULL)
        x->right->path_parent_ptr = x;
    x->right = y;
    if(y != NULL)
        y->parent = x;
}

splay_t::node link_cut::*find_root(node *v){
    node *aux = access(v);
    node *R = aux;
    while(R->left != NULL)
        R = R->left;
    splay(R);
}



