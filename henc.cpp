#include <iostream>
#include <map>
#include <fstream>
#include <queue>

#include <cstring>


#include "/Users/irfanshaik/Desktop/stdc++.h"
#include <stdio.h>      /* printf */
#include <math.h>       /* floor */
#include <tuple>

#define MAX_TREE_HT 256
#define MARKER -1

using namespace std;

map<char, string> codes;

map<char, int> freq;

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
//         uncomment to debug
//        cout << root->data << ": " << str << "\n";
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
    for (map<char, int>::iterator v=freq.begin(); v!=freq.end(); v++)
        minHeap.push(new MinHeapNode(v->first, v->second));
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
void calcFreq(std::vector<char> str, int n, int size)
{
    for (int i=0; i<size; i++){
//        cout << i << " ";
//        int value = (char)str[i] - '\0';

//        cout << value << "\n";
        freq[str[i]]++;
//        cout << freq[str[i]] << endl; char 100
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
        // uncomment to debug
//        cout << pq.top() << endl;
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


int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;
    
    // Initializing base value to 1, i.e 2^0
    int base = 1;
    
    int temp = num;
    while (temp)
    {
        int last_digit = temp % 10;
        temp = temp/10;
        
        dec_value += last_digit*base;
        
        base = base*2;
    }
    
    return dec_value;
}

static std::vector<char> ReadAllBytes(char const* filename)
{
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    
    std::vector<char>  result(pos);
    
    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);
    
    return result;
}


int main(int argc, char * argv[]) {
    
    string user_input_target_file;
    user_input_target_file = argv[1];
    
//    char *buffer;
//    long size;
    
    ifstream file (user_input_target_file.c_str(), ios::in | ios::binary | ios::ate);
////    ifstream file (user_input_target_file.c_str(),ios::binary);

    int length;
    char *buffer;
    
//    std::ifstream stream;
//    stream.open(user_input_target_file.c_str(), std::ios_base::binary);


//    length = stream.rdbuf()->pubseekoff(0, std::ios_base::end);
//    buffer = new char[length];
//    stream.rdbuf()->pubseekoff(0, std::ios_base::beg);
//    stream.read(buffer, length);
//    stream.close();
//    cout << "buffer " << endl;
//    for(int i =0; i < length; i++){
//        cout << buffer[i] << endl;}
    
//    ifstream infile (user_input_target_file.c_str(),ios::binary);

    //get length of file
//    infile.seekg(0, infile.end);
//    size_t length = infile.tellg();
//    infile.seekg(0, infile.beg);
    // char array
    // byte array
    // vector
    // or arraylist
    
//    size = file.tellg();
//    file.seekg(0,ios::beg);
//    buffer = new char[size];
//    file.read(buffer, size);
//    file.close();
//
//    string str;
//    str = buffer;
    
    static std::vector<char> str_vec = ReadAllBytes(user_input_target_file.c_str());
    
    ifstream file1(user_input_target_file.c_str(), ios::binary|ios::ate);
    int size = file1.tellg();
    file.close();

    string encodedString, decodedString;
    int _len_str;
    _len_str = (int) size;
    
    
    calcFreq(str_vec, _len_str, size);
    HuffmanCodes(_len_str);
    
    for (auto i: str_vec)
        encodedString+=codes[i];
    
//    decodedString = decode_file(minHeap.top(), encodedString);

//    delete[] buffer;
    
    priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> curr = minHeap;
    
    string huf_target_file = user_input_target_file + ".huf";
    remove(huf_target_file.c_str());

    std::ofstream outfile;
    outfile.open(huf_target_file.c_str(),std::ios::app);
    outfile << "Characters and Frequencies:\n";
    for ( const auto &myPair : freq ) {
        int temp_key = myPair.first;
        int value = int(temp_key);
//        if (temp_key == '\n'){
//            outfile << '@' << ' ' << freq[temp_key] << endl;
//        }else if (temp_key == ' '){
//            outfile << '`' << ' ' << freq[temp_key] << endl;
//        }else{
        outfile << value << ' ' << freq[temp_key] << endl;
        
//    }
    };

    
    outfile << "encoded file:\n";
    outfile << encodedString << std::endl;
    outfile.close();
    
    
    remove(user_input_target_file.c_str());
    
    return 0;
}


// every file is just a set of characters
// im reading characters with my encoding function
// im printing these characters
