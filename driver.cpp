#include "link_cut.hpp"


void test_splay_tree(){
    splay_t splay_tree;
    splay_t::node *T = NULL;
    //test insert
    T = splay_tree.insert(T, 1, NULL);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 2, NULL);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 3, NULL);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 4, NULL);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 5, NULL);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 6, NULL);
    splay_tree.inorder(T); std::cout << std::endl;


    std::cout << "testing delete: "; 
    splay_tree.delete_key(T, 2);
    std::cout << std::endl;
    splay_tree.inorder(T);

    auto existing_e = splay_tree.select(T, 3);
    T = splay_tree.splay(existing_e);

    splay_tree.delete_tree(T);
}


int main(int argc, char *argv[]){

    //test_splay_tree();

    splay_t sp;
    link_cut lc_tree;
    splay_t::node *RT = lc_tree.make_tree(10);
    sp.delete_tree(RT);

    return 0;
}