//IDENTIFIER  = 50EB44D3F029ED934858FFFCEAC3547C68768FC9
//  Header.h
//  project1
//
//  Created by Jinhao Su on 9/9/20.
//  Copyright © 2020 Jinhao Su. All rights reserved.
//

#ifndef dictionary_h
#define dictionary_h
#include <iostream>
#include <string>
using namespace std;
struct word {
    string word;
    bool discovered;
    uint32_t morphed_index;
};
class dictionary {
private:
    friend class letterman;
    vector<word> wordlist;
    size_t num_words;
    bool simple_dictionary=0;
    
    
public:
    size_t b_word_size;
    bool no_len_change = 1;
    
    void detect_dictionary () {
        char x;
        cin >> x;
        if (x=='S') {
            simple_dictionary = 1;
        }
        cin >> num_words;
        string junk;
        getline(cin, junk);
    } // detect
    void process_reversal (string &x, size_t i) {
        x.erase(i, 1);
        if (no_len_change && (x.length() != b_word_size)) {
                return;
        }
        else {
        word w;
        w.word = x;
        w.discovered = 0;
        wordlist.push_back(w);
        reverse(x.begin(), x.end());
        w.word = x;
        wordlist.push_back(w);
        }
    }
    void process_insert(const string &x, size_t i) {
        string front;
        string back;
        string ins;
        size_t pos;
        size_t len;
        pos = x.find_first_of(']');
        front = x.substr(0, i);
        back = x.substr(pos+1);
        len = pos-i-1;
        ins = x.substr(i+1, len);
        word w;
        w.discovered = 0;
        for (size_t a=0; a<len; ++a) {
            string temp;
            temp = front+ins[a]+back;
            if (no_len_change && (temp.length() != b_word_size)) {
                    return;
            }
            else {
            w.word = temp;
            wordlist.push_back(w);
            }
        } // for
    } // process
    
    void process_swap(string &x, size_t i) {
        x.erase(i, 1);
        if (no_len_change && (x.length() != b_word_size)) {
                return;
        }
        else {
        word w;
        w.word = x;
        w.discovered = 0;
        wordlist.push_back(w);
        char temp = x[i-1];
        x[i-1] = x[i-2];
        x[i-2] = temp;
        w.word = x;
        wordlist.push_back(w);
        }
    } // process_swap
    
    void process_double(string &x, size_t i) {
        x.erase(i, 1);
        if (no_len_change) {
            if (x.length() == b_word_size) {
                word w;
                w.word = x;
                w.discovered = 0;
                wordlist.push_back(w);
            }
            else if (x.length() == b_word_size - 1) {
                word w;
                string temp;
                temp = x[i-1];
                x.insert(i, temp);
                w.word = x;
                wordlist.push_back(w);
            }
            else {
                return;
            }
        }
        else {
        word w;
        w.word = x;
        w.discovered = 0;
        wordlist.push_back(w);
        string temp;
        temp = x[i-1];
        x.insert(i, temp);
        w.word = x;
        wordlist.push_back(w);
        }
    }
    
    void central_processor (string &x) {
        size_t found = x.find_first_of("!&?[");
        if (found != std::string::npos) {
            if (x[found]=='!') {
                process_swap(x, found);
            }
            else if (x[found]=='&') {
                process_reversal(x, found);
            }
            else if (x[found]=='?') {
                process_double(x, found);
            }
            else {
                process_insert(x, found);
            }
        } // if
        else {
            if (no_len_change && (x.length() != b_word_size)) {
                    return;
            }
            else {
            word w;
            w.word = x;
            w.discovered = 0;
            wordlist.push_back(w);
            }
        }
    } // central_proc
    
    void read_dictionary () {
        wordlist.reserve(num_words);
        if (simple_dictionary) {
        while(cin) {
            string temp;
            cin >> temp;
            if (temp.length() == 0) {
                break;
            }
            size_t found = temp.find("//");
            if (found!=std::string::npos) {
                string junk;
                getline (cin, junk);
            }
            else {
                if (no_len_change && temp.length() != b_word_size) {
                        continue;
                }
                else {
                    word w;
                    w.discovered = 0;
                    w.word = temp;
                    wordlist.push_back(w);
                }
            } //else
        } // while
    } // if
        else {
            while(cin) {
            string temp;
                cin >> temp;
                if (temp.length() == 0) {
                    break;
                }
            size_t found = temp.find("//");
            if (found!=std::string::npos) {
                string junk;
                getline(cin, junk);
            }
            else {
                central_processor(temp);
            } //else
        }
    }
    } // read dic
}; // dictionary

#endif /* dictionary_h */
