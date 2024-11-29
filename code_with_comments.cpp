#include <iostream>
#include <string>
using namespace std;

struct Student {
    int id;
    string name;
    int course;
    string enrollment_date;
    string contact_info;
};

struct AVLNode {
    Student data;
    int height;
    AVLNode* left;
    AVLNode* right;
};

// Функции для работы с АВЛ-деревом

int height(AVLNode* node) {
    return node ? node->height : 0;
}

int balanceFactor(AVLNode* node) {
    return height(node->right) - height(node->left);
}

void updateHeight(AVLNode* node) {
    node->height = (height(node->left) > height(node->right) ? height(node->left) : height(node->right)) + 1;
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    y->left = x->right;
    x->right = y;
    updateHeight(y);
    updateHeight(x);
    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    x->right = y->left;
    y->left = x;
    updateHeight(x);
    updateHeight(y);
    return y;
}

AVLNode* balance(AVLNode* node) {
    updateHeight(node);
    if (balanceFactor(node) == 2) {
        if (balanceFactor(node->right) < 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    if (balanceFactor(node) == -2) {
        if (balanceFactor(node->left) > 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    return node;
}

AVLNode* insert(AVLNode* node, Student student) {
    if (!node) {
        node = new AVLNode{ student, 1, nullptr, nullptr };
        return node;
    }
    if (student.id < node->data.id)
        node->left = insert(node->left, student);
    else
        node->right = insert(node->right, student);
    return balance(node);
}

AVLNode* findMin(AVLNode* node) {
    return node->left ? findMin(node->left) : node;
}

AVLNode* removeMin(AVLNode* node) {
    if (!node->left) return node->right;
    node->left = removeMin(node->left);
    return balance(node);
}

AVLNode* remove(AVLNode* node, int id) {
    if (!node) return nullptr;
    if (id < node->data.id)
        node->left = remove(node->left, id);
    else if (id > node->data.id)
        node->right = remove(node->right, id);
    else {
        AVLNode* left = node->left;
        AVLNode* right = node->right;
        delete node;
        if (!right) return left;
        AVLNode* min = findMin(right);
        min->right = removeMin(right);
        min->left = left;
        return balance(min);
    }
    return balance(node);
}

void clearTree(AVLNode* node) {
    if (node) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

AVLNode* search(AVLNode* node, int id) {
    if (!node || node->data.id == id) return node;
    if (id < node->data.id)
        return search(node->left, id);
    else
        return search(node->right, id);
}

void displayTree(AVLNode* node, int level = 0) {
    if (node) {
        displayTree(node->right, level + 1);
        for (int i = 0; i < level; ++i) cout << "   ";
        cout << node->data.id << " " << node->data.name << "\n";
        displayTree(node->left, level + 1);
    }
}

// Функции меню

void addStudent(AVLNode*& root) {
    Student student;
    cout << "Введите номер студенческого билета: ";
    cin >> student.id;
    cin.ignore();
    cout << "Введите ФИО студента: ";
    getline(cin, student.name);
    cout << "Введите курс: ";
    cin >> student.course;
    cin.ignore();
    cout << "Введите дату зачисления: ";
    getline(cin, student.enrollment_date);
    cout << "Введите контактную информацию: ";
    getline(cin, student.contact_info);
    root = insert(root, student);
    cout << "Студент добавлен.\n";
}

void searchStudent(AVLNode* root) {
    int id;
    cout << "Введите номер студенческого билета для поиска: ";
    cin >> id;
    AVLNode* node = search(root, id);
    if (node) {
        cout << "Найден студент:\n";
        cout << "ФИО: " << node->data.name << "\n";
        cout << "Курс: " << node->data.course << "\n";
        cout << "Дата зачисления: " << node->data.enrollment_date << "\n";
        cout << "Контактная информация: " << node->data.contact_info << "\n";
    }
    else {
        cout << "Студент с таким номером не найден.\n";
    }
}

void deleteStudent(AVLNode*& root) {
    int id;
    cout << "Введите номер студенческого билета для удаления: ";
    cin >> id;
    root = remove(root, id);
    cout << "Студент удален.\n";
}

void displayAllStudents(AVLNode* root) {
    cout << "Список студентов:\n";
    displayTree(root);
}

int main() {
    system("chcp 1251>NULL");
    AVLNode* root = nullptr;
    int choice;
    do {
        cout << "Меню:\n";
        cout << "1. Добавить студента\n";
        cout << "2. Вывести всех студентов\n";
        cout << "3. Поиск студента\n";
        cout << "4. Удалить студента\n";
        cout << "5. Очистить данные\n";
        cout << "6. Выйти\n";
        cout << "Выберите пункт меню: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1:
            addStudent(root);
            break;
        case 2:
            displayAllStudents(root);
            break;
        case 3:
            searchStudent(root);
            break;
        case 4:
            deleteStudent(root);
            break;
        case 5:
            clearTree(root);
            root = nullptr;
            cout << "Все данные очищены.\n";
            break;
        case 6:
            clearTree(root);
            cout << "Выход из программы.\n";
            break;
        default: cout << "Неверный выбор. Повторите попытку.\n";
        }
    } while (choice != 6);
    return 0;
}
