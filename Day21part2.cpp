#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <queue>

using namespace std::chrono_literals;

#if 0
inline int64_t Mix(int64_t tmp, int64_t secret)
{
    return tmp ^ secret;
}

inline int64_t Prune(int64_t secret)
{
    return secret % 16777216;
}

inline int64_t NewSecretNumber(int64_t old_secret)
{
    int64_t tmp = old_secret * 64;
    int64_t new_secret = Prune(Mix(tmp, old_secret));
    tmp = new_secret >> 5; // Divide by 32
    new_secret = Prune(Mix(tmp, new_secret));
    tmp = new_secret << 11; // Multiply by 2048
    new_secret = Prune(Mix(tmp, new_secret));
    return new_secret;
}
#else
inline int64_t NewSecretNumber(int64_t old_secret)
{
    int64_t tmp = old_secret * 64;
    int64_t new_secret = (tmp ^ old_secret) % 16777216;
    tmp = new_secret >> 5; // Divide by 32
    new_secret = (tmp ^ new_secret) % 16777216;
    tmp = new_secret << 11; // Multiply by 2048
    new_secret = (tmp ^ new_secret) % 16777216;
    return new_secret;
}
#endif

typedef std::vector<std::pair<int8_t, int8_t>> PricesAndChanges;
typedef std::map<int, PricesAndChanges> BuyersPricesAndChanges;

BuyersPricesAndChanges &CalculatePricesAndChanges(std::vector<int64_t> &buyer_secrets, int iterations)
{
    static BuyersPricesAndChanges bpac;
    bpac.clear();
    for (int b = 0; b < buyer_secrets.size(); b++) {
        PricesAndChanges pac;
        int64_t prev_price = buyer_secrets[b] % 10; // Only use last digit as price
        //std::cout << "Org prev price: " << prev_price << std::endl;
        //bool first = true;
        for (int i = 0; i < iterations; i++) {
            buyer_secrets[b] = NewSecretNumber(buyer_secrets[b]);
            //std::cout << "Iteration " << i << " secret: " << buyer_secrets[b];
            int8_t price = buyer_secrets[b] % 10; // Only use last digit as price
            //std::cout << " price: " << (int)price;
            int8_t change = price - prev_price;
            //std::cout << " change: " << (int)change << std::endl;
            //if (!first) {
                pac.push_back(std::make_pair(price, change));
            //}
            //first = false;
            prev_price = price;
        }
        //std::cout << "Buyer " << b << ": " << buyer_secrets[b] << std::endl;
        bpac[b] = pac;
    }
    return bpac;
}

void test1()
{
    std::vector<int64_t> buyer_secrets;
    buyer_secrets.push_back(123);
    BuyersPricesAndChanges bpac = CalculatePricesAndChanges(buyer_secrets, 10);
    for (auto smth : bpac[0]) {
        std::cout << (int)smth.first << " (" << (int)smth.second << ")" << std::endl;
    }
    // 0 (-3)
    // 6 (6)
    // 5 (-1)
    // 4 (-1)
    // 4 (0)
    // 6 (2)
    // 4 (-2)
    // 4 (0)
    // 2 (-2)

    int sequence_length = 0;
    for (int target_price = 9; target_price > 0; target_price--) {
        for (int i = bpac[0].size() - 1; i > 0; i--) {
            if (bpac[0][i].first == target_price) {
                sequence_length = 4;
                std::cout << "Sequence for price: " << target_price << std::endl;
            }
            if (sequence_length > 0) {
                sequence_length--;
                std::cout << (int)bpac[0][i].second << std::endl;
            }
        }
    }
}

void test2()
{
    std::vector<int64_t> buyer_secrets;
    buyer_secrets.push_back(1);
    buyer_secrets.push_back(2);
    buyer_secrets.push_back(3);
    buyer_secrets.push_back(2024);
    BuyersPricesAndChanges bpac = CalculatePricesAndChanges(buyer_secrets, 50);
    for (auto smth : bpac[0]) {
        std::cout << (int)smth.first << " (" << (int)smth.second << ")" << std::endl;
    }
}

typedef std::vector<int8_t> sequence;

int HuntSequence(const sequence &seq, const BuyersPricesAndChanges &bpac)
{
    int sum = 0;
    for (const auto &buyer : bpac) {
        size_t idx = 0;
        int buyer_id = buyer.first;
        int sequence_tracker = 0;
        for (const auto &smth : buyer.second) {
            //if (sequence_tracker > 2)
            //    std::cout << "Change: " << (int)smth.second << " seqence index: " << sequence_tracker << " sequence: " << (int)sequence[sequence_tracker] << std::endl;
            if (smth.second == seq[sequence_tracker]) {
                sequence_tracker++;
            } else {
                sequence_tracker = 0;
            }
            if (sequence_tracker == seq.size()) {
                //std::cout << "Buyer " << buyer_id << " bought at idx: " << idx << " price: " << (int)smth.first << std::endl;
                sum += smth.first;
                break;
            }
            idx++;
        }
    }

    return sum;
}

