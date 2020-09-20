//IDENTIFIER  = 50EB44D3F029ED934858FFFCEAC3547C68768FC9
//
//  Created by Marcus Darden on 1/13/19.
//  Copyright © 2019 Marcus Darden. All rights reserved.
// reading in dictionary, vector variable
// Usage: add the following line to main().
//    xcode_redirect(argc, argv);

#ifndef letterman_h
#define letterman_h
using namespace std;
#include <iostream>
#include <string>
#include <deque>
#include "dictionary.h"
struct morphmode {char mode; size_t index; char letter;};
class letterman {
private:
    friend class dictionary;
    dictionary d;
    deque<uint32_t> deck;  //pass the index
    string b_word;
    string e_word;
    uint32_t b_word_index;
    uint32_t e_word_index;
    bool output_w = 1;
    bool has_solution = 1;
    bool stack_mode;
    bool allow_change = 0;
    bool allow_swap = 0;
    bool allow_length = 0;
    
    void help() {
        cout << "This program takes in the specified letterman magic and" <<
        " determines how he could defeat the spell binder\n";
        cout << "--stack -s:               use stack based scheme\n" ;
        cout << "--queue -q:               use queue based scheme\n" ;
        cout << "--change -c:              Letterman can change letters\n";
        cout << "--swap -p:                Letterman can swap letters\n" ;
        cout << "--length -l:              letterman can change lengths" ;
        cout << "--output (W|M) -o(W|M):   output format\n" ;
        cout << "--begin <word> -b <word>: starting word\n" ;
        cout << "--end <word> -e <word>:   target word\n" ;
        cout << "--help -h:                call the help menu\n" ;
        cout << "--You evil autograder, no help for you!!\n";
    } // help()
    

public:
    void dictionary_stuff() {
        d.detect_dictionary();
        d.read_dictionary();
    }
   
    void get_options (int argc, char* argv[]) {
        int choice;
        int option_index = 0;
        int s_or_q = 0;
        bool b_word_given = 0;
        bool e_word_given = 0;
        bool magic = 0;
        option long_opts[] = {
            {"stack",  no_argument, nullptr, 's'},
            {"queue",  no_argument, nullptr, 'q'},
            {"change", no_argument, nullptr, 'c'},
            {"swap",   no_argument, nullptr, 'p'},
            {"length", no_argument, nullptr, 'l'},
            {"help",   no_argument, nullptr, 'h'},
            {"output", required_argument, nullptr, 'o'},
            {"begin",  required_argument, nullptr, 'b'},
            {"end",    required_argument, nullptr, 'e'},
            {nullptr, 0, nullptr, '\0'},
        };// option
        
        while ((choice = getopt_long(argc, argv, "sqcplho:b:e:", long_opts, &option_index))!=-1) {
            switch(choice) {
                case 'h':
                    help();
                    exit(0);
                case 's':
                    stack_mode = 1;
                    s_or_q += 1;
                    break;
                case 'q':
                    stack_mode = 0;
                    s_or_q += 1;
                    break;
                case 'c':
                    allow_change = 1;
                    magic = 1;
                    break;
                case 'p':
                    allow_swap = 1;
                    magic = 1;
                    break;
                case 'l':
                    allow_length = 1;
                    d.no_len_change = 0;
                    magic = 1;
                    break;
                case 'o':
                    if (*optarg == 'M') {
                        output_w = 0;
                    }
                    if (*optarg != 'M' && *optarg != 'W') {
                        cerr << "Error, output mode invalid" << endl;;
                        exit(1);
                    }
                    break;
                case 'b':
                    b_word = optarg;
                    d.b_word_size = b_word.length();
                    b_word_given = 1;
                    break;
                case 'e':
                    e_word = optarg;
                    e_word_given = 1;
                    break;
            } // switch
        } // while
        if (s_or_q != 1) {
            cerr << "Error, no scheme selected or duplicate scheme" << endl;
            exit(1);
        }
        if (b_word_given != 1) {
        cerr << "Error, no begin word given" << endl;
            exit(1);
        }
        if (e_word_given != 1) {
            cerr << "Error, no end word given" << endl;
                exit(1);
        }
        if (magic != 1) {
            cerr << "Error, no tricks for letterman" << endl;
            exit(1);
        }
        if ((b_word.length() != e_word.length()) && allow_length == 0) {
            cerr << "Error, cant morph without length change" << endl;
            exit(1);
        }
    } // get_options
    bool can_change(const string &a, const string &b) {
        if (a.length()!=b.length()) {
            return 0;
        }
        int diff_count=0;
        for (size_t i=0; i<a.length();++i) {
            if (a[i]!=b[i]) {
                if (diff_count ==1) {
                    return 0;
                }
                diff_count+=1;
            }
        }
        return 1;
    }
    
    bool can_len_change(const string &a, const string &b) {
        int bad_count = 0;
        if ((a.length() - b.length()) == 1) {
            size_t x = 0;
            size_t y = 0;
            while (x < a.length() && y < b.length()) {
                if (a[x]!=b[y]) {
                    if (bad_count == 1) {
                        return 0;
                    }
                    bad_count +=1;
                    x++;
                }
                else {
                    x++;
                    y++;
                }
            }
            return 1;
        }
        else if ((b.length() - a.length())==1) {
            size_t x = 0;
            size_t y = 0;
            while (x < b.length() && y < a.length()) {
                if (b[x]!=a[y]) {
                    if (bad_count == 1) {
                        return 0;
                    }
                    bad_count +=1;
                    x++;
                }
                else {
                    x++;
                    y++;
                }
            }
            return 1;
        } // else if
        else {
            return 0;
        }
    }
    
