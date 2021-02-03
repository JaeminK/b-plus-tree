#include <iostream>
#include <map>

#include "node.h"
#include "b-plus-tree.h"

using namespace Tree;

int main()
{
    unsigned int capacity;
    int input;

    std::cout << "This is B+Tree made by Jaemin Kim" << endl;

start:
    std::cout << "enter capacity of b+ tree : ";

    cin >> capacity;

    if (cin.fail()) {
        cout << "Not a valid number. try again!" << endl;
        cin.clear();
        goto start;
    }
    if (capacity < 3) {
        cout << "capacity should be greater than 2" << endl;
        cin.clear();
        goto start;
    }

    Node* root = new Node(capacity);

    while (true) {
        std::cout << "0:insert / 1:delete / 2:print / 3:refresh / 4:exit" << endl;
        std::cout << "Enter an option: ";

        cin >> input;

        if (cin.fail()) {
            cout << "Not a valid number. try again!" << endl;
            cin.clear();
            continue;
        }
        if (input < 0 || input > 5) {
            cout << "Not a valid option. try again!" << endl;
            cin.clear();
            continue;
        }

        switch (input) {
        case 0:
            std::cout << "Enter a number to insert: ";
            cin.clear();
            cin >> input;
            if (cin.fail()) {
                cout << "Not a valid number. try again!" << endl;
                cin.clear();
                continue;
            }
            insert_node(root, input);
            cin.clear();
            break;

        case 1:
            std::cout << "Enter a number to delete: ";
            cin.clear();
            cin >> input;
            if (cin.fail()) {
                cout << "Not a valid number. try again!" << endl;
                cin.clear();
                continue;
            }
            root = delete_node(root, input);
            cin.clear();
            break;

        case 2:
            std::cout << "0:print leaf / 1:print tree" << endl;
            std::cout << "Enter an option: ";
            cin.clear();

            cin >> input;
            if (cin.fail()) {
                cout << "Not a valid number. try again!" << endl;
                cin.clear();
                continue;
            }
            if (input == 0) {
                print_leaf(root);
            }
            else if (input == 1) {
                print_tree(root, 0);
            }
            else {
                cout << "Not a valid option. try again!" << endl;
                cin.clear();
                continue;
            }
            cin.clear();
            break;

        case 3:
            delete(root);
            goto start;

        case 4:
            return 0;

        default:
            std::cout << "read and follow instruction carefully. try again!" << endl;
            continue;
        }

        cin.clear();
    }
}