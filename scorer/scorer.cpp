#include "scorer.h"
#include <algorithm>
#include <tuple>
#include <unordered_map>

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::vector<const Event*> sorted_events;
    for (const Event& event : events) {
        sorted_events.push_back(&event);
    }
    sort(sorted_events.begin(), sorted_events.end(), [](const Event* a, const Event* b) { return a->time < b->time; });

    ScoreTable result;

    // В unordered_map для каждого студента хранится unordered_map, в котором для каждого задания
    // хранится пара bool значений {последняя проверка прошла; нет открытых мердж реквестов}
    std::unordered_map<StudentName, std::unordered_map<TaskName, std::pair<bool, bool>>> check;
    for (const Event* event : sorted_events) {
        if (event->time > score_time) {
            break;
        }

        // Инициализация check, если таких значений не существует
        // А также сохранение итераторов, чтобы не выполнять поиск заново
        auto [it1, ok1] = check.emplace(event->student_name, std::unordered_map<TaskName, std::pair<bool, bool>>());
        auto [it2, ok2] = it1->second.emplace(event->task_name, std::make_pair(false, true));
        (void)ok1; (void)ok2;
        auto& student_task_info = it2->second;

        switch (event->event_type) {
            case EventType::CheckFailed:
                student_task_info.first = false;
                break;
            case EventType::CheckSuccess:
                student_task_info.first = true;
                break;
            case EventType::MergeRequestOpen:
                student_task_info.first = false;
                break;
            case EventType::MergeRequestClosed:
                student_task_info.first = true;
                break;
            default:
                break;
        }
    }

    for (const auto& [student_name, tasks_map] : check) {
        for (const auto& [task_name, kvp] : tasks_map) {
            if (kvp.first && kvp.second) {
                result[student_name].insert(task_name);
            }
        }
    }

    return result;
}
