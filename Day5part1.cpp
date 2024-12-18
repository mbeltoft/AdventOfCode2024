#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

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
    for (auto job : jobs) {
        bool job_ok = true;
        // 88,31,23,83,39
        for (size_t p = 0; p < job.size(); p++) {
            int page = job[p];

            // Compare this page with the rest of the pages in the job against the rules
            bool violation = false;
            for (size_t q = p + 1; q < job.size(); q++) {
                int next_page = job[q];
                // Find rules for this page and next page
                // Job: 75,97,47,61,53
                // Rule violation: 97|75
                for (auto rule : rules) {
                    if (rule.first == next_page && rule.second == page) {
                        violation = true;
                        break;
                    }
                }
            }
            if (violation) {
                job_ok = false;
                break;
            }
        }
        if (job_ok) {
            // Find middle number in job
            int middle = job.size() / 2;
            // Find middle page number
            int middle_page = job[middle];
            sum += middle_page;
        }
    }
    std::cout << "Sum of middle pages: " << sum << std::endl;
    // 6051
}