#include "SetTester.h"

#include <iostream>
#include <functional>
#include <vector>

void single_testing(SetTester& set_tester) {
    std::vector<std::function<void()>> tester_functions = {
        // [&set_tester]() { return set_tester.test_insert(1000000); },
        // [&set_tester]() { return set_tester.test_insert_duplicates(290000); },

        // [&set_tester]() { return set_tester.test_find(290000); },
        
        // [&set_tester]() { return set_tester.test_remove(50000); },
        // [&set_tester]() { return set_tester.test_remove_even(290000); },

        // [&set_tester]() { return set_tester.test_clear(10000); },

        // [&set_tester]() { return set_tester.test_iterator_traversal(10000); },
        // [&set_tester]() { return set_tester.test_iterator_after_last_elem_removal(); },
        // [&set_tester]() { return set_tester.test_iterator_empty_set(); },
        // [&set_tester]() { return set_tester.test_iterator_cleared_set(10000); },
        [&set_tester]() { return set_tester.test_duplicate_iterators_removal(); },
        
        // [&set_tester]() { return set_tester.test_user_data_type(); },
};

    for (int i = 0; i < tester_functions.size(); ++i) {
        try {
            tester_functions[i]();
            std::cout << "✓ Test " << i + 1 << " passed\n\n";
        } catch (ContainerTesterException& e) {
            std::cout << e.what() << "\n\n";
            continue;
        } catch (Container::Error& err) {
            std::cout << err.msg << "\n\n";
            continue;
        }
    }
}

// 290_000 critical quantity

int main() {
    SetTester set_tester = SetTester(1000000);
    // set_tester.run_all_tests(290000);
    // set_tester.check_perfomance(1000000);
    
    single_testing(set_tester);

    return 0;
}
