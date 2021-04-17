#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm> 
#include <list>
using namespace std;



class huffman_tree {
private:
    //node for the huffman tree
    struct node
    {
        node* leftChild;
        node* rightChild;
        int count;
        string str;
        node()
        {
            leftChild  = nullptr;
            rightChild  = nullptr;
            count = 0;
            str = "";
        }
        //Recursively calls delete on all the non-nullptr children
        ~node()
        {
            if (leftChild != nullptr)
            {
                delete leftChild;
            }
            if (rightChild != nullptr)
            {
                delete rightChild;
            }
        }
    };
    //Prints every node's str and count recursively
    void treePrint(node* root)
    {
        cout << root->str << root->count << " ";
        if (root->leftChild != nullptr)
        {
            treePrint(root->leftChild);
        }
        if (root->rightChild != nullptr)
        {
            treePrint(root->rightChild);
        }
    }
    //returns the huffman code for a character
    //Peforms recursion with each left traversal adding a "0"
    //and every right traversal adding a "1"
    string helper_get_character_code(string chr, node* root,string code) const
    {
        if (root->str == chr)
        {
            return code;
        }
        else if(root->leftChild != nullptr && root->leftChild->str.find(chr) != string::npos)
        {
            return helper_get_character_code(chr, root->leftChild, code + "0");
        }
        else if (root->rightChild != nullptr && root->rightChild->str.find(chr) != string::npos)
        {
            return helper_get_character_code(chr, root->rightChild, code + "1");
        }
        else
        {
            return "";
        }
    }
    huffman_tree::node* root;
    
public:


    /*
    Preconditions: input is a string of characters with ascii values 0-127
    Postconditions: Reads the characters of input and constructs a
    huffman tree based on the character frequencies of the file contents
    */
    ~huffman_tree()
    {
        delete root;
    }
    huffman_tree(const string& input)
    {
        
        //Lamda function that returns a boolean
        //dependent on whether the count of the
        //first node is larger than the second
        auto ascNode = [](node* i, node* j)
        {
            return(i->count < j->count);
        };
        unordered_map<char, int> charFreq;
        //Takes every character in the string input
        //and transfer the counts of each character into the unordered_map
        for (auto iter = input.begin(); iter != input.end(); ++iter)
        {
            charFreq[*iter]++;
        }
        list<huffman_tree::node*> nodes;
        //Takes every item in the unordered_map and create a node with that information
        for (auto iter = charFreq.begin(); iter != charFreq.end(); ++iter)
        {
            node* temp = new node();
            temp->str.push_back(iter->first);
            temp->count = iter->second;
            nodes.push_back(temp);
        }
        //"nodes" is a list which is sorted according the the earlier lamda function
        //From smallest to largest counts
        nodes.sort(ascNode);

        //Taking the 2 smallest node counts
        //(which is always the first 2 items in the list due to the sorting at the end of the loop)
        //Create a parent node and insert the parent to the list plus remove the children nodes from the list
        while (nodes.size() > 1)
        {
            node* first = *(nodes.begin());
            nodes.pop_front();
            node* second = *(nodes.begin());
            nodes.pop_front();
            node* p = new node();
            p->count = first->count + second->count;
            p->str = first->str + second->str;
            p->leftChild = first;
            p->rightChild = second;
            nodes.push_front(p);
            nodes.sort(ascNode);
        }

        //set root to the only value in the list
        root = *(nodes.begin());
        //treePrint(root);
    }


    /*
    Preconditions: input is a character with ascii value between 0-127
    Postconditions: Returns the Huffman code for character if character is in the tree
    and an empty string otherwise.
    */

    string get_character_code(char character) const
    {
        //Special case where the root is the only node
        if (root->leftChild == nullptr && root->rightChild == nullptr)
        {
            return "0";
        }
        string chr = string(1, character);
        return helper_get_character_code(chr, root,"");
    }


    /*
    Preconditions: input is a string of characters with ascii values 0-127
    Postconditions: Returns the Huffman encoding for the contents of file_name
    if file name exists and an empty string otherwise.
    If the file contains letters not present in the huffman_tree,
    return an empty string
    */

    string encode(const string& input) const
    {
        //Special case where the root is the only node
        if (root->leftChild == nullptr && root->rightChild == nullptr)
        {
            string append = "";
            for (auto iter = input.begin(); iter != input.end(); ++iter)
            {
                if((*iter) == root->str[0])
                {
                    append += "0";
                }
                    
            }
            return append;
        }
        string append = "";
        //Use the previous function to change each to the appropriate code
        for (auto iter = input.begin(); iter != input.end(); ++iter)
        {
            append += get_character_code(*iter);
        }
        return append;
    }


    /*
    Preconditions: string_to_decode is a string containing Huffman-encoded text
    Postconditions: Returns the plaintext represented by the string if the string
    is a valid Huffman encoding and an empty string otherwise
    */

    string decode(const string& string_to_decode) const 
    {
        //Special case where the root is the only node
        if (root->leftChild == nullptr && root->rightChild == nullptr)
        {
            string append = "";
            for (int i = 0; i < string_to_decode.size(); ++i)
            {
                if (string_to_decode[i] == '0')
                {
                    append += root->str;
                }
            }
            return append;
        }
        //The string is inputed by reference, but the function 
        //is const(This is not a requirement from const, rather I personally would
        //not expect a const function to destroy an inputed string)
        //Since the function would destroy the string, a copy of the string
        //is used instead
        string copy = string_to_decode;
        //currRoot is the node being considered
        node* currRoot = root;
        string append = "";
        
        //Every round, the first character of string "copy" is used
        //to determine wether to go down the left or right child
        //of the current node. If the new currRoot has a str length
        //of 1, indicating that it is the last node of the path, that str
        //will be appended to string "append". The character in the string 
        //"copy" is then erased.
        while (copy.size() != 0)
        {
            if (*(copy.begin()) == '0')
            {
                currRoot = currRoot->leftChild;
            }
            else
            {
                currRoot = currRoot->rightChild;
            }
            if (currRoot->str.size() == 1)
            {
                append += currRoot->str;
                currRoot = root;
            }
            copy.erase(copy.begin());
        }
        return append; 
    }
};
