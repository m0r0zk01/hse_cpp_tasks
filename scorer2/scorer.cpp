#include "scorer.h"

bool Scorer::IsTaskSolved(const std::pair<bool, bool>& info) const {
    return info.first && info.second;
}

void Scorer::RemoveTask(const StudentName& student_name, const TaskName& task_name) {
    auto it = score_table_.find(student_name);
    size_t new_size = it->second.erase(task_name);
    if (new_size == 0) {
        score_table_.erase(it);
    }
}

auto Scorer::GetTaskInfo(const StudentName& student_name, const TaskName& task_name) {
    auto [it1, ok1] = check_.emplace(student_name, std::map<TaskName, std::pair<bool, bool>>());
    auto [it2, ok2] = it1->second.emplace(task_name, std::make_pair(false, true));
    (void)ok1, (void)ok2;
    return it2;
}

void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    auto student_info_it = GetTaskInfo(student_name, task_name);
    auto& student_info = student_info_it->second;
    bool was_task_solved = IsTaskSolved(student_info);
    student_info.first = false;
    if (was_task_solved && !IsTaskSolved(student_info)) {
        RemoveTask(student_name, task_name);
    }
}

void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    auto student_info_it = GetTaskInfo(student_name, task_name);
    auto& student_info = student_info_it->second;
    bool was_task_solved = IsTaskSolved(student_info);
    student_info.first = true;
    if (!was_task_solved && IsTaskSolved(student_info)) {
        score_table_[student_name].emplace(task_name);
    }
}

void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    auto student_info_it = GetTaskInfo(student_name, task_name);
    auto& student_info = student_info_it->second;
    bool was_task_solved = IsTaskSolved(student_info);
    student_info.second = false;
    if (was_task_solved && !IsTaskSolved(student_info)) {
        RemoveTask(student_name, task_name);
    }
}

void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    auto student_info_it = GetTaskInfo(student_name, task_name);
    auto& student_info = student_info_it->second;
    bool was_task_solved = IsTaskSolved(student_info);
    student_info.second = true;
    if (!was_task_solved && IsTaskSolved(student_info)) {
        score_table_[student_name].emplace(task_name);
    }
}

void Scorer::Reset() {
    check_.clear();
    score_table_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    return score_table_;
}
