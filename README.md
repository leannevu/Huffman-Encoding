# Huffman-Encoding

Program takes in 26 frequencies where the growing indices represent the letters in the alphalbelt. The constructor maps each char of lowercase letter to the given frequency and creates nodes. A tree builds based on the minimum frequency of two nodes until there is only one node left- representing the one tree. Encoder function encodes the given string and packs the code into chars (with 8 bits each). The Decoder function takes in a vector of chars and decodes based on the packed bits.

FEATURES:
- a special delimiter code (how Decoder will know when to stop searching the built tree)
- recursive function backtracking the paths it takes to get to a leaf
- recursive function that follows paths to one leaf
