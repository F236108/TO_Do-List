#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
// A simple task manager program that allows users to add, mark, and view tasks.
class Task {
public:
    string title;
    string category;
    bool completed;

    Task(string t, string c = "") {
        title = t;
        category = c;
        completed = false;
    }

    void display(int index) const {
        cout << setw(3) << index + 1 << (completed ? " " : " ") <<"-> " << title;
        if (!category.empty())
            cout << " (" << category << ")";
        cout << endl;
    }

    string toFileFormat() const {
        return title + ";" + category + ";" + (completed ? "1" : "0");
    }

    static Task fromFileFormat(const string& line) {
        size_t pos1 = line.find(';');
        size_t pos2 = line.rfind(';');
        string t = line.substr(0, pos1);
        string c = line.substr(pos1 + 1, pos2 - pos1 - 1);
        bool comp = (line.substr(pos2 + 1) == "1");
        Task task(t, c);
        task.completed = comp;
        return task;
    }
};

class TaskManager {
private:
    vector<Task> tasks;
    const string filename = "tasks.txt";
public:
    TaskManager() {
        loadFromFile();
    }

    ~TaskManager() {
        saveToFile();
    }
     
    void addTask() {
        cin.ignore();
        string title, category;
        cout << "\n Enter task title: ";
        getline(cin, title);
        cout << " Enter category (optional): ";
        getline(cin, category);
        tasks.push_back(Task(title, category));
        cout << " Task added successfully!\n";
    }

    void markCompleted() {
        viewTasks(false);
        int index;
        cout << "\n Enter the number of the task to mark as completed: ";
        cin >> index;
        if (index > 0 && index <= tasks.size() && !tasks[index - 1].completed) {
            tasks[index - 1].completed = true;
            cout << " Task marked as completed!\n";
        }
        else {
            cout << " Invalid task number or task already completed.\n";
        }
    }

    void viewTasks(bool completedStatus) {
        cout << "\n==============================\n";
        cout << (completedStatus ? "    Completed Tasks\n" : "    Pending Tasks\n");
        cout << "==============================\n";
        bool found = false;
        for (int i = 0; i < tasks.size(); ++i) {
            if (tasks[i].completed == completedStatus) {
                tasks[i].display(i);
                found = true;
            }
        }
        if (!found)
            cout << " No tasks to display.\n";
    }

    void saveToFile() {
        ofstream file(filename);
        for (const auto& task : tasks) {
            file << task.toFileFormat() << endl;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                tasks.push_back(Task::fromFileFormat(line));
        }
        file.close();
    }

    void menu() {
        int choice;
        do {
            cout << "\n========================================\n";
            cout << "    TASK MANAGER - MAIN MENU\n";
            cout << "========================================\n";
            cout << "1.  Add New Task\n";
            cout << "2.  Mark Task as Completed\n";
            cout << "3.  View Pending Tasks\n";
            cout << "4.  View Completed Tasks\n";
            cout << "5.  Save and Exit\n";
            cout << "----------------------------------------\n";
            cout << " Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: addTask();
                break;
            case 2: markCompleted(); 
                break;
            case 3: viewTasks(false); 
                break;
            case 4: viewTasks(true); 
                break;
            case 5: saveToFile(); cout << "\n Tasks saved. Goodbye!\n"; 
                break;
            default: cout << " Invalid option! Try again.\n";
            }

        } while (choice != 5);
    }
};

int main() {
    TaskManager manager;
    manager.menu();
    return 0;
}
