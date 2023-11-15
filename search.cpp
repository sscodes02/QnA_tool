// Do NOT add any other includes
#include "search.h"
// #include <cmath>/************************************************/
const string sepo = ".,-:!\"\'()?[];@ ";
class BinaryHeap
{
private:
    std::vector<pair<float, Node *>> heap;

public:
    void insert(float score, Node *rt)
    {
        heap.push_back({score, rt});
        heapifyUp(heap.size() - 1);
    }
    void Insert_2(float score, Node *rt)
    {
        if (heap[0].first < score)
        {
            delete heap[0].second;
            heap[0] = {score, rt};
            heapifyDown(0);
        }
        else
        {
            delete rt;
        }
    }
    void heapifyUp(int index)
    {
        while (index > 0)
        {
            if (heap[index].first < heap[(index - 1) / 2].first)
            {
                auto temp = heap[index];
                heap[index] = heap[(index - 1) / 2];
                heap[(index - 1) / 2] = temp;
                index = (index - 1) / 2;
            }
            else
            {
                return;
            }
        }
        return;
    }
    int size()
    {
        return heap.size();
    }
    void heapifyDown(int index)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        while (leftChild < (int)heap.size())
        {
            int minIndex = index;

            if (heap[minIndex].first > heap[leftChild].first)
            {
                minIndex = leftChild;
            }
            if (rightChild < (int)heap.size())
            {
                if (heap[minIndex] > heap[rightChild])
                {
                    minIndex = rightChild;
                }
            }
            if (minIndex != index)
            {
                auto temp = heap[minIndex];
                heap[minIndex] = heap[index];
                heap[index] = temp;
            }
            else
            {
                return;
            }
            index = minIndex;
            leftChild = 2 * index + 1;
            rightChild = 2 * index + 2;
        }
        return;
    }

    Node *res(int k)
    {
        if ((int)heap.size() < k)
        {
            cout << "ERR_HP\n";
            exit(1);
        }
        Node *rt = heap[0].second;
        rt->right = NULL;
        while (heap.size() > 1)
        {
            heap[0] = heap.back();
            heap.pop_back();
            heapifyDown(0);
            rt->left = heap[0].second;
            rt->left->right = rt;
            rt = rt->left;
        }
        rt->left = NULL;
        heap.pop_back();
        return rt;
    }
    void deleteMin()
    {
        if (heap.empty())
        {
            std::cout << "ERR_HP_empty :(" << std::endl;
            return;
        }
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    void printHeap()
    {
        for (const auto &elem : heap)
        {
            std::cout << elem.first << " ";
        }
        std::cout << std::endl;
    }
    ~BinaryHeap()
    {
        for (size_t i = 0; i < heap.size(); i++)
        {
            if (heap[i].second)
            {
                delete heap[i].second;
            }
        }
    }
};
SearchEngine::SearchEngine()
{
    ifstream ip;
    ip.open("unigram_freq.csv");
    if (ip.is_open())
    {
        string p;
        string v;
        getline(ip, p);
        while (std::getline(ip, p, ',') && std::getline(ip, v))
        {
            long long ct = 0;
            try
            {
                ct = stoll(v);
            }
            catch (const std::exception &e)
            {
                continue;
            }
            gen_word_freq.insert_sentence(ct, p);
        }
    }
    ip.close();
    // cout << "CSV sucess\n";
}

