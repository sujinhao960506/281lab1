//
//  main.cpp
//  project1
//
//  Created by Jinhao Su on 9/8/20.
//  Copyright © 2020 Jinhao Su. All rights reserved.
//
//IDENTIFIER  = 50EB44D3F029ED934858FFFCEAC3547C68768FC9
#include <iostream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "xcode_redirect.hpp"
#include "letterman.h"
#include "dictionary.h"
#include <deque>
using namespace std;


int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    letterman l;
    l.get_options(argc, argv);
    l.dictionary_stuff();
    l.search();
    l.output();
}
