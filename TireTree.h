#ifndef TIRETREE_H
#define TIRETREE_H

#include <unordered_map>
#include <string>
#include <vector>


template<typename T>
class TireTreeNode
{
public:
    T value;
    std::unordered_map<char,TireTreeNode<T>*> childNodes;
    bool isEnd=false;
};
template<typename T>
class TrieTree
{
public:
    ~TrieTree()
    {
        release(_root);
    }
    void insert(const std::string& key,T value)
    {
        _root=insert(key,value,_root,0);
    }
private:
    TireTreeNode<T>* insert(const std::string& key,T value,TireTreeNode<T>* node,int index)
    {
        if(!node)
        {
            node=new TireTreeNode<T>;
        }
        if(key.length()==index)
        {
            node->value=value;
            node->isEnd=true;
            return  node;
        }
        else
        {
            char cur=key[index];
            node->childNodes[cur]=insert(key,value,node->childNodes[cur],++index);
            return node;
        }
    }
public:
    bool get(const std::string& key,T& value)
    {
        TireTreeNode<T>*node=get(key,_root,0);

        if(node&&node->isEnd)
        {
            value=node->value;
            return true;
        }else
        {
            return  false;
        }
    }
private:
     TireTreeNode<T>* get(const std::string& key,TireTreeNode<T>*node,int index)
     {
         if(!node)
         {
             return  nullptr;
         }
         if(key.length()==index)
         {
             return  node;
         }else
         {
             char cur=key[index];
             return get(key,node->childNodes[cur],++index);
         }
     }
public:
     bool remove(const std::string& key)
     {
         remove(key,_root,0);
     }
    bool remove(const std::string& key,TireTreeNode<T>*node,int index)
    {
        if(!node)
        {
            return  false;
        }
        if(key.length()==index&&node->isEnd)
        {
            delete  node;
            return  true;
        }
        else
        {
            char cur=key[index];
           if(remove (key,node->childNodes[cur],++index))
           {
                node->childNodes.erase(cur);
                return false;//only remove the lef node
           }else
           {
               return false;
           }

        }
    }
public:
    void prefix(const std::string& key,std::unordered_map<std::string,T>&values )
    {
        collect("",get(key,_root,0),values);
    }
private:
    void collect(const std::string& key,TireTreeNode<T>*node,std::unordered_map<std::string,T>&values )
    {
        if(!node)
        {
            return ;
        }
        if(node->isEnd)
        {
            values[key]=node->value;
        }
        else
        {
            for(auto &kv:node->childNodes)
            {
                collect(key+kv.first,kv.second,values);
            }
        }

    }
    void release(TireTreeNode<T>*node)
    {
        if(!node)
        {
            return ;
        }
        for(auto &kv:node->childNodes)
        {
            release(kv.second);
        }
        node->childNodes.clear();
        delete node;
        node=nullptr;
    }
public:
    void getAll(std::unordered_map<std::string,T>&values)
    {
          getAll("",_root,values);
    }
private:
    void getAll(const std::string& key,TireTreeNode<T>*node,std::unordered_map<std::string,T>&values)
    {
        if(!node)
        {
            return ;
        }
        if(node->isEnd)
        {
            values[key]=node->value;
        }
        else
        {
            for(auto &kv:node->childNodes)
            {
                getAll(key+kv.first,kv.second,values);
            }
        }
    }
private:
    TireTreeNode<T> *_root=nullptr;
};



#endif // TIRETREE_H
