#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

QNA_tool::QNA_tool()
{
    // Implement your function here
}

QNA_tool::~QNA_tool()
{
    // Implement your function here
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    src.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    return;
}

Node *QNA_tool::get_top_k_para(string question, int k)
{
    return src.search(question, k);
}
Node *QNA_tool::get_k(string question, int k)
{
    return src.search(question, k, 1);
}

void QNA_tool::query(string question, string filename)
{
    string q = question;
    ofstream que("query.txt");
    que.clear();
    if (que.is_open())
    {
        que << "Find the most relevant words in the following query: ";
        que << q;
    }
    que.close();
    string command = "python3 ";
    command += filename;
    command += " ";
    command += "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";
    command += " ";
    command += "0";
    command += " ";
    command += "query.txt";

    system(command.c_str());
    ifstream que2;
    que2.open("Output.txt");
    string r;
    q.clear();
    while (getline(que2, r))
    {
        q += r;
    }
    cout << q << "    :Q   \n";
    Node *p = this->get_k(r, 4);
    std::cout
        << "Q: " << q << std::endl;
    query_llm("api_call.py", p, 4, "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", question);
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{
    bool z = false;
    ofstream Out;
    Out.open("Output.txt", ios::app);
    cout << "\nBook_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename = "./txtfiles/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))
        {
            res += sentence;
            z = 0;
        }
        else if (z)
        {
            return res;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{
    Node *traverse = root;
    int num_paragraph = 0;

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        // Needed for clear the memory
        Node *prv = traverse;
        traverse = traverse->right;
        delete prv;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}