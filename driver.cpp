#include "link_cut.hpp"


int main(int argc, char *argv[]){
    std::cout << "starting test" << std::endl;

    splay_t splay_tree;
    splay_t::node *T = NULL;

    //test insert
    T = splay_tree.insert(T, 12);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 7);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 19);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 4);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 9);
    splay_tree.inorder(T); std::cout << std::endl;
    T = splay_tree.insert(T, 5);
    splay_tree.inorder(T); std::cout << std::endl;

    std::cout << "printing initial tree: " << std::endl;
    splay_tree.inorder(T);

    // auto existing_e = splay_tree.select(T, 4);
    // std::cout << "selected element: " << existing_e->key << ", should be (4)" << std::endl;
    // auto nonexisting_e = splay_tree.select(T, 8);
    // std::cout << "selected element: " << nonexisting_e->key << ", should be (7 or 9)" << std::endl;

    // splay_tree.splay(existing_e);
    // std::cout << "printing updated tree" << std::endl;
    // splay_tree.inorder(T);

    return 0;
}