#pragma once
using namespace std;
#include <string>
#include <vector>
#include <memory>

//Huffman tree node
struct Node {
	char ch;
	int freq;
	shared_ptr<Node> left = nullptr;
	shared_ptr<Node> right = nullptr;
};

class Encoder {
public:
	//public constructor passing array of letter frequencies. Creates huffman tree using priority queue to store nodes
	Encoder(shared_ptr<vector<int>> data);

	//finds lowest frequency in list of nodes
	int MinFrequency(vector<shared_ptr<Node>> nodes);

	shared_ptr<Node> setNode(char ch, int freq, shared_ptr<Node> left, shared_ptr<Node> right);

	//Encodes each character as string (that is then packed into chars)
	shared_ptr<vector<char>> Encode(string data);

	//Find letter in tree and backtrack tree recording labels; return the string value 
	string EncodeHelper(shared_ptr<Node> root, char ch, string str);

	//Decode an array of bytes into a string.
	string Decode(shared_ptr<vector<char>> data);

	//Use paths as guidance when going through tree to find letter; deletes found paths and continues until hits delimiter
	string DecodeHelper(shared_ptr<Node> root, string str_result, int track_path);

private:
	shared_ptr<Node> root;
	int track_path_;
};