    bool can_swap(const string &a, const string &b) {
        if (a.length()!=b.length()) {
            return 0;
        }
        size_t diff_index = 0;
        int diff_count=0;
        for (size_t i=0; i<a.length();++i) {
            if (a[i]!=b[i]) {
                diff_count+=1;
                diff_index=i;
            }
        }
        if (diff_count != 2) {
            return 0;
        }
        else {
            return (a[diff_index]==b[diff_index-1] && a[diff_index-1]==b[diff_index]);
        }
    }
    
    void no_results() {
        int count = 0;
        for (size_t i=0; i<d.wordlist.size(); ++i) {
            if (d.wordlist[i].discovered == 1) {
                count +=1;
            }
        }
        cout << "No solution, " << count << " words discovered.\n";
    }
    
    void detect_morph(const string &a, const string &b, morphmode &m) { //b is changed from a
        size_t &index = m.index;
        char &letter = m.letter;
        char &mode = m.mode;
        if (can_change (a, b)) {
            mode = 'c';
            for (size_t i=0; i<a.length(); ++i) {
                if (a[i]!=b[i]) {
                    index = i;
                    letter = b[i];
                    break;
                }
            }
        }
        else if (can_swap (a, b)) {
            mode = 's';
            for (size_t i=0; i<a.length(); ++i) {
                if (a[i]!=b[i]) {
                    index = i;
                    letter = 0;
                    break;
                }
            }
        }
        else {
            if (a.length() > b.length()) {
                mode = 'd';
                letter = 0;
                bool diff_at_end = 1;
                for (size_t i=0; i<b.length(); ++i) {
                    if (a[i]!=b[i]) {
                        index = i;
                        diff_at_end = 0;
                        break;
                    }
                }
                if (diff_at_end) {
                    index = b.length();
                }
            }
            else {
                mode = 'i';
                bool diff_at_end = 1;
                for (size_t i=0; i<a.length(); ++i) {
                    if (a[i]!=b[i]) {
                        index = i;
                        letter = b[i];
                        diff_at_end = 0;
                        break;
                    }
                }
                if (diff_at_end) {
                    index = a.length();
                    letter = b[a.length()];
                }
            }
        }
    }
    void output() {
        if (has_solution == 0) {
            no_results();
            return;
        }
        vector<string> output_list;
        word temp;
        word b;
        b.word = b_word;
        temp = d.wordlist[e_word_index];
        while (temp.morphed_index != b_word_index) {
            output_list.push_back(temp.word);
            temp = d.wordlist[temp.morphed_index];
        }
        output_list.push_back(temp.word);
        output_list.push_back(b.word);
        size_t size = output_list.size();
        cout << "Words in morph: " << size << "\n";
        if (output_w) {
            for (size_t i=0; i<size; ++i) {
                cout << output_list[size-i-1] << "\n";
            } // for
        } // if
        else {
            morphmode m{0, 0, 0};
            cout << b_word << "\n";
            for (size_t i=0; i<(size-1); ++i) {
                detect_morph(output_list[size-i-1], output_list[size-i-2], m);
                if (m.mode == 'c' || m.mode == 'i')
                cout << m.mode <<"," << m.index << "," << m.letter << "\n";
                else {
                    cout << m.mode <<"," << m.index << "\n";
                }
            }
        }
    } //output
    
    void search () {
        word current_word;
        current_word.word = b_word;
        bool bword_in_dict=0;
        bool eword_in_dict=0;
        for (uint32_t i=0; i<d.wordlist.size(); ++i) {
            if (d.wordlist[i].word == current_word.word) {
                d.wordlist[i].discovered = 1;
                b_word_index = i;
                bword_in_dict = 1;
            }
            if (d.wordlist[i].word == e_word) {
                e_word_index = i;
                eword_in_dict = 1;
            }
        }
        if (!(bword_in_dict && eword_in_dict)) {
            cerr << "Error, either bword or eword is not in dictionary, or both" << endl;
            exit(1);
        }
        uint32_t this_index;
        this_index = b_word_index;
        bool found = 0;
        while(!found) {
            for (uint32_t i=0; i<d.wordlist.size(); ++i) {
                word & temp = d.wordlist[i];
                if (temp.discovered == 1) {
                    continue;
                }
                if (found == 1) {
                    break;
                }
                if (allow_change) {
                    if (can_change(current_word.word, temp.word)) {
                            temp.discovered = 1;
                            temp.morphed_index = this_index;
                            deck.push_back(i);
                            if (temp.word == e_word) {
                                found = 1;
                            }
                        continue;
                    }
                    
                }
                if (allow_swap) {
                    if (can_swap(current_word.word, temp.word)) {
                            temp.discovered = 1;
                            temp.morphed_index = this_index;
                            deck.push_back(i);
                            if (temp.word == e_word) {
                                found = 1;
                            }
                        continue;
                        }
                    }
                if (allow_length) {
                    if (can_len_change(current_word.word, temp.word)) {
                            temp.discovered = 1;
                            temp.morphed_index = this_index;
                            deck.push_back(i);
                            if (temp.word == e_word) {
                                found = 1;
                            }
                        continue;
                    }
                    
                }
            } // for
            if (found == 1) {
                break;
            }
            
            if (deck.empty()) {
                has_solution = 0;
                break;
            }
            if (stack_mode) {
                this_index = deck.back();
               deck.pop_back();
                current_word = d.wordlist[this_index];
            }
            else {
                this_index = deck.front();
                deck.pop_front();
                current_word = d.wordlist[this_index];
            }
        } // while
    } // search
}; // class


#endif /* letterman_h */
