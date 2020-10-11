#include <bits/stdc++.h>
#define ll long long int
#define dd long double
#define minsupport 0.07
#define max_leaf 3
#define max_node 3
#define hash_key(x) ((x - 1) % max_node)
using namespace std;
using namespace std::chrono;
#define intra 5
//**************************generate_own_dataset****************************************************************************************
void generatedataset()
{
    srand(time(0));
    ll transactions;
    cout << "Enter the number of transactions: ";
    cin >> transactions;
    int items;
    cout << "Enter the number of items: ";
    cin >> items;
    dd average_width;
    cout << "Enter the average width of the transitions: ";
    cin >> average_width;
    ll curritem = average_width * intra;
    ll cc;
    ll total = (transactions * average_width);
    ll tt = total;
    ll curr = 0;
    int temp = 0;
    vector<int> perline;
    while (curr < transactions)
    {
        temp = 0;
        cc = curritem;

        while (temp < intra)
        {
            int r;
            if (temp == intra - 1)
                r = cc;
            else
            {
                int l = rand() % 100000;
                r = l % (cc - intra + 1 + temp);
                r++;
            }
            perline.push_back(r);
            cc -= r;
            temp++;
        }
        curr += intra;
    }
    cout << "Restart the program and Set the input file name in the 'takeinputdataset' function as 'Dataset.txt' and Enter: 0 for asking new dataset generation ";
    freopen("Dataset.txt", "w", stdout);
    for (ll i = 0; i < perline.size(); i++)
    {
        for (int j = 0; j < perline[i]; j++)
        {
            int r = rand() % items;
            r++;
            cout << r << " ";
        }
        cout << "\n";
    }
}

//********************************************************Take_Input*************************************************************************
void takeinputdataset(map<ll, set<int>> &mydataset)
{
    ifstream in("input3.txt");
    string str;
    ll transid = 1;
    map<string, int> itemsmaping;
    int curr = 1;
    while (getline(in, str))
    {
        if (str.size())
        {
            istringstream ss(str);
            string word;
            ss >> word;
            while (ss)
            {
                int item = 0;
                if (word[0] >= '0' && word[0] <= '9')
                {
                    stringstream sst(word);
                    sst >> item;
                    mydataset[transid].insert(item);
                }
                else
                {
                    if (itemsmaping.count(word))
                        mydataset[transid].insert(itemsmaping[word]);
                    else
                    {
                        mydataset[transid].insert(curr);
                        itemsmaping[word] = curr;
                        curr++;
                    }
                }

                ss >> word;
            }
            transid++;
        }
    }
}
//********************************************************************************************************************************************
//***********************************************Apriori-Algorithm**********************************************************************************
struct hash_node
{
    set<set<int>> leaf[max_node];
    hash_node *next[max_node];
};

hash_node *create_hashnode()
{
    hash_node *temp = new hash_node;
    for (int i = 0; i < max_node; i++)
        temp->next[i] = NULL;
    return temp;
}

void insert_hash_node(set<int> itemset, int key, hash_node *&hashroot)
{
    int hashkeyitem;
    int x = 1;
    auto itr = itemset.begin();
    while (x < key)
    {
        itr++;
        x++;
    }
    hashkeyitem = *itr;
    if (hashroot == NULL)
    {
        hashroot = create_hashnode();
        hashroot->leaf[hash_key(hashkeyitem)].insert(itemset);
        return;
    }
    else
    {

        if (hashroot->next[hash_key(hashkeyitem)])
            insert_hash_node(itemset, key + 1, hashroot->next[hash_key(hashkeyitem)]);

        else
        {
            hashroot->leaf[hash_key(hashkeyitem)].insert(itemset);
            if (hashroot->leaf[hash_key(hashkeyitem)].size() > max_leaf && key != itemset.size())
            {

                for (auto itr = hashroot->leaf[hash_key(hashkeyitem)].begin(); itr != hashroot->leaf[hash_key(hashkeyitem)].end(); itr++)
                {
                    insert_hash_node(*itr, key + 1, hashroot->next[hash_key(hashkeyitem)]);
                    // hashroot->leaf[hash_key(hashkeyitem)].erase(*itr);
                }
            }
            else
                return;
        }
    }
}

