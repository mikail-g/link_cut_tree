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
    splay(e);
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
void splay_t::splay(node *e) {
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
    splay(e);
    return e;
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
