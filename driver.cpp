#include "link_cut.hpp"

#define MAX_KEY 100

void test_splay_tree(int n){
    splay_t splay_tree;
    splay_t::node *T = NULL;
    std::vector<int> keys;
    srand(89784654L);

    std::cout << "TEST 1: Insert..." << std::endl;
    for(int i = 0; i < n; i++) {
        auto k = rand() % MAX_KEY;
        std::cout << "NEW KEY " << k << std::endl;
        keys.push_back(k); 
        T = splay_tree.insert(T, i, k, NULL);
        splay_tree.inorder(T); std::cout << std::endl;
    }
    std::cout << "done, tree built" << std::endl;
    std::cout << "END TEST 1" << std::endl << std::endl;

    std::cout << "TEST 2: Select..." << std::endl;
    auto r_k = rand() % keys.size();
    std::cout << "get the " << r_k << "th element" << std::endl;
    auto sorted_keys = keys;
    std::sort(sorted_keys.begin(), sorted_keys.end());
    sorted_keys.erase(std::unique(sorted_keys.begin(), sorted_keys.end()), sorted_keys.end());
    auto e = splay_tree.select(T, r_k);
    std::cout << r_k << "th element found, expexted key = " << sorted_keys[r_k] << ", got e = "; e->print_node();
    if(e->key != sorted_keys[r_k])
        std::cout << "FAILURE: select returned wrong element, expected " << sorted_keys[r_k] << ", got " << e->key << std::endl;
    std::cout << "END TEST 2" << std::endl << std::endl ;


    std::cout << "TEST 3: Split..." << std::endl;
    auto r = rand() % keys.size();
    r_k = keys[r]; 
    std::cout << "splitting the tree on element " << r_k << std::endl;
    auto r_e = splay_tree.find(T, r_k);
    auto sp = splay_tree.split(r_e);
    std::cout << "original tree (after split): "; splay_tree.inorder(r_e); std::cout << std::endl;
    std::cout << "newly split tree: "; splay_tree.inorder(sp); std::cout << std::endl;
    std::cout << "END TEST 3" << std::endl << std::endl;

    std::cout << "TEST 4: Join..." << std::endl;
    std::cout << "rejoing recently split trees..." << std::endl;
    T = splay_tree.join(r_e, sp);
    std::cout << "Join(T1,T2): "; splay_tree.inorder(T); std::cout << std::endl;
    std::cout << "END TEST 4" << std::endl << std::endl;
    
    std::cout << "TEST 5: Delete..." << std::endl;
    r = rand() % keys.size();
    r_k = keys[r]; 
    r_e = splay_tree.find(T, r_k);
    std::cout << "testing delete element: deleteing e = "; r_e->print_node();
    splay_tree.delete_element(T, r_e);
    splay_tree.inorder(T); std::cout << std::endl;
    std::cout << "END TEST 5" << std::endl << std::endl;

    std::cout << "SPLAY TEST COMPLETE" << std::endl << std::endl;
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
    splay_t sp;
    link_cut lc(dbg);

   std::cout << "TEST 1: Build..." <<std::endl;
    lc.make_tree(10);
    std::cout << "END TEST 1" << std::endl;

    //query some elements and make sure they look right
    for(int i = 0; i < 12; i++) {
        std::cout << "getting random element..." << std::endl;
        auto *e = lc.get_rand_element();
        print_subtree(e);
    }

    //testing path
    std::cout << "TEST 2: Solidify Path..." << std::endl;
    for(int i = 0; i < 5; i++){
        auto e = lc.get_rand_element();
        std::cout << "NEW path e = "; e->print_node();
        auto p = lc.path(e, 0);
        print_subtree(p);
        std::cout << "END path" << std::endl;
    }

    for(auto &p : lc.paths) {
        std::cout << "path " << &p - &lc.paths[0] << " "; 
        sp.inorder(p); std::cout << std::endl;
    }
    std::cout << "END TEST 2" << std::endl;


    // //test some cuts
    // std::cout << "Testing cut..." << std::endl;
    // splay_t::node* r_e;
    // for(int i = 0; i < 8; i++)
    //     r_e = lc.get_rand_element(); 
    // auto cut_re = lc.cut(r_e);


    // std::cout << "Testing link..." << std::endl;
    // auto w = lc.paths[5];
    // auto v = lc.paths[6];
    // auto l = lc.link(v, w);
    // std::cout << "in order after link: "; sp.inorder(l); std::cout << std::endl;

    std::cout << "TEST 5: PATH QUERY..." << std::endl;
    auto i = lc.get_rand_element();
    auto j = lc.get_rand_element();
    while(i == j)
        j = lc.get_rand_element();
    std::cout << "i: "; i->print_node(); std::cout << "j: "; j->print_node();
    auto path_sum = lc.counting_query(i, j);
    std::cout << "number of elements between i and j = " << path_sum << std::endl;
    std::cout << "END TEST 5" << std::endl;

    
    std::cout << "Test done, exiting" << std::endl;

    return 0;
}