void frequent_itemsets_of_size_one(vector<set<int>> &sc1, map<ll, set<int>> mydataset, dd total_transition)
{
    map<int, ll> support_count;
    for (ll i = 1; i <= mydataset.size(); i++)
    {
        for (auto itr = mydataset[i].begin(); itr != mydataset[i].end(); itr++)
        {
            if (support_count.count(*itr) == 0)
                support_count[*itr] = 1;
            else
                support_count[*itr]++;
        }
    }
    set<int> temp;
    dd currsupport;
    for (auto itr = support_count.begin(); itr != support_count.end(); itr++)
    {
        currsupport = itr->second / total_transition;

        if (currsupport >= minsupport)
        {
            temp.insert(itr->first);
            sc1.push_back(temp);
            temp.clear();
        }
    }
}

void potentially_frequent_aprior_candidate_generation(vector<set<int>> &potentially_frequent, vector<set<int>> pre_frequent, int size)
{
    set<set<int>> presets;
    for (int i = 0; i < pre_frequent.size(); i++)
    {
        presets.insert(pre_frequent[i]);
    }
    set<int> curritemset;
    bool check = true;
    for (int i = 0; i < pre_frequent.size(); i++)
    {
        for (int j = i + 1; j < pre_frequent.size(); j++)
        {
            auto itr1 = pre_frequent[i].begin();
            auto itr2 = pre_frequent[j].begin();
            check = true;
            curritemset.clear();
            int k = 0;
            while (k < size - 2)
            {
                if (*itr1 != *itr2)
                {
                    check = false;
                    break;
                }
                itr1++;
                itr2++;
                k++;
            }
            if (check)
            {
                curritemset = pre_frequent[i];
                curritemset.insert(*itr2);
                set<int> sss;
                int n = 0;
                auto itr = curritemset.begin();
                while (n > size - 2)
                {
                    sss = curritemset;
                    sss.erase(*itr);
                    if (!presets.count(sss))
                    {
                        check = false;
                        break;
                    }
                    itr++;
                    n++;
                }
                if (check)
                    potentially_frequent.push_back(curritemset);
            }
            else
                break;
        }
    }
}
void show(hash_node *hashroot)
{
    if (hashroot == NULL)
        return;
    else
    {
        for (int i = 0; i < max_node; i++)
        {
            if (hashroot->next[i] == NULL)
            {
                for (auto itr = hashroot->leaf[i].begin(); itr != hashroot->leaf[i].end(); itr++)
                {
                    cout << "{ ";
                    for (auto it = (*itr).begin(); it != (*itr).end(); it++)
                    {
                        cout << *it << " ";
                    }
                    cout << "}";
                }
            }
            else
                show(hashroot->next[i]);
        }
    }
}
void make_hash_tree_for_candidate_itemsets(hash_node *&hashroot, vector<set<int>> potentially_frequent, int size)
{
    for (int i = 0; i < potentially_frequent.size(); i++)
        insert_hash_node(potentially_frequent[i], 1, hashroot);
}

