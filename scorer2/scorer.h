#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
public:
    bool IsTaskSolved(const std::pair<bool, bool>& info) const;
    auto GetTaskInfo(const StudentName& student_name, const TaskName& task_name);

    void RemoveTask(const StudentName& student_name, const TaskName& task_name);
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();

    ScoreTable GetScoreTable() const;

private:
    // В map check_ для каждого студента хранится map, в котором для каждой задачи
    // хранится пара bool значений {последняя проверка прошла; нет открытых мердж реквестов}
    std::map<StudentName, std::map<TaskName, std::pair<bool, bool>>> check_;
    ScoreTable score_table_;
};
