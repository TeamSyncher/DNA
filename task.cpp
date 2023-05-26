#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <algorithm>
//#define cout fout
//#define cin fin
//#define int long long
//ifstream fin("input.txt");
//ofstream fout("movetofront.out"); 
using namespace std;


const int alpsz = 2;
const char start_char = '0';

class aho_caras
{
private:
    struct node
    {
        node()
        {
            next_loop.resize(alpsz, -1);
            next_node.resize(alpsz, -1);
            suf_lnk = -1;
            leaf = false;
            p = -1;
            pch = -1;
        }
        vector<int> next_loop;
        vector<int> next_node;
        bool leaf;
        int p;
        int pch;
        int suf_lnk;
        vector<int> rev_suf_lnk;
    };


public:
    aho_caras()
    {
        trie.push_back(node());
    };

    vector<node> trie;
    vector<int> used;

    void add_str(string& s)
    {
        int v = 0;
        for (int i = 0; i < s.size(); i++)
        {
            int curc = s[i] - start_char;
            if (trie[v].next_node[curc] == -1)
            {
                trie.push_back(node());
                trie.back().p = v;
                trie.back().pch = curc;
                trie[v].next_node[curc] = trie.size() - 1;
            }
            v = trie[v].next_node[curc];

        }
        trie[v].leaf = true;
    }


    int get_suf_lnk(int v)
    {
        if (trie[v].suf_lnk == -1)
        {
            if (v == 0) trie[v].suf_lnk = -1;
            else
            {
                int ch = trie[v].pch;
                int p_suf = trie[trie[v].p].suf_lnk;
                while (p_suf != -1)
                {

                    if (trie[p_suf].next_node[ch] != -1)
                    {
                        p_suf = trie[p_suf].next_node[ch];
                        break;
                    }
                    else
                    {
                        p_suf = trie[p_suf].suf_lnk;
                    }
                }
                if (p_suf == -1) trie[v].suf_lnk = 0;
                else trie[v].suf_lnk = p_suf;
            }
        }
        return trie[v].suf_lnk;
    }
    int loop(int v, int ccur)
    {
        if (trie[v].next_loop[ccur] == -1)
        {
            if (trie[v].next_node[ccur] != -1) trie[v].next_loop[ccur] = trie[v].next_node[ccur];
            else
            {
                if (v == 0) trie[v].next_loop[ccur] = 0;
                else trie[v].next_loop[ccur] = loop(get_suf_lnk(v), ccur);

            }
        }
        return trie[v].next_loop[ccur];
    }



    void dfs_helper(int s)
    {
        queue<int> q;
        q.push(s);
        used[s] = 1;

        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            get_suf_lnk(v);
            for (int i = 0; i < trie[v].next_node.size(); i++)
            {
                int to = trie[v].next_node[i];
                if (to != -1 && used[to] == 0)
                {
                    used[to] = 1;
                    q.push(to);
                }
            }
        }
    }
};

void made_mad(int v, aho_caras& k, vector<int>& used)
{
    if (used[v] == 0)
    {
        used[v] = -1;
        for (int i = 0; i < k.trie[v].rev_suf_lnk.size(); i++)
        {
            made_mad(k.trie[v].rev_suf_lnk[i], k, used);
        }
    }
}
int flag = 0;
void nie_tak(vector<vector<int>>& g, vector<int>& color, int v, int p = -1)
{
    color[v] = 1;
    if (flag == 1) return;
    for (int i = 0; i < g[v].size(); i++)
    {
        if (flag == 1) return;
        int to = g[v][i];
        if (color[to] == 0)
        {
            nie_tak(g, color, to, v);
            if (flag == 1) return;
        }
        if (color[to] == 1)
        {
            flag = 1;
            cout << "TAK";
            return;
        }
    }
    color[v] = 2;
}

int32_t main()
{
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    vector<vector<int>> g;
    aho_caras k;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string inn;
        cin >> inn;
        k.add_str(inn);
    }

    k.used.resize(k.trie.size());
    k.dfs_helper(0);


    for (int i = 1; i < k.trie.size(); i++)
    {
        k.trie[k.trie[i].suf_lnk].rev_suf_lnk.push_back(i);
        
    }

    vector<int> used(k.trie.size(), 0);
    g.resize(used.size());

    for (int i = 1; i < used.size(); i++)
    {
        if (k.trie[i].leaf == true)
        {
            made_mad(i, k, used);
        }
    }



    for (int i = 0; i < used.size(); i++)
    {
        if (used[i] == 0)
        {
            
            used[i] = 1;
            for (int j = 0; j < alpsz; j++)
            {

                int to = k.loop(i, j);
                if (used[to] != -1)
                {
                    g[i].push_back(to);
                    
                }
            }
            
        }
    }



    vector<int> color(used.size(), 0);
    nie_tak(g, color, 0);
    if (flag == 0)
    {
        cout << "NIE";
    }

}