void generate_set(set<set<int>> &all, int size, int index, vector<int> trans, set<int> curr)
{
    if (size == 0)
    {
        all.insert(curr);
        return;
    }
    else if (index >= trans.size())
        return;
    else
    {
        for (int i = index; i < trans.size(); i++)
        {
            curr.insert(trans[i]);
            generate_set(all, size - 1, i + 1, trans, curr);
            curr.erase(trans[i]);
        }
    }
}
void support_count(hash_node *hashroot, map<set<int>, ll> &frequency, vector<int> trans, int size, int index, int level, set<int> itemset)
{
    int hashkey = hash_key(trans[index]);
    if (hashroot == NULL)
        return;
    if (!hashroot->next[hashkey])
    {
        set<set<int>> all;
        generate_set(all, size - level - 1, index + 1, trans, itemset);
        if (hashroot->leaf[hashkey].size())
        {
            for (auto itr = hashroot->leaf[hashkey].begin(); itr != hashroot->leaf[hashkey].end(); itr++)
            {
                if (all.count(*itr))
                {
                    if (frequency.count(*itr) == 0)
                        frequency[*itr] = 1;
                    else
                        frequency[*itr]++;
                }
            }
        }
    }
    else
    {
        for (int i = index + 1; i < trans.size(); i++)
        {
            itemset.insert(trans[i]);
            support_count(hashroot->next[hashkey], frequency, trans, size, i, level + 1, itemset);
            itemset.erase(trans[i]);
        }
    }
}

void support_count_of_my_hashtree_item(hash_node *hashroot, set<int> transition, map<set<int>, ll> &frequency, int size)
{
    vector<int> trans;
    for (auto itr = transition.begin(); itr != transition.end(); itr++)
    {
        trans.push_back(*itr);
    }
    set<int> itemset;
    for (int i = 0; i < trans.size(); i++)
    {
        itemset.clear();
        itemset.insert(trans[i]);
        int level = 0;
        support_count(hashroot, frequency, trans, size, i, 0, itemset);
    }
}

vector<set<int>> all_frequent_itemsets(map<ll, set<int>> mydataset, vector<set<int>> potentially_frequent, int size, dd total_transitions)
{
    hash_node *hashroot = NULL;
    make_hash_tree_for_candidate_itemsets(hashroot, potentially_frequent, size);
    map<set<int>, ll> frequency;
    //show(hashroot);
    vector<set<int>> finalfrequentset;
    for (ll i = 1; i <= mydataset.size(); i++)
    {
        support_count_of_my_hashtree_item(hashroot, mydataset[i], frequency, size);
    }
    for (auto itr = frequency.begin(); itr != frequency.end(); itr++)
    {
        dd check = itr->second / total_transitions;
        if (check >= minsupport)
            finalfrequentset.push_back(itr->first);
    }
    sort(finalfrequentset.begin(), finalfrequentset.end());
    return finalfrequentset;
}

//**************************************************************************************************************************************************
//**********************************************************FP-Growth***************************************************************************
struct node
{
    int count;
    int item;
    vector<node *> child;
    node *parent;
    map<int, int> childposition; //map<itemnumber, indexin_child_vector>
};

node *createnode()
{
    node *temp = new node;
    temp->count = 0;
    temp->parent = NULL;
    return temp;
}

void insertnode(node *&root, node *parent, vector<int> trans, int index, vector<node *> position[])
{
    if (index >= trans.size())
        return;
    else if (root == NULL)
    {
        root = createnode();
        root->item = trans[index];
        position[root->item].push_back(root);
        root->count++;
        root->parent = parent;
        if (index + 1 < trans.size())
        {
            root->child.push_back(NULL);
            root->childposition[trans[index + 1]] = (root->child.size()) - 1;
            insertnode(root->child[root->childposition[trans[index + 1]]], root, trans, index + 1, position);
        }
    }
    else
    {
        root->count++;
        if (index + 1 < trans.size())
        {
            if (root->childposition.count(trans[index + 1]) == 0)
            {
                root->child.push_back(NULL);
                root->childposition[trans[index + 1]] = (root->child.size()) - 1;
                insertnode(root->child[root->childposition[trans[index + 1]]], root, trans, index + 1, position);
            }
            else
                insertnode(root->child[root->childposition[trans[index + 1]]], root, trans, index + 1, position);
        }
    }
}
void showfptree(node *root)
{
    if (root == NULL)
        return;
    else
    {
        cout << "{ ";
        cout << root->item << " " << root->count << " }";
        for (int i = 0; i < root->child.size(); i++)
        {
            showfptree(root->child[i]);
        }
    }
}
bool compare(pair<int, ll> a, pair<int, ll> b)
{
    return a.second > b.second;
}

