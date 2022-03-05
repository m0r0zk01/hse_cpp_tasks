#include "sort_students.h"
#include <tuple>

bool cmpNameDate(const Student& a, const Student& b) {
    return std::tie(a.last_name, a.name, a.birth_date.year, a.birth_date.month, a.birth_date.day) <
           std::tie(b.last_name, b.name, b.birth_date.year, b.birth_date.month, b.birth_date.day);
}

bool cmpDateName(const Student& a, const Student& b) {
    return std::tie(a.birth_date.year, a.birth_date.month, a.birth_date.day, a.last_name, a.name) <
           std::tie(b.birth_date.year, b.birth_date.month, b.birth_date.day, b.last_name, b.name);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    switch (sortKind) {
        case SortKind::Name:
            sort(students.begin(), students.end(), cmpNameDate);
            break;
        
        case SortKind::Date:
            sort(students.begin(), students.end(), cmpDateName);
            break;

        default:
            break;
    }
}
