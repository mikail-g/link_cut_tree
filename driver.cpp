#include "link_cut.hpp"

#define MAX_KEY 100

void test_splay_tree(int n){
    splay_t splay_tree;
    splay_t::node *T = NULL;
    std::vector<int> keys;
    srand(89784654L);

    std::cout << "testing insert " << std::endl;
    for(int i = 0; i < n; i++) {
        auto k = rand() % MAX_KEY;
        std::cout << "NEW KEY " << k << std::endl;
        keys.push_back(k); 
        T = splay_tree.insert(T, i, k, NULL);
        splay_tree.inorder(T); std::cout << std::endl;
    }
    std::cout << "done, tree built" << std::endl;

    auto r_k = rand() % keys.size();
    std::cout << "get the " << r_k << "th element" << std::endl;
    auto sorted_keys = keys;
    std::sort(sorted_keys.begin(), sorted_keys.end());
    sorted_keys.erase(std::unique(sorted_keys.begin(), sorted_keys.end()), sorted_keys.end());
    auto e = splay_tree.select(T, r_k);
    if(e->key != sorted_keys[r_k])
        std::cout << "FAILURE: select returned wrong element, expected " << sorted_keys[r_k] << ", got " << e->key << std::endl;


    std::cout << "testing delete element: deleteing e = "; e->print_node();
    splay_tree.delete_element(T, e);
    splay_tree.inorder(T);

    std::cout << "deleting tree: ";
    splay_tree.delete_tree(T);
}

void print_subtree(splay_t::node *e){
    std::cout << "e = ";
    e->print_node();
    std::cout << "e->left = ";
    if(e->left != NULL)
        e->left->print_node(); 
    std::cout << "e->right = ";
    if(e->right != NULL)
        e->right->print_node(); 
    std::cout << std::endl;
}


int main(int argc, char *argv[]){
    bool dbg = argv[1] == "-v" ? true : false;
    //test_splay_tree(20);

    link_cut lc_tree(dbg);
    std::cout << "making tree..." << std::endl;
    lc_tree.make_tree(10);


    //query some elements and make sure they look right
    for(int i = 0; i < 12; i++) {
        std::cout << "getting random element..." << std::endl;
        auto *e = lc_tree.get_rand_element();
        print_subtree(e);
    }

    std::cout << "done" << std::endl;

    // //mix up the splay tree some by accessing some elements
    for(int i = 0; i < 10; i++){
        auto *e = lc_tree.get_rand_element();
        std::cout << "NEW ACCESS e = "; e->print_node();
        lc_tree.access(e, 0);
        std::cout << "END" << std::endl;
        print_subtree(e);
    }


    // //test some cuts
    // std::cout << "Testing cut..." << std::endl;
    // auto r_e = lc_tree.get_rand_element();
    // auto t = lc_tree.access(r_e);
    // auto cut_re = lc_tree.cut(r_e);
    // std::cout << "old tree = "; t->print_node(); std::cout << std::endl;
    // std::cout << "cut tree = "; cut_re->print_node(); std::cout << std::endl;

    // std::cout << "check find root after cut" << std::endl;
    // assert(lc_tree.find_root(t) == lc_tree.find_root(cut_re));

    std::cout << "Test done, exiting" << std::endl;

    return 0;
}