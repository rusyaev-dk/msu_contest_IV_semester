#include "SetTester.h"
#include <iostream>

#include <functional>
#include <vector>


int main() {
    SetTester set_tester = SetTester(1000000000);
    // find test failed (50k size and 1.75mln elems)
    std::vector<std::function<void()>> tester_functions = {
        // [&set_tester]() { return set_tester.insert_test(1750000); },
        [&set_tester]() { return set_tester.find_test(1750000); },
        // [&set_tester]() { return set_tester.remove_even_test(1500000); },
        // [&set_tester]() { return set_tester.duplicate_iterator_test(); },
    };

    for (int i = 0; i < tester_functions.size(); ++i) {
        try {
            tester_functions[i]();
            std::cout << "✓ Test " << i + 1 << " passed\n";
            std::cout << "_____________________________\n";
        } catch (SetTesterInsertException exception) {
            std::cout << "Caught insert exception. Address: " << exception.elem() << " at position: " << exception.index() << " callback code: " << exception.err_code() << "\n";  
            std::cout << "⚠️ Test " << i << " failed\n";
            continue;
        } catch (SetTesterFindException exception) {
            std::cout << "Caught find exception. Address: " << exception.elem() << " at position: " << exception.index() << "\n";  
            std::cout << "⚠️ Test " << i << " failed\n";
            continue;
        } catch (SetTesterRemoveException exception) {
            int* elem = (int*)exception.elem();
            std::cout << "Caught remove exception." << " Elem " << *elem <<" at address " << elem << "\n";  
            std::cout << "⚠️ Test " << i << " failed\n";
            continue;
        }
    }
    return 0;
}
