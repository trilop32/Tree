#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Структура для представления узла B-дерева
class BTreeNode {
public:
    vector<int> keys;  // Массив ключей
    vector<BTreeNode*> children;  // Массив указателей на дочерние узлы
    bool isLeaf;  // Флаг, указывающий, является ли узел листом
    int t;  // Порядок дерева (минимальное количество ключей в узле)

    BTreeNode(int t, bool isLeaf);  // Конструктор для создания узла
};

// Конструктор узла
BTreeNode::BTreeNode(int t, bool isLeaf) {
    this->t = t;
    this->isLeaf = isLeaf;
    keys.resize(2 * t - 1);  // Максимальное количество ключей в узле
    children.resize(2 * t);  // Максимальное количество детей
}

// Класс для B-дерева
class BTree {
public:
    BTreeNode* root;  // Корень дерева
    int t;  // Порядок дерева

    BTree(int t);  // Конструктор
    void insert(int key);  // Вставка нового ключа
    void traverse();  // Обход дерева
    void print(BTreeNode* node, int level);  // Печать дерева

private:
    void insertNonFull(BTreeNode* node, int key);  // Вставка в неполный узел
    void splitChild(BTreeNode* parent, int i, BTreeNode* child);  // Разделение узла
    void traverse(BTreeNode* node);  // Рекурсивный обход дерева
};

// Конструктор BTree
BTree::BTree(int t) {
    root = new BTreeNode(t, true);  // Создаем корень как лист
    this->t = t;
}

// Вставка нового ключа в B-дерево
void BTree::insert(int key) {
    if (root->keys.size() == 2 * t - 1) {  // Если корень полон
        BTreeNode* newRoot = new BTreeNode(t, false);  // Создаем новый корень
        newRoot->children[0] = root;  // Старая корневая страница становится дочерним элементом нового корня
        splitChild(newRoot, 0, root);  // Разделяем старый корень
        root = newRoot;  // Новый корень
    }
    insertNonFull(root, key);  // Вставляем в неполный узел
}

// Вставка в неполный узел
void BTree::insertNonFull(BTreeNode* node, int key) {
    int i = node->keys.size() - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        node->keys.insert(node->keys.begin() + i + 1, key);  // Вставляем ключ
    }
    else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;  // Увеличиваем индекс, так как вставка будет после i-го элемента
        if (node->children[i]->keys.size() == 2 * t - 1) {  // Если дочерний узел полон
            splitChild(node, i, node->children[i]);  // Разделяем дочерний узел
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);  // Рекурсивно вставляем в дочерний узел
    }
}

// Разделение дочернего узла
void BTree::splitChild(BTreeNode* parent, int i, BTreeNode* child) {
    BTreeNode* newChild = new BTreeNode(t, child->isLeaf);
    parent->children.insert(parent->children.begin() + i + 1, newChild);
    parent->keys.insert(parent->keys.begin() + i, child->keys[t - 1]);

    newChild->keys.assign(child->keys.begin() + t, child->keys.end());  // Переносим половину ключей в новый узел
    child->keys.resize(t - 1);  // Убираем перемещенные ключи из старого узла

    if (!child->isLeaf) {
        newChild->children.assign(child->children.begin() + t, child->children.end());  // Переносим дочерние узлы
        child->children.resize(t);  // Убираем перенесенные дочерние узлы
    }
}

// Рекурсивный обход дерева
void BTree::traverse(BTreeNode* node) {
    int i;
    for (i = 0; i < node->keys.size(); i++) {
        if (!node->isLeaf) {
            traverse(node->children[i]);
        }
        cout << node->keys[i] << " ";  // Выводим ключ
    }

    if (!node->isLeaf) {
        traverse(node->children[i]);
    }
}

// Обход дерева
void BTree::traverse() {
    traverse(root);  // Запускаем рекурсивный обход с корня
}

// Печать дерева
void BTree::print(BTreeNode* node, int level) {
    if (node == nullptr) return;
    cout << "Level " << level << ": ";
    for (int i = 0; i < node->keys.size(); i++) {
        cout << node->keys[i] << " ";
    }
    cout << endl;

    if (!node->isLeaf) {
        for (int i = 0; i <= node->keys.size(); i++) {
            print(node->children[i], level + 1);
        }
    }
}

int main() {
    srand(time(0));

    BTree tree(3);  // Создаем B-дерево порядка 3

    // Вставка случайных чисел
    for (int i = 0; i < 1000; i++) {
        int num = rand() % 10000 + 1;  // Генерация случайного числа от 1 до 10000
        tree.insert(num);  // Вставка числа в дерево
    }

    // Печать дерева (по уровням)
    cout << "Дерево после вставки 1000 случайных чисел:" << endl;
    tree.print(tree.root, 0);
    cout << endl;

    // Обход дерева и вывод всех элементов
    cout << "Все числа в дереве (обход в порядке возрастания):" << endl;
    tree.traverse();
    cout << endl;

    return 0;
}
/*
### Объяснение кода :

1. * *Конструктор BTreeNode * *:
    -Этот конструктор создает новый узел с определенным порядком \(t \), который указывает минимальное количество ключей в узле.
    - Мы задаем размер массивов для ключей и детей.Количество ключей в узле — это \(2t - 1 \), а количество детей — \(2t \).

    2. * *Класс BTree * *:
-Этот класс представляет B - дерево.Мы используем указатель на корень дерева и порядок дерева \(t \).

3. * *Вставка ключа * *:
-Если корень полон, создается новый корень, и старый корень разделяется.Новый корень будет иметь два дочерних узла.
- Вставка в неполный узел — ключ вставляется в правильное место, если узел является листом.Если это не лист, то мы рекурсивно ищем подходящее место в поддереве.

4. * *Разделение узла * *:
    -Когда дочерний узел полон, его необходимо разделить, и новый ключ перемещается в родительский узел.

    5. * *Обход дерева и печать * *:
    -Мы используем рекурсивный обход дерева и выводим все элементы в порядке возрастания.
    - Функция `print` помогает нам вывести дерево по уровням, чтобы лучше визуализировать структуру.
*/