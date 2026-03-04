#include <bits/stdc++.h>

using namespace std;

class FileReader
{
private:
    ifstream file;

public:
    bool open(const string &fileName)
    {
        file.open(fileName);
        return file.is_open();
    }
    bool close()
    {
        if (file.is_open())
        {
            file.close();
        }
        return !file.is_open();
    }
    unsigned int ReadChunks(char *buffer, unsigned int size)
    {
        file.read(buffer, size);
        return file.gcount();
    }
};

class Tokenizor
{
private:
    string carry;

public:
    vector<string> Tokenize(const string &chunk)
    {
        vector<string> words;
        string curr = carry;
        for (char c : chunk)
        {
            if (isalnum(c))
            {
                curr += tolower(c);
            }
            else if (!curr.empty())
            {
                words.push_back(curr);
                curr.clear();
            }
        }
        carry = curr;
        return words;
    }

    void clear(vector<string> &words)
    {
        if (!carry.empty())
        {
            words.push_back(carry);
            carry.clear();
        }
    }
};

class VersionIndexer
{
private:
    unordered_map<string, unordered_map<string, int>> versions;

public:
    void MakeIndexer(const string &version, FileReader &filereader, unsigned int sizeKB)
    {

        if (sizeKB > 1024 || sizeKB < 256)
        {
            throw invalid_argument("Buffer Size must be in between 256KB to 1024KB\n");
        }
        unsigned int size = sizeKB * 1024;
        vector<char> buffer(size);

        Tokenizor tokenizer;

        while (1)
        {
            unsigned int ReadSize = filereader.ReadChunks(buffer.data(), size);

            if (ReadSize == 0)
                break;

            string data(buffer.data(), ReadSize);
            vector<string> words = tokenizer.Tokenize(data);

            for (string &word : words)
            {
                versions[version][word]++;
            }
        }
        vector<string> lastwords;
        tokenizer.clear(lastwords);
        for (string &word : lastwords)
        {
            versions[version][word]++;
        }
    }
    // Example of Function overloading , function : cntWords()
    unsigned int cntWords(string &version, string &word)
    {
        auto it1 = versions.find(version);
        if (it1 == versions.end())
            return 0;

        for (char &c : word)
            c = tolower(c);

        auto it2 = it1->second.find(word);
        if (it2 == it1->second.end())
            return 0;

        return it2->second;
    }
    // Function overloading : 
    long long cntWords(string &v1, string &v2, string &word)
    {
        return (long long)cntWords(v2, word) - (long long)cntWords(v1, word);
    }

    static bool comp(const pair<string, int> &a, const pair<string, int> &b)
    {
        return a.second > b.second;
    }

    vector<pair<string, unsigned int>> TopK(string &version, int k)
    {
        vector<pair<string, unsigned int>> topK;
        auto it = versions.find(version);
        if (it == versions.end())
            return topK;

        for (auto &pair : it->second)
            topK.push_back(pair);

        sort(topK.begin(), topK.end(), comp);

        if (k < topK.size())
            topK.resize(k);

        return topK;
    }
};
// Abstract Base class : Query
// This also an example of Inheritance
class Query
{
public:
    virtual void RunQuery() = 0;
    virtual ~Query() {}
};
// Inheritence
class Word : public Query
{
private:
    VersionIndexer &indexer;
    string word, version;

public:
    Word(VersionIndexer &indx, string &word, string &v) : indexer(indx), version(v), word(word) {}

    void RunQuery()
    {
        cout << "Version: " << version << "\n";
        cout << "Count: " << indexer.cntWords(version, word) << "\n";
    }
};
// Inheritence
class TopK : public Query
{
private:
    VersionIndexer &indexer;
    string version;
    int k;

public:
    TopK(VersionIndexer &indx, string v, int k) : indexer(indx), version(v), k(k) {}

    void RunQuery()
    {
        cout << "Top-" << k << " words in version " << version << ": " << "\n";

        auto result = indexer.TopK(version, k);
        for (auto &p : result)
            cout << p.first << " " << p.second << "\n";
    }
};
// Inheritence
class Diff : public Query
{
private:
    VersionIndexer &indexer;
    string v1, v2, word;

public:
    Diff(VersionIndexer &indx, string &v1, string &v2, string &word) : indexer(indx), v1(v1), v2(v2), word(word) {}

    void RunQuery()
    {
        cout << "Difference (" << v2 << " - " << v1 << "): "
             << indexer.cntWords(v1, v2, word) << "\n";
    }
};

class CommandLine
{
private:
    unordered_map<string, string> inputs;

public:
    CommandLine(char *input[], int n)
    {
        for (int i = 1; i < n - 1; i += 2)
        {
            inputs[input[i]] = input[i + 1];
        }
    }
    string get(const string &s)
    {
        auto it = inputs.find(s);
        if (it == inputs.end())
        {
            throw runtime_error(s + " Not Found");
        }
        else
            return it->second;
    }
};

// Teplate Class Example :   user-defined template
template <typename Clock = chrono::high_resolution_clock>

class Time
{
private:
    typename Clock::time_point startTime;

public:
    void start()
    {
        startTime = Clock ::now();
    }

    double stop()
    {
        auto endTime = Clock::now();
        return chrono::duration<double>(endTime - startTime).count();
    }
};

// Main Function:
int main(int n, char *input[])
{
    // Exception Handling :
    try
    {
        CommandLine In(input, n);
        unsigned int size = stoul(In.get("--buffer"));
        string query = In.get("--query");

        VersionIndexer indexer;
        FileReader Reader;

        Time<> time;
        time.start();

        Query *q = NULL;

        if (query == "word")
        {

            string version = In.get("--version");
            string word = In.get("--word");
            string file = In.get("--file");

            if (!Reader.open(file))
            {
                throw runtime_error("Failed to open file : " + file);
            }
            indexer.MakeIndexer(version, Reader, size);
            Reader.close();

            q = new Word(indexer, word, version);
        }
        else if (query == "top")
        {

            string version = In.get("--version");
            int k = stoul(In.get("--top"));
            string file = In.get("--file");

            if (!Reader.open(file))
            {
                throw runtime_error("Failed to open file : " + file);
            }
            indexer.MakeIndexer(version, Reader, size);
            Reader.close();

            q = new TopK(indexer, version, k);
        }
        else if (query == "diff")
        {

            string v1 = In.get("--version1");
            string v2 = In.get("--version2");
            string file1 = In.get("--file1");
            string file2 = In.get("--file2");

            if (!Reader.open(file1))
            {
                throw runtime_error("Failed to open file : " + file1);
            }
            indexer.MakeIndexer(v1, Reader, size);
            Reader.close();

            if (!Reader.open(file2))
            {
                throw runtime_error("Failed to open file : " + file2);
            }
            indexer.MakeIndexer(v2, Reader, size);
            Reader.close();

            string word = In.get("--word");

            q = new Diff(indexer, v1, v2, word);
        }
        else
        {
            throw runtime_error("Invalid Query type");
        }

        // Run-Time Polymorphism
        q->RunQuery();

        double Reqtime = time.stop();
        cout << "Buffer Size (KB): " << size << '\n';
        cout << "Execution Time (s): " << Reqtime << '\n';

        delete q;
    }
    // Exception Handling :
    catch (exception &e)
    {
        cerr << "Error found : " << e.what() << '\n';
        return 1;
    }
    return 0;
}