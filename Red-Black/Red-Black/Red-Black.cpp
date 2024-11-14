#include <iostream>
#include <cstdlib>   // Для генерации случайных чисел
#include <chrono>    // Для замера времени

using namespace std;
using namespace std::chrono;

// Структура узла для красно-черного дерева
struct Node {
    int value;         // Значение узла
    Node* left;        // Левый потомок
    Node* right;       // Правый потомок
    Node* parent;      // Родитель
    bool isRed;        // Цвет узла: красный (true) или черный (false)

    // Конструктор для создания узла
    Node(int val) : value(val), left(nullptr), right(nullptr), parent(nullptr), isRed(true) {}
};

// Класс для красно-черного дерева
class RedBlackTree {
public:
    RedBlackTree() : root(nullptr), TNULL(nullptr) {
        TNULL = new Node(0);  // Лист пустой узел
        TNULL->isRed = false;  // Лист всегда черный
    }

    // Вставка нового элемента в дерево
    void insert(int key) {
        Node* newNode = new Node(key);
        newNode->left = TNULL;
        newNode->right = TNULL;

        // Стандартная вставка узла
        Node* y = nullptr;
        Node* x = root;

        while (x != TNULL) {
            y = x;
            if (newNode->value < x->value)
                x = x->left;
            else
                x = x->right;
        }

        newNode->parent = y;
        if (y == nullptr)
            root = newNode;
        else if (newNode->value < y->value)
            y->left = newNode;
        else
            y->right = newNode;

        // Балансировка дерева после вставки
        insertFix(newNode);
    }

    // Поиск элемента в дереве
    Node* search(Node* node, int key) {
        if (node == TNULL || node->value == key)
            return node;

        if (key < node->value)
            return search(node->left, key);

        return search(node->right, key);
    }

    // Функция для замера времени поиска
    double measureSearchTime(Node* root, int key) {
        auto start = high_resolution_clock::now();
        search(root, key);  // Поиск элемента
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        return duration.count() / 1000000.0;  // Переводим в секунды
    }

    // Функция для вывода дерева в порядке возрастания
    void printInOrder(Node* node) {
        if (node != TNULL) {
            printInOrder(node->left);
            cout << node->value << " ";
            printInOrder(node->right);
        }
    }

    // Публичный метод для получения корня дерева
    Node* getRoot() {
        return root;
    }

private:
    Node* root;
    Node* TNULL;  // Лист пустой узел

    // Функция для балансировки дерева после вставки
    void insertFix(Node* k) {
        Node* u;
        while (k->parent->isRed) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->isRed) {
                    u->isRed = false;
                    k->parent->isRed = false;
                    k->parent->parent->isRed = true;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->isRed = false;
                    k->parent->parent->isRed = true;
                    leftRotate(k->parent->parent);
                }
            }
            else {
                u = k->parent->parent->right;
                if (u->isRed) {
                    u->isRed = false;
                    k->parent->isRed = false;
                    k->parent->parent->isRed = true;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->isRed = false;
                    k->parent->parent->isRed = true;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root)
                break;
        }
        root->isRed = false;  // Корень всегда черный
    }

    // Левый поворот
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TNULL)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // Правый поворот
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != TNULL)
            y->right->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }
};

int main() {
    RedBlackTree tree;
    int n = 1000;  // Количество случайных чисел

    // Заполнение дерева случайными числами
    for (int i = 0; i < n; ++i) {
        int num = rand() % 10000 + 1;  // Генерация случайного числа от 1 до 10000
        tree.insert(num);  // Вставка числа в дерево
    }

    // Выводим все элементы дерева
    cout << "Все числа, добавленные в дерево:" << endl;
    tree.printInOrder(tree.getRoot());  // Используем метод getRoot() для получения корня
    cout << endl;

    // Выбираем случайное число для поиска
    int searchKey = rand() % 10000 + 1;
    cout << "Ищем число: " << searchKey << endl;

    // Замер времени поиска
    double rb_time = tree.measureSearchTime(tree.getRoot(), searchKey);

    // Выводим время поиска с точностью до миллионных долей секунды
    cout.precision(6);
    cout << "Время поиска в красно-черном дереве: " << fixed << rb_time << " секунд" << endl;

    return 0;
}
/*
1. Структура узла `Node`:
   - Узел содержит:
     - `value`: Значение, хранимое в узле.
     - `left`, `right`, `parent`: Указатели на левого, правого потомков и родителя.
     - `isRed`: Флаг, показывающий, является ли узел красным (если `true`) или черным (если `false`).

2. Класс `RedBlackTree`:
   - Метод `insert`: Вставляет новый элемент в дерево и вызывает метод `insertFix` для балансировки.
   - Методы `leftRotate` и `rightRotate`: Повороты, которые используются для балансировки дерева.
   - Метод `insertFix`: Выполняет балансировку дерева после вставки нового узла. Основные операции — это изменение цветов и выполнение поворотов.
   - Метод `search`: Стандартный поиск элемента в бинарном дереве.
   - Метод `measureSearchTime`: Замеряет время поиска с использованием `std::chrono`.
   - Метод `printInOrder`: Выполняет обход дерева в порядке возрастания (in-order).

3. Основная функция:
   - Генерирует 1000 случайных чисел и вставляет их в красно-черное дерево.
   - Выводит все числа в порядке возрастания с помощью метода `printInOrder`.
   - После этого выбирается случайное число для поиска, и замеряется время поиска с использованием `measureSearchTime`.
   */