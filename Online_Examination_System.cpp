#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class User {
public:
    string username;
    string password;
    User(const string& u, const string& p) : username(u), password(p) {}
};

class Exam {
public:
    string examName;
    map<string, string> questions;
    Exam(const string& name, const map<string, string>& qs) : examName(name), questions(qs) {}
};

class OnlineExamSystem {
private:
    vector<User> users;
    vector<Exam> exams;
    map<string, vector<string>> results; // Username -> Results
    map<string, bool> isAdmin;

public:
    OnlineExamSystem() {
        // Add some users
        users.push_back(User("admin", "admin"));
        isAdmin["admin"] = true; // Make admin user an admin

        // Add some exams with questions
        map<string, string> gkQuestions = {
            {"What is the capital of France?", "Paris"},
            {"What is 2 + 2?", "4"},
            {"What is the largest mammal?", "Blue Whale"}
        };
        exams.push_back(Exam("General Knowledge", gkQuestions));

        map<string, string> mathQuestions = {
            {"What is the square root of 16?", "4"},
            {"What is 10/2?", "5"},
            {"What is 7 * 6?", "42"}
        }; exams.push_back(Exam("Mathematics", mathQuestions));

        map<string, string> scienceQuestions = {
            {"What planet is known as the Red Planet?", "Mars"},
            {"What is the chemical symbol for water?", "H2O"},
            {"What gas do plants absorb from the atmosphere?", "Carbon Dioxide"}
        };
        exams.push_back(Exam("Science", scienceQuestions));

        map<string, string> historyQuestions = {
            {"Who was the first President of the United States?", "George Washington"},
            {"In which year did World War I begin?", "1914"},
            {"What ancient civilization built the pyramids?", "Egyptians"}
        };
        exams.push_back(Exam("History", historyQuestions));
    }

    bool authenticateUser(const string& username, const string& password) {
        for (const User& user : users) {
            if (user.username == username && user.password == password)
                return true;
        }
        return false;
    }

    bool isUserAdmin(const string& username) {
        return isAdmin[username];
    }

    void registerUser(const string& username, const string& password) {
        users.push_back(User(username, password));
        isAdmin[username] = false; // Default new users to non-admin
    }

    void changePassword(const string& username, const string& newPassword) {
        for (User& user : users) {
            if (user.username == username) {
                user.password = newPassword;
                cout << "Password changed successfully." << endl;
                return;
            }
        }
        cout << "User not found." << endl; }

    void startExam(const string& username, const Exam& exam) {
        int score = 0;
        string answer;

        cout << "Starting " << exam.examName << " exam..." << endl;
        for (const auto& question : exam.questions) {
            cout << question.first << endl;
            cout << "Your answer: ";
            getline(cin, answer);

            if (answer == question.second) {
                score++;
                cout << "Correct!" << endl;
            } else {
                cout << "Incorrect. The correct answer is: " << question.second << endl;
            }
        }

        string result = exam.examName + ": " + to_string(score) + "/" + to_string(exam.questions.size());
        results[username].push_back(result);
        cout << "Your total score is: " << score << "/" << exam.questions.size() << endl;
    }

    void displayResults(const string& username) {
        cout << "Results for " << username << ":" << endl;
        for (const string& result : results[username]) {
            cout << result << endl;
        }
    }

    void listExams() {
        cout << "Available Exams:" << endl;
        for (const Exam& exam : exams) {
            cout << "- " << exam.examName << endl;
        }
    }

    Exam selectExam(int choice) {
        if (choice >= 1 && choice <= exams.size()) {
            return exams[choice - 1];
        }
        throw invalid_argument("Invalid choice.");
    }
};

int main() {
    OnlineExamSystem examSystem;
    string username, password, examName, newPassword;
    int choice;

    cout << "Welcome to the Online Exam System!" << endl;
    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nChoose an option: ";
        cin >> choice;
        cin.ignore(); // To ignore the newline character left in the buffer

        if (choice == 1) {
            cout << "Enter new username: ";
            getline(cin, username);
            cout << "Enter new password: ";
            getline(cin, password);
            examSystem.registerUser(username, password);
            cout << "User registered successfully." << endl;
        } else if (choice == 2) {
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            if (examSystem.authenticateUser(username, password)) {
                if (examSystem.isUserAdmin(username)) {
                    cout << "Welcome admin!" << endl;
                    cout << "1. Register User\n2. Change Password\n3. Start Exam\n4. Display Results" << endl;
                    cin >> choice;
                    cin.ignore(); // To ignore the newline character left in the buffer
                    switch (choice) {
                        case 1:
                            cout << "Enter new username: ";
                            getline(cin, username);
                            cout << "Enter new password: ";
                            getline(cin, password);
                            examSystem.registerUser(username, password);
                            break;
                        case 2:
                            cout << "Enter username: ";
                            getline(cin, username);
                            cout << "Enter new password: ";
                            getline(cin, newPassword);
                            examSystem.changePassword(username, newPassword);
                            break;
                        case 3:
                            cout << "Enter username: ";
                            getline(cin, username);
                            cout << "Available exams:" << endl;
                            examSystem.listExams();
                            cout << "Enter the number of the exam you want to take: ";
                            cin >> choice; cin.ignore();
                            try {
                                Exam selectedExam = examSystem.selectExam(choice);
                                examSystem.startExam(username, selectedExam);
                                examSystem.displayResults(username);
                            } catch (invalid_argument& e) {
                                cout << e.what() << endl;
                            }
                            break;
                        case 4:
                            cout << "Enter username: ";
                            getline(cin, username);
                            examSystem.displayResults(username);
                            break;
                        default:
                            cout << "Invalid choice." << endl;
                            break;
                    }
                } else {
                    cout << "Authentication successful. Available exams:" << endl;
                    examSystem.listExams();
                    cout << "Enter the number of the exam you want to take: ";
                    cin >> choice; cin.ignore();
                    try {
                        Exam selectedExam = examSystem.selectExam(choice);
                        examSystem.startExam(username, selectedExam);
                        examSystem.displayResults(username);
                    }
                    catch (invalid_argument& e) {
                        cout << e.what() << endl;
                    }
                }
            } else {
                cout << "Authentication failed. Please try again." << endl;
            }
        } else if (choice == 3) {
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
