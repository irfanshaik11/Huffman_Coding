//
//  main.cpp
//  jessica_project1_decoder
//
//  Created by Irfan Shaik on 12/2/18.
//  Copyright © 2018 Irfan Shaik. All rights reserved.
//
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <queue>


#include "/Users/irfanshaik/Desktop/stdc++.h"
#define MAX_TREE_HT 256
#define MARKER -1

using namespace std;

map<char, string> codes;

// to store the frequency of character of the input data
map<char, int> freq;

// A Huffman tree node
struct MinHeapNode
{
    char data;             // One of the input characters
    int freq;             // Frequency of the character
    MinHeapNode *left, *right; // Left and right child
    
    MinHeapNode(char data, int freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

// utility function for the priority queue
struct compare
{
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

// utility function to print characters along with
// there huffman value
void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;
    if (root->data != '$')
        cout << root->data << ": " << str << "\n";
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

// utility function to store characters along with
// there huffman value in a hash table, here we
// have C++ STL map
void storeCodes(struct MinHeapNode* root, string str)
{
    if (root==NULL)
        return;
    if (root->data != '$')
        codes[root->data]=str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

// STL priority queue to store heap tree, with respect
// to their heap root node value
priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

// function to build the Huffman tree and store it
// in minHeap
void HuffmanCodes(int size)
{
    struct MinHeapNode *left, *right, *top;
    for (map<char, int>::iterator v=freq.begin(); v!=freq.end(); v++){ // s 6
        minHeap.push(new MinHeapNode(v->first, v->second));
        cout << "v: " << v->first <<" "<< v->second << endl;}
    while (minHeap.size() != 1)
    {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    
    storeCodes(minHeap.top(), "");
}

// utility function to store map each character with its
// frequency in input string
void calcFreq(string str, int n)
{
    for (int i=0; i<str.size(); i++){
        //        cout << i << " ";
        freq[str[i]]++;
        //        cout << freq[str[i]] << endl;
    }
}

// function iterates through the encoded string s
// if s[i]=='1' then move to node->right
// if s[i]=='0' then move to node->left
// if leaf node append the node->data to our output string
string decode_file(struct MinHeapNode* root, string s)
{
    string ans = "";
    struct MinHeapNode* curr = root;
    for (int i=0;i<s.size();i++)
    {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;
        
        // reached leaf node
        if (curr->left==NULL and curr->right==NULL)
        {
            ans += curr->data;
            curr = root;
        }
    }
    // cout<<ans<<endl;
    return ans+'\0';
}



template<typename A> void print_queue(A& pq)
{
    while (!pq.empty())
    {
        cout << pq.top() << endl;
        pq.pop();
    }
}
//serialize

void serialize(MinHeapNode *root, FILE *fp)
{
    if (root == NULL)
    {
        fprintf(fp, "%d ", MARKER);
        return;
    }
    
    fprintf(fp, "%d ", root->freq);
    serialize(root->left, fp);
    serialize(root->right, fp);
}

void deSerialize(MinHeapNode *&root, FILE *fp)
{
    int val;
    if ( !fscanf(fp, "%d ", &val) || val == MARKER)
        return;
    
    root = new MinHeapNode('$', val);
    deSerialize(root->left, fp);
    deSerialize(root->right, fp);
}




int main(int argc, char * argv[]) {
    string user_input_target_file;
    user_input_target_file = argv[1];

    ifstream file (user_input_target_file.c_str());
    
    string sent;
    int x;
    int y;
    
    int _len_str = 0;
    bool tok = true;
    string encodedString = "";
    int cnt1 = 1;
    while(getline(file, sent)){
        if (tok == true){
        
            if (sent.empty() or cnt1 == 1){
                
            }
            else if (sent != "encoded file:"){
                istringstream is(sent);
                
                is >> x;
                is >> y;
//                if (x == '@'){
//                    x =  '\n';
//                    freq['\n'] = y;
//                }
//                else if(x == '`'){
//                    x = ' ';
//                    freq[x] = y;
//                }else{
                char x_char = (char) x;
                freq[x_char]= y;
//                }

// uncomment to debug
//                cout << x << " " << freq[x] << endl;
                _len_str = y + _len_str;
                
            }
            else{
                tok = false;
            }}
        else{
            encodedString = encodedString + sent;
        }
        
        cnt1 = cnt1 + 1;
    }
    
    HuffmanCodes(_len_str);
    
    string decodedString;
    decodedString = decode_file(minHeap.top(), encodedString);
    
    std::ofstream outfile;
    string user_output_file;
    user_output_file = user_input_target_file.substr(0, user_input_target_file.size()-4);
    outfile.open(user_output_file.c_str(),std::ios::app);
    outfile << decodedString;
    outfile.close();
    
    remove(user_input_target_file.c_str());

    
    return 0;
    
    
    
}


//
//// inorder traversal via while loop
//priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> curr = minHeap;
//
//vector<int> freArray;
////    MinHeapNode stack[30];
//vector<MinHeapNode*> s;
//bool done = true;
//MinHeapNode *current = curr.top();
//int count = 0;
//while (done) {
//    //        count++;
//    if (current != NULL) {
//        s.push_back(current);
//        //            s.append(current);
//        current = current->left;
//
//    } else {
//        if (s.size() > 0) {
//            current = s.back();
//            s.pop_back();
//            cout << current->freq << "\n";
//            freArray.push_back(current->freq);
//            count++;
//            current = current -> right;
//
//            } else {
//                done = false;
//            }
//
//            }
//            }
//
//            cout << "\n";
//            for (int cnt = 0; cnt < count; cnt++){
//                cout << freArray[cnt] << endl;
//                };
