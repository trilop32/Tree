#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Структура узла B-дерева
template <typename T>
struct BTreeNode {
    vector<T> keys;         // Вектор ключей
    vector<BTreeNode*> children; // Вектор дочерних узлов
    bool isLeaf;           // Флаг, указывающий, является ли узел листом
    int minDegree;         // Минимальная степень (минимальное количество ключей)

    // Конструктор узла
    BTreeNode(int t, bool leaf) : minDegree(t), isLeaf(leaf) {}
};

// Класс B-дерева
template <typename T>
class BTree {
private:
    BTreeNode<T>* root;   // Корень дерева
public:
    BTree(int t) : root(nullptr), minDegree(t) {}

    void traverse() {
        if (root != nullptr)
            traverse(root);
        cout << endl;
    }

    void insert(T key) {
        if (root == nullptr) {  // Если дерево пустое, создаем корень
            root = new BTreeNode<T>(minDegree, true);
            root->keys.push_back(key); // Добавляем ключ в корень
        }
        else {  // Если дерево не пустое, вставляем ключ в подходящее место
            if (root->keys.size() == 2 * minDegree - 1) { // Если корень заполнен
                auto newRoot = new BTreeNode<T>(minDegree, false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0);
                int i = 0;
                if (newRoot->keys[0] < key)
                    i++;
                insertNonFull(newRoot->children[i], key);
                root = newRoot;
            }
            else {
                insertNonFull(root, key);
            }
        }
    }

private:
    int minDegree;
    void splitChild(BTreeNode<T>* parent, int index) {
        auto child = parent->children[index];

        // Проверяем, достаточно ли ключей у дочернего узла для разделения
        if (child->keys.size() < minDegree) {
            throw std::runtime_error("Недостаточно ключей для разделения узла.");
        }

        auto newChild = new BTreeNode<T>(child->minDegree, child->isLeaf);

        // Переносим ключи из старого узла в новый
        for (int j = 0; j < minDegree - 1; j++) {
            newChild->keys.push_back(child->keys[j + minDegree]);
        }

        // Если узел не является листом, переносим дочерние узлы
        if (!child->isLeaf) {
            for (int j = 0; j < minDegree; j++) {
                newChild->children.push_back(child->children[j + minDegree]);
            }
        }

        // Обрезаем старый узел
        child->keys.resize(minDegree - 1);

        // Вставляем новый дочерний узел в родительский
        parent->children.insert(parent->children.begin() + index + 1, newChild);

        // Вставляем "подъем" ключа в родительский узел
        parent->keys.insert(parent->keys.begin() + index, child->keys[minDegree - 1]);
    }
    void insertNonFull(BTreeNode<T>* node, T key) {
        int i = node->keys.size() - 1;

        if (node->isLeaf) {
            node->keys.push_back(0);
            while (i >= 0 && node->keys[i] > key) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        }
        else {
            while (i >= 0 && node->keys[i] > key)
                i--;

            if (node->children[i + 1]->keys.size() == 2 * minDegree - 1) {
                splitChild(node, i + 1);

                if (node->keys[i + 1] < key)
                    i++;
            }

            insertNonFull(node->children[i + 1], key);
        }
    }

    void traverse(BTreeNode<T>* node) {
        for (size_t i = 0; i < node->keys.size(); ++i) {
            if (!node->isLeaf)
                traverse(node->children[i]);

            cout << node->keys[i] << " ";
        }

        if (!node->isLeaf)
            traverse(node->children[node->keys.size()]);
    }
};

int main() {
    const int numElements = 1000;

    // Инициализируем генератор случайных чисел.
    srand(static_cast<unsigned>(time(0)));

    BTree<int> btree(3);

    // Генерация и вставка случайных элементов в B-дерево.
    for (int i = 0; i < numElements; ++i) {
        int randomValue = rand() % numElements;
        btree.insert(randomValue);
    }

    cout << "Элементы в B-дереве: ";
    btree.traverse();

    return 0;
}