void test3()
{
    std::vector<int64_t> buyer_secrets;
    buyer_secrets.push_back(1);
    buyer_secrets.push_back(2);
    buyer_secrets.push_back(3);
    buyer_secrets.push_back(2024);

    std::vector<int8_t> sequence;
    sequence.push_back(-2);
    sequence.push_back(1);
    sequence.push_back(-1);
    sequence.push_back(3);

    BuyersPricesAndChanges bpac = CalculatePricesAndChanges(buyer_secrets, 2000);

    int sum = HuntSequence(sequence, bpac);
    std::cout << "Sum: " << sum << std::endl;
    // 23
}

bool sequence_compare(const sequence& a, const sequence& b)
{
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}


class ThreadPool
{
public:
    ThreadPool(int no_of_threads) : m_pool(no_of_threads)
    {
        for (int i = 0; i < no_of_threads; i++)
        {
            m_pool[i] = std::thread(&ThreadPool::thread_func, this, i);
            m_pool[i].detach();
        }
    }

    void add_task(std::function<void(int)> task_fn)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_task_queue.push(task_fn);
        m_cv.notify_all();
    }

    size_t queue_size()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        return m_task_queue.size();
    }

    void stop_processing()
    {
        m_stop_all_threads = true;
    }

private:
    void thread_func(int thread_id)
    {
        while (!m_stop_all_threads)
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            // wait for some task to be added in queue
            if (!m_cv.wait_for(lck, 100us, [this]() { return !m_task_queue.empty(); }))
                continue;
            // pick up task, update queue
            auto fn = m_task_queue.front();
            m_task_queue.pop();
            lck.unlock();
            // execute task
            fn(thread_id);
            // std::this_thread::sleep_for(1us);
        }
    }

    std::vector<std::thread> m_pool;
    std::atomic<bool> m_stop_all_threads{false};
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue< std::function<void(int)> > m_task_queue;
};

std::atomic<int> m_max_sum{0};
std::mutex g_mutex;

void main()
{
    std::vector<int64_t> buyer_secrets;
    std::fstream file("input21.txt");
    std::string line;

    while (std::getline(file, line)) {
        buyer_secrets.push_back(strtoull(line.c_str(), nullptr, 10));
    }

    std::cout << "Buyers: " << buyer_secrets.size() << std::endl;

    BuyersPricesAndChanges bpac = CalculatePricesAndChanges(buyer_secrets, 2000);

    std::cout << "Done calculating Prices and Ranges for all buyers" << std::endl;

    std::vector<sequence> sequence_candidates;

    int max_search = 10 * bpac.size() * 2000;
    int search_idx = 0;
    int old_percentage = 0;

    for (int target_price = 9; target_price > 6; target_price--) {
        // Find sequences that hit target_price
        
        // Run through all buyers
        for (const auto &buyer : bpac) {
            //std::cout << "Buyer: " << buyer.first << std::endl;
            auto pac = buyer.second;
            for (int i = 0; i < pac.size(); i++) {
                if (pac[i].first == target_price && i > 3) {
                    sequence seq;
                    seq.push_back(pac[i-3].second);
                    seq.push_back(pac[i-2].second);
                    seq.push_back(pac[i-1].second);
                    seq.push_back(pac[i].second);
                    // Weed out duplicates
                    //if (find_if(sequence_candidates.begin(), sequence_candidates.end(), std::bind(sequence_compare, std::placeholders::_1, seq)) == sequence_candidates.end()) {
                    sequence_candidates.push_back(seq);
                    //}
                }
            }
        }
        //break;
    }
    std::cout << "Sequence candidates: " << sequence_candidates.size() << std::endl;

    ThreadPool pool(30);

    for (const auto &seq : sequence_candidates) {
        //sequence seq = sequence_candidates[i];
        pool.add_task([the_seq = seq, the_bpac = bpac](int id) {
            int sum = HuntSequence(the_seq, the_bpac);
            //std::cout << "Thread " << id << " Sum: " << sum << std::endl;
            //std::cout << "Thread " << id << " Sequence size: " << x.size() << std::endl;
            //std::cout << "This is task " << x << " in thread " << id << '\n';
            std::unique_lock<std::mutex> lck(g_mutex);
            if (sum > m_max_sum) {
                m_max_sum = sum;
                std::cout << "New max sum: " << sum << std::endl;
            }
        });
    }

    while (pool.queue_size()) {
        std::this_thread::sleep_for(1s);
    }
    pool.stop_processing();

#if 0
    // Run through all sequences and track the best overall sum
    int max_sum = 0;
    int idx = 0;
    old_percentage = 0;
    for (const auto &seq : sequence_candidates) {
        int sum = HuntSequence(seq, bpac);
        if (sum > max_sum) {
            max_sum = sum;

            std::cout << "Sequence: ";
            for (size_t x = 0; x < seq.size(); x++) {
                std::cout << (int)seq[x] << " ";
            }
            std::cout << " sum: " << sum << std::endl;
        }
        idx++;
        if (idx % 100 == 0)
            std::cout << idx << std::endl;
        int percentage = 100 * idx / sequence_candidates.size();
        if (percentage != old_percentage) {
            old_percentage = percentage;
            std::cout << "Hunt progress: " << percentage << "%" << std::endl;
        }
    }
    std::cout << "Sum: " << max_sum << std::endl;
#endif
}
