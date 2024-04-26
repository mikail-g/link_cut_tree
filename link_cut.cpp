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
    //std::cout << "deleting element " << T->data << std::endl;
    delete T;
}

/* up_rotate_right(x): rotates an element, x, up towards the root on the right */
void splay_t::up_rotate_right(node *x){
    if(x->parent == NULL) //nothing to rotate with
        return;

    node *y = x->parent;

    // std::cout << "x = "; x->print_node();
    // std::cout << "y = "; y->print_node();

    assert(x != y);

    
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

    // std::cout << "(after) x = "; x->print_node();
    // std::cout << "(after) y = "; y->print_node();

}

/* up_rotate_left(x): rotates an element, x, up towards the root on the left */
void splay_t::up_rotate_left(node* x) {
    if(x->parent == NULL)
        return;

    node *y = x->parent;
    assert(x != y);

    // std::cout << "x = "; x->print_node();
    // std::cout << "y = "; y->print_node();

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

    // std::cout << "(after) x = "; x->print_node();
    // std::cout << "(after) y = "; y->print_node();
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
    if(ret != NULL) {
        e->size -= ret->size;
        ret->parent = NULL;
    }
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
    T1 = splay(subtree_max(T1));
    T1->right = T2;
    T1->size += T2->size;
    T2->parent = T1;
    T2->path_parent_ptr = NULL;
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

    //std::cout << "looking for element " << k << ", current element id = " << T->key << std::endl;
    if(k == T->key) { 
        //std::cout << "found key, returning e "; 
        //T->print_node(); std::cout << std::endl;
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

splay_t::node* splay_t::bst_insert(node *T, node *e){
    if(e->key == T->key) {
        T->dups.push_back(e);
        return T;
    }

    if(e->key < T->key) {
        if(T->left != NULL)
            return bst_insert(T->left, e);
        else {
            T->left = e;
            e->parent = T;
        }
    }
    else {
        if(T->right != NULL)
            return bst_insert(T->right, e);
        else {
            T->right = e;
            e->parent = T;
        }
    }
    return e;
}

splay_t::node* splay_t::insert(node *T, int data, int k, node *P) {
    if(T == NULL)
        return new node(data, k, P);

    //find parent node    
    node *e = new node(data, k, P);
    auto ret = bst_insert(T, e);
    if(ret == e) {
        auto r = ret->parent; 
        while(r != NULL){
            r->size++;
            r = r->parent;
        }
    }
    //spay e to the root
    return splay(ret);
}

/* swap_edge(x,y): swap all edges associated with x to be associated with y */
void splay_t::swap_edge(node *x, node *y){
    if(x->parent != NULL) {
        if(x == x->parent->left)
            x->parent->left = y;
        else 
            x->parent->right = y;
    }
    if(x->left != NULL)
        x->left->parent = y;
    if(x->right != NULL)
        x->right->parent = y;

}

void splay_t::delete_element(node *T, node *e){
    if(e == NULL) 
        return; 

    auto search = find(T, e->key);
    if(search == NULL)
        return;

    if(search != e){
        auto it = find_if(search->dups.begin(), search->dups.end(), [&] (const node* d) { return d == e; });
        if(it != search->dups.end())
            search->dups.erase(it);
        return;
    }
    else {
        if(!search->dups.empty()){
            auto new_head = search->dups.front();
            new_head->parent = e->parent;
            swap_edge(e, new_head);
            new_head->dups = e->dups;
            std::swap(e, new_head);
            new_head->dups.erase(new_head->dups.begin());
            return;
        }
        else{ //disconnect e from the tree, making a new tree with e as the root
            auto e_parent = e->parent;
            e->parent = NULL; 
            auto new_head = join(e->left, e->right);
            new_head->parent = e_parent;
            //update sizes along the upwards path
            if(e_parent != NULL){
                if(e == e_parent->left)
                    e_parent->left = new_head;
                else
                    e_parent->right = new_head;
            }
            // std::cout << "e parent = "; e_parent->print_node();
            // std::cout << "new head = "; new_head->print_node();
            while(e_parent != NULL){
                e_parent->size--;
                e_parent = e_parent->parent;
            }
            //delete mem
            delete e;
        }
    } 
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
    int d = 0;
    for(int i = 0; i < n; i++) {
        //std::cout << "current path count: " << i << std::endl;
        splay_t::node *T = NULL, *parent = NULL; 
        int start = 0;
        if(i > 0) {
            parent = get_rand_element();
            //std::cout << "random e = " << std::endl; parent->print_node(); std::cout << std::endl;
            start = parent->key+1;
        }
        T = insert(T, d++, start, parent);
        int len = rand() % MAX_KEY; //generate random length of path
        //std::cout << "len = " << len << std::endl;
        for(int j = start+1; j < start+1+len; j++){ //build the path of nodes
            T = insert(T, d++, j, NULL);
        }
        std::cout << "tree " << i << ": "; T->print_node(); splay_t::inorder(T); std::cout << std::endl;
        paths.push_back(T);
    }
}


splay_t::node* link_cut::path(splay_t::node *v, int i){
        // if(i > 10){
        //     std::cout << "ERROR" << std::endl;
        //     return v; 
        // }
        splay(v); //v is now root of it's auxillary tree
        auto v_right = split(v);  //split v and return it's right subtree
        if(v_right != NULL)
            v_right->path_parent_ptr = v;
        if(v->path_parent_ptr != NULL) {
            std::cout << "v = "; v->print_node();
            auto *w = v->path_parent_ptr;
            std::cout << "path parent = ";  w->print_node(); 
            splay(w);
            auto w_right = split(w);
            if(w_right != NULL) {
                w_right->path_parent_ptr = w;
                //std::cout << "w->right = "; w_right->print_node();
            }
            //std::cout << "w (after split) = ";  w->print_node();
            join(w, v);
            std::cout << "path parent after join = "; w->print_node();
            path(v,++i);
        }
        //std::cout << "v now at root of tree of the represented tree" << std::endl;
        return v;
}

/* find_root(v): Finds the original root of the tree that contains v (e.g. not it's splay tree root, but the root of the tree the splay trees detail) */
splay_t::node* link_cut::find_root(splay_t::node *v){
    splay_t::node *R = path(v,0);
    while(R->left != NULL)
        R = R->left;
    splay(R);
    return R;
}

splay_t::node* link_cut::find_branch(splay_t::node *v) {
    auto R = splay(v);
    while(R->left != NULL)
        R = R->left;
    splay(R);
    return R;
}

/* Link(v,w): is v is a tree-root and w is any other node, make v a child of w
        Assumptions:
            (1): all keys in v are higher than those in w (keyed by depth)
            (2): p is the true root of the splay tree
*/
splay_t::node* link_cut::link(splay_t::node *v, splay_t::node *w) {
    path(v,0);
    if(v->left != NULL) {
        std::cout << "ERROR: v not a tree root, cannot link, must call cut first" << std::endl;
        assert(v->left == NULL);
    }
    path(w,0);
    //note: cannot call join here because join might splay an element > w (which is not the path pointer) to the root of the tree before adding v
    // instead, reverse it and since v has to have no left child, it is safe to make w the left child of v. 
    v->left = w;
    w->parent = v; 
    v->path_parent_ptr = w->path_parent_ptr; //now that w is predecessor of v, set update v's parent path ptr
    w->path_parent_ptr = NULL;
    return v;
}

/* Cut(v): Detach v from it's parent:
            (1): move v to root of the virtual tree, such that all nodes that come below v in $T$ are in the right subtree
            (2): split v 
            
            */
splay_t::node* link_cut::cut(node *v){
    std::cout << "original auxiliary tree: "; splay_t::inorder(v); std::cout << std::endl;

    path(v, 0);
    auto path_parent = v->left;
    auto v_subtree = split(v); 
    if(v_subtree != NULL)
        v_subtree->path_parent_ptr = path_parent;
    if(v_subtree->left != NULL)
        v_subtree->left = NULL;
    std::cout << "updated auxiliary tree: " ;splay_t::inorder(v);  std::cout << std::endl;
    std::cout << "cut tree: "; splay_t::inorder(v); std::cout << std::endl;
    return v_subtree;
}

int link_cut::counting_query(node *i, node *j){
    path(i, 0); //bring i to the root of the virtual tree
    std::cout << "splaying j.." << std::endl;
    splay(j); //get j to the root it's base tree
    std::cout << "done" << std::endl;
    while(j->path_parent_ptr != NULL){
        auto *lca = j->path_parent_ptr; 
        std::cout << "lca = "; lca->print_node();
        splay(lca);
        if(lca->path_parent_ptr == NULL){ //found the lca of i and j
            int lca_size = lca->left != NULL ? lca->size - lca->left->size : lca->size;
            int j_size = j->right != NULL ? j->size - j->right->size : j->size;
            return lca_size + j_size;
        }
        auto lca_right = split(lca);
        if(lca_right != NULL) {
            lca_right->path_parent_ptr = lca;
        }
        join(lca, j);
        splay(j);
    }
    return 0;
}







