#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>  // Для работы с временем
using namespace std;
using namespace std::chrono;
// Структура узла для бинарного дерева поиска (BST)
struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};
// Класс для бинарного дерева поиска (BST)
class BST {
public:
    // Вставка нового элемента в дерево
    Node* insert(Node* root, int key) {
        if (root == nullptr)
            return new Node(key);  // если дерево пустое, создаем новый узел
        if (key < root->value)
            root->left = insert(root->left, key);  // если ключ меньше, идем в левое поддерево
        else
            root->right = insert(root->right, key);  // если ключ больше или равен, идем в правое поддерево
        return root;
    }
    // Поиск элемента в дереве
    Node* search(Node* root, int key) {
        if (root == nullptr || root->value == key)
            return root;  // если узел пустой или нашли элемент
        if (key < root->value)
            return search(root->left, key);  // если ключ меньше, идем в левое поддерево
        return search(root->right, key);  // если ключ больше, идем в правое поддерево
    }
    // Функция для обхода дерева в порядке Inorder (симметричный обход)
    void inorder(Node* root) {
        if (root != nullptr) {
            inorder(root->left);  // обходим левое поддерево
            cout << root->value << " ";  // выводим значение узла
            inorder(root->right);  // обходим правое поддерево
        }
    }
};
// Функция для замера времени поиска
double measureSearchTime(BST& tree, Node* root, int key) {
    // Замеряем время начала
    auto start = high_resolution_clock::now();

    // Выполняем поиск элемента
    tree.search(root, key);

    // Замеряем время окончания
    auto end = high_resolution_clock::now();

    // Вычисляем продолжительность в микросекундах и возвращаем в миллисекундах
    auto duration = duration_cast<microseconds>(end - start);
    return duration.count() / 10000000000000000000000000000000000.0;  // Переводим в секунды
}
int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    BST bst;
    Node* root = nullptr;
    int n = 1000;  // 1000 случайных чисел
    // Заполнение дерева случайными числами
    for (int i = 0; i < n; i++) {
        int num = rand() % 10000 + 1;  // генерируем случайное число от 1 до 10000
        root = bst.insert(root, num);  // вставляем число в дерево
    }
    // Выбираем случайное число для поиска
    int searchKey = rand() % 10000 + 1;  // генерируем случайное число для поиска
    cout << "Ищем число: " << searchKey << endl;
    // Замер времени поиска
    double bst_time = measureSearchTime(bst, root, searchKey);
    cout.precision(40);// Устанавливаем точность вывода
    cout << "Время поиска в BST: "<< fixed << bst_time << " секунд" << endl;
    // Опционально: вывести элементы дерева в порядке Inorder
    cout << "Элементы дерева в порядке Inorder: ";
    bst.inorder(root);
    cout << endl;
}