SearchEngine::~SearchEngine()
{
    for (SymbolTable *tp : lib)
    {
        if (tp)
        {
            delete tp;
        }
    }
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    //  We are using a simple lib vector that stores all words count as it was stroing in dict, so there is no need
    // to count word of query again & again. Reason : This will decrease memory as well as time.

    // Now further if want to use it for a genral corpus we can modify hash function and add an attribute for storing other data of sentences.

    /*For given corpus we are using bound book_code <= 100.  page_ct<=1000. para.<=100.   */
    book_code -= 1;
    // page -= 1;
    // paragraph -= 1;
    string p = "";

    if (int(book_code / 10) == 0)
    {
        p = p + '0';
    }
    if (int(book_code / 100) >= 1)
    {
        cout << "ERR: crossed bound for BOOK CODE\n";
        exit(0);
    }
    p = p + to_string(book_code);

    if (int(page / 10) == 0)
    {
        p = p + "00";
    }
    else if (page / 100 == 0)
    {
        p = p + "0";
    }
    if (book_code / 1000 >= 1)
    {
        cout << "ERR: crossed bound for Paragraph\n";
        exit(0);
    }
    p += to_string(page);
    if (paragraph / 10 == 0)
    {
        p += '0';
    }
    if (paragraph / 100 >= 1)
    {
        cout << "ERR: crossed bound for Paragraph\n";
        exit(1);
    }
    p += to_string(paragraph);
    // cout << p << "   p\n";

    int hash = stoi(p);

    if ((int)lib.size() <= hash)
    {
        lib.resize(hash + 1, NULL);
    }
    if (lib[hash] == NULL)
    {
        lib[hash] = new SymbolTable();
    }
    string a = "";

    for (size_t i = 0; i < sentence.size(); i++)
    {
        while (sepo.find(sentence[i]) == string::npos)
        {
            if (sentence[i] <= 'Z' && sentence[i] >= 'A')
            {
                a += sentence[i] + 32;
            }
            else
            {
                a += sentence[i];
            }
            i++;
            if (i >= sentence.size())
                break;
        }
        if (a.size() >= 1)
        {
            lib[hash]->insert(a, 1);
            word_ct.insert_sentence(1, a);
        }
        a = "";
    }
    return;
}

Node *SearchEngine::search(string pattern, int n_matches, int p)
{
    if (n_matches == 0)
        return NULL;
    // float max_sc = 0;
    // int bok = 0, pag = 0, part = 0;
    vector<float> score;
    vector<string> query;
    string str = "";
    for (size_t i = 0; i < pattern.size(); i++)
    {
        while (sepo.find(pattern[i]) == string::npos)
        {
            if (pattern[i] <= 'Z' && pattern[i] >= 'A')
            {
                str += pattern[i] + 32;
            }
            else
            {
                str += pattern[i];
            }
            i++;
            if (i >= pattern.size())
                break;
        }
        if (str.size() > 0)
        {
            query.push_back(str);
            float sc = float(1 + word_ct.get_word_count(str)) / float(1 + gen_word_freq.get_word_count(str));
            score.push_back(sc);
        }
        str = "";
    }

    /****************************/
    if (p)
    {
        score.clear();
        score = relavency_q(query);
    }
    /****************************/

    BinaryHeap top_k;
    for (size_t i = 0; i < lib.size(); i++)
    {
        if (lib[i] != NULL)
        {
            float sr = 0;
            for (size_t j = 0; j < query.size(); j++)
            {
                int a = lib[i]->search(query[j]);
                sr += float(a) * score[j];
            }
            if (sr > 0)
            {
                string k = to_string(i);
                while (k.size() <= 6)
                {
                    k = '0' + k;
                }
                int bk = stoi(k.substr(0, 2)) + 1;
                int pg = stoi(k.substr(2, 3));
                int para = stoi(k.substr(5, 2));

                // if (sr > max_sc)
                // {
                //     max_sc = sr;
                //     bok = bk;
                //     pag = pg;
                //     part = para;
                // }
                Node *rt = new Node();
                rt->book_code = bk;
                rt->left = NULL;
                rt->right = NULL;
                rt->page = pg;
                rt->paragraph = para;

                if (top_k.size() < n_matches)
                {
                    top_k.insert(sr, rt);
                }
                else if (n_matches > 0)
                {
                    top_k.Insert_2(sr, rt);
                }
            }
        }
    }
    return top_k.res(n_matches);
}
vector<float> SearchEngine::relavency_q(vector<string> q)
{
    vector<float> ans;
    long long num_words = 17215668;
    for (size_t i = 0; i < q.size(); i++)
    {
        long long ctt = word_ct.get_word_count(q[i]);
        float k = (num_words / ctt + 1);
        ans.push_back(k);
        cout << q[i] << "  =  " << ans[i] << "\n";
    }
    return ans;
}