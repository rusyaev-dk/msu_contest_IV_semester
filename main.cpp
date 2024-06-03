#include "SetTester.h"

#include <iostream>
#include <functional>
#include <vector>

void old_testing(SetTester& set_tester) {
    std::vector<std::function<void()>> tester_functions = {
        // [&set_tester]() { return set_tester.test_insert(290000); },
        // [&set_tester]() { return set_tester.test_find(290000); },
        // [&set_tester]() { return set_tester.test_remove(290000); },
        // [&set_tester]() { return set_tester.test_remove_even(290000); },
        // [&set_tester]() { return set_tester.test_duplicated_iterator(); },
        // [&set_tester]() { return set_tester.test_perfomance(7500000); },
        // [&set_tester]() { return set_tester.test_user_data_types(); },
        // [&set_tester]() { return set_tester.test_clear(10000); },
        // [&set_tester]() { return set_tester.check_perfomance(10000); },
        [&set_tester]() { return set_tester.test_iterator_empty_set(); },
    };

    for (int i = 0; i < tester_functions.size(); ++i) {
        try {
            tester_functions[i]();
            std::cout << "✓ Test " << i + 1 << " passed\n\n";
        } catch (SetTesterException& e) {
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
    set_tester.run_all_tests(290000);
    // set_tester.check_perfomance(1000000);
    
    // old_testing(set_tester);

    return 0;
}
