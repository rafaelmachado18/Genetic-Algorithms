#include <bits/stdc++.h>

#define pii pair<int, int>
#define nd &&
#define f first
#define s second
#define ll long long
#define nl '\n'
#define pb push_back
#define RED     "\033[31m" 
#define RESET   "\033[0m"  

using namespace std;

vector<ll> prefix; 
int board_size;

ll choose_index(int total) {
    int chosen = (ll)((double)rand() / RAND_MAX * total);
    if (chosen == 0) chosen = 1;

    return chosen;
}

int maxi(int board_size) {
    if(board_size % 2 == 0) return board_size/2 * (board_size-1);
    else return board_size * (board_size-1)/2;
}

void mutation(vector<int>& son, double mutation_rate) {
    if ((double)rand() / RAND_MAX < mutation_rate) {
        int target_column = rand() % board_size;
        int new_row = rand() % board_size;
        son[target_column] = new_row;
    }
}

pair<vector<int>, vector<int>> crossover(const vector<int> &dad, const vector<int> &mom) {

    vector<int> child1 = dad;
    vector<int> child2 = mom;

    int idx_cut = (rand() % board_size-1) + 1; 

    for(int i = idx_cut; i < board_size; i++) {
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
    int vezes = 300;
    cout << "Insert board size: ";
    board_size; cin >> board_size;
    if(board_size <= 3) {
        cout << "There is no way to solve this problem with a board this size." << nl;
        return 0;
    }
    vector<vector<int>> tab(vezes, vector<int>(board_size));
    vector<int> ans;
    bool found = false;

    for(int i = 0; i < vezes; i++) {
        for(int j = 0; j < board_size; j++) {
            tab[i][j] = rand() % board_size; 
        }
    }
    int geracao = 0;
    while(!found) {
        geracao++;
        prefix.assign(vezes, 0); 
        
        for(int i = 0; i < vezes; i++) {
        vector<int> rows(board_size, 0);
        vector<int> diag1(2 * board_size - 1, 0);
        vector<int> diag2(2 * board_size - 1, 0);

        for(int j = 0; j < board_size; j++) {
            rows[tab[i][j]]++;
            diag1[tab[i][j] - j + board_size - 1]++;
            diag2[tab[i][j] + j]++;
        }

        int collisions = 0;
        for(int j = 0; j < 2 * board_size - 1; j++) {
            if(j < board_size) collisions += (rows[j] * (rows[j] - 1)) / 2;
            collisions += (diag1[j] * (diag1[j] - 1)) / 2;
            collisions += (diag2[j] * (diag2[j] - 1)) / 2;
        }

        int score = maxi(board_size) - collisions;

        if(collisions == 0) {
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

    cout << "Solution found on generation [" << geracao << "]:" << nl;
    vector<vector<char>> board(board_size, vector<char>(board_size, empty_symbol));

    for(int col = 0; col < board_size; col++) {
        board[ans[col]][col] = symbol_queen; 
    }

    for(int i = 0; i < board_size; i++) {
        for(int j = 0; j < board_size; j++) {
            if(board[i][j] == symbol_queen) {
                cout << RED << board[i][j] << RESET << " ";
            }else cout << board[i][j] << " ";
        }
        cout << nl;
    }
}
