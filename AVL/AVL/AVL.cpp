#include <iostream>
#include <cstdlib>   // Для генерации случайных чисел
#include <chrono>    // Для замера времени
using namespace std;
using namespace std::chrono;
// Структура узла для AVL дерева
struct Node {
    int value;         // Значение, которое хранится в узле
    Node* left;        // Указатель на левое поддерево
    Node* right;       // Указатель на правое поддерево
    int height;        // Высота узла в дереве

    // Конструктор для создания узла
    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};
// Класс для AVL дерева
class AVLTree {
public:
    // Вставка нового элемента в дерево
    Node* insert(Node* root, int key) {
        // 1. Стандартная вставка узла в дерево
        if (root == nullptr)
            return new Node(key);  // Если дерево пустое, создаем новый узел
        if (key < root->value)
            root->left = insert(root->left, key);  // Вставка в левое поддерево
        else if (key > root->value)
            root->right = insert(root->right, key);  // Вставка в правое поддерево
        else
            return root;  // Если ключ уже существует, не вставляем (уникальные ключи)
        // 2. Обновляем высоту текущего узла
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        // 3. Балансировка дерева
        int balance = getBalance(root);  // Получаем баланс текущего узла
        // Левый поворот
        if (balance > 1 && key < root->left->value)
            return rightRotate(root);
        // Правый поворот
        if (balance < -1 && key > root->right->value)
            return leftRotate(root);
        // Левый правый поворот
        if (balance > 1 && key > root->left->value) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        // Правый левый поворот
        if (balance < -1 && key < root->right->value) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;  // Возвращаем корень дерева
    }
    // Поиск элемента в дереве
    Node* search(Node* root, int key) {
        if (root == nullptr || root->value == key)
            return root;  // Если дерево пустое или нашли элемент
        if (key < root->value)
            return search(root->left, key);  // Ищем в левом поддереве
        return search(root->right, key);  // Ищем в правом поддереве
    }
private:
    // Функции для балансировки дерева
    // Левый поворот
    Node* leftRotate(Node* x) {
        Node* y = x->right;  // Правая часть поворачивается влево
        Node* T2 = y->left;  // Сохраняем левое поддерево правого узла
        // Выполняем поворот
        y->left = x;
        x->right = T2;
        // Обновляем высоты
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        return y;  // Новый корень
    }
    // Правый поворот
    Node* rightRotate(Node* y) {
        Node* x = y->left;  // Левое поддерево поворачивается вправо
        Node* T2 = x->right;  // Сохраняем правое поддерево левого узла
        // Выполняем поворот
        x->right = y;
        y->left = T2;
        // Обновляем высоты
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        return x;  // Новый корень
    }
    // Функция для получения высоты узла
    int getHeight(Node* root) {
        if (root == nullptr)
            return 0;  // Если узел пустой, высота равна 0
        return root->height;
    }
    // Функция для вычисления баланса узла
    int getBalance(Node* root) {
        if (root == nullptr)
            return 0;  // Если узел пустой, баланс равен 0
        return getHeight(root->left) - getHeight(root->right);  // Баланс = высота левого поддерева - высота правого
    }
};
// Функция для замера времени поиска
double measureSearchTime(AVLTree& tree, Node* root, int key) {
    auto start = high_resolution_clock::now();  // Время начала
    tree.search(root, key);  // Поиск элемента
    auto end = high_resolution_clock::now();  // Время окончания
    auto duration = duration_cast<microseconds>(end - start);  // Разница во времени
    return duration.count() / 1000000.0;  // Переводим в секунды
}
// Функция для вывода элементов дерева в порядке возрастания (in-order)
void printInOrder(Node* root) {
    if (root != nullptr) {
        printInOrder(root->left);  // Сначала левое поддерево
        cout << root->value << " ";  // Затем текущий узел
        printInOrder(root->right);  // Затем правое поддерево
    }
}
int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    AVLTree tree;
    Node* root = nullptr;
    int n = 1000;  // Количество случайных чисел
    // Заполнение дерева случайными числами
    for (int i = 0; i < n; ++i) {
        int num = rand() % 10000 + 1;  // Генерация случайного числа от 1 до 10000
        root = tree.insert(root, num);  // Вставка числа в дерево
    }
    // Выводим все элементы дерева
    cout << "Все числа, добавленные в дерево:" << endl;
    printInOrder(root);
    cout << endl;
    // Выбираем случайное число для поиска
    int searchKey = rand() % 10000 + 1;
    cout << "Ищем число: " << searchKey << endl;
    // Замер времени поиска
    double avl_time = measureSearchTime(tree, root, searchKey);
    // Выводим время поиска с точностью до миллионных долей секунды
    cout.precision(6);
    cout << "Время поиска в AVL дереве: " << fixed << avl_time << " секунд" << endl;
}