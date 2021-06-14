#ifndef TIRETREE_H
#define TIRETREE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
static std::unordered_map<char,int> base32index={

    {'0',0},{'1',1},{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},
    {'8',8},{'9',9},{'b',10},{'c',11},{'d',12},{'e',13},{'f',14},{'g',15},
    {'h',16},{'j',17},{'k',18},{'m',19},{'n',20},{'p',21},{'q',22},{'r',23},
    {'s',24},{'t',25},{'u',26},{'v',27},{'w',28},{'x',29},{'y',30},{'z',31},
};
template<typename T>
class TireeTreeNode
{
public:


    TireeTreeNode()  :  leaf(false)  { 
       for(int i=0;i<32;i++)
       {
         _childs[i]=nullptr;
       }
    }

    ~TireeTreeNode()  {
        for(int i=0;i<32;i++)
        {
            if(_childs[i])
            {
                delete _childs[i] ;
                _childs[i]=nullptr;
            }

        }

    }

    TireeTreeNode*  insert(char c) {


        if  (base32index.find(c)!=base32index.end()) {
            int index=base32index[c];
            if(!_childs[index])
                _childs[index] = new TireeTreeNode();
            return _childs[index];
        }
        else
        {
            return nullptr;
        }

    }

    TireeTreeNode* get(char c) {

        if  (base32index.find(c)!=base32index.end()) {

            return _childs[base32index[c]] ;
        }
        return nullptr;
    }
     TireeTreeNode**getchild(){

        return _childs ;
    }

    bool isLeaf() const { return leaf; }
    void setLeaf() { leaf = true; }

    void addValue(const T& v)  { value.push_back(v);}
    const std::vector<T>& getValue() const { return value; }


private:
    //std::unordered_map<char,TireeTreeNode*> _childs;
    TireeTreeNode* _childs[32];
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
            if(!node)
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
                    TireeTreeNode<T> ** _childs=node->getchild();
                    for(int i=0;i<32;i++)  {

                        if(_childs[i])
                        {
                           nodes.push(_childs[i]);
                        }

                    }

                }
            }
        }
    }



        private:
        TireeTreeNode<T> root;
    };



#endif // TIRETREE_H
