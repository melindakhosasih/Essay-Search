#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<queue>
#include<iostream>
#define TOTAL 26

using namespace std;

struct TrieNode {
    struct TrieNode* child[TOTAL];
    bool isEndofWord;
};

class Trie {
public:
    Trie(string str){
        root_prefix = createNode();
        root_suffix = createNode();
        title = str;
    }
    TrieNode* child[TOTAL];
    TrieNode* createNode();
    void createPrefix(string str);
    void createSuffix(string str);
    bool search(string key);
    string get_title();
private:
    TrieNode* root_prefix, *root_suffix;
    string title;
};

TrieNode* Trie::createNode() {
    TrieNode *node = new TrieNode;
    node->isEndofWord = false;
    for(int i = 0; i < TOTAL; i++) {
        node->child[i] = NULL;
    }
    return node;
}

void Trie::createPrefix(string str) {
    TrieNode *crawl = root_prefix;
    for(int i = 0; i < str.length(); i++) {
        int index;
        if(str[i] >= 'A' && str[i] <= 'Z') {
            index = str[i] - 'A';
        } else if(str[i] >= 'a' && str[i] <= 'z') {
            index = str[i] - 'a';
        } else {
            continue;
        }
        if(!crawl->child[index]) {
            crawl->child[index] = createNode();
        }
        crawl = crawl->child[index];
    }
    crawl->isEndofWord = true;
}

void Trie::createSuffix(string str) {
    TrieNode *crawl = root_suffix;
    for(int i = str.length()-1; i >= 0; i--) {
        int index;
        if(str[i] >= 'A' && str[i] <= 'Z') {
            index = str[i] - 'A';
        } else if(str[i] >= 'a' && str[i] <= 'z') {
            index = str[i] - 'a';
        } else {
            continue;
        }
        if(!crawl->child[index]) {
            crawl->child[index] = createNode();
        }
        crawl = crawl->child[index];
    }
    crawl->isEndofWord = true;
}

bool Trie::search(string str) {
    TrieNode *crawl;
    if(str[0] == '*') {
        crawl = root_suffix;
        for(int i = str.length()-2; i >= 1; i--) {
            int index;
            if(str[i] >= 'A' && str[i] <= 'Z') {
                index = str[i] - 'A';
            } else if(str[i] >= 'a' && str[i] <= 'z') {
                index = str[i] - 'a';
            } else {
                continue;
            }
            if(!crawl->child[index]) {
                return false;
            }
            crawl = crawl->child[index];
        }
    } else if(str[0] == '"') {
        crawl = root_prefix;
        for(int i = 1; i < str.length()-1; i++) {
            int index;
            if(str[i] >= 'A' && str[i] <= 'Z') {
                index = str[i] - 'A';
            } else if(str[i] >= 'a' && str[i] <= 'z') {
                index = str[i] - 'a';
            } else {
                continue;
            }
            if(!crawl->child[index]) {
                return false;
            }
            crawl = crawl->child[index];
        }
        return crawl->isEndofWord;
    } else {
        crawl = root_prefix;
        for(int i = 0; i < str.length(); i++) {
            int index;
            if(str[i] >= 'A' && str[i] <= 'Z') {
                index = str[i] - 'A';
            } else if(str[i] >= 'a' && str[i] <= 'z') {
                index = str[i] - 'a';
            } else {
                continue;
            }
            if(!crawl->child[index]) {
                return false;
            }
            crawl = crawl->child[index];
        }
    }
    return true;
}

string Trie::get_title() {
    return title;
}

// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);

	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}


int main(int argc, char *argv[]) {
    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Read File & Parser Example
	string file, title_name, tmp;
	fstream fi, output_file;
	vector<string> tmp_string;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
	int DATA_SIZE = 0;
	//int num = DATA_TOTAL_NUM;
	string data_file;
	vector<Trie*> tree;
	while(1) {
        data_file = data_dir + string(to_string(DATA_SIZE)) + string(FILE_EXTENSION);
        //cout << data_file << endl;
        fi.open(data_file, ios::in);
        if(fi.fail()) {
            break;
        }
        // GET TITLENAME
        getline(fi, title_name);

        // GET TITLENAME WORD ARRAY
        tmp_string = split(title_name, " ");

        vector<string> title = word_parse(tmp_string);

        tree.push_back(new Trie(title_name));

        for(auto &word : title){
            tree[DATA_SIZE]->createPrefix(word);
            tree[DATA_SIZE]->createSuffix(word);
        }

        // GET CONTENT LINE BY LINE
        while(getline(fi, tmp)){
            // GET CONTENT WORD VECTOR
            tmp_string = split(tmp, " ");

            // PARSE CONTENT
            vector<string> content = word_parse(tmp_string);

            for(auto &word : content){
                tree[DATA_SIZE]->createPrefix(word);
                tree[DATA_SIZE]->createSuffix(word);
            }
            //......
        }
        // CLOSE FILE
        fi.close();
        DATA_SIZE++;
	}
    DATA_SIZE--;
    fi.open(query, ios::in);
    output_file.open(output, ios::out);
    bool *match = new bool [DATA_SIZE+1];
    int op;
    while(getline(fi, tmp)) {
        for(int i = 0; i <= DATA_SIZE; i++) {
            match[i] = false;
        }
        bool found = false;
        op = 0;
        tmp_string = split(tmp, " ");
        for(auto &str : tmp_string) {
            for(int i = 0; i <= DATA_SIZE; i++) {
                if(op != 0) {
                    if(op == 1) {   // && AND
                        if(str[0] == '*') {
                            if(match[i]) {
                                if(tree[i]->search(str)) {
                                    match[i] = true;
                                } else {
                                    match[i] = false;
                                }
                            }
                        } else {
                            if(match[i]) {
                                if(tree[i]->search(str)) {
                                    match[i] = true;
                                } else {
                                    match[i] = false;
                                }
                            }
                        }
                    } else if(op == 2) {    // || OR
                        if(str[0] == '*') {
                            if(!match[i]) {
                                if(tree[i]->search(str)) {
                                    match[i] = true;
                                }
                            }
                        } else {
                            if(!match[i]) {
                                if(tree[i]->search(str)) {
                                    match[i] = true;
                                }
                            }
                        }
                    }
                    if(i == DATA_SIZE) {
                        op = 0;
                    }
                    continue;
                }
                if(str[0] == '"') {
                    if(tree[i]->search(str)) {
                        match[i] = true;
                    }
                } else if(str[0] == '*') {
                    if(tree[i]->search(str)) {
                        match[i] = true;
                    }
                } else if(str[0] == '+') {
                    op = 1;
                    break;
                } else if(str[0] == '/') {
                    op = 2;
                    break;
                } else {
                    if(tree[i]->search(str)) {
                        match[i] = true;
                    }
                }
            }
        }
        for(int i = 0; i <= DATA_SIZE; i++) {
            if(match[i]) {
                output_file << tree[i]->get_title() << endl;
                found = true;
            }
        }
        if(!found) {
            output_file << "Not Found!\n";
        }
    }
    fi.close();
    output_file.close();
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
