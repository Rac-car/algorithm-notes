#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <utility>
#include <cctype>
using namespace std;

// 将文档ID和行ID映射到对应的文本
map<pair<int, int>, string> mp;
// 存储每一行的所有单词（小写）
map<pair<int, int>, set<string>> s;

string normalize_word(const string &str)
{
    string res = "";
    for (char c : str)
    {
        if (isalpha((unsigned char)c))
        {
            res += tolower((unsigned char)c);
        }
    }
    return res;
}

void print_lines_of_doc(int doc_id, const set<int> &lines)
{
    for (set<int>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        cout << mp[make_pair(doc_id, *it)] << endl;
    }
}

void print_full_doc(int doc_id, const map<int, int> &doc_line_cnt)
{
    int cnt = doc_line_cnt.find(doc_id)->second;
    for (int i = 0; i < cnt; i++)
    {
        cout << mp[make_pair(doc_id, i)] << endl;
    }
}

int main()
{
    int n;
    cin >> n;
    string tmp;
    pair<int, int> p;
    getline(cin, tmp); // 跳过第一行的换行符
    for (int i = 0; i < n; i++)
    {              // pair的第一个int为i
        int j = 0; // pair的第二个int为j
        while (getline(cin, tmp) && tmp != "**********")
        {
            p = make_pair(i, j);
            mp[p] = tmp;
            set<string> lower_alpha;
            stringstream ss(tmp);
            while (ss >> tmp)
            {
                string word = "";
                for (char c : tmp)
                {
                    if (isalpha((unsigned char)c))
                    { // char 在 C++ 里不一定是非负的。有些编译器里 char 默认是有符号的，字符一旦超出普通 ASCII 范围，就可能变成负数。
                        word += tolower((unsigned char)c);
                    }
                }
                if (!word.empty())
                {
                    lower_alpha.insert(word);
                }
            }
            s[p] = lower_alpha;
            j++;
        }
    }

    // 统计每篇文档有多少行
    map<int, int> doc_line_cnt;
    for (map<pair<int, int>, string>::iterator it = mp.begin(); it != mp.end(); ++it)
    {
        int doc_id = it->first.first;
        int line_id = it->first.second;
        if (doc_line_cnt[doc_id] < line_id + 1)
        {
            doc_line_cnt[doc_id] = line_id + 1;
        }
    }

    // 倒排索引：单词 -> 文档ID -> 该文档中哪些行出现了这个单词
    map<string, map<int, set<int>>> index_map;
    for (map<pair<int, int>, set<string>>::iterator it = s.begin(); it != s.end(); ++it)
    {
        int doc_id = it->first.first;
        int line_id = it->first.second;
        for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
        {
            index_map[*sit][doc_id].insert(line_id);
        }
    }

    int m;
    cin >> m;
    getline(cin, tmp); // 跳过查询数量后的换行

    for (int q = 0; q < m; q++)
    {
        string query;
        getline(cin, query);

        bool found = false;
        bool first_doc = true;

        if (query.size() >= 4 && query.substr(0, 4) == "NOT ")
        {
            string word = normalize_word(query.substr(4));

            for (int doc_id = 0; doc_id < n; doc_id++)
            {
                bool has_word = false;
                if (index_map.count(word) && index_map[word].count(doc_id))
                {
                    has_word = true;
                }

                if (!has_word)
                {
                    if (!first_doc)
                        cout << "----------" << endl;
                    print_full_doc(doc_id, doc_line_cnt);
                    first_doc = false;
                    found = true;
                }
            }
        }
        else if (query.find(" AND ") != string::npos)
        {
            int pos = query.find(" AND ");
            string word1 = normalize_word(query.substr(0, pos));
            string word2 = normalize_word(query.substr(pos + 5));

            if (index_map.count(word1) && index_map.count(word2))
            {
                for (map<int, set<int>>::iterator it = index_map[word1].begin(); it != index_map[word1].end(); ++it)
                {
                    int doc_id = it->first;
                    if (index_map[word2].count(doc_id))
                    {
                        set<int> lines = index_map[word1][doc_id];
                        lines.insert(index_map[word2][doc_id].begin(), index_map[word2][doc_id].end());

                        if (!first_doc)
                            cout << "----------" << endl;
                        print_lines_of_doc(doc_id, lines);
                        first_doc = false;
                        found = true;
                    }
                }
            }
        }
        else if (query.find(" OR ") != string::npos)
        {
            int pos = query.find(" OR ");
            string word1 = normalize_word(query.substr(0, pos));
            string word2 = normalize_word(query.substr(pos + 4));

            set<int> docs;
            if (index_map.count(word1))
            {
                for (map<int, set<int>>::iterator it = index_map[word1].begin(); it != index_map[word1].end(); ++it)
                {
                    docs.insert(it->first);
                }
            }
            if (index_map.count(word2))
            {
                for (map<int, set<int>>::iterator it = index_map[word2].begin(); it != index_map[word2].end(); ++it)
                {
                    docs.insert(it->first);
                }
            }

            for (set<int>::iterator dit = docs.begin(); dit != docs.end(); ++dit)
            {
                int doc_id = *dit;
                set<int> lines;
                if (index_map.count(word1) && index_map[word1].count(doc_id))
                {
                    lines.insert(index_map[word1][doc_id].begin(), index_map[word1][doc_id].end());
                }
                if (index_map.count(word2) && index_map[word2].count(doc_id))
                {
                    lines.insert(index_map[word2][doc_id].begin(), index_map[word2][doc_id].end());
                }

                if (!first_doc)
                    cout << "----------" << endl;
                print_lines_of_doc(doc_id, lines);
                first_doc = false;
                found = true;
            }
        }
        else
        {
            string word = normalize_word(query);

            if (index_map.count(word))
            {
                for (map<int, set<int>>::iterator it = index_map[word].begin(); it != index_map[word].end(); ++it)
                {
                    int doc_id = it->first;
                    if (!first_doc)
                        cout << "----------" << endl;
                    print_lines_of_doc(doc_id, it->second);
                    first_doc = false;
                    found = true;
                }
            }
        }

        if (!found)
        {
            cout << "Sorry, I found nothing." << endl;
        }
        cout << "==========" << endl;
    }

    return 0;
}