#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>

void main()
{
    std::map<std::string,std::set<std::string>> adj;
    std::set<std::string> computers;
    std::string s;
    
    std::ifstream ifs("input23.txt");
    // kh-tc
    while (ifs >> s) {
        std::string u = s.substr(0,2); // First computer
        std::string v = s.substr(3,2); // Second computer
        computers.insert(u);
        computers.insert(v);
        adj[u].insert(v); // These two computers are connected
        adj[v].insert(u); // These two computers are connected
    }
    std::vector<std::string> v;
    for (auto &i : computers) {
        v.push_back(i);
    }
    std::cout << "Computers: " << v.size() << std::endl;
    std::sort(v.begin(), v.end());
    //find maximal clique
    for(size_t i=0; i < v.size(); i++) {
        for(size_t j = i+1; j < v.size(); j++) {
            if (adj[v[i]].find(v[j]) == adj[v[i]].end()) {
                continue;
            }
            for(size_t k = j+1; k < v.size(); k++) {
                if (adj[v[i]].find(v[k]) == adj[v[i]].end() || adj[v[j]].find(v[k]) == adj[v[j]].end()) {
                    continue;
                }
                for(size_t l = k+1; l < v.size(); l++) {
                    if (adj[v[i]].find(v[l]) == adj[v[i]].end() || adj[v[j]].find(v[l]) == adj[v[j]].end() || adj[v[k]].find(v[l]) == adj[v[k]].end()) {
                        continue;
                    }
                    for(size_t m = l+1; m < v.size(); m++) {
                        if (adj[v[i]].find(v[m]) == adj[v[i]].end() || adj[v[j]].find(v[m]) == adj[v[j]].end() || adj[v[k]].find(v[m]) == adj[v[k]].end() || adj[v[l]].find(v[m]) == adj[v[l]].end()) {
                            continue;
                        }
                        for(size_t n = m+1; n < v.size(); n++) {
                            if (adj[v[i]].find(v[n]) == adj[v[i]].end() || adj[v[j]].find(v[n]) == adj[v[j]].end() || adj[v[k]].find(v[n]) == adj[v[k]].end() || adj[v[l]].find(v[n]) == adj[v[l]].end() || adj[v[m]].find(v[n]) == adj[v[m]].end()) {
                                continue;
                            }
                            for(size_t o = n+1; o < v.size(); o++) {
                                if (adj[v[i]].find(v[o]) == adj[v[i]].end() || adj[v[j]].find(v[o]) == adj[v[j]].end() || adj[v[k]].find(v[o]) == adj[v[k]].end() || adj[v[l]].find(v[o]) == adj[v[l]].end() || adj[v[m]].find(v[o]) == adj[v[m]].end() || adj[v[n]].find(v[o]) == adj[v[n]].end()) {
                                    continue;
                                }
                                for(size_t p = o+1; p < v.size(); p++) {
                                    if (adj[v[i]].find(v[p]) == adj[v[i]].end() || adj[v[j]].find(v[p]) == adj[v[j]].end() || adj[v[k]].find(v[p]) == adj[v[k]].end() || adj[v[l]].find(v[p]) == adj[v[l]].end() || adj[v[m]].find(v[p]) == adj[v[m]].end() || adj[v[n]].find(v[p]) == adj[v[n]].end() || adj[v[o]].find(v[p]) == adj[v[o]].end()) {
                                        continue;
                                    }
                                    for(size_t q = p+1; q < v.size(); q++) {
                                        if (adj[v[i]].find(v[q]) == adj[v[i]].end() || adj[v[j]].find(v[q]) == adj[v[j]].end() || adj[v[k]].find(v[q]) == adj[v[k]].end() || adj[v[l]].find(v[q]) == adj[v[l]].end() || adj[v[m]].find(v[q]) == adj[v[m]].end() || adj[v[n]].find(v[q]) == adj[v[n]].end() || adj[v[o]].find(v[q]) == adj[v[o]].end() || adj[v[p]].find(v[q]) == adj[v[p]].end()) {
                                            continue;
                                        }
                                        for(size_t r = q+1; r < v.size(); r++) {
                                            if (adj[v[i]].find(v[r]) == adj[v[i]].end() || adj[v[j]].find(v[r]) == adj[v[j]].end() || adj[v[k]].find(v[r]) == adj[v[k]].end() || adj[v[l]].find(v[r]) == adj[v[l]].end() || adj[v[m]].find(v[r]) == adj[v[m]].end() || adj[v[n]].find(v[r]) == adj[v[n]].end() || adj[v[o]].find(v[r]) == adj[v[o]].end() || adj[v[p]].find(v[r]) == adj[v[p]].end() || adj[v[q]].find(v[r]) == adj[v[q]].end()) {
                                                continue;
                                            }
                                            for(size_t s = r+1; s < v.size(); s++) {
                                                if (adj[v[i]].find(v[s]) == adj[v[i]].end() || adj[v[j]].find(v[s]) == adj[v[j]].end() || adj[v[k]].find(v[s]) == adj[v[k]].end() || adj[v[l]].find(v[s]) == adj[v[l]].end() || adj[v[m]].find(v[s]) == adj[v[m]].end() || adj[v[n]].find(v[s]) == adj[v[n]].end() || adj[v[o]].find(v[s]) == adj[v[o]].end() || adj[v[p]].find(v[s]) == adj[v[p]].end() || adj[v[q]].find(v[s]) == adj[v[q]].end() || adj[v[r]].find(v[s]) == adj[v[r]].end()) {
                                                    continue;
                                                }
                                                for(size_t t = s+1; t < v.size(); t++) {
                                                    if (adj[v[i]].find(v[t]) == adj[v[i]].end() || adj[v[j]].find(v[t]) == adj[v[j]].end() || adj[v[k]].find(v[t]) == adj[v[k]].end() || adj[v[l]].find(v[t]) == adj[v[l]].end() || adj[v[m]].find(v[t]) == adj[v[m]].end() || adj[v[n]].find(v[t]) == adj[v[n]].end() || adj[v[o]].find(v[t]) == adj[v[o]].end() || adj[v[p]].find(v[t]) == adj[v[p]].end() || adj[v[q]].find(v[t]) == adj[v[q]].end() || adj[v[r]].find(v[t]) == adj[v[r]].end() || adj[v[s]].find(v[t]) == adj[v[s]].end()) {
                                                        continue;
                                                    }
                                                    for(size_t u = t+1; u < v.size(); u++) {
                                                        if (adj[v[i]].find(v[u]) == adj[v[i]].end() || adj[v[j]].find(v[u]) == adj[v[j]].end() || adj[v[k]].find(v[u]) == adj[v[k]].end() || adj[v[l]].find(v[u]) == adj[v[l]].end() || adj[v[m]].find(v[u]) == adj[v[m]].end() || adj[v[n]].find(v[u]) == adj[v[n]].end() || adj[v[o]].find(v[u]) == adj[v[o]].end() || adj[v[p]].find(v[u]) == adj[v[p]].end() || adj[v[q]].find(v[u]) == adj[v[q]].end() || adj[v[r]].find(v[u]) == adj[v[r]].end() || adj[v[s]].find(v[u]) == adj[v[s]].end() || adj[v[t]].find(v[u]) == adj[v[t]].end()) {
                                                            continue;
                                                        }
                                                        std::cout << v[i] << "," << v[j] << "," << v[k] << "," << v[l] << "," << v[m] << "," << v[n] << "," << v[o] << "," << v[p] << "," << v[q] << "," << v[r] << "," << v[s] << "," << v[t] << "," << v[u] << std::endl;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}