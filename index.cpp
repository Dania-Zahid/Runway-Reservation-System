#include <iostream>
using namespace std;
struct Node {
    int data;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(int value) : data(value) {}
};

class RunwayReservationSystem {
private:
    Node* root = nullptr;
    int k;

public:
    RunwayReservationSystem(int k) : k(k) {}

    // Function to request a landing time
    string request(const string& timeStr) {
        int t = convertTimeToInt(timeStr);

        if (search(root, t) || !checkKMinute(root, t) || t <= getCurrentTime()) {
            return "Requested time conflicts with existing reservations, does not meet the k-minute criteria, or is in the past.";
        }

        root = insert(root, t);
        return "Reservation successfully made.";
    }

    // Function to simulate a plane landing
    string land() {
        if (!root) {
            return "No reservations to land.";
        }

        int landingTime = findMin(root)->data;
        root = deleteNode(root, landingTime);

        return "Plane landed successfully at time " + convertIntToTime(landingTime) + ".";
    }

    // Function to get the maximum reservation time
    string getMaxTime() const {
        if (!root) {
            cout<<"No reservations made."<<endl;
        }
        return convertIntToTime(findMax(root)->data);
    }
    
     string searchReservation(const string& timeStr) const {
        int t = convertTimeToInt(timeStr);

        if (search(root, t)) {
            return "Reservation already made for the requested landing time.";
        } else {
            return "No reservation at this landing time.";
        }
    }
    // Function to get the minimum reservation time
    string getMinTime() const {
        if (!root) {
            cout<<"No reservations made."<<endl;
        }
        return convertIntToTime(findMin(root)->data);
    }

    // Function to get the next landing time in the queue
    string getNextLandingTime() const {
        if (!root) {
            cout<<"No reservations made."<<endl;
        }
        return convertIntToTime(findMin(root)->data);
    }
      // Function to get the number of reservations before the given time
    int getReservationRank(const string& timeStr) const {
        int targetTime = convertTimeToInt(timeStr);

        if (!search(root, targetTime)) {
            cout << "No reservation made at this time." << endl;
            return -1; // Return a special value for non-existent time
        }

        return rank(root, targetTime);
    }
    
    void displayReservations() const {
        cout << "Landing times reserved at the moment: ";
        displayInOrder(root);
        cout << endl;
    }

private:

       void displayInOrder(Node* node) const {
        if (node) {
            displayInOrder(node->left);
            cout << convertIntToTime(node->data) << " ";
            displayInOrder(node->right);
        }
    }

    int convertTimeToInt(const string& timeStr) const {
        int hours = 0, minutes = 0;
        int index = 0;

        while (index < timeStr.size() && timeStr[index] != ':') {
            if (timeStr[index] < '0' || timeStr[index] > '9') {
                cout<<"Invalid time format."<<endl;
            }
            hours = hours * 10 + (timeStr[index] - '0');
            ++index;
        }

        if (index == timeStr.size() || timeStr[index] != ':') {
            cout<<"Invalid time format."<<endl;
        }

        ++index;

        while (index < timeStr.size()) {
            if (timeStr[index] < '0' || timeStr[index] > '9') {
               cout<<"Invalid time format."<<endl;
            }
            minutes = minutes * 10 + (timeStr[index] - '0');
            ++index;
        }

        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
            cout<<"Invalid time value."<<endl;
        }

        return hours * 60 + minutes;
    }

    string convertIntToTime(int timeInt) const {
        int hours = timeInt / 60;
        int minutes = timeInt % 60;

        return to_string(hours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes);
    }

    bool search(Node* root, int key) const {
        if (!root) {
            return false;
        }

        if (key == root->data) {
            return true;
        }

        if (key < root->data) {
            return search(root->left, key);
        } else {
            return search(root->right, key);
        }
    }

    Node* insert(Node* root, int key) {
        if (!root) {
            return new Node(key);
        }

        if (key < root->data) {
            root->left = insert(root->left, key);
        } else if (key > root->data) {
            root->right = insert(root->right, key);
        } else {
            // Duplicate values are not allowed
            return root;
        }

        return root;
    }

    Node* findMin(Node* root) const {
        while (root->left) {
            root = root->left;
        }
        return root;
    }

    Node* findMax(Node* root) const {
        while (root->right) {
            root = root->right;
        }
        return root;
    }

    Node* deleteNode(Node* root, int key) {
        if (!root) {
            return root;
        }

        if (key < root->data) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->data) {
            root->right = deleteNode(root->right, key);
        } else {
            // Node with only one child or no child
            if (!root->left) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children, get the inorder successor (smallest in the right subtree)
            root->data = findMin(root->right)->data;

            // Delete the inorder successor
            root->right = deleteNode(root->right, root->data);
        }
        return root;
    }

    bool checkKMinute(Node* node, int newTime) const {
    if (!node) {
        return true;
    }

    int nodeTime = node->data;

    // Calculate the absolute difference without using abs
    int timeDifference = (newTime > nodeTime) ? (newTime - nodeTime) : (nodeTime - newTime);

    if (timeDifference < k) {
        return false;  // Violates k-minute criteria
    }

    return checkKMinute(node->left, newTime) && checkKMinute(node->right, newTime);
}

    int getCurrentTime() const {
        if (!root) {
            return -1; // Return a special value for an empty tree
        }
        return findMax(root)->data;
    }
     int rank(Node* node, int targetTime) const {
        if (!node) {
            return 0;
        }

        if (targetTime <= node->data) {
            return rank(node->left, targetTime);
        } else {
            // Count the current node + nodes in the left subtree + nodes in the right subtree
            return 1 + size(node->left) + rank(node->right, targetTime);
        }
    }

    int size(Node* node) const {
        if (!node) {
            return 0;
        }

        return 1 + size(node->left) + size(node->right);
    }
};

int main() {
    int k;
    cout << "Enter the value of k: ";
    cin >> k;

    RunwayReservationSystem runway(k);

    int choice;
    do {
        cout << "1. Request a landing time" << endl;
        cout << "2. Simulate a plane landing" << endl;
        cout << "3. Get Maximum Landing Time" << endl;
        cout << "4. Get Minimum Landing Time" << endl;
        cout << "5. Get Next Landing Time" << endl;
        cout << "6. Search for Landing Time Reservation" << endl;
        cout << "7. Check Rank" <<endl;
        cout << "8. Display the Reservations" <<endl;
        cout << "9. Exit" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string landingTime;
                cout << "Enter the requested landing time (in 24-hour format, e.g., 21:32): ";
                cin >> landingTime;
                cout << runway.request(landingTime) << endl;
                break;
            }
            case 2: {
                cout << runway.land() << endl;
                break;
            }
            case 3: {
                    cout << "Maximum Landing Time: " << runway.getMaxTime() << endl;
                break;
            }
            case 4: {
                    cout << "Minimum Landing Time: " << runway.getMinTime() << endl;
                break;
            }
            case 5: {
                    cout << "Next Landing Time: " << runway.getNextLandingTime() << endl;
                break;
            }
            case 6: {
                string searchTime;
                cout << "Enter the landing time to search (in 24-hour format, e.g., 21:32): ";
                cin >> searchTime;
                cout << runway.searchReservation(searchTime) << endl;
                break;
            }
            case 7: {
            
                string rankTime;
                cout << "Enter the time to get the reservation rank (in 24-hour format, e.g., 21:32): ";
                cin >> rankTime;
                int reservationRank = runway.getReservationRank(rankTime);

                if (reservationRank != -1) {
                    cout << "Number of reservations before " << rankTime << ": " << reservationRank << endl;
                }
                break;
            
            }
            case 8:{
                runway.displayReservations();
                break;
            }
             case 9: {
                 cout << "Exiting program."<<endl;
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 9);

    return 0;
}
