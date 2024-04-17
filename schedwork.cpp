#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    std::vector<size_t> shifts_avail,
    std::vector<std::vector<int>> workers_avail,
    DailySchedule& sched,
    int row,
    int col
);

void scheduleIntialize(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
);

std::vector<std::vector<int>>& w_avail2(int size1, int size2, const AvailabilityMatrix& avail, std::vector<std::vector<int>>& workers_avail);

std::vector<std::vector<int>>& w_avail1(int size, const AvailabilityMatrix& avail, std::vector<std::vector<int>>& workers_avail);

std::vector<size_t>& s_avail(size_t size, std::vector<size_t>& shifts_avail, const AvailabilityMatrix& avail, const size_t maxShifts);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    scheduleIntialize(avail, dailyNeed, maxShifts, sched);

    std::vector<std::vector<int>> workers_avail;
    std::vector<size_t> shifts_avail;

    workers_avail = w_avail1(0, avail, workers_avail);
    shifts_avail = s_avail(0, shifts_avail, avail, maxShifts);

    return scheduleHelper(shifts_avail, workers_avail, sched, 0, 0);
}


std::vector<size_t>& s_avail(size_t size, std::vector<size_t>& shifts_avail, const AvailabilityMatrix& avail, const size_t maxShifts){
  if (size == avail[0].size()){
    return shifts_avail;
  }

  else{
    shifts_avail.push_back(maxShifts);
    s_avail(size + 1, shifts_avail, avail, maxShifts);
  }
}


std::vector<std::vector<int>>& w_avail2(int size1, int size2, const AvailabilityMatrix& avail, std::vector<std::vector<int>>& workers_avail){
  if (size2 == avail.size()){
    return workers_avail;
  }

  else{
    if (avail[size2][size1]){
      workers_avail[size1].push_back(size2);
    }
    w_avail2(size1, size2 + 1, avail, workers_avail);
  }

}

std::vector<std::vector<int>>& w_avail1(int size, const AvailabilityMatrix& avail, std::vector<std::vector<int>>& workers_avail){
  
  if (size == avail[0].size()){
    return workers_avail;
  }

  else{
    std::vector<int> days_avail;
    workers_avail.push_back(days_avail);
    workers_avail = w_avail2(size, 0, avail, workers_avail);
    w_avail1(size+1, avail, workers_avail);
    return workers_avail;
  }

}


void scheduleIntialize(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    size_t days = avail.size();
 

    for (size_t i = 0; i < days; ++i){
        std::vector<Worker_T> day;
        for (size_t j = 0; j < dailyNeed; ++j){
            Worker_T k = -1;
            day.push_back(k);
        }
        sched.push_back(day);
    } 

}


bool scheduleHelper(
    std::vector<size_t> shifts_avail,
    std::vector<std::vector<int>> workers_avail,
    DailySchedule& sched,
    int row,
    int col
)
{
    if (col > sched[0].size() - 1){
        return scheduleHelper(shifts_avail, workers_avail, sched, row + 1, 0);
    }

    if (row > sched.size() -1 ){
        return true;
    }

    else{
        bool answer = false;
        int workers = workers_avail.size();
        for (int i = 0; i < workers; ++i){
            auto it = std::find(workers_avail[i].begin(), workers_avail[i].end(), row);
            if (it != workers_avail[i].end()){
                sched[row][col] = i;
                workers_avail[i].erase(it);
                shifts_avail[i] -= 1;
                if (shifts_avail[i] != -1){
                    answer = scheduleHelper(shifts_avail, workers_avail, sched, row, col + 1);
                }
                if (!answer){
                    sched[row][col] = -1;
                    workers_avail[i].push_back(row);
                    shifts_avail[i] += 1;
                }
            }
            if (answer == true){
                return answer;
            }
        }
        return answer;
    }




}