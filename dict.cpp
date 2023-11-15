// Do NOT add any other includes
#include "dict.h"

Dict::Dict()
{
    hash = Hash();
}

Dict::~Dict()
{
    // Implement your function here
}

void Dict::assign_word_ct(string w, int ct)
{
    hash.assign_val(w, ct);
}
void Dict::insert_sentence(long long count, string sentence)
{
    hash.insert(sentence, count);
    return;
}

long long Dict::get_word_count(string word)
{
    string wd;
    for (char i : word)
    {

        if (i <= 'Z' && i >= 'A')
        {
            wd += i + 32;
        }
        else
        {
            wd += i;
        }
    }
    return hash.val(wd);
}

void Dict::dump_dictionary(string filename)
{
    hash.wto(filename);
    return;
}
SymNode::SymNode()
{
    key = "";
    height = -1;
    right = NULL;
    left = NULL;
    par = NULL;
}

SymNode::SymNode(string k, long long ct)
{
    key = k;
    right = NULL;
    left = NULL;
    par = NULL;
    height = 0;
    address = ct;
}

SymNode *SymNode::LeftLeftRotation()
{
    SymNode *temp = this->right;
    this->right = this->right->left;
    temp->left = this;
    this->par = temp;
    if (this->right)
    {
        this->right->par = this;
    }
    if (this->left && this->right)
    {
        this->height = max(this->left->height, this->right->height) + 1;
    }
    else if (this->right)
    {
        this->height = this->right->height + 1;
    }
    else if (this->left)
    {
        this->height = this->left->height + 1;
    }
    else
    {
        this->height = 0;
    }
    if (temp->right)
    {
        temp->height = max(temp->left->height, temp->right->height) + 1;
    }
    else
    {
        temp->height = temp->left->height + 1;
    }
    return temp;
}

SymNode *SymNode::RightRightRotation()
{
    SymNode *temp = this->left;
    this->left = this->left->right;
    temp->right = this;
    this->par = temp;
    if (this->left)
        this->left->par = this;
    if (this->left && this->right)
    {
        this->height = max(this->left->height, this->right->height) + 1;
    }
    else if (this->right)
    {
        this->height = this->right->height + 1;
    }
    else if (this->left)
    {
        this->height = this->left->height + 1;
    }
    else
    {
        this->height = 0;
    }
    if (temp->left)
    {
        temp->height = max(temp->left->height, temp->right->height) + 1;
    }
    else
    {
        temp->height = temp->right->height + 1;
    }
    return temp;
}

SymNode *SymNode::LeftRightRotation()
{
    this->right = this->right->RightRightRotation();
    this->right->par = this;
    return this->LeftLeftRotation();
}

SymNode *SymNode::RightLeftRotation()
{
    this->left = this->left->LeftLeftRotation();
    this->left->par = this;
    return this->RightRightRotation();
}

SymNode::~SymNode()
{
}
void Destructor(SymNode *rt)
{
    if (rt)
    {
        Destructor(rt->left);
        Destructor(rt->right);
        delete rt;
    }
}

SymNode *Balance_AVL(SymNode *rt)
{
    if (rt->right && rt->left)
    {
        if ((rt->right->height - rt->left->height) > 1)
        {
            if (rt->right->left && rt->right->right)
            {
                if (rt->right->left->height > rt->right->right->height)
                {
                    return rt->LeftRightRotation();
                }
                else
                {
                    return rt->LeftLeftRotation();
                }
            }
            else if (rt->right->left)
            {
                return rt->LeftRightRotation();
            }
            else
            {
                return rt->LeftLeftRotation();
            }
        }
        else if ((rt->left->height - rt->right->height) > 1)
        {
            if (rt->left->left && rt->left->right)
            {
                if (rt->left->right->height > rt->left->left->height)
                {
                    return rt->RightLeftRotation();
                }
                else
                {
                    return rt->RightRightRotation();
                }
            }
            else if (rt->left->right)
            {
                return rt->RightLeftRotation();
            }
            else
            {
                return rt->RightRightRotation();
            }
        }
        else
        {
            return rt;
        }
    }
    else if ((rt->left))
    {
        if (rt->left->height >= 1)
        {
            if (rt->left->right)
            {
                return rt->RightLeftRotation();
            }
            return rt->RightRightRotation();
        }
        else
        {
            return rt;
        }
    }
    else if ((rt->right))
    {
        if (rt->right->height >= 1)
        {
            if (rt->right->left)
            {
                return rt->LeftRightRotation();
            }
            return rt->LeftLeftRotation();
        }
        else
        {
            return rt;
        }
    }
    else
    {
        return rt;
    }
}

