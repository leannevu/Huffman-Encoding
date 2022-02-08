#include "Encoder.h"
#include <unordered_map>
#include <iostream>
#define EMPTY_STRING ""
#define INT_MIN 0

shared_ptr<Node> Encoder::setNode(char ch, int freq, shared_ptr<Node> left, shared_ptr<Node> right) {
    shared_ptr<Node> node = make_shared<Node>();
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

Encoder::Encoder(shared_ptr<vector<int>> data) {
    // count the frequency of each character and map the pairs
    vector<shared_ptr<Node>> nodes;

    unordered_map<char, int> pair_freq;
    for (int i = 0; i < 26; i++) {//each letter
        pair_freq[i + 97] = data->at(i);
    }
    pair_freq[123] = INT_MIN;

        //add leaf nodes to vector
        for (auto pair : pair_freq) {
            nodes.push_back(setNode(pair.first, pair.second, nullptr, nullptr));
        }

    // link nodes to one another based on frequency level
    while (nodes.size() != 1) {
        shared_ptr<Node> temp_a = make_shared<Node>();
        temp_a = nodes.at(MinFrequency(nodes));
        nodes.erase(nodes.begin() + (MinFrequency(nodes)));

        shared_ptr<Node> temp_b = make_shared<Node>();
        temp_b = nodes.at(MinFrequency(nodes));
        nodes.erase(nodes.begin() + MinFrequency(nodes));

        int sum = temp_a->freq + temp_b->freq;
        nodes.push_back(setNode('\0', sum, temp_a, temp_b));
    }
    root = nodes[0];

}

int Encoder::MinFrequency(vector<shared_ptr<Node>> nodes) {
    int curr_freq = nodes[0]->freq;
    int num = 0;
    for (int i = 1; i < nodes.size(); i++) {
        if (nodes[i]->freq < curr_freq) { 
            curr_freq = nodes[i]->freq; //replaces current frequency with lower frequency alphabetically
            num = i;
        }
    }
    return num;
}


shared_ptr<vector<char>> Encoder::Encode(string data) {
    //find path for each character in string and THROW ERROR if invalid character
    string paths = "";
    for (int i = 0; i < data.length(); i++) { 
        char ch = data[i];  //traverse for each ch here
        if (!(ch >= 97 && ch <= 122))
            throw "ERROR";
        //check for error
        //try {
        //    if (!(ch >= 97 && ch <= 122))
        //    {
        //        throw ch;
        //        cout << "Throw not executed";
        //    }
        //}
        //catch (char ch) {
        //    //cout << "Exception Caught. Letter " << ch << " is not encoded." << "\n";
        //}

        string curr_path_ = EncodeHelper(root, ch, EMPTY_STRING); //store path for given ch to curr_path_
        paths += curr_path_;
    }

    //add delimiter to path
    paths += EncodeHelper(root, 123, EMPTY_STRING);

    //convert string to char, pack 8 bits, and add into vector<char>
    vector<char> encode_result_;
        while (paths.length() > 0) {
            char temp_c_ = 00000000;
            if (paths.length() < 8) {
                for (int index = 0; index < paths.length(); index++) { //int index = 0; index < paths.length(); index++
                    char temp_num_ = (paths.at(index));
                    if (temp_num_ == 49) {
                        temp_c_ |= 1 << index;
                    }
                    else {
                        temp_c_ |= 0 << index;
                    }
                }
                paths.erase(0, paths.length()); //erase the delimiter; don't over erase
            }
            else {
                for (int index = 0; index < 8; index++) { //int index = 0; index < 8; index++
                    char temp_num_ = (paths.at(index));
                    if (temp_num_ == 49) {
                        temp_c_ |= 1 << index;
                    }
                    else {
                        temp_c_ |= 0 << index;
                    }
                }
                paths.erase(0, 8);
            }
            encode_result_.push_back(temp_c_);
        }

    shared_ptr<vector<char>> myPtr = make_shared<vector<char>>(encode_result_);
    return myPtr; 
}

string Encoder::EncodeHelper(shared_ptr<Node> root, char ch, string str) {

    if (root == nullptr) {
        return "";
    }

    if (root->ch == ch) { //return string only if it's the right character
        return (str != EMPTY_STRING) ? str : "1";
    }

    return EncodeHelper(root->left, ch, str + "0") + EncodeHelper(root->right, ch, str + "1");
}

bool getBit(unsigned char byte, int position) { // position in range 0-7
    return (byte >> position) & 1;
}

string Encoder::Decode(shared_ptr<vector<char>> data) {
    //convert chars of bits to a string
    string paths = "";
    for (int i = 0; i < data->size(); i++) {
        char temp_c_ = data->at(i);

        for (int index = 0; index < 8; index++) { 
            (getBit(temp_c_, index)) ? paths += "1" : paths += "0";
        }
    }
    //decode path recursively
    string result;
    while (EncodeHelper(root, 123, EMPTY_STRING) != paths.substr(0, EncodeHelper(root, 123, EMPTY_STRING).length())) {
        result += DecodeHelper(root, paths, 0);
        paths.erase(0, track_path_);
    }
    return result;
}

string Encoder::DecodeHelper(shared_ptr<Node> root, string str, int track_path) {
    if (root == nullptr) {
        return "";
    }

    //found a leaf node
    if (root->left == nullptr && root->right == nullptr) {
        string str_result;
        track_path_ = track_path;
        str_result.push_back(root->ch);
        return str_result;
        //return DecodeHelper(root, root, str, str_result, 0); //restart decode, but with new path

    }
    else {
        if (str[track_path] == '0') { //moves to node based on path
            return DecodeHelper(root->left, str, track_path + 1);
        }
        else {
            return DecodeHelper(root->right, str, track_path + 1);
        }
    }
    
}
