#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <vector>

uint64_t min(std::vector<uint64_t> input)
{
    if(input.size())
    {
        uint64_t temp = *input.begin();
        std::vector<uint64_t>::iterator it;
        for(it = input.begin(); it!=input.end(); ++it)
        {
            if(temp > *it)
                temp = *it;
        }
        return temp;
    }
    else
        return 0;
}

uint64_t max(std::vector<uint64_t> input)
{
    if(input.size())
    {
        uint64_t temp = *input.begin();
        std::vector<uint64_t>::iterator it;
        for(it = input.begin(); it!=input.end(); ++it)
        {
            if(temp < *it)
                temp = *it;
        }
        return temp;
    }
    else
        return 0;
}

uint64_t ave(std::vector<uint64_t> input)
{
    uint64_t sum = 0;
    std::vector<uint64_t>::iterator it;
    for(it = input.begin(); it!=input.end(); ++it)
    {
        sum += (*it);
    }
    return sum/input.size();
}

uint64_t sdev(std::vector<uint64_t> input)
{
    if(input.size())
    {
        uint64_t average = ave(input);
        uint64_t temp = 0;
        std::vector<uint64_t>::iterator it;
        for(it = input.begin(); it!=input.end(); ++it)
        {
            temp += ((*it) - average) * ((*it) - average);
        }
        temp = temp/input.size();
        return sqrt(temp);
    }
    else
        return 0;
    
}


std::vector<uint64_t> merge(std::vector<uint64_t> left, std::vector<uint64_t> right)
{
    std::vector<uint64_t> result;
    std::vector<uint64_t>::iterator left_it = left.begin();
    std::vector<uint64_t>::iterator right_it = right.begin();
    int i = 0;
    
    while(left_it != left.end() && right_it != right.end())
    {
        if((*left_it)<(*right_it))
        {
            result.push_back(*left_it);
            ++left_it;
        }
        else
        {
            result.push_back(*right_it);
            ++right_it;
        }
        i++;
    }
    
    if(left_it != left.end())
    {
        result.insert(result.end(), left_it, left.end());
    }
    if(right_it != right.end())
    {
        result.insert(result.end(), right_it, right.end());
    }
    
    return result;
}

std::vector<uint64_t> sort(std::vector<uint64_t> input)
{
    
    std::vector<uint64_t> left, right;
    if(input.size()==1)
    {
        return input;
    }
    else
    {
      left.insert(left.begin(),input.begin(),input.begin()+input.size()/2);
      right.insert(right.begin(),input.begin()+input.size()/2,input.end());
      return merge(sort(left), sort(right));
    }
}

void printStats(std::vector<uint64_t> input, std::string name)
{
    std::vector<uint64_t>::iterator it;
    std::vector<uint64_t> sortedInput;
    std::cout << name << std::endl;
    
    //for(it=input.begin();it!=input.end();++it)
    //{
    //  std::cout << *it << " ";
    //}
    //std::cout << "\n" << std::endl;
    
    sortedInput = sort(input);
    
    //for(it=sortedInput.begin();it!=sortedInput.end();++it)
    //{
    //  std::cout << *it << " ";
    //}
    
    std::cout << std::endl;
    std::cout << "min                " << sortedInput.front() << std::endl;
    std::cout << "max                " << sortedInput.back() << std::endl;
    std::cout << "mean               " << sortedInput.at(sortedInput.size()/2) << std::endl;
    std::cout << "average            " << ave(input) << std::endl;
    std::cout << "standard deviation " << sdev(input) << std::endl;
    std::cout << std::endl;
}

void printStatsRef(std::vector<uint64_t> input, std::string name, uint64_t refTime)
{
    std::vector<uint64_t>::iterator it;
    std::vector<uint64_t> sortedInput;
    std::cout << name << std::endl;
    
    sortedInput = sort(input);
    
    //double dif;
    
    std::cout << "reference          " << refTime << std::endl;
    
    std::cout << "min                " << sortedInput.front() << std:: endl;
    //dif = static_cast<double> (sortedInput.front() - refTime);
    //std::cout << "   dif             " << dif << std::endl;
    std::cout << "                  %" << refTime*100/sortedInput.front() << std::endl;
    
    std::cout << "max                " << sortedInput.back() << std::endl;
    //dif = static_cast<double> (sortedInput.back() - refTime);
    //std::cout << "   dif             " << dif << std::endl;
    std::cout << "                  %" << refTime*100/sortedInput.back() << std::endl;
    
    std::cout << "mean               " << sortedInput.at(sortedInput.size()/2) << std::endl;
    //dif = static_cast<double> (sortedInput.at(sortedInput.size()/2) - refTime);
    //std::cout << "   dif             " << dif << std::endl;
    std::cout << "                  %" << refTime*100/(sortedInput.at(sortedInput.size()/2)) << std::endl;
    
    std::cout << "average            " << ave(input) << std::endl;
    //dif = static_cast<double> (ave(input) - refTime);
    //std::cout << "   dif             " << dif << std::endl;
    std::cout << "                  %" << refTime*100/(ave(input)) << std::endl;
    
    std::cout << "standard deviation " << sdev(input) <<  std::endl;
    //dif = static_cast<double> (sdev(input) - refTime);
    //std::cout << "   dif             " << dif << std::endl;
    std::cout << "                  %" << refTime*100/(sdev(input)) << std::endl;
    
    std::cout << std::endl;
}

void writeStats(std::vector<uint64_t> input, std::string name, std::string fileName)
{
    std::vector<uint64_t> sortedInput = sort(input);
    std::vector<uint64_t>::iterator it;
    
    char *a = new char[fileName.size()+1];
    a[fileName.size()]=0;
    memcpy(a,fileName.c_str(),fileName.size());
    
    std::ofstream myFile;
    myFile.open(a, std::ios::out | std::ios::app);
    myFile << name << std::endl;
    for(it=input.begin();it!=input.end();++it)
    {
      myFile << *it << ", ";
    }
    myFile << std::endl;
    myFile << std::endl;
    myFile << "min, " << sortedInput.front() << std::endl;
    myFile << "max, " << sortedInput.back() << std::endl;
    myFile << "mean, " << sortedInput.at(sortedInput.size()/2) << std::endl;
    myFile << "average, " << ave(input) << std::endl;
    myFile << "standard deviation, " << sdev(input) << std::endl;
    myFile << std::endl;
    delete [] a;
}