void takeinputfpdataset(map<ll, set<int>> mydataset, map<ll, vector<int>> &myfbdataset, dd total, int &max, vector<int> &supporder)
{
    map<int, ll> supcount;
    for (int i = 1; i <= mydataset.size(); i++)
    {
        for (auto itr = mydataset[i].begin(); itr != mydataset[i].end(); itr++)
        {
            if (supcount.count(*itr) == 0)
                supcount[*itr] = 1;
            else
                supcount[*itr]++;
        }
    }
    pair<int, ll> flag;
    vector<pair<int, ll>> temp;
    for (auto itr = supcount.begin(); itr != supcount.end(); itr++)
    {
        dd check = itr->second / total;
        if (check >= minsupport)
        {
            flag.first = itr->first;
            flag.second = itr->second;
            temp.push_back(flag);
        }
    }
    sort(temp.begin(), temp.end(), compare);
    for (int j = 0; j < temp.size(); j++)
    {
        supporder.push_back(temp[j].first);
    }
    temp.clear();
    ll index = 1;
    for (int i = 1; i <= mydataset.size(); i++)
    {
        temp.clear();
        for (auto itr = mydataset[i].begin(); itr != mydataset[i].end(); itr++)
        {
            dd check = (supcount[*itr] / total);
            if (check >= minsupport)
            {
                flag.first = *itr;
                flag.second = supcount[*itr];
                temp.push_back(flag);
                if (max < *itr)
                    max = *itr;
            }
        }
        if (temp.size())
        {
            sort(temp.begin(), temp.end(), compare);
            for (int j = 0; j < temp.size(); j++)
            {
                myfbdataset[index].push_back(temp[j].first);
            }
            index++;
        }
    }
}

void collect_all(node *item, map<int, pair<ll, set<pair<int, node *>>>> &countt, ll initial)
{
    if (item == NULL)
        return;
    pair<ll, set<pair<int, node *>>> p;
    pair<int, node *> tt;
    while (item->parent != NULL)
    {

        if (countt.count(item->parent->item) == 0)
        {
            p.first = initial;
            p.second.clear();
            tt.first = initial;
            tt.second = item->parent;
            p.second.insert(tt);
            countt[item->parent->item] = p;
        }
        else
        {
            countt[item->parent->item].first += initial;
            tt.first = initial;
            tt.second = item->parent;
            countt[item->parent->item].second.insert(tt);
        }
        item = item->parent;
    }
}

void fbfrequentsets(vector<vector<int>> &fbfreq, vector<int> curr, map<int, pair<ll, set<pair<int, node *>>>> count, dd total)
{
    for (auto itr = count.begin(); itr != count.end(); itr++)
    {
        dd check = (itr->second).first / total;
        if (check >= minsupport)
        {
            curr.push_back(itr->first);
            fbfreq.push_back(curr);
            map<int, pair<ll, set<pair<int, node *>>>> cc;
            for (auto it = (itr->second).second.begin(); it != (itr->second).second.end(); it++)
                collect_all((*it).second, cc, (*it).first);
            fbfrequentsets(fbfreq, curr, cc, total);
            curr.pop_back();
        }
    }
}
//******************************************************************************************************************************************
//**********************************************************Eclat-Algorithm******************************************************************
void potentially_frequent_Eclat_candidate_generation(vector<vector<set<int>>> &potentially_frequent, vector<set<int>> pre_frequent, int size)
{
    set<set<int>> presets;
    for (int i = 0; i < pre_frequent.size(); i++)
    {
        presets.insert(pre_frequent[i]);
    }
    vector<set<int>> curritemset;
    set<int> flag;
    bool check = true;
    for (int i = 0; i < pre_frequent.size(); i++)
    {
        for (int j = i + 1; j < pre_frequent.size(); j++)
        {
            auto itr1 = pre_frequent[i].begin();
            auto itr2 = pre_frequent[j].begin();
            check = true;
            curritemset.clear();
            flag.clear();
            int k = 0;
            while (k < size - 2)
            {
                if (*itr1 != *itr2)
                {
                    check = false;
                    break;
                }
                itr1++;
                itr2++;
                k++;
            }
            if (check)
            {
                flag = pre_frequent[i];
                flag.insert(*itr2);
                set<int> sss;
                int n = 0;
                auto itr = flag.begin();
                while (n > size - 2)
                {
                    sss = flag;
                    sss.erase(*itr);
                    if (!presets.count(sss))
                    {
                        check = false;
                        break;
                    }
                    itr++;
                    n++;
                }
                if (check)
                    curritemset.push_back(flag);
                curritemset.push_back(pre_frequent[i]);
                curritemset.push_back(pre_frequent[j]);
                potentially_frequent.push_back(curritemset);
            }
            else
                break;
        }
    }
}

