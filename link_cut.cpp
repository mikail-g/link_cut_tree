#include "link_cut.hpp"

void splay_t::inorder(node *T)
{
    if (T == NULL)
        return;
    inorder(T->left);
    std::cout << T->key << " ";
    inorder(T->right);
}

void splay_t::up_rotate_right(node *x){
    if(x->parent == NULL) //nothing to rotate with
        return;

    std::cout << "x = ";
    x->print_node(); std::cout << std::endl;

    node *y = x->parent;
    std::cout << "y = ";
    y->print_node(); std::cout << std::endl;

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

    std::cout << "y = ";
    y->print_node(); std::cout << std::endl;

     std::cout << "updated x = ";
    x->print_node(); std::cout << std::endl;

}

void splay_t::up_rotate_left(node* x) {
    if(x->parent == NULL)
        return;
    std::cout << "x = ";
    x->print_node(); std::cout << std::endl;

    node *y = x->parent;
    std::cout << "y = ";
    y->print_node(); std::cout << std::endl;

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

    y->size -= x->size; //y loses size(x)
    x->size += y->size; //x gains size(updated y)
    if(y->right != NULL)
        y->size += y->right->size; //y regains size of x's previous left sub-tree

    std::cout << "y = ";
    y->print_node(); std::cout << std::endl;

    std::cout << "updated x = ";
    x->print_node(); std::cout << std::endl;
}

bool splay_t::inline_left(node *e){
    if(e->parent->left != e || e->parent->parent->left != e->parent)
        return false;
    return true;
}

bool splay_t::inline_right(node *e){
    if(e->parent->right != e || e->parent->parent->right != e->parent)
        return false;
    return true;
}

splay_t::node* splay_t::subtree_max(node *T){
    if(T == NULL || T->right == NULL)
        return T;
    return subtree_max(T->right);
}

splay_t::node* splay_t::split(node *e){
    splay(e);
    node *ret = e->right;
    e->right = NULL;
    return ret;
};

splay_t::node* splay_t::join(node *T1, node *T2) {
    splay(subtree_max(T1));
    T1->right = T2;
    return T1;
}

splay_t::node* splay_t::find(node *T, int k) {
    if(T == NULL) return NULL;
    if(k == T->key)
        return T;
    if(k < T->key)
        return find(T->left, k);
    else
        return find(T->right, k);
}

splay_t::node* splay_t::select(node *T, int k) {
    if(T == NULL || T->left == NULL || T->right == NULL)
        return T;

    int r = T->left->size;
    if(k == r) 
        return T;
    if(k < r)
        return select(T->left, k);
    else
        return select(T->right, k-r-1);
}

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

void splay_t::del(node *e){
    e = join(e->left, e->right);
}
