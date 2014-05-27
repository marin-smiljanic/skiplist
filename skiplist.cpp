#include <map>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstdio>

typedef unsigned int uint;

template<typename K, typename V>
class skipList {

    class listNode {
    public:
	K key_;
	V value_;
	std::vector<listNode*> forward_;

	listNode(uint level) {
	    forward_.resize(level + 1);
	}
    };

public:
    skipList(double prob = 0.5) :
    prob_(prob) {
	head_ = new listNode(1);
    }

    V search(const K &key) const {
	listNode *curr = head_;
	uint listLevel = head_->forward_.size() - 1;
	for (uint i = listLevel; i >= 1; --i) {
	    while (curr->forward_[i] != NULL && curr->forward_[i]->key_ < key)
		curr = curr->forward_[i];
	}
	curr = curr->forward_[1];
	if (curr != NULL && curr->key_ == key)
	    return curr->value_;
	return V();
    }

    void insert(const K &key, const V &value) {
	uint listLevel = head_->forward_.size() - 1;
	std::vector<listNode*> update(listLevel + 1);
	listNode *curr = head_;

	for (uint i = listLevel; i >= 1; --i) {
	    while (curr->forward_[i] != NULL && curr->forward_[i]->key_ < key)
		curr = curr->forward_[i];
	    update[i] = curr;
	}

	curr = curr->forward_[1];
	if (curr != NULL && curr->key_ == key) {
	    curr->value_ = value;
	    return;
	}
	
	uint lvl = randomLevel(prob_);
	if (lvl > listLevel) {
	    for (uint i = listLevel + 1; i <= lvl; ++i)
		update.push_back(head_);
	    head_->forward_.resize(lvl + 1, NULL);
	}
	listNode *newNode = new listNode(lvl);
	newNode->key_ = key;
	newNode->value_ = value;
	for (uint i = 1; i <= lvl; ++i) {
	    newNode->forward_[i] = update[i]->forward_[i];
	    update[i]->forward_[i] = newNode;
	}
    }

    void remove(const K &key) {
	uint listLevel = head_->forward_.size() - 1;
	std::vector<listNode*> update(listLevel + 1);
	listNode *curr = head_;

	for (uint i = listLevel; i >= 1; --i) {
	    while (curr->forward_[i] != NULL && curr->forward_[i]->key_ < key)
		curr = curr->forward_[i];
	    update[i] = curr;
	}

	curr = curr->forward_[1];
	if (curr == NULL || curr->key_ != key)
	    return;
	for (uint i = 1; i <= listLevel; ++i) {
	    if (update[i]->forward_[i] != curr)
		break;
	    update[i]->forward_[i] = curr->forward_[i];
	}
	free(curr);
	while (listLevel > 1 && head_->forward_.back() == NULL) {
	    --listLevel;
	    head_->forward_.pop_back();
	}    
    }

    void printList() const {
	listNode *curr = head_->forward_[1];
	while (curr != NULL) {
	    std::cout << curr->key_ << " " << curr->forward_.size()-1 << std::endl;
	    curr = curr->forward_[1];
	}
    }

    std::vector<K> keySet() const {
	std::vector<K> ret;
	listNode *curr = head_->forward_[1];
	while (curr != NULL) {
	    ret.push_back(curr->key_);
	    curr = curr->forward_[1];
	}
	return ret;
    }

private:
    uint randomLevel(double prob) const {
	uint ret = 1;
	while (1.0 * rand() / RAND_MAX < prob)
	    ++ret;
	return ret;
    }

    listNode *head_;
    double prob_;
};

int main(int argc, char **argv) {
    skipList<int, int> lst(0.25);

    std::vector<int> ins;
    for (int i = 0; i < 100; ++i)
    	ins.push_back(rand() % 1000);

    // // for (int i = 0; i < 100; ++i)
    // // 	std::cout << ins[i] << std::endl;

    // std::map<int, int> mp;
    // clock_t start = clock();
    for (std::vector<int>::iterator it = ins.begin(); it != ins.end(); ++it) {
    	lst.insert(*it, 1);
    }

    for (uint i = 0; i < 50; ++i) {
	lst.remove(ins[i]);
    }

    for (uint i = 0; i < 100; ++i)
	std::cout << ins[i] << " " << lst.search(ins[i]) << std::endl;
    
    // Std::cout << (1.0 * clock() - start) / CLOCKS_PER_SEC << std::endl;

    // start = clock();
    // for (std::vector<int>::iterator it = ins.begin(); it != ins.end(); ++it) {
    // 	mp[*it] = it - ins.begin();
    // }
    // std::cout << (1.0 * clock() - start) / CLOCKS_PER_SEC << std::endl;

    std::vector<int> ret = lst.keySet();
    for (int i = 0; i < ret.size(); ++i)
    	std::cout << ret[i] << " ";
    std::cout << std::endl;

    // // for (std::vector<int>::iterator it = ins.begin(); it != ins.end(); ++it)
    // // 	std::cout << lst.search(*it) << std::endl;

    return 0;
}