SymNode *Insert_Helper(SymNode *rt, string k, int ct)
{
    if (rt)
    {
        if (rt->key < k)
        {
            rt->right = Insert_Helper(rt->right, k, ct);
            rt->right = Balance_AVL(rt->right);
            if (rt->left)
            {
                rt->height = max(rt->left->height, rt->right->height) + 1;
            }
            else
            {
                rt->height = rt->right->height + 1;
            }
            rt->right->par = rt;
            return rt;
        }
        else if (rt->key > k)
        {
            rt->left = Insert_Helper(rt->left, k, ct);
            rt->left = Balance_AVL(rt->left);
            if (rt->right)
            {
                rt->height = max(rt->left->height, rt->right->height) + 1;
            }
            else
            {
                rt->height = rt->left->height + 1;
            }
            rt->left->par = rt;
            return rt;
        }
        else
        {
            rt->address++;
            return rt;
        }
    }
    else
    {
        return new SymNode(k, ct);
    }
}

SymbolTable::SymbolTable()
{
    this->size = 0;
    this->root = NULL;
}
void SymbolTable::insert(string k, long long ct = 1)
{
    if (size == 0)
    {
        this->root = new SymNode(k, ct);
        size++;
        return;
    }
    else
    {
        if (root->key > k)
        {
            root->left = Insert_Helper(root->left, k, ct);
            root->left = Balance_AVL(root->left);
            if (root->right)
            {
                root->height = max(root->left->height, root->right->height) + 1;
            }
            else
            {
                root->height = root->left->height + 1;
            }
            root->left->par = root;
            size++;
        }
        else if (root->key < k)
        {
            root->right = Insert_Helper(root->right, k, ct);
            root->right = Balance_AVL(root->right);
            if (root->left)
            {
                root->height = max(root->left->height, root->right->height) + 1;
            }
            else
            {
                root->height = root->right->height + 1;
            }
            root->right->par = root;
            size++;
        }
        else
        {
            root->address++;
            return;
        }
        this->root = Balance_AVL(root);
        // rk(this->root, k);
        // this->root = Balance_AVL(this->root);
    }
}

long long SymbolTable::search(string k)
{

    SymNode *tp = this->root;
    while (tp)
    {
        if (tp->key < k)
        {
            tp = tp->right;
        }
        else if (tp->key > k)
        {
            tp = tp->left;
        }
        else
        {
            break;
        }
    }
    if (tp)
    {
        return tp->address;
    }
    else
    {
        return 0;
    }
}
void SymbolTable::assign_val(string k, int v)
{
    SymNode *tp = this->root;
    while (tp)
    {
        if (tp->key < k)
        {
            tp = tp->right;
        }
        else if (tp->key > k)
        {
            tp = tp->left;
        }
        else
        {
            break;
        }
    }
    if (tp)
    {
        tp->address = v;
    }
    else
    {
        this->insert(k);
        assign_val(k, v);
    }
}

SymbolTable::~SymbolTable()
{
    this->size = 0;
    Destructor(root);
}
Hash::Hash()
{
    symtb.resize(100000, nullptr);
}
void Hash::insert(string str, long long ct = 1)
{
    int a = Hash_val(str);
    if (!symtb[a])
        symtb[a] = new SymbolTable();
    symtb[a]->insert(str, ct);
}
int Hash::Hash_val(string id)
{
    static const int hashPrime = 100003;
    int hash_value = 0;
    int hash_Primes[] = {419, 643, 883, 1103, 1307, 1543, 1747, 1993, 2213, 2417, 2677, 37, 751, 659, 1361, 2879, 3137, 257, 3371, 3607, 3847, 31};
    int a = 0;
    for (char character : id)
    {
        hash_value += hash_Primes[a % 22] * int(character);
        a++;
        hash_value %= hashPrime;
    }
    return abs(hash_value % 100000);
}
long long Hash::val(string k)
{
    int a = Hash_val(k);
    if (symtb[a])
        return symtb[a]->search(k);
    else
    {
        return 0;
    }
}
void Hash::assign_val(string k, long long v)
{
    int a = Hash_val(k);
    symtb[a]->assign_val(k, v);
}
void Hash::wto(string Fn)
{
    ofstream po(Fn.c_str());
    ofstream out;
    out.open(Fn, ios::app);
    for (SymbolTable *a : symtb)
    {
        if (a)
            if (a->root)
            {
                vector<SymNode *> s;
                s.push_back(a->root);
                while (s.size())
                {
                    SymNode *rt = s.back();
                    out << rt->key + ", " + to_string(rt->address) + "\n";
                    s.pop_back();
                    if (rt->right)
                    {
                        s.push_back(rt->right);
                    }
                    if (rt->left)
                    {
                        s.push_back(rt->left);
                    }
                }
            }
    }
}
Hash::~Hash()
{
    for (size_t i = 0; i < symtb.size(); i++)
    {
        if (symtb[i])
        {
            delete symtb[i];
        }
    }
    symtb.clear();
};