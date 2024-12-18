#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

bool AOC5_ValidateJob(std::vector<std::pair<int, int>> &rules, std::vector<std::vector<int>> &jobs, std::vector<int> &job, size_t &index1, size_t &index2)
{
    for (size_t p = 0; p < job.size(); p++) {
        int page = job[p];

        // Compare this page with the rest of the pages in the job against the rules
        for (size_t q = p + 1; q < job.size(); q++) {
            int next_page = job[q];
            // Find rules for this page and next page
            // Job: 75,97,47,61,53
            // Rule violation: 97|75
            for (auto rule : rules) {
                if (rule.first == next_page && rule.second == page) {
                    index1 = p;
                    index2 = q;
                    return false;
                }
            }
        }
    }
    return true;
}

void main()
{
    std::vector<std::pair<int, int>> rules;
    std::string l;
    std::fstream rules_file("input5rules.txt");
    while (std::getline(rules_file, l)) {
        int a=0;
        int b=0;
        sscanf(l.c_str(), "%d|%d", &a, &b);
        rules.push_back(std::make_pair(a, b));
    }
    //std::cout << "Rules: " << rules.size() << std::endl;

    std::vector<std::vector<int>> jobs;
    std::fstream jobs_file("input5jobs.txt");
    while (std::getline(jobs_file, l)) {
        std::stringstream ss;
        ss << l;
        std::vector<int> job;
        int num = 0;
        std::string tmp;
        while (std::getline(ss, tmp, ',')) {
            std::istringstream(tmp) >> num;
            job.push_back(num);
        }
        jobs.push_back(job);
    }
    //std::cout << "Jobs: " << jobs.size() << std::endl;

    long sum = 0;
    int job_index = 0;
    for (auto job : jobs) {
        job_index++;

        size_t index1 = 0;
        size_t index2 = 0;
        int attempt = 0;
        int max_attempts = 200;
        while (!AOC5_ValidateJob(rules, jobs, job, index1, index2) && attempt < max_attempts) {
            attempt++;
            // Swap offending indices and try again
            int tmp = job[index1];
            job[index1] = job[index2];
            job[index2] = tmp;
        }
        if (attempt == max_attempts) {
            std::cout << "Job " << job_index << " Aborted" << std::endl;
        } else if (attempt > 0 && attempt < max_attempts) {
            std::cout << "Job " << job_index << " OK at attempt " << attempt << std::endl;
            // Find middle number in job
            int middle = job.size() / 2;
            // Find middle page number
            int middle_page = job[middle];
            sum += middle_page;
        }
    }
    std::cout << "Sum of middle pages: " << sum << std::endl;
    // 5093
}
