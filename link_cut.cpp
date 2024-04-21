#include "link_cut.hpp"

#include "debug.hpp"
#define __DEBUG

/* Print in-order traversal of BST */
void splay_t::inorder(node *T)
{
    if (T == NULL)
        return;
    inorder(T->left);
    std::cout << T->key << " ";
    inorder(T->right);
}

void splay_t::delete_tree(node *T)
{
    if (T == NULL)
        return;
    delete_tree(T->left);
    delete_tree(T->right);
    delete T;
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
    //swap parent path ptrs
    x->path_parent_ptr = y->path_parent_ptr;
    y->path_parent_ptr = NULL;
    
    y->size -= x->size; //y loses size(x)
    x->size += y->size; //x gains size(updated y)
    if(y->left != NULL)
        y->size += y->left->size; //y regains size of x's previous right sub-tree

}

/* up_rotate_left(x): rotates an element, x, up towards the root on the left */
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
    x->path_parent_ptr = y->path_parent_ptr;
    y->path_parent_ptr = NULL;


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
    if(ret != NULL)
        e->size -= ret->size;
    e->right = NULL;
    return ret;
};

/*  Join two splay trees T1 and T2:
    1. splay the maximum element in T1 to the root
    2. attach T2 as its right subtree
    3. update sizes and parent pointers
*/
splay_t::node* splay_t::join(node *T1, node *T2) {
    if(T1 == NULL)
        return T2;
    if(T2 == NULL)
        return T1;
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
    if(T == NULL) 
        return NULL;

    std::cout << "looking for element " << k << ", current element id = " << T->key << std::endl;
    if(k == T->key) {
        std::cout << "found element, returning e "; 
        T->print_node(); std::cout << std::endl;
        return T;
    }
    if(k < T->key)
        return find(T->left, k);
    else
        return find(T->right, k);
}