void takeinputverticaldataset(map<ll, set<int>> mydataset, map<int, vector<ll>> &myverticaldataset)
{
    for (ll i = 1; i <= mydataset.size(); i++)
    {
        for (auto itr = mydataset[i].begin(); itr != mydataset[i].end(); itr++)
        {
            myverticaldataset[*itr].push_back(i);
        }
    }
}

void allfrequent_of_size_one(map<int, vector<ll>> myverticaldataset, map<set<int>, vector<ll>> &potentialfrequent, dd total)
{
    set<int> s;
    vector<set<int>> temp;
    for (auto itr = myverticaldataset.begin(); itr != myverticaldataset.end(); itr++)
    {
        dd check = (itr->second).size() / total;
        s.clear();
        if (check >= minsupport)
        {
            s.insert(itr->first);
            potentialfrequent[s] = myverticaldataset[itr->first];
        }
    }
}

void AllfrequentEclat_itemset(map<set<int>, vector<ll>> prefrequent, map<set<int>, vector<ll>> &temp, vector<vector<set<int>>> &potential, dd total)
{
    set<ll> tempp;
    vector<ll> flag1;
    vector<ll> flag2;
    vector<ll> inter;
    for (int i = 0; i < potential.size(); i++)
    {
        tempp.clear();
        flag1.clear();
        flag2.clear();
        inter.clear();
        flag1 = prefrequent[potential[i][1]];
        flag2 = prefrequent[potential[i][2]];
        dd check = min(flag1.size(), flag2.size()) / total;
        if (check >= minsupport)
        {
            for (int j = 0; j < flag1.size(); j++)
                tempp.insert(flag1[j]);
            for (int j = 0; j < flag2.size(); j++)
            {
                if (tempp.count(flag2[j]))
                {
                    inter.push_back(flag2[j]);
                }
            }

            check = inter.size() / total;
            if (check >= minsupport)
            {
                temp[potential[i][0]] = inter;
            }
        }
    }
}
//**************************************************************************************************************************************************
void averagewidth(map<ll, set<int>> mydataset, dd &avgwidth, ll &totalitems)
{
    ll curr = 0;
    set<int> temp;
    for (ll i = 0; i < mydataset.size(); i++)
    {
        for (auto itr = mydataset[i].begin(); itr != mydataset[i].end(); itr++)
        {
            curr++;
            temp.insert(*itr);
        }
    }
    avgwidth = curr / (mydataset.size() * 1.0);
    totalitems = temp.size();
}

