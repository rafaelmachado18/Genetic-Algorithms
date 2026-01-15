#include <bits/stdc++.h>

#define pii pair<int, int>
#define nd &&
#define f first
#define s second
#define ll long long
#define pb push_back
#define RED     "\033[31m" 
#define RESET   "\033[0m"  

using namespace std;

vector<ll> prefix; 

ll choose_index(int total) {
    int chosen = (ll)((double)rand() / RAND_MAX * total);
    if (chosen == 0) chosen = 1;

    return chosen;
}

void mutation(vector<int>& filho, double taxa_mutacao) {
    if ((double)rand() / RAND_MAX < taxa_mutacao) {
        int coluna_alvo = rand() % 8; 
        int nova_linha = rand() % 8;  
        filho[coluna_alvo] = nova_linha;
    }
}

pair<vector<int>, vector<int>> crossover(const vector<int> &dad, const vector<int> &mom) {

    vector<int> child1 = dad;
    vector<int> child2 = mom;

    int idx_cut = (rand() % 7) + 1; 

    for(int i = idx_cut; i < 8; i++) {
        child1[i] = mom[i];
        child2[i] = dad[i];
    }
    return {child1, child2};
}

int bs(int low, int high, ll target) {
    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(prefix[mid] >= target) {
            high = mid - 1; 
        } else {
            low = mid + 1; 
        }
    }
    return low; 
}

int main() {
    srand(time(NULL));
    int vezes = 1000;
    vector<vector<int>> tab(vezes, vector<int>(8));
    vector<int> ans;
    bool found = false;

    for(int i = 0; i < vezes; i++) {
        for(int j = 0; j < 8; j++) {
            tab[i][j] = rand() % 8; 
        }
    }
    int geracao = 0;
    while(!found) {
        geracao++;
        prefix.assign(vezes, 0); 
        
        for(int i = 0; i < vezes; i++) {
            int score = 0;
            for(int j = 0; j < 8; j++) {
                for(int k = 0; k < 8; k++) {
                    if(k != j && tab[i][j] != tab[i][k] && abs(k - j) != abs(tab[i][k] - tab[i][j])) {
                        score++;
                    }
                }
            }
        
            if(score == 56) {
                ans = tab[i];
                found = true;
                break;
            }
            
            if(i != 0) prefix[i] = (ll)score + prefix[i-1];
            else prefix[i] = score;
        }

        if(found) break;

        ll total_score = prefix[vezes-1];
        vector<vector<int>> tab_next;
        
        for(int i = 0; i < vezes / 2; i++) {

            int idx1 = bs(0, vezes - 1, choose_index(total_score));
            int idx2 = bs(0, vezes - 1, choose_index(total_score));

            pair<vector<int>, vector<int>> pair_child = crossover(tab[idx1], tab[idx2]);

            mutation(pair_child.f, 0.1); 
            mutation(pair_child.s, 0.1);

            tab_next.pb(pair_child.f);
            tab_next.pb(pair_child.s);
        }
        tab = tab_next; 
    }

    char symbol_queen = 'W';
    char empty_symbol = 'o';

    cout << "Solution found on generation [" << geracao << "]:" << endl;
    vector<vector<char>> board(8, vector<char>(8, empty_symbol));

    for(int col = 0; col < 8; col++) {
        board[ans[col]][col] = symbol_queen; 
    }

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(board[i][j] == symbol_queen) {
                cout << RED << board[i][j] << RESET << " ";
            }else cout << board[i][j] << " ";
        }
        cout << endl;
    }
}