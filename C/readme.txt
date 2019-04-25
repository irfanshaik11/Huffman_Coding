Testing

Our code is intended for use on a text file but can be applied to any file type. The limitation is that the words in the file must be space separated and the codeword must not exceed 50 characters.  

We did thorough testing on a variety of test cases to ensure the quality of my code. we executed my code on a text file with hundreds of words as well as one with only one word. My code will work as long as the number of unique words in the file does not cause the huffman tree to exceed 50 levels as this will cause the code word to exceed 50 levels which exceeds the coded word size limit.

The limit on the file that can be encoded by my program is the number of unique words in the file.


Another testcase we had was running the file with a lot of punctuation. We also ran the code with tabs and spaces as well as paragraphs. The formatting is not always preserved.

There is also an issue where if the code words are too large the buffer will only read 50 chars at a time and the word buffer is reset so that a new word is read instead.

