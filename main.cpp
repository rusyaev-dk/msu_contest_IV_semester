#include "SetTester.h"
#include <iostream>

#include <functional>
#include <vector>


int main() {
    SetTester set_tester = SetTester(1000000000);

    std::vector<std::function<void()>> tester_functions = {
        [&set_tester]() { return set_tester.insert_test(8000000); },
        // [&set_tester]() { return set_tester.find_test(175000); },
        // [&set_tester]() { return set_tester.remove_even_test(1500000); },
        // [&set_tester]() { return set_tester.duplicate_iterator_test(); },
        // [&set_tester]() { return set_tester.iterator_test(10); },
    };

    for (int i = 0; i < tester_functions.size(); ++i) {
        try {
            tester_functions[i]();
            std::cout << "✓ Test " << i + 1 << " passed\n";
            std::cout << "_____________________________\n";
        } catch (SetTesterInsertException exception) {
            size_t* elem = static_cast<size_t*>(exception.get_elem());
            std::cout << "Caught insert exception. Elem: " << *elem << ". Error code: " << exception.get_err_code() << "\n";  
            std::cout << "⚠️ Test " << i << " failed\n";
            continue;
        } catch (SetTesterFindException exception) {
            size_t* elem = static_cast<size_t*>(exception.get_elem());
            std::cout << "Caught find exception. Elem: " << *elem << "\n";  
            std::cout << "⚠️ Test " << i << " failed\n";
            continue;
        } catch (SetTesterRemoveException exception) {
            size_t* elem = static_cast<size_t*>(exception.get_elem());
            std::cout << "Caught remove exception." << " Elem " << *elem <<" at address " << elem << "\n";  
            std::cout << "⚠️ Test " << i << " failed\n";
            continue;
        }
    }
    return 0;
}