/* Given an index in the sequence, return the element (e.g. order-index element) */
splay_t::node* splay_t::select(node *T, int k) {
    if(T == NULL)
        return T;

    int r = T->left == NULL ? 0 : T->left->size;
    if(k == r)
        return T;
    if(k < r)
        return select(T->left, k);
    else
        return select(T->right, k-r-1);
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

splay_t::node* splay_t::insert(node *T, int k, node *P) {
    if(T == NULL)
        return new node(k, P);

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
    node *e = new node(k, P);
    e->parent = parent;
    if(k < parent->key) 
        parent->left = e;
    else 
        parent->right = e;
    //update subtree sizes along the parent path
    while(parent != NULL) {
        parent->size++;
        parent = parent->parent;
    }
    //spay e to the root
    return splay(e);
}

void splay_t::delete_element(node *e){
    if(e == NULL) 
        return; 
    
    auto e_parent = e->parent;
    e->parent = NULL; //disconnect e from the tree, making a new tree with e as the root
    auto ret = join(e->left, e->right);
    ret->parent = e_parent;
    std::cout << " joined children = ";
    ret->print_node(); std::cout << std::endl;

    //reconnect e's children back to the original tree
    if(ret->parent != NULL) {
        if(e == ret->parent->left)
            ret->parent->left = ret;
        else
            ret->parent->right = ret;
    }
    //update sizes along the upwards path
    while(e_parent != NULL){
        e_parent->size--;
        e_parent = e_parent->parent;
    }

    //delete mem
    delete e;

}

void splay_t::delete_key(node *T, int key) {
    auto *e = find(T, key);
    delete_element(e);
}


splay_t::node* link_cut::find_path(splay_t::node *P, int lvl, std::vector<splay_t::node*> &paths) {
    splay_t::node *path = splay_t::insert(NULL, lvl, P);
    while(P != NULL) { 
        if(P->left != NULL) {
            if(P->right != NULL) {
                if(P->right->size >= P->left->size) {
                    paths.push_back(find_path(P->left, ++lvl, paths)); //recurse the left subtree for paths
                    path =  splay_t::insert(path, lvl, NULL); //add the desired node to path and get the other paths
                    P = P->right;
                }
            } else { // recurse the right subtree for other paths and continue left
                paths.push_back(find_path(P->right, ++lvl, paths));
                path =  splay_t::insert(path, lvl, NULL); //add left and cont
                P = P->left;
            }
        } 
        if(P->right != NULL) { //add the desired node to path and cont
            path = splay_t::insert(path, ++lvl, NULL); 
            P = P->right;
        }
    }
    paths.push_back(path);
    return path;
}

splay_t::node* link_cut::get_rand_element(){
    int r_path = rand() % paths.size(); 
    //std::cout << "rand path " << r_path << std::endl;

    int r_node = rand() % paths[r_path]->size; 
    //std::cout << "rand node = " << r_node << std::endl;

    auto e = splay_t::select(paths[r_path], r_node);
    return e;
}


/* Make tree(int n): Generate n random splay trees that represent paths along some tree, T
        Notes: The first splay tree is designated the root and all other splay trees choose 
        a random node in a random previously built path as it's path-parent
*/
void link_cut::make_tree(int n){
    assert(n > 0);
    srand(89784654L); 
    
    for(int i = 0; i < n; i++) {
        //std::cout << "current path count: " << i << std::endl;
        splay_t::node *T = NULL, *parent = NULL; 
        int start = 0;
        if(i > 0) {
            parent = get_rand_element();
            //std::cout << "random e = " << std::endl; parent->print_node(); std::cout << std::endl;
            start = parent->key+1;
        }
        T = splay_t::insert(T, start, parent);
        int len = rand() % MAX_KEY; //generate random length of path
        //std::cout << "len = " << len << std::endl;
        for(int j = start+1; j < start+1+len; j++){ //build the path of nodes
            T = splay_t::insert(T, j, NULL);
        }
        //splay_t::inorder(T); std::cout << std::endl;

        paths.push_back(T);
    }
}


splay_t::node* link_cut::access(splay_t::node *v){
        splay_t::splay(v); //v is now root of it's auxillary tree
        auto v_right = split(v);  //split v and return it's right subtree
        if(v_right != NULL)
            v_right->path_parent_ptr = v;
        if(v->path_parent_ptr != NULL) {
            //std::cout << "v = "; v->print_node(); std::cout << std::endl;
            auto *w = v->path_parent_ptr;
            splay(w);
            auto w_right = split(w);
            w_right->path_parent_ptr = w;
            //std::cout << "w = ";  w->print_node(); std::cout << std::endl;
            //std::cout << "w->right = ";w_right->print_node(); std::cout << std::endl;
            join(w, v);
            //std::cout << "w after join = "; w->print_node(); std::cout << std::endl;
            access(v);
        }
        //std::cout << "v now at root of tree" << std::endl;
        return v;
}

/* find_root(v): Gets the root of the splay tree that currently holds v */
splay_t::node* link_cut::find_root(splay_t::node *v){
    int i = 0;
    splay_t::node *R = access(v);
    while(R->left != NULL)
        R = R->left;
    splay(R);
    return R;
}

/* Link(v,w): make aux tree, v, a new child of aux tree, w
        Assumptions: 
            1. v and w are not already contained within the same path 
            2. v is keyed such that all of it's keys are larger than any key stored in w
*/
splay_t::node* link_cut::link(splay_t::node *v, splay_t::node *w) {
    access(v);
    access(w);
    join(w, v);
    v->path_parent_ptr = NULL; //now that v is attached to w, it will no longer hold a path-parent-ptr
    return v;
}

/* Cut(v): Detach v from it's current aux tree */
splay_t::node* link_cut::cut(node *v){
    access(v);
    auto path_parent = v->left;
    auto v_subtree = split(v->left); 
    v_subtree->path_parent_ptr = path_parent;
    return v_subtree;
}





