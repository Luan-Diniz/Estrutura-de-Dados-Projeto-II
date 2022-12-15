#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int ALPHABET_SIZE = 26;


typedef struct {
    int position;
    int length;
    int ocurrences;
    bool error_prefix;
    bool found;

} wordInfo;


// nó trie
struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];
    int position;
    int length;
    bool is_root;
	bool isEndOfWord;
};

// Retorna um novo nó da trie
struct TrieNode *getNode(bool root)
{
	struct TrieNode *pNode = new TrieNode;

	pNode->isEndOfWord = false;
    pNode-> is_root = root;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = nullptr;

	return pNode;
}


void insert(struct TrieNode *root, string key, int pos, int length)
{
	struct TrieNode *pCrawl = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index = key[i] - "a"[0];
		if (!pCrawl->children[index])
			pCrawl->children[index] = getNode(false);

		pCrawl = pCrawl->children[index];
	}

	// mark last node as leaf
	pCrawl->isEndOfWord = true;
    pCrawl->position = pos;
    pCrawl->length = length;
}


int findOcurrences(struct TrieNode *node, int &cont) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            findOcurrences(node->children[i], cont);
        }  

        if (i == ALPHABET_SIZE - 1 and node->isEndOfWord){      
            cont += 1;
        }
        }
    return cont;
}



wordInfo search(struct TrieNode *root, string key) {
	struct TrieNode *pCrawl = root;
    wordInfo info;
    info.error_prefix = false;
    //info.found recebera uma atribuicao mais a frente
    int cont_ocurrences = 0;

    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - "a"[0];
        if  (pCrawl->children[index] != nullptr) {
            pCrawl = pCrawl->children[index];
        } else {
            info.error_prefix = true; //marca que o prefixo nao existe)
        }
    }
    //analisa se a palavra existe
    info.found = pCrawl->isEndOfWord;

    if (info.found) {
        info.position = pCrawl->position; 
        info.length = pCrawl->length;
    } else {
        info.position = 0;
        info.length = 0;
    }

    if (!info.error_prefix) {
        info.ocurrences = findOcurrences(pCrawl, cont_ocurrences);

    } else {
        info.ocurrences = 0;
    }

    return info;
}


//funcao recursiva para deletar a trie
void deleteTrie(struct TrieNode* node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            deleteTrie(node->children[i]);
        } else {
            delete node->children[i];
        }
    }
    if (node->is_root = true) {
        delete node;
    }
}


int main() {
	struct TrieNode *root = getNode(true);

    string filename;
    string word = "";
    string line;
    string input;
    int position = 0;
    int next_word;
    wordInfo information;


    cin >> filename;

    ifstream file;
    file.open(filename);
    
    // Armazena a trie
    if (file.is_open()) {
        while (!file.eof()) {  // Leitura do arquivo dict
            getline(file,line);
            for (int i = 1; i < line.length(); i++) {
                if (line[i] == ']')
                    break;
                word += line[i];
            }
            insert(root, word, position, line.length());
            position = position + line.length() + 1;
            word = "";
        }

        file.close();

    } else {
        throw out_of_range("Não foi possível abrir o arquivo");
    }

    // Procura pelas palavras até chegar em 0
    while (true) {
        cin >> input;
        if (input == "0") {
            break;
        } else {
            information = search(root, input);

            if (information.error_prefix == 1) {
                cout << input << " is not prefix" << endl;
            } else {
                cout << input << " is prefix of " << information.ocurrences << " words" << endl;

                if (information.found) {
                    cout << input << " is at (" << information.position << "," << information.length << ")" << endl;
                }

            }

        }

    }

    deleteTrie(root);
    return 0;
}
	
