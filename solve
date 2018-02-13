#include <iostream>

struct Node {
    int value;
    Node* next;

    Node()
            : value(0)
            , next(nullptr)
    { }

    explicit Node(int value)
            : value(value)
            , next(nullptr)
    { }
};

class ForwardList {
public:
    ForwardList()
            : _begin(nullptr)
    { }

    ~ForwardList() {
        Node* removableNode = _begin;
        while (removableNode) {
            Node* nextNode = removableNode->next;
            delete removableNode;
            removableNode = nextNode;
        }
    }

    void assign(Node* other) {
        _begin = other;
    }

    Node* begin() const {
        return _begin;
    }

    void push_front(int x) {
        Node* newNode = new Node(x);
        if (_begin) {
            newNode->next = _begin;
        }
        _begin = newNode;
    }

    void reverse() {
        Node* current = _begin;
        Node* previous = nullptr;
        while (current) {
            Node* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }
        _begin = previous;
    }

    ForwardList(const ForwardList&) = delete;

    ForwardList& operator=(const ForwardList&) = delete;

    friend std::ostream& operator<<(std::ostream& out, const ForwardList& forwardList);

private:
    Node* _begin;
};

std::ostream& operator<<(std::ostream& out, const ForwardList& forwardList) {
    Node* node = forwardList.begin();
    while (node) {
        out << node->value;
        if (node->next) {
            out << ' ';
        }
        node = node->next;
    }
    return out;
}

Node* Merge(Node* lhs, Node* rhs) {
    Node* newBegin = nullptr;
    Node* newEnd = nullptr;

    while (lhs || rhs) {
        if (!rhs || (lhs && lhs->value < rhs->value)) {
            if (newEnd) {
                newEnd->next = lhs;
            } else {
                newBegin = lhs;
            }
            newEnd = lhs;
            lhs = lhs->next;
        } else {
            if (newEnd) {
                newEnd->next = rhs;
            } else {
                newBegin = rhs;
            }
            newEnd = rhs;
            rhs = rhs->next;
        }
    }

    newEnd->next = nullptr;

    return newBegin;
}

Node* MergeSort(Node* begin, size_t size) {
    if (size == 1) {
        begin->next = nullptr;
        return begin;
    }

    Node* firstBegin = begin;
    Node* secondBegin = begin;

    for (int i = 0; i != size / 2; ++i) {
        secondBegin = secondBegin->next;
    }

    firstBegin = MergeSort(firstBegin, size / 2);
    secondBegin = MergeSort(secondBegin, size - size / 2);

    return Merge(firstBegin, secondBegin);
}

void Sort(ForwardList& forwardList) {
    Node* begin = forwardList.begin();
    size_t length = 0;
    while (begin) {
        ++length;
        begin = begin->next;
    }
    forwardList.assign(MergeSort(forwardList.begin(), length));
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int elementsNumber;
    std::cin >> elementsNumber;

    ForwardList forwardList;

    for (int i = 0; i != elementsNumber; ++i) {
        int x;
        std::cin >> x;
        forwardList.push_front(x);
    }

    Sort(forwardList);

    std::cout << forwardList << '\n';

    return 0;
}
