#include <iostream>
#include <string>

using namespace std;

// Node for gaming machine linked list
struct MachineNode {
    string machineID;
    string gameType;
    string status; // Available, In Use, Reserved, Out of Order
    int tokenType; // 1, 2, or 3
    MachineNode* next;
    
    MachineNode(string id, string type, int tokens) 
        : machineID(id), gameType(type), tokenType(tokens), 
          status("Available"), next(nullptr) {}
};

// Linked List for machines in a zone
class MachineList {
private:
    MachineNode* head;
    
public:
    MachineList(){
    	head = nullptr;
	} 
    
    ~MachineList() {
        MachineNode* current = head;
        while (current) {
            MachineNode* next = current->next;
            delete current;
            current = next;
        }
    }
    
    void addMachine(string id, string type, int tokens) {
        MachineNode* newNode = new MachineNode(id, type, tokens);
        if (!head) {
            head = newNode;
        } else {
            MachineNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    
    MachineNode* findAvailableMachine(string gameType) {
        MachineNode* temp = head;
        while (temp) {
            if (temp->gameType == gameType && temp->status == "Available") {
                return temp;
            }
            
            temp = temp->next;
        }
        return nullptr;
    }
    
    void updateMachineStatus(string id, string newStatus) {
        MachineNode* temp = head;
        while (temp) {
            if (temp->machineID == id) {
                temp->status = newStatus;
                return;
            }
            temp=temp->next;
        }
    }
    
    void displayMachines() {
        MachineNode* temp=head;
        cout<<"Machines in this zone:\n";
        while (temp) {
            cout<<"ID: "<<temp->machineID 
                 <<", Type:"<<temp->gameType 
                 <<", Tokens: "<<temp->tokenType 
                 <<", Status: "<<temp->status<<endl;
            temp=temp->next;
        }
    }
    
    int countMachinesByStatus(string status) {
        int count = 0;
        MachineNode* temp = head;
        while (temp){
            if (temp->status == status) {
                count++;
            }
            temp= temp->next;
        }
        return count;
    }
};

// Tree node for arcade zones
struct ZoneNode {
    string zoneName;
    MachineList machines;
    ZoneNode* left;
    ZoneNode* right;
    
    ZoneNode(string name) : zoneName(name), left(nullptr), right(nullptr) {}
};

// Tree for arcade zones
class ZoneTree {
private:
    ZoneNode* root;
    
    ZoneNode* insertRec(ZoneNode* node, string zoneName) {
        if (!node) {
            return new ZoneNode(zoneName);
        }
        
        if (zoneName < node->zoneName) {
            node->left = insertRec(node->left, zoneName);
        } else if (zoneName > node->zoneName) {
            node->right = insertRec(node->right, zoneName);
        }
        
        return node;
    }
    
    ZoneNode* searchRec(ZoneNode* node, string zoneName) {
        if (!node || node->zoneName == zoneName) {
            return node;
        }
        
        if (zoneName < node->zoneName) {
            return searchRec(node->left, zoneName);
        }
        
        return searchRec(node->right, zoneName);
    }
    
    void displayZonesRec(ZoneNode* node) {
        if (node) {
            displayZonesRec(node->left);
            cout << "\nZone: " << node->zoneName << endl;
            node->machines.displayMachines();
            displayZonesRec(node->right);
        }
    }
    
    void destroyRec(ZoneNode* node) {
        if (node) {
            destroyRec(node->left);
            destroyRec(node->right);
            delete node;
        }
    }
    
public:
    ZoneTree() {
               root = nullptr;	
	} 
    
    ~ZoneTree() {
        destroyRec(root);
    }
    
    void addZone(string zoneName) {
        root = insertRec(root, zoneName);
    }
    
    ZoneNode* getZone(string zoneName) {
        return searchRec(root, zoneName);
    }
    
    void displayAllZones() {
        cout << "\n=== All Zones ===\n";
        displayZonesRec(root);
    }
    
    MachineNode* findMachineInZones(string gameType) {
        if (!root) return nullptr;

        // Use iterative in-order traversal
        ZoneNode* current = root;
        ZoneNode** stack = new ZoneNode*[100];
        int top = -1;
        MachineNode* result = nullptr;

        while (current || top != -1) {
            while (current) {
                stack[++top] = current;
                current = current->left;
            }
            
            current = stack[top--];
            
            // Check machines in this zone
            result = current->machines.findAvailableMachine(gameType);
            if (result) {
                delete[] stack;
                return result;
            }
            
            current = current->right;
        }
        
        delete[] stack;
        return nullptr;
    }
};

// Node for token request queue
struct RequestNode {
    string playerName;
    string gameType;
    int duration; // in minutes
    bool isPriority; // true for high priority
    RequestNode* next;
    
    RequestNode(string name, string type, int time, bool priority){
    	playerName=name;
    	gameType=type;
    	duration=time;
    	isPriority = priority;
    	next = nullptr;
	}
        
};

// Queue for token requests with priority
class RequestQueue {
private:
    RequestNode* front;
    RequestNode* rear;
    
public:
    RequestQueue(){
    	front = nullptr;
    	rear = nullptr;
	} 
    
    ~RequestQueue() {
        while (!isEmpty()) {
            RequestNode* temp = dequeue();
            delete temp;
        }
    }
    
    void enqueue(string name, string type, int time, bool priority) {
        RequestNode* newNode = new RequestNode(name, type, time, priority);
        
        if (!front) {
            front = rear = newNode;
        } else if (priority) {
            // High priority goes to the front
            newNode->next = front;
            front = newNode;
        } else {
            // Regular request goes to the end
            rear->next = newNode;
            rear = newNode;
        }
    }
    
    RequestNode* dequeue() {
        if (!front) {
            return nullptr;
        }
        
        RequestNode* temp = front;
        front = front->next;
        
        if (!front) {
            rear = nullptr;
        }
        
        return temp;
    }
    
    void displayQueue() {
        RequestNode* temp = front;
        if (!temp) {
            cout << "Request queue is empty.\n";
            return;
        }
        
        cout << "\n=== Request Queue ===\n";
        while (temp) {
            cout <<"Player: "<<temp->playerName 
                 <<", Game: "<<temp->gameType 
                 <<", Duration: "<<temp->duration<<" mins"
                 <<", Priority: "<<(temp->isPriority ? "High" : "Normal")<<endl;
            temp = temp->next;
        }
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
};

// Node for session history stack
struct SessionNode {
    string playerName;
    string machineID;
    int duration;
    SessionNode* next;
    
    SessionNode(string name, string machine, int time){
    	playerName = name;
    	machineID = machine;
    	duration = time;
    	next = nullptr;
	}
  
};

// Stack for session history
class SessionStack {
private:
    SessionNode* top;
    
public:
    SessionStack() {
    	top = nullptr;
	} 
    
    ~SessionStack() {
        while (top) {
            pop();
        }
    }
    
    void push(string name, string machine, int time) {
        SessionNode* newNode = new SessionNode(name, machine, time);
        newNode->next = top;
        top = newNode;
    }
    
    void pop() {
        if (!top) {
            cout << "Session stack is empty.\n";
            return;
        }
        
        SessionNode* temp = top;
        top = top->next;
        delete temp;
    }
    
    void displayStack() {
        SessionNode* temp = top;
        if (!temp) {
            cout<<"Session history is empty.\n";
            return;
        }
        
        cout<<"\n=== Session History ===\n";
        cout<<"(Most recent first)\n";
        while (temp) {
            cout<<"Player: "<<temp->playerName 
                 <<", Machine: "<<temp->machineID 
                 <<", Duration: "<<temp->duration<<" mins"<<endl;
            temp = temp->next;
        }
    }
    
    SessionNode* getTop() {
        return top;
    }
};

// Main arcade system class
class GameArcadeSystem {
private:
    ZoneTree zones;
    RequestQueue requests;
    SessionStack sessions;
    
public:
    void initializeArcade() {
        // Add zones
        zones.addZone("VR");
        zones.addZone("Racing");
        zones.addZone("Puzzle");
        zones.addZone("Shooting");
        
        // Add machines to VR zone
        ZoneNode* vrZone = zones.getZone("VR");
        for (int i = 1; i <= 8; i++) {
            vrZone->machines.addMachine("VR" + to_string(i), "VR Game", 2);
        }
        
        // Add machines to Racing zone
        ZoneNode* racingZone = zones.getZone("Racing");
        for (int i = 1; i <= 8; i++) {
            racingZone->machines.addMachine("RC" + to_string(i), "Racing Game", 1);
        }
        
        // Add machines to Puzzle zone
        ZoneNode* puzzleZone = zones.getZone("Puzzle");
        for (int i = 1; i <= 8; i++) {
            puzzleZone->machines.addMachine("PZ" + to_string(i), "Puzzle Game", 1);
        }
        
        // Add machines to Shooting zone
        ZoneNode* shootingZone = zones.getZone("Shooting");
        for (int i = 1; i <= 8; i++) {
            shootingZone->machines.addMachine("SH" + to_string(i), "Shooting Game", 3);
        }
        
        cout << "Arcade initialized with 4 zones and 8 machines per zone.\n";
    }
    
    void addTokenRequest() {
        string playerName, gameType;
        int duration;
        char priority;
        bool isPriority = false;
        
        cout << "\nEnter player name: ";
        cin.ignore();
        getline(cin, playerName);
        
        // Validate game type
        while (true) {
            cout << "Enter game type (VR Game, Racing Game, Puzzle Game, Shooting Game): ";
            getline(cin, gameType);
            
            if (gameType == "VR Game" || gameType == "Racing Game" || 
                gameType == "Puzzle Game" || gameType == "Shooting Game") {
                break;
            }
            cout << "Invalid game type. Please try again.\n";
        }
        
        // Validate duration
        while (true) {
            cout << "Enter duration (minutes, positive number): ";
            cin >> duration;
            if (duration > 0) break;
            cout << "Invalid duration. Please enter a positive number.\n";
        }
        
        // Validate priority
        while (true) {
            cout << "Is this a priority request? (y/n): ";
            cin >> priority;
            if (priority == 'y' || priority == 'Y' || 
                priority == 'n' || priority == 'N') {
                break;
            }
            cout << "Please enter 'y' or 'n'.\n";
        }
        
        isPriority = (priority == 'y' || priority == 'Y');
        requests.enqueue(playerName, gameType, duration, isPriority);
        cout << "Token request added to queue.\n";
    }
    
    void processRequests() {
        cout << "\nProcessing requests...\n";
        while (!requests.isEmpty()) {
            RequestNode* request = requests.dequeue();
            MachineNode* machine = zones.findMachineInZones(request->gameType);
            
            if (machine) {
                string zoneName = machine->gameType.substr(0, machine->gameType.find(' '));
                ZoneNode* zone = zones.getZone(zoneName);
                
                if (zone) {
                    zone->machines.updateMachineStatus(machine->machineID, "In Use");
                    sessions.push(request->playerName, machine->machineID, request->duration);
                    
                    cout << "Assigned machine " << machine->machineID 
                         << " to " << request->playerName 
                         << " for " << request->duration << " minutes.\n";
                } else {
                    cout << "Zone not found for machine " << machine->machineID << endl;
                }
            } else {
                cout << "No available machine for " << request->playerName 
                     << "'s request (" << request->gameType << ").\n";
            }
            
            delete request;
        }
    }
    
    void cancelLastSession() {
        SessionNode* lastSession = sessions.getTop();
        if (lastSession) {
            string machineID = lastSession->machineID;
            string zoneName;
            
            // Determine zone based on machine prefix
            if (machineID.find("VR") == 0) {
                zoneName = "VR";
            } else if (machineID.find("RC") == 0) {
                zoneName = "Racing";
            } else if (machineID.find("PZ") == 0) {
                zoneName = "Puzzle";
            } else if (machineID.find("SH") == 0) {
                zoneName = "Shooting";
            } else {
                cout << "Unknown machine type: " << machineID << endl;
                return;
            }
            
            ZoneNode* zone = zones.getZone(zoneName);
            if (zone) {
                zone->machines.updateMachineStatus(machineID, "Available");
                sessions.pop();
                cout << "Last session canceled and machine " << machineID << " set to Available.\n";
            } else {
                cout << "Zone not found for machine " << machineID << endl;
            }
        } else {
            cout << "No sessions to cancel.\n";
        }
    }
    
    void displaySystemStatus() {
        zones.displayAllZones();
        requests.displayQueue();
        sessions.displayStack();
    }
    
    void showMachineStatistics() {
        int totalAvailable = 0;
        int totalInUse = 0;
        
        ZoneNode* vrZone = zones.getZone("VR");
        if (vrZone) {
            totalAvailable += vrZone->machines.countMachinesByStatus("Available");
            totalInUse += vrZone->machines.countMachinesByStatus("In Use");
        }
        
        ZoneNode* racingZone = zones.getZone("Racing");
        if (racingZone) {
            totalAvailable += racingZone->machines.countMachinesByStatus("Available");
            totalInUse += racingZone->machines.countMachinesByStatus("In Use");
        }
        
        ZoneNode* puzzleZone = zones.getZone("Puzzle");
        if (puzzleZone) {
            totalAvailable += puzzleZone->machines.countMachinesByStatus("Available");
            totalInUse += puzzleZone->machines.countMachinesByStatus("In Use");
        }
        
        ZoneNode* shootingZone = zones.getZone("Shooting");
        if (shootingZone) {
            totalAvailable += shootingZone->machines.countMachinesByStatus("Available");
            totalInUse += shootingZone->machines.countMachinesByStatus("In Use");
        }
        
        cout << "\n=== Machine Statistics ===\n";
        cout << "Available Machines: " << totalAvailable << endl;
        cout << "Machines In Use: " << totalInUse << endl;
        cout << "Total Machines: " << (totalAvailable + totalInUse) << endl;
    }
};

// Function to display menu
void displayMenu() {
    cout << "\n=== Game Arcade Management System ===\n";
    cout << "1. Initialize Arcade\n";
    cout << "2. Add Token Request\n";
    cout << "3. Process Requests\n";
    cout << "4. Cancel Last Session\n";
    cout << "5. View System Status\n";
    cout << "6. View Machine Statistics\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    GameArcadeSystem arcade;
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                arcade.initializeArcade();
                break;
            case 2:
                arcade.addTokenRequest();
                break;
            case 3:
                arcade.processRequests();
                break;
            case 4:
                arcade.cancelLastSession();
                break;
            case 5:
                arcade.displaySystemStatus();
                break;
            case 6:
                arcade.showMachineStatistics();
                break;
            case 7:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (choice != 7);
    
    return 0;
}