int main()
{
    map<ll, set<int>> mydataset;
    takeinputdataset(mydataset);
    dd avgwidth = 1;
    ll totalintems = 0;
    averagewidth(mydataset, avgwidth, totalintems);
    dd total_transactions = mydataset.size() * 1.0;
    ll totaltrans = total_transactions;
    int dsgenerate;
    cout << "To generate own-dataset Enter: 1 otherwise Enter 0: ";
    cin >> dsgenerate;
    if (dsgenerate)
    {
        generatedataset();
    }
    if (!dsgenerate)
    {
        cout << "Set the input file name in the 'takeinputdataset' function  and 'Value of minsupport'on the Top\n";
        cout << "Total number of transactions : " << totaltrans << "\n";
        cout << "The Average-Width-Of the transactions : " << avgwidth << "\n";
        cout << "Total Number of items : " << totalintems << "\n";
        cout << "Operation-1. Apriori Algorithm\n";
        cout << "Opeartion-2. Fp-Growth\n";
        cout << "Operation-3. Eclat Algorithm\n";
        cout << "Enter the Operation: ";
        int op;
        cin >> op;
        if (op == 1)
        {
            auto start = high_resolution_clock::now();
            map<ll, vector<set<int>>> frequent_itemsets_of_size;
            vector<set<int>> potentially_frequent, temp;
            frequent_itemsets_of_size_one(potentially_frequent, mydataset, total_transactions);
            frequent_itemsets_of_size[1] = potentially_frequent;
            for (int k = 2; k <= frequent_itemsets_of_size[1].size(); k++)
            {
                potentially_frequent.clear();
                potentially_frequent_aprior_candidate_generation(potentially_frequent, frequent_itemsets_of_size[k - 1], k);
                if (potentially_frequent.size())
                {
                    temp = all_frequent_itemsets(mydataset, potentially_frequent, k, total_transactions);
                    if (!temp.size())
                        break;
                    frequent_itemsets_of_size[k] = temp;
                }
            }
            ll count = 0;
            int size = 0;
            for (int i = 1; i <= frequent_itemsets_of_size.size(); i++)
            {
                for (auto j = 0; j < frequent_itemsets_of_size[i].size(); j++)
                {
                    count++;
                    if (size < frequent_itemsets_of_size[i][j].size())
                        size = frequent_itemsets_of_size[i][j].size();
                }
            }
            cout << "All frequent item sets for the minsupport -> " << minsupport << ": " << count << "\n";
            cout << "Size of maximum frequent item set : " << size << "\n";
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            cout << "Time taken by Apriori-Algorithm: "
                 << duration.count() << " microseconds" << endl;
            cout << "See all the frequent itemset in the output.txt file\n";
            freopen("output.txt", "w", stdout);
            for (int i = 1; i <= frequent_itemsets_of_size.size(); i++)
            {
                for (auto j = 0; j < frequent_itemsets_of_size[i].size(); j++)
                {
                    cout << "{ ";
                    for (auto itr = frequent_itemsets_of_size[i][j].begin(); itr != frequent_itemsets_of_size[i][j].end(); itr++)
                    {
                        cout << *itr << " ";
                    }
                    cout << "}";
                    count++;
                }
                cout << "\n";
            }
        }
        else if (op == 2)
        {
            auto start = high_resolution_clock::now();
            map<ll, vector<int>> myfbdataset;
            vector<int> supporder;

            int max_nodevalue = 0;
            takeinputfpdataset(mydataset, myfbdataset, total_transactions, max_nodevalue, supporder);
            vector<node *> fptree(max_nodevalue + 1, NULL);
            vector<node *> position[max_nodevalue + 1];

            for (int i = 1; i <= myfbdataset.size(); i++)
            {
                if (myfbdataset[i].size())
                    insertnode(fptree[myfbdataset[i][0]], NULL, myfbdataset[i], 0, position);
            }
            // for(int i=0;i<fptree.size();i++)
            // {
            //     showfptree(fptree[i]);
            // }
            map<int, vector<vector<int>>> fbfrequent;
            vector<int> curr;
            map<int, pair<ll, set<pair<int, node *>>>> count;
            for (int i = supporder.size() - 1; i >= 0; i--)
            {
                curr.clear();
                curr.push_back(supporder[i]);
                fbfrequent[supporder[i]].push_back(curr);
                count.clear();
                for (int j = 0; j < position[supporder[i]].size(); j++)
                {
                    collect_all(position[supporder[i]][j], count, position[supporder[i]][j]->count);
                }

                fbfrequentsets(fbfrequent[supporder[i]], curr, count, total_transactions);
                for (int j = 0; j < position[supporder[i]].size(); j++)
                {
                    node *tempp = position[supporder[i]][j];
                    if (tempp->parent)
                    {
                        int index = tempp->parent->childposition[tempp->item];
                        tempp->parent->child[index] = NULL;
                    }
                }
            }
            ll countt = 0;
            int size = 0;
            for (auto itr = fbfrequent.begin(); itr != fbfrequent.end(); itr++)
            {
                for (int j = 0; j < itr->second.size(); j++)
                {
                    countt++;
                    if (size < itr->second[j].size())
                        size = itr->second[j].size();
                }
            }
            cout << "All frequent item sets for the minsupport -> " << minsupport << ": " << countt << "\n";
            cout << "Size of maximum frequent item set : " << size << "\n";
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            cout << "Time taken by Fp-Growth: "
                 << duration.count() << " microseconds" << endl;
            cout << "See all the frequent itemset in the output.txt file\n";
            freopen("output.txt", "w", stdout);
            for (auto itr = fbfrequent.begin(); itr != fbfrequent.end(); itr++)
            {
                for (int j = 0; j < itr->second.size(); j++)
                {
                    cout << "{ ";
                    for (int k = 0; k < itr->second[j].size(); k++)
                        cout << itr->second[j][k] << " ";
                    cout << "}";
                }
                cout << "\n";
            }
        }
        else if (op == 3)
        {
            auto start = high_resolution_clock::now();
            map<int, vector<ll>> myverticaldataset;
            takeinputverticaldataset(mydataset, myverticaldataset);
            map<set<int>, vector<ll>> prefrequent, temp; // potentially frequent item set and corresponding involve transitions;
            map<ll, vector<set<int>>> allfrequent;       // size and frequent item set
            allfrequent_of_size_one(myverticaldataset, prefrequent, total_transactions);
            vector<vector<set<int>>> potentially_frequent;
            for (auto itr = prefrequent.begin(); itr != prefrequent.end(); itr++)
            {
                allfrequent[1].push_back(itr->first);
            }

            for (int k = 2; k <= allfrequent[1].size(); k++)
            {
                potentially_frequent.clear();
                potentially_frequent_Eclat_candidate_generation(potentially_frequent, allfrequent[k - 1], k);
                if (potentially_frequent.size())
                {
                    AllfrequentEclat_itemset(prefrequent, temp, potentially_frequent, total_transactions);
                    if (temp.size())
                    {
                        prefrequent = temp;
                        temp.clear();
                        for (auto itr = prefrequent.begin(); itr != prefrequent.end(); itr++)
                        {
                            allfrequent[k].push_back(itr->first);
                        }
                    }
                    else
                        break;
                }
                else
                    break;
            }
            ll count = 0;
            int size = 0;
            for (int i = 1; i <= allfrequent.size(); i++)
            {
                for (int j = 0; j < allfrequent[i].size(); j++)
                {
                    count++;
                    if (size < allfrequent[i][j].size())
                        size = allfrequent[i][j].size();
                }
            }
            cout << "All frequent item sets for the minsupport -> " << minsupport << ": " << count << "\n";
            cout << "Size of maximum frequent item set : " << size << "\n";
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            cout << "Time taken by Eclat-Algorithm: "
                 << duration.count() << " microseconds" << endl;
            cout << "See all the frequent itemset in the output.txt file\n";
            freopen("output.txt", "w", stdout);
            for (int i = 1; i <= allfrequent.size(); i++)
            {
                for (int j = 0; j < allfrequent[i].size(); j++)
                {
                    cout << "{ ";
                    for (auto itr = allfrequent[i][j].begin(); itr != allfrequent[i][j].end(); itr++)
                    {
                        cout << *itr << " ";
                    }
                    cout << "}";
                }
                cout << "\n";
            }
        }
    }
    return 0;
}