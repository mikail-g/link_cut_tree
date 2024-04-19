#include "link_cut.hpp"

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
    T = NULL;
}

/* up_rotate_right(x): rotates an element, x, up towards the root on the right */
void splay_t::up_rotate_right(node *x){
    if(x->parent == NULL) //nothing to rotate with
        return;

    if(x->parent->parent == NULL){ //if parent is the root, take the path_parent_ptr;
        x->path_parent_ptr = x->path_parent_ptr;
        x->parent->path_parent_ptr = NULL;
    }

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

/* up_rotate_left(x): rotates an element, x, up towards the root on the left */
void splay_t::up_rotate_left(node* x) {
    if(x->parent == NULL)
        return;

    if(x->parent->parent == NULL){ //if parent is the root, take the path_parent_ptr;
        x->path_parent_ptr = x->path_parent_ptr;
        x->parent->path_parent_ptr = NULL;
    }

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

    std::cout << "looking for element " << k << "my element id = " << T->key << std::endl;
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
}



splay_t::node *link_cut::make_tree(int n){
    assert(n > 0);

    srand(89784654L); //seed rand so we get the same numbers for testing 
    //generate n splay trees
    std::vector<splay_t::node*> paths; 
    for(int i = 0; i < n; i++) {
        std::cout << "current path count: " << i << std::endl;
        splay_t::node *T = NULL, *parent = NULL; 
        int start = 0;
        if(i > 0) {
            int prev_path = rand() % paths.size(); // choose a random previous path
            std::cout << "prev path is " << prev_path << std::endl;

            int path_parent = rand() % paths[prev_path]->size; //pick node in prev path
            std::cout << "rand node = " << path_parent << std::endl;

            parent = splay_t::select(paths[prev_path], path_parent);
            std::cout << "random parent = "; parent->print_node(); std::cout << std::endl;
            
            start = parent->key+1;
        }

        T = splay_t::insert(T, start, parent);
        
        int len = rand() % MAX_KEY; //generate random length of path
        std::cout << "len = " << len << std::endl;
        for(int j = start+1; j < start+1+len; j++){ //build the path of nodes
            T = splay_t::insert(T, j, NULL);
        }
         
        splay_t::inorder(T); std::cout << std::endl;
        paths.push_back(T);
    }
    return paths[0];
}


// splay_t::node *link_cut::access(splay_t::node *v){
//         // node v will have no preferred children, and is placed at end of path
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


//         splay_t::node *path = splay_t::splay(v); 
//         if(v->path_parent_ptr != NULL){
//             node *w = v->path_parent_ptr;
//             splay(w);
//             switch_preferred_child(w,v);
//             access(v);
//         }
//         return path;
// }

// void link_cut::link(lc_node *v, lc_node *w) {
//     access(v->path);
//     access(w->path);
//     v->left = w;
//     w->path_parent_ptr = v;
// }

// void link_cut::cut(node *v){
//     access(S, v);
//     if(v->left != NULL){
//         v->left->path_parent_ptr = v->path_parent_ptr;
//         v->left = NULL;
//     }
//     v->path_parent_ptr = NULL;
// }

// void link_cut::switch_preferred_child(node *x, node *y){
//     if(x->right != NULL)
//         x->right->path_parent_ptr = x;
//     x->right = y;
//     if(y != NULL)
//         y->parent = x;
// }

// link_cut::lc_node* link_cut::find_root(node *v){
//     node *aux = access(v);
//     node *R = aux;
//     while(R->left != NULL)
//         R = R->left;
//     splay(R);
// }



