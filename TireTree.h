#ifndef TIRETREE_H
#define TIRETREE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
template<typename T>
class TireeTreeNode
{
public:


    TireeTreeNode()  :  leaf(false)  {

    }

    ~TireeTreeNode()  {
        for(auto kv:_childs)  {
            delete kv.second ;
        }
        _childs.clear();
    }

    TireeTreeNode*  insert(char c) {


        if  (_childs.find(c)==_childs.end()) {

            _childs[c] = new TireeTreeNode();
        }
        return _childs[c];
    }

    TireeTreeNode* get(char c) {
        if  (_childs.find(c)!=_childs.end()) {

            return _childs[c] ;
        }
        return nullptr;
    }
    const std::unordered_map<char,TireeTreeNode*>&getchild()const{

        return _childs ;
    }

    bool isLeaf() const { return leaf; }
    void setLeaf() { leaf = true; }

    void addValue(const T& v)  { value.push_back(v);}
    const std::vector<T>& getValue() const { return value; }


private:
    std::unordered_map<char,TireeTreeNode*> _childs;
    bool leaf;
    std::vector<T> value;
};

template<typename T>
class TireTree
{
public:
    bool add(const std::string& word, const T& v) {
        int n = word.length();
        TireeTreeNode<T>* node = &root;
        for(int i =0;  i < n && node; i++) {
            node = node->insert(word[i]);
        }

        if(node) {
            node->setLeaf();
            node->addValue(v);
        }
        return node != 0;
    }

    void  get(const std::string& word ,std::vector<T>& value,bool onlyLeaf) {
        int n = word.length();
        TireeTreeNode<T>* node = &root;
        for (int i =0;  i < n; i++) {
            node = node->get(word[i]);
            if(node)
                break;
        }
        if(onlyLeaf)
        {
            if(node)
            {
                value= node->getValue();
            }

        }else
        {
            if(node)
            {
                std::stack<TireeTreeNode<T>* > nodes;
                nodes.push(node);
                while (!nodes.empty()) {
                    TireeTreeNode<T>*node= nodes.top();
                    nodes.pop();
                    const std::vector<T>& v=node->getValue();
                    value.insert(value.end(),v.begin(),v.end());
                    for(auto kv:node->getchild())  {
                        nodes.push(kv.second);
                    }

                }
            }
        }
    }



        private:
        TireeTreeNode<T> root;
    };



#endif // TIRETREE_H
