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


    std::cout << "testing delete key: "; 
    splay_tree.delete_key(T, 2);
    std::cout << std::endl;
    splay_tree.inorder(T);

    auto existing_e = splay_tree.select(T, 3);
    T = splay_tree.splay(existing_e);
    std::cout << "new canonical head of tree = "; T->print_node(); std::cout << std::endl;

    std::cout << "deleting tree: ";
    splay_tree.delete_tree(T);
}


int main(int argc, char *argv[]){
    bool dbg = argv[1] == "-v" ? true : false;
    //test_splay_tree();

    link_cut lc_tree(dbg);
    std::cout << "making tree..." << std::endl;
    lc_tree.make_tree(10);

    std::cout << "getting random element..." << std::endl;
    auto *e = lc_tree.get_rand_element();
    e->print_node();

    std::cout << "getting new random element..." << std::endl;
    e = lc_tree.get_rand_element();
    e->print_node();


    std::cout << "get root of the current element..." << std::endl;
    auto r = lc_tree.find_root(e);
    std::cout << "root of current element = "; r->print_node(); std::cout << std::endl;

    std::cout << "Testing cut with random e" << std::endl;
    r = lc_tree.cut(e);
    std::cout << "root of current element = "; r->print_node(); std::cout << std::endl;

    std::cout << "Testing link..." << std::endl;
    auto r_parent = lc_tree.get_rand_element();
    std::cout << "random parent = ";  r_parent->print_node(); std::cout << std::endl;
    auto link = lc_tree.link(r, r_parent);
    std::cout << "element = "; r->print_node(); std::cout << std::endl;
    std::cout << "rand parent after link = "; r_parent->print_node(); std::cout << std::endl;

    return 0;
}