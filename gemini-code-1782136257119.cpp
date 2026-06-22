#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>

// === 類別繼承功能 ===
class Student {
protected:
    std::string id;
    std::string name;
    std::vector<int> scores; // STL 類別庫應用
public:
    Student(std::string i, std::string n) : id(i), name(n) {}
    virtual ~Student() = default;
    
    void addScore(int score) { scores.push_back(score); }
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    
    virtual double getAverage() const {
        if (scores.empty()) return 0.0;
        double sum = 0;
        for (int s : scores) sum += s;
        return sum / scores.size();
    }
    virtual std::string getRole() const = 0; // 純虛擬函式
};

class UndergradStudent : public Student {
public:
    UndergradStudent(std::string i, std::string n) : Student(i, n) {}
    std::string getRole() const override { return "Undergraduate"; }
};

class GradStudent : public Student {
private:
    std::string advisor;
public:
    GradStudent(std::string i, std::string n, std::string adv) : Student(i, n), advisor(adv) {}
    std::string getRole() const override { return "Graduate (Advisor: " + advisor + ")"; }
};

// === 檔案讀寫與管理系統 ===
class StudentManager {
private:
    std::vector<std::unique_ptr<Student>> students; // STL 容器
public:
    void addStudent(std::unique_ptr<Student> s) {
        students.push_back(std::move(s));
    }

    // 寫檔功能
    void saveToFile(const std::string& filename) {
        std::ofstream outFile(filename);
        for (const auto& s : students) {
            outFile << s->getRole() << "," << s->getId() << "," << s->getName() << "\n";
        }
        std::cout << "系統資料已成功存檔！\n";
    }

    // 讀檔功能
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) return; // 第一次執行若無檔案則跳過
        std::string role, id, name;
        while (std::getline(inFile, role, ',') && std::getline(inFile, id, ',') && std::getline(inFile, name)) {
            if (role.find("Undergraduate") != std::string::npos) {
                addStudent(std::unique_ptr<Student>(new UndergradStudent(id, name)));
            }
        }
    }

    void displayAll() {
        std::cout << "\n--- 學生清單 ---\n";
        for (const auto& s : students) {
            std::cout << "[" << s->getRole() << "] 學號: " << s->getId() << " | 姓名: " << s->getName() << "\n";
        }
    }
};

// === 終端機模擬 UI 介面 ===
int main() {
    StudentManager manager;
    manager.loadFromFile("students.txt"); // 啟動時讀檔
    int choice;

    while (true) {
        std::cout << "\n=================================" << std::endl;
        std::cout << "      學生成績管理系統 UI        " << std::endl;
        std::cout << "=================================" << std::endl;
        std::cout << " 1. 新增大學部學生" << std::endl;
        std::cout << " 2. 新增研究所學生" << std::endl;
        std::cout << " 3. 顯示所有學生資料" << std::endl;
        std::cout << " 4. 儲存資料並退出" << std::endl;
        std::cout << "請輸入選項 (1-4): ";
        
        if (!(std::cin >> choice)) {
            std::cout << "無效輸入，請輸入數字！\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            std::string id, name;
            std::cout << "請輸入學號: "; std::cin >> id;
            std::cout << "請輸入姓名: "; std::cin >> name;
            manager.addStudent(std::unique_ptr<Student>(new UndergradStudent(id, name)));
        } else if (choice == 2) {
            std::string id, name, adv;
            std::cout << "請輸入學號: "; std::cin >> id;
            std::cout << "請輸入姓名: "; std::cin >> name;
            std::cout << "請輸入指導教授: "; std::cin >> adv;
            manager.addStudent(std::unique_ptr<Student>(new GradStudent(id, name, adv)));
        } else if (choice == 3) {
            manager.displayAll();
        } else if (choice == 4) {
            manager.saveToFile("students.txt"); // 關閉時寫檔
            break;
        }
    }
    return 0;